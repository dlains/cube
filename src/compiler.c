/** @file compiler.c
 *
 * @brief Implementation of the compiler module.
 *
 * @author David J. Lains (dlains)
 * @bug No known bugs.
 */
#include <stdio.h>
#include <stdlib.h>
#include "common.h"
#include "compiler.h"
#include "scanner.h"

#ifdef DEBUG_PRINT_CODE
#include "debug.h"
#endif

/** @struct Parser
 *
 * Holds the current and the previous token from the scanner.
 */
typedef struct {
  Token current;
  Token previous;
  bool had_error;
  bool panic_mode;
} Parser;

Parser parser;

Chunk *compiling_chunk;

typedef enum {
  PREC_NONE,
  PREC_ASSIGNMENT,  // =
  PREC_OR,          // or
  PREC_AND,         // and
  PREC_EQUALITY,    // == !=
  PREC_COMPARISON,  // < > <= >=
  PREC_TERM,        // + -
  PREC_FACTOR,      // * / %
  PREC_POWER,       // ^
  PREC_UNARY,       // ! - +
  PREC_CALL,        // . () []
  PREC_PRIMARY,
} Precedence;

typedef void (*ParseFn)();

typedef struct {
  ParseFn prefix;
  ParseFn infix;
  Precedence precedence;
} ParseRule;

/*
 * Parsing Functions
 */

/** @brief Get a parse rule from the rules table.
 *
 * Given a TokenType find the correct parse rule in the rules table.
 *
 * @param type The token type to look up.
 * @return The found parse rule.
 */
static ParseRule *get_rule(TokenType type);

/** @brief Advance the parser to the next token in the stream.
 *
 * Adjusts the Parser tokens and gets the next token in the
 * stream. Prints an error if TOKEN_ERROR is received.
 */
static void advance(void);

/** @brief Verify the next token and advance.
 *
 * Verify that the next token is the one expected and
 * advance if it is, otherwise show an error.
 *
 * @param type The token type to verify.
 * @param message The error message to display if the token is not verified.
 */
static void consume(TokenType type, const char *message);

/** @brief Parse a binary expression.
 *
 * Parse binary expressions and write the byte code to the Chunk array.
 */
static void binary();

/** @brief Parse a number.
 *
 * Parse a number constant from the source code.
 */
static void number();

/** @brief Parge a grouped expression.
 *
 * Parse an expression inside parentheses.
 */
static void grouping();

/** @brief Parse a unary negation expression.
 *
 * Parse a prefix negation expression.
 */
static void unary();

/** @brief Set the precedence level for parsing.
 *
 * Set the current precedence level to the given precedence.
 *
 * @param precedence The Precedence level to set.
 */
static void parse_precedence(Precedence precedence);

/*
 * Compiler Functions
 */

/** @brief Get the currently compiling Chunk.
 *
 * Get a reference to the current Chunk.
 *
 * @return The current Chunk.
 */
static Chunk *current_chunk();

/** @brief Create a byte code operation and add it to the Chunk array.
 *
 * Take the given byte and add it as a bytecode to the Chunk array.
 *
 * @param byte The byte code to add.
 */
static void emit_byte(Byte byte);

/** @brief Create two byte code operations and add them to the Chunk array.
 *
 * Add two byte code operations to the Chunk array.
 *
 * @param byte1 The first byte code to add.
 * @param byte2 The second byte code to add.
 */
static void emit_bytes(Byte byte1, Byte byte2);

/** @brief End compilation and emit a return operation.
 *
 * Finalizes the compilation process and emits a return
 * bytecode.
 */
static void end_compiler();

/** @brief Emit a number constant operation.
 *
 * Add the byte codes to access a number constant to the Chunk arra.
 *
 * @param value The number value to reference.
 */
static void emit_constant(Value value);

/** @brief Make a constant.
 *
 * Add a constant to the list of constants in the Chunk.
 *
 * @param value The constant value to make.
 * @return The index to the constant in the Chunk.
 */
