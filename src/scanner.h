#ifndef SCANNER_H
#define SCANNER_H

#include "common.h"

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
  TOKEN_AND,
  TOKEN_OR,

  // One or two character tokens.
  TOKEN_STAR,
  TOKEN_POWER,
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
  TOKEN_IMPORT,
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
  const char *start;
  int length;
  int line;
} Token;

// Set the source code for the scanner to tokenize.
void initScanner(const char *source);
// Get the next token in the stream.
Token nextToken();

// Private scanner methods.

static Token makeToken(TokenType type);
static Token errorToken(const char *message);
static bool isAtEnd();
static char advance();
static bool match(char expected);
static char peek();
static char peekNext();
static void skipWhitespace();
static Token string();
static Token number();
static Token identifier();
static bool isAlpha(char c);
static bool isDigit(char c);
static TokenType identifierType();
static TokenType checkKeyword(int start, int length, const char *rest, TokenType type);

#endif // SCANNER_H
