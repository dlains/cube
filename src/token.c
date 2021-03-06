/** @file token.c
 *
 * @brief Implementation of the Token interface.
 *
 * Provide a way to display a Token in a user friendly way. Will be
 * useful for debugging purposes during the implementation of the
 * Cube language.
 *
 * @author David J. Lains
 * @bug No known bugs
 */
#include "config.h"
#include <stdio.h>
#include <string.h>
#include "common.h"
#include "memory.h"
#include "token.h"

/** @brief Create a new token.
 *
 * Create a token with the given TokenType and Lexeme.
 *
 * @param type The TokenType to assign to this Token.
 * @param lexeme The String that makes up the Token.
 * @param line The line number in the source where the token originated.
 * @param col The column number in the line where the token originated.
 * @return The newly created Token.
 */
Token token_create(TokenType type, String lexeme, int line, int col)
{
  Token token;

  token.type   = type;
  token.lexeme = lexeme;
  token.line   = line;
  token.col    = col;

  return token;
}

/** @brief Duplicate a token.
 *
 * Deep copy the source token to a new token.
 *
 * @param source The source token to copy.
 * @return The newly created duplicate token.
 */
Token token_duplicate(Token source)
{
  Token token;

  token.type   = source.type;
  token.line   = source.line;
  token.col    = source.col;

  if(source.lexeme != NULL)
    token.lexeme = string_copy(source.lexeme, strlen(source.lexeme));

  return token;
}

/** @brief Free the tokens resources.
 *
 * Check to see if lexeme is not null. If not, free the memory.
 *
 * @param token The token to free.
 */
void token_free(Token token)
{
  if(token.lexeme != NULL)
  {
    FREE(char, token.lexeme);
  }
}

/** @brief Translate a token enum id to a user friendly token name.
 *
 * This function returns the token name for a given token id. If the token
 * id is unknown it returns an error message.
 *
 * @param type The token type value to get the name for.
 * @return The user friendly name of the token type supplied or an error string.
 */
const char *token_name(TokenType type)
{
  switch(type)
  {
    case TOKEN_LEFT_PAREN:
      return "TOKEN_LEFT_PAREN";
    case TOKEN_RIGHT_PAREN:
      return "TOKEN_RIGHT_PAREN";
    case TOKEN_LEFT_BRACE:
      return "TOKEN_LEFT_BRACE";
    case TOKEN_RIGHT_BRACE:
      return "TOKEN_RIGHT_BRACE";
    case TOKEN_LEFT_BRACKET:
      return "TOKEN_LEFT_BRACKET";
    case TOKEN_RIGHT_BRACKET:
      return "TOKEN_RIGHT_BRACKET";
    case TOKEN_PERCENT:
      return "TOKEN_PERCENT";
    case TOKEN_COMMA:
      return "TOKEN_COMMA";
    case TOKEN_CARET:
      return "TOKEN_CARET";
    case TOKEN_DOT:
      return "TOKEN_DOT";
    case TOKEN_MINUS:
      return "TOKEN_MINUS";
    case TOKEN_PLUS:
      return "TOKEN_PLUS";
    case TOKEN_SEMICOLON:
      return "TOKEN_SEMICOLON";
    case TOKEN_SLASH:
      return "TOKEN_SLASH";
    case TOKEN_STAR:
      return "TOKEN_STAR";
    case TOKEN_AND:
      return "TOKEN_AND";
    case TOKEN_OR:
      return "TOKEN_OR";
    case TOKEN_BANG:
      return "TOKEN_BANG";
    case TOKEN_BANG_EQUAL:
      return "TOKEN_BANG_EQUAL";
    case TOKEN_EQUAL:
      return "TOKEN_EQUAL";
    case TOKEN_EQUAL_EQUAL:
      return "TOKEN_EQUAL_EQUAL";
    case TOKEN_GREATER:
      return "TOKEN_GREATER";
    case TOKEN_GREATER_EQUAL:
      return "TOKEN_GREATER_EQUAL";
    case TOKEN_LESS:
      return "TOKEN_LESS";
    case TOKEN_LESS_EQUAL:
      return "TOKEN_LESS_EQUAL";
    case TOKEN_IDENTIFIER:
      return "TOKEN_IDENTIFIER";
    case TOKEN_STRING:
      return "TOKEN_STRING";
    case TOKEN_INTEGER:
      return "TOKEN_INTEGER";
    case TOKEN_REAL:
      return "TOKEN_REAL";
    case TOKEN_BEGIN:
      return "TOKEN_BEGIN";
    case TOKEN_BREAK:
      return "TOKEN_BREAK";
    case TOKEN_CASE:
      return "TOKEN_CASE";
    case TOKEN_CLASS:
      return "TOKEN_CLASS";
    case TOKEN_DEF:
      return "TOKEN_DEF";
    case TOKEN_DO:
      return "TOKEN_DO";
    case TOKEN_ELSE:
      return "TOKEN_ELSE";
    case TOKEN_END:
      return "TOKEN_END";
    case TOKEN_ENSURE:
      return "TOKEN_ENSURE";
    case TOKEN_FALSE:
      return "TOKEN_FALSE";
    case TOKEN_IF:
      return "TOKEN_IF";
    case TOKEN_NEXT:
      return "TOKEN_NEXT";
    case TOKEN_NIL:
      return "TOKEN_NIL";
    case TOKEN_PRINT:
      return "TOKEN_PRINT";
    case TOKEN_RESCUE:
      return "TOKEN_RESCUE";
    case TOKEN_RETURN:
      return "TOKEN_RETURN";
    case TOKEN_SUPER:
      return "TOKEN_SUPER";
    case TOKEN_SWITCH:
      return "TOKEN_SWITCH";
    case TOKEN_THIS:
      return "TOKEN_THIS";
    case TOKEN_TRUE:
      return "TOKEN_TRUE";
    case TOKEN_UNLESS:
      return "TOKEN_UNLESS";
    case TOKEN_UNTIL:
      return "TOKEN_UNTIL";
    case TOKEN_VAR:
      return "TOKEN_VAR";
    case TOKEN_WHILE:
      return "TOKEN_WHILE";
    case TOKEN_ERROR:
      return "TOKEN_ERROR";
    case TOKEN_EOF:
      return "TOKEN_EOF";
    default:
      return "UNKNOWN TOKEN";
  }
}

/** @brief Display the token for debugging purposes.
 *
 * Prints a token and some of the information about the token to stdout.
 *
 * @param token The token to be printed.
 */
void print_token(Token token)
{
  printf("%4d:%-4d %-20s '%s'\n", token.line, token.col, token_name(token.type), token.lexeme);
}
