/** @file token.h
 *
 * @brief Define the tokens used by the Cube language.
 *
 * The token module provides an enumeration of all the valid
 * tokens used by the Cube language. It also provides a couple
 * functions to display a token that can be useful during debugging.
 *
 * @author David J. Lains
 * @bug No known bugs
 */

#ifndef TOKEN_H
#define TOKEN_H

#define LEXEME_LEN 50

typedef enum
{
  // Single character tokens.
  TOKEN_LEFT_PAREN,
  TOKEN_RIGHT_PAREN,
  TOKEN_LEFT_BRACE,
  TOKEN_RIGHT_BRACE,
  TOKEN_LEFT_BRACKET,
  TOKEN_RIGHT_BRACKET,
  TOKEN_PERCENT,
  TOKEN_COMMA,
  TOKEN_CARET,
  TOKEN_DOT,
  TOKEN_MINUS,
  TOKEN_PLUS,
  TOKEN_SEMICOLON,
  TOKEN_SLASH,
  TOKEN_STAR,
  TOKEN_AND,
  TOKEN_OR,

  // One or two character tokens.
  TOKEN_BANG,
  TOKEN_BANG_EQUAL,
  TOKEN_EQUAL,
  TOKEN_EQUAL_EQUAL,
  TOKEN_GREATER,
  TOKEN_GREATER_EQUAL,
  TOKEN_LESS,
  TOKEN_LESS_EQUAL,

  // Literals
  TOKEN_IDENTIFIER,
  TOKEN_STRING,
  TOKEN_NUMBER,

  // Keywords.
  TOKEN_BEGIN,
  TOKEN_BREAK,
  TOKEN_CASE,
  TOKEN_CLASS,
  TOKEN_DEF,
  TOKEN_DO,
  TOKEN_ELSE,
  TOKEN_END,
  TOKEN_ENSURE,
  TOKEN_FALSE,
  TOKEN_IF,
  TOKEN_NEXT,
  TOKEN_NIL,
  TOKEN_RESCUE,
  TOKEN_RETURN,
  TOKEN_SUPER,
  TOKEN_SWITCH,
  TOKEN_THIS,
  TOKEN_TRUE,
  TOKEN_UNLESS,
  TOKEN_UNTIL,
  TOKEN_WHILE,

  TOKEN_ERROR,
  TOKEN_EOF
} TokenType;

typedef struct
{
  TokenType type;
  char lexeme[LEXEME_LEN];
  int line;
  int col;
} Token;

/** @brief Translate a token enum id to a user friendly token name.
 *
 * This function returns the token name for a given token id. If the token
 * id is unknown it returns an error message.
 *
 * @param TokenType The token type value to get the name for.
 * @return The user friendly name of the token type supplied or an error string.
 */
const char *token_name(TokenType type);

/** @brief Display the token for debugging purposes.
 *
 * Prints a token and some of the information about the token to stdout.
 *
 * @param Token The token to be printed.
 */
void print_token(Token token);

#endif // TOKEN_H
