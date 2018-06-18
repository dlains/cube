#ifndef SCANNER_H
#define SCANNER_H

#include "common.h"
#include "token.h"

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

#endif // SCANNER_H
