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
#include "memory.h"

/** @brief The Scanner struct holds the current scanning information.
 */
typedef struct
{
  const char *start;     /**< Marks the start of the current token. */
  const char *current;   /**< Markes the current position of the scan. */
  int line;              /**< Tracks the current line number for error reporting. */
} Scanner;

Scanner scanner;

// Private scanner methods.
static Token makeToken(TokenType type);
static Token errorToken(const char *message);
static Token string();
static Token number();
static Token identifier();
static TokenType identifierType();
static char advance();
static char peek();
static char peekNext();
static void skipWhitespace();
static bool isAtEnd();
static bool match(char expected);
static bool isAlpha(char c);
static bool isDigit(char c);

void initScanner(const char *source)
{
  scanner.start = source;
  scanner.current = source;
  scanner.line = 1;
}

Token nextToken()
{
  skipWhitespace();

  scanner.start = scanner.current;

  if(isAtEnd())
    return makeToken(TOKEN_EOF);

  char c = advance();

  if(isAlpha(c))
    return identifier();
  if(isDigit(c))
    return number();

  switch(c)
  {
    case '(':
      return makeToken(TOKEN_LEFT_PAREN);
    case ')':
      return makeToken(TOKEN_RIGHT_PAREN);
    case '{':
      return makeToken(TOKEN_LEFT_BRACE);
    case '}':
      return makeToken(TOKEN_RIGHT_BRACE);
    case '[':
      return makeToken(TOKEN_LEFT_BRACKET);
    case ']':
      return makeToken(TOKEN_RIGHT_BRACKET);
    case '%':
      return makeToken(TOKEN_PERCENT);
    case ',':
      return makeToken(TOKEN_COMMA);
    case '^':
      return makeToken(TOKEN_CARET);
    case '.':
      return makeToken(TOKEN_DOT);
    case '-':
      return makeToken(TOKEN_MINUS);
    case '+':
      return makeToken(TOKEN_PLUS);
    case ';':
      return makeToken(TOKEN_SEMICOLON);
    case '/':
      return makeToken(TOKEN_SLASH);
    case '&':
      return makeToken(TOKEN_AND);
    case '|':
      return makeToken(TOKEN_OR);
    case '*':
      return makeToken(match('*') ? TOKEN_POWER : TOKEN_STAR);
    case '!':
      return makeToken(match('=') ? TOKEN_BANG_EQUAL : TOKEN_BANG);
    case '=':
      return makeToken(match('=') ? TOKEN_EQUAL_EQUAL : TOKEN_EQUAL);
    case '>':
      return makeToken(match('=') ? TOKEN_GREATER_EQUAL : TOKEN_GREATER);
    case '<':
      return makeToken(match('=') ? TOKEN_LESS_EQUAL : TOKEN_LESS);
    case '"':
      return string();
  }

  return errorToken("Unexpected character.");
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
static Token makeToken(TokenType type)
{
  int length = scanner.current - scanner.start;
  if(length > LEXEME_LEN - 1)
    return errorToken("Identifier length is too long.");

  Token token;
  token.type = type;
  memcpy(token.lexeme, scanner.start, length);
  token.lexeme[length] = '\0';
  token.line = scanner.line;

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
static Token errorToken(const char *message)
{
  Token token;
  token.type = TOKEN_ERROR;
  memcpy(token.lexeme, message, strlen(message));
  token.line = scanner.line;

  return token;
}

/** @brief Scan for a string literal.
 *
 * The string function is called when a double quote character
 * if found in the source. It scans forward until if finds
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
  while(peek() != '"' && !isAtEnd())
  {
    if(peek() == '\n')
    {
      scanner.line++;
    }
    advance();
  }

  if(isAtEnd())
    return errorToken("Unterminated string.");

  // Advance past the closing quote.
  advance();
  return makeToken(TOKEN_STRING);
}

static Token number()
{
  while(isDigit(peek()))
    advance();

  // Look for a fractional part.
  if(peek() == '.' && isDigit(peekNext()))
  {
    advance();
    while(isDigit(peek()))
      advance();
  }

  return makeToken(TOKEN_NUMBER);
}

static Token identifier()
{
  while(isAlpha(peek()) || isDigit(peek()))
    advance();

  return makeToken(identifierType());
}

static TokenType identifierType()
{
  char identifier[LEXEME_LEN];
  memcpy(identifier, scanner.start, scanner.current - scanner.start);
  identifier[scanner.current - scanner.start] = '\0';

  int type = find_keyword(identifier);
  if(type != 0)
    return type;

  return TOKEN_IDENTIFIER;
}

static char advance()
{
  scanner.current++;
  return scanner.current[-1];
}

static char peek()
{
  return *scanner.current;
}

static char peekNext()
{
  if(isAtEnd())
    return '\0';
  return scanner.current[1];
}

static void skipWhitespace()
{
  for(;;)
  {
    char c = peek();
    switch(c)
    {
      case ' ':
      case '\r':
      case '\t':
        advance();
        break;
      case '\n':
        scanner.line++;
        advance();
        break;
      case '#':
        while(peek() != '\n' && !isAtEnd())
          advance();
        break;
      default:
        return;
    }
  }
}

static bool isAtEnd()
{
  return *scanner.current == '\0';
}

static bool match(char expected)
{
  if(isAtEnd())
    return false;

  if(*scanner.current != expected)
    return false;

  scanner.current++;
  return true;
}

static bool isAlpha(char c)
{
  return(c >= 'a' && c <= 'z') || (c >= 'A' && c <= 'Z') || c == '_';
}

static bool isDigit(char c)
{
  return c >= '0' && c <= '9';
}