static Byte make_constant(Value value);

/** @brief Emit a return byte code.
 *
 * Write a return operation to the Chunk array.
 */
static void emit_return();

/*
 * Error Handling Functions
 */

/** @brief Display an error message about the previous token.
 *
 * Use the parser previous token as the message for an error
 * report.
 *
 * @param token The token that caused the error.
 * @param message The error message to display.
 */
static void error(Token *token, const char *message);

/** @brief Compile the source code available in the scanner.
 *
 * Compile the source code available in the scanner.
 *
 * @param chunk Store the generated bytecode in the chunk array.
 * @return True if the compilation step had no errors, false otherwise.
 */
bool compile(Chunk *chunk)
{
  compiling_chunk = chunk;

  parser.had_error = false;
  parser.panic_mode = false;

  advance();
  expression();
  consume(TOKEN_EOF, "Expect end of expression.");
  /*
   * TODO: Move the buffer checking to the advance function.
  for(;;)
  {
    Token token = next_token();
    print_token(token);

    if(token.type == TOKEN_EOF)
    {
      if(source_buffers_remain())
        activate_next_buffer();
      else
      {
        remove_current_buffer();
        break;
      }
    }
  }
  */

  end_compiler();
  return !parser.had_error;
}

/** @brief Parse an expression.
 *
 * Parse a full expression and write the byte code to the Chunk array.
 */
void expression(void)
{
  parse_precedence(PREC_ASSIGNMENT);
}

ParseRule rules[] = {
  { grouping, NULL,    PREC_CALL },       // TOKEN_LEFT_PAREN
  { NULL,     NULL,    PREC_NONE },       // TOKEN_RIGHT_PAREN
  { NULL,     NULL,    PREC_NONE },       // TOKEN_LEFT_BRACE
  { NULL,     NULL,    PREC_NONE },       // TOKEN_RIGHT_BRACE
  { NULL,     NULL,    PREC_NONE },       // TOKEN_LEFT_BRACKET
  { NULL,     NULL,    PREC_NONE },       // TOKEN_RIGHT_BRACKET
  { NULL,     binary,  PREC_FACTOR },     // TOKEN_PERCENT
  { NULL,     NULL,    PREC_NONE },       // TOKEN_COMMA
  { NULL,     binary,  PREC_POWER },      // TOKEN_CARET
  { NULL,     NULL,    PREC_CALL },       // TOKEN_DOT
  { unary,    binary,  PREC_TERM },       // TOKEN_MINUS
  { NULL,     binary,  PREC_TERM },       // TOKEN_PLUS
  { NULL,     NULL,    PREC_NONE },       // TOKEN_SEMICOLON
  { NULL,     binary,  PREC_FACTOR },     // TOKEN_SLASH
  { NULL,     binary,  PREC_FACTOR },     // TOKEN_STAR
  { NULL,     NULL,    PREC_AND },        // TOKEN_AND
  { NULL,     NULL,    PREC_OR },         // TOKEN_OR
  { NULL,     NULL,    PREC_NONE },       // TOKEN_BANG
  { NULL,     NULL,    PREC_EQUALITY },   // TOKEN_BANG_EQUAL
  { NULL,     NULL,    PREC_NONE },       // TOKEN_EQUAL
  { NULL,     NULL,    PREC_EQUALITY },   // TOKEN_EQUAL_EQUAL
  { NULL,     NULL,    PREC_COMPARISON }, // TOKEN_GREATER
  { NULL,     NULL,    PREC_COMPARISON }, // TOKEN_GREATER_EQUAL
  { NULL,     NULL,    PREC_COMPARISON }, // TOKEN_LESS
  { NULL,     NULL,    PREC_COMPARISON }, // TOKEN_LESS_EQUAL
  { NULL,     NULL,    PREC_NONE },       // TOKEN_IDENTIFIER
  { NULL,     NULL,    PREC_NONE },       // TOKEN_STRING
  { number,   NULL,    PREC_NONE },       // TOKEN_NUMBER
  { NULL,     NULL,    PREC_NONE },       // TOKEN_BEGIN
  { NULL,     NULL,    PREC_NONE },       // TOKEN_BREAK
  { NULL,     NULL,    PREC_NONE },       // TOKEN_CASE
  { NULL,     NULL,    PREC_NONE },       // TOKEN_CLASS
  { NULL,     NULL,    PREC_NONE },       // TOKEN_DEF
  { NULL,     NULL,    PREC_NONE },       // TOKEN_DO
  { NULL,     NULL,    PREC_NONE },       // TOKEN_ELSE
  { NULL,     NULL,    PREC_NONE },       // TOKEN_END
  { NULL,     NULL,    PREC_NONE },       // TOKEN_ENSURE
  { NULL,     NULL,    PREC_NONE },       // TOKEN_FALSE
  { NULL,     NULL,    PREC_NONE },       // TOKEN_IF
  { NULL,     NULL,    PREC_NONE },       // TOKEN_NEXT
  { NULL,     NULL,    PREC_NONE },       // TOKEN_NIL
  { NULL,     NULL,    PREC_NONE },       // TOKEN_RESCUE
  { NULL,     NULL,    PREC_NONE },       // TOKEN_RETURN
  { NULL,     NULL,    PREC_NONE },       // TOKEN_SUPER
  { NULL,     NULL,    PREC_NONE },       // TOKEN_SWITCH
  { NULL,     NULL,    PREC_NONE },       // TOKEN_THIS
  { NULL,     NULL,    PREC_NONE },       // TOKEN_TRUE
  { NULL,     NULL,    PREC_NONE },       // TOKEN_UNLESS
  { NULL,     NULL,    PREC_NONE },       // TOKEN_UNTIL
  { NULL,     NULL,    PREC_NONE },       // TOKEN_WHILE
  { NULL,     NULL,    PREC_NONE },       // TOKEN_ERROR
  { NULL,     NULL,    PREC_NONE },       // TOKEN_EOF
};

