/** @file scanner.c
 *
 * @brief Implementation of the scanner module.
 *
 * @author David J. Lains (dlains)
 * @bug No known bugs
 */

#include "common.h"
#include "keywords.h"
#include "scanner.h"
#include "source.h"
#include "memory.h"

Source scanner = NULL;

// Private scanner methods.
static Token make_token(TokenType type);
static Token error_token(const char *message);
static Token string();
static Token number();
static Token identifier();
static TokenType identifier_type();
static bool is_alpha(char c);
static bool is_digit(char c);

void make_current(Source source)
{
  if(scanner && !is_at_end(source))
  {
    // TODO: Push it on the source stack.
  }

  scanner = source;
}

void add_source_line(const char *line)
{
  Source s = source_create(line);
  make_current(s);
}

void add_source_file(const char *file_path)
{
  Source s = source_create_file(file_path);
  make_current(s);
}

Token next_token()
{
  skip_whitespace(scanner);

  scanner->start = scanner->current;

  if(is_at_end(scanner))
    return make_token(TOKEN_EOF);

  char c = advance(scanner);

  if(is_alpha(c))
    return identifier();
  if(is_digit(c))
    return number();

  switch(c)
  {
    case '(':
      return make_token(TOKEN_LEFT_PAREN);
    case ')':
      return make_token(TOKEN_RIGHT_PAREN);
    case '{':
      return make_token(TOKEN_LEFT_BRACE);
    case '}':
      return make_token(TOKEN_RIGHT_BRACE);
    case '[':
      return make_token(TOKEN_LEFT_BRACKET);
    case ']':
      return make_token(TOKEN_RIGHT_BRACKET);
    case '%':
      return make_token(TOKEN_PERCENT);
    case ',':
      return make_token(TOKEN_COMMA);
    case '^':
      return make_token(TOKEN_CARET);
    case '.':
      return make_token(TOKEN_DOT);
    case '-':
      return make_token(TOKEN_MINUS);
    case '+':
      return make_token(TOKEN_PLUS);
    case ';':
      return make_token(TOKEN_SEMICOLON);
    case '/':
      return make_token(TOKEN_SLASH);
    case '&':
      return make_token(TOKEN_AND);
    case '|':
      return make_token(TOKEN_OR);
    case '*':
      return make_token(match(scanner, '*') ? TOKEN_POWER : TOKEN_STAR);
    case '!':
      return make_token(match(scanner, '=') ? TOKEN_BANG_EQUAL : TOKEN_BANG);
    case '=':
      return make_token(match(scanner, '=') ? TOKEN_EQUAL_EQUAL : TOKEN_EQUAL);
    case '>':
      return make_token(match(scanner, '=') ? TOKEN_GREATER_EQUAL : TOKEN_GREATER);
    case '<':
      return make_token(match(scanner, '=') ? TOKEN_LESS_EQUAL : TOKEN_LESS);
    case '"':
      return string();
  }

  return error_token("Unexpected character.");
}

/** @brief Make a new token with the given token type.
 *
 * Creates a new token with the given token type and fills in the
 * other information. If the token lexeme is longer than LEXEME_LEN
 * an error token will be created instead.
 *
 * @param type The token type to create.
 * @return Token the newly created token.
 */
static Token make_token(TokenType type)
{
  int length = scanner->current - scanner->start;
  if(length > LEXEME_LEN - 1)
    return error_token("Identifier length is too long.");

  Token token;
  token.type = type;
  memcpy(token.lexeme, scanner->start, length);
  token.lexeme[length] = '\0';
  token.line = scanner->line;

  return token;
}

/** @brief Make an error token to signal a scan error.
 *
 * Creates a new token with a token type of TOKEN_ERROR. The
 * message passed in will be copied to the lexeme field.
 *
 * @param message The error message for this token error.
 * @return Token The newly created error token.
 */
static Token error_token(const char *message)
{
  Token token;
  token.type = TOKEN_ERROR;
  memcpy(token.lexeme, message, strlen(message));
  token.line = scanner->line;

  return token;
}

/** @brief Scan for a string literal.
 *
 * The string function is called when a double quote character
 * is found in the source. It scans forward until it finds
 * either another double quote character or hits the end of
 * the source code.
 *
 * If it finds the ending double quote it creates a new string
 * token.
 *
 * If it hits the end of the file without finding the closing
 * double quote an error token is returned.
 *
 * @return Token Either the string token or an error token.
 */
static Token string()
{
  while(peek(scanner) != '"' && !is_at_end(scanner))
  {
    if(peek(scanner) == '\n')
    {
      scanner->line++;
    }
    advance(scanner);
  }

  if(is_at_end(scanner))
    return error_token("Unterminated string.");

  // Advance past the closing quote.
  advance(scanner);
  return make_token(TOKEN_STRING);
}

/** @brief Scan for a number literal.
 *
 * The number function is called when a digit character
 * is found in the source. It scans forward through any
 * additional digits. If it finds a '.' character it
 * checks for digits after it and combins them into a
 * double value.
 *
 * @return Token The number token.
 */
static Token number()
{
  while(is_digit(peek(scanner)))
    advance(scanner);

  // Look for a fractional part.
  if(peek(scanner) == '.' && is_digit(peek_next(scanner)))
  {
    advance(scanner);
    while(is_digit(peek(scanner)))
      advance(scanner);
  }

  return make_token(TOKEN_NUMBER);
}

/** @brief Scan for an identifier or a keyword.
 *
 * The identifier function is called when a alpha character
 * is found in the source. Additional alpha numeric
 * characters are scanned until something other than a
 * character or a digit is found. The identifier_type
 * function is then used to determine if the token
 * is a keyword or an identifier.
 *
 * @return Token Either an identifier or a keyword token.
 */
static Token identifier()
{
  while(is_alpha(peek(scanner)) || is_digit(peek(scanner)))
    advance(scanner);

  return make_token(identifier_type());
}

/** @brief Determine if the current token is an identifier or a keyword.
 *
 * Check the current marked token to see if it is a keyword. This uses
 * the 'keywords' module. If a keyword is identified its token type is
 * returned, otherwise the identifier token type is returned.
 *
 * @return TokenType The token type for this identifier.
 */
static TokenType identifier_type()
{
  char identifier[LEXEME_LEN];
  memcpy(identifier, scanner->start, scanner->current - scanner->start);
  identifier[scanner->current - scanner->start] = '\0';

  int type = find_keyword(identifier);
  if(type != 0)
    return type;

  return TOKEN_IDENTIFIER;
}

/** @brief Is the given character an alpha character.
 *
 * If the given character is in the range of lower case characters
 * or upper case characters or is an underscore it is considered
 * an alpha character.
 *
 * @param char The character to check.
 * @return Boolean True if the character is alpha, false otherwise.
 */
static bool is_alpha(char c)
{
  return(c >= 'a' && c <= 'z') || (c >= 'A' && c <= 'Z') || c == '_';
}

/** @brief Is the given character a digit?
 *
 * If the given character is a digit (0 - 9) then return true.
 *
 * @param char The character to check.
 * @return Boolean True if the character is a digit, false otherwise.
 */
static bool is_digit(char c)
{
  return c >= '0' && c <= '9';
}