/** @brief Get a parse rule from the rules table.
 *
 * Given a TokenType find the correct parse rule in the rules table.
 *
 * @param type The token type to look up.
 * @return The found parse rule.
 */
static ParseRule *get_rule(TokenType type)
{
  return &rules[type];
}

/** @brief Advance the parser to the next token in the stream.
 *
 * Adjusts the Parser tokens and gets the next token in the
 * stream. Prints an error if TOKEN_ERROR is received.
 */
static void advance(void)
{
  parser.previous = parser.current;

  for(;;)
  {
    parser.current = next_token();
    if(parser.current.type != TOKEN_ERROR)
      break;

    error(&parser.current, parser.current.lexeme);
  }
}

/** @brief Verify the next token and advance.
 *
 * Verify that the next token is the one expected and
 * advance if it is, otherwise show an error.
 *
 * @param type The token type to verify.
 * @param message The error message to display if the token is not verified.
 */
static void consume(TokenType type, const char *message)
{
  if(parser.current.type == type)
  {
    advance();
    return;
  }

  error(&parser.current, message);
}

/** @brief Parse a binary expression.
 *
 * Parse binary expressions and write the byte code to the Chunk array.
 */
static void binary()
{
  TokenType type = parser.previous.type;

  // Compile the right operand.
  ParseRule *rule = get_rule(type);
  parse_precedence((Precedence)(rule->precedence + 1));

  // Emit the operator instruction.
  switch(type)
  {
    case TOKEN_PLUS:
      emit_byte(OP_ADD);
      break;
    case TOKEN_MINUS:
      emit_byte(OP_SUBTRACT);
      break;
    case TOKEN_STAR:
      emit_byte(OP_MULTIPLY);
      break;
    case TOKEN_SLASH:
      emit_byte(OP_DIVIDE);
      break;
    case TOKEN_PERCENT:
      emit_byte(OP_MODULUS);
      break;
    case TOKEN_CARET:
      emit_byte(OP_POWER);
      break;
    default:
      return;
  }
}

/** @brief Parse a number.
 *
 * Parse a number constant from the source code.
 */
static void number()
{
  double value = strtod(parser.previous.lexeme, NULL);
  emit_constant(value);
}

/** @brief Parse a grouped expression.
 *
 * Parse an expression inside parentheses.
 */
static void grouping()
{
  expression();
  consume(TOKEN_RIGHT_PAREN, "Expect ')' after grouped expression.");
}

/** @brief Parse a unary negation expression.
 *
 * Parse a prefix negation expression.
 */
static void unary()
{
  TokenType type = parser.previous.type;

  parse_precedence(PREC_UNARY);

  switch(type)
  {
    case TOKEN_MINUS:
      emit_byte(OP_NEGATE);
      break;
    default:
      return;
  }
}

/** @brief Set the precedence level for parsing.
 *
 * Set the current precedence level to the given precedence.
 *
 * @param precedence The Precedence level to set.
 */
static void parse_precedence(Precedence precedence)
{
  advance();
  ParseFn prefix_rule = get_rule(parser.previous.type)->prefix;
  if(prefix_rule == NULL)
  {
    error(&parser.previous, "Expect expression.");
    return;
  }

  prefix_rule();

  while(precedence <= get_rule(parser.current.type)->precedence)
  {
    advance();
    ParseFn infix_rule = get_rule(parser.previous.type)->infix;
    infix_rule();
  }
}

/** @brief Get the currently compiling Chunk.
 *
 * Get a reference to the current Chunk.
 *
 * @return The current Chunk.
 */
static Chunk *current_chunk()
{
  return compiling_chunk;
}

/** @brief Create a byte code operation and add it to the Chunk array.
 *
 * Take the given byte and add it as a bytecode to the Chunk array.
 *
 * @param byte The byte code to add.
 */
static void emit_byte(Byte byte)
{
  write_chunk(current_chunk(), byte, parser.previous.line);
}

/** @brief Create two byte code operations and add them to the Chunk array.
 *
 * Add two byte code operations to the Chunk array.
 *
 * @param byte1 The first byte code to add.
 * @param byte2 The second byte code to add.
 */
static void emit_bytes(Byte byte1, Byte byte2)
{
  emit_byte(byte1);
  emit_byte(byte2);
}

/** @brief End compilation and emit a return operation.
 *
 * Finalizes the compilation process and emits a return
 * bytecode.
 */
static void end_compiler()
{
  emit_return();

#ifdef DEBUG_PRINT_CODE
  if(!parser.had_error)
  {
    disassembleChunk(current_chunk(), "code");
  }
#endif
}

/** @brief Emit a number constant operation.
 *
 * Add the byte codes to access a number constant to the Chunk arra.
 *
 * @param value The number value to reference.
 */
static void emit_constant(Value value)
{
  emit_bytes(OP_CONSTANT, make_constant(value));
}

/** @brief Make a constant.
 *
 * Add a constant to the list of constants in the Chunk.
 *
 * @param value The constant value to make.
 * @return The index to the constant in the Chunk.
 */
static Byte make_constant(Value value)
{
  int index = add_constant(current_chunk(), value);
  if(index > UINT8_MAX)
  {
    error(&parser.previous, "Too many constants in one chunk.");
    return 0;
  }

  return (Byte)index;
}

/** @brief Emit a return byte code.
 *
 * Write a return operation to the Chunk array.
 */
static void emit_return()
{
  emit_byte(OP_RETURN);
}

/** @brief Wrapper for the parse_error function. 
 *
 * Send the token and error message along to parse_error and
 * set the Parser error recovery flags appropriately.
 *
 * @param token The token that caused the error.
 * @param message The error message to display.
 */
static void error(Token *token, const char *message)
{
  if(parser.panic_mode)
    return;
  parser.panic_mode = true;
  parser.had_error  = true;

  parse_error(token, message);
}
