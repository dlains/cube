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

/** @struct Parser
 *
 * Holds the current and the previous token from the scanner. It
 * also holds information regarding the current error status and
 * if the compiler is in panic mode and recovering from a parse error.
 */
typedef struct {
  Token current;   /**< The current token from the input stream. */
  Token previous;  /**< The previous token from the input stream. */
  bool had_error;  /**< Was an error generated during compilation? */
  bool panic_mode; /**< Is the compiler in panic mode? */
} Parser;

Parser parser;           /**< Global Parser struct used by the compiler. */

Chunk *compiling_chunk;  /**< Global Chunk used by the compiler. */

/** @enum Precedence
 *
 * Set up precedence levels for the expression parser.
 */
typedef enum {
  PREC_NONE,        /**< No precedence level. */
  PREC_ASSIGNMENT,  /**< Assignment (=). */
  PREC_OR,          /**< Logical or. */
  PREC_AND,         /**< Logical and. */
  PREC_EQUALITY,    /**< Equal (==) and Not Equal (!=). */
  PREC_COMPARISON,  /**< Comparison operators (<, >, <=, >=). */
  PREC_TERM,        /**< Addition and Subtraction (+, -). */
  PREC_FACTOR,      /**< Multiplication, Division and Modulo (*, /, %). */
  PREC_POWER,       /**< Exponentiation (^). */
  PREC_UNARY,       /**< Unary operations (!, -, +). */
  PREC_CALL,        /**< Function, Method and Index calls. {., (), []) */
  PREC_PRIMARY,     /**< Highest level precedence, sentinal value. */
} Precedence;

/** @brief Define a function pointer to a parse function.
 *
 * The ParseRule structure holds function pointers of type
 * ParseFn to use for parsing the token input stream.
 */
typedef void (*ParseFn)();

/** @struct ParseRule
 *
 * Describes a single parse rule and functions to call for prefix
 * and infix versions of the parse rule.
 */
typedef struct {
  ParseFn prefix;        /**< Function to call for prefix parse of the token. */
  ParseFn infix;         /**< Function to call for infix parse of the token. */
  Precedence precedence; /**< The precedence of this parse rule. */
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

/** @brief Parse a literal value such as `true` or `false`.
 *
 * Look for literal values and convert them to the correct OpCode.
 */
static void literal();

/** @brief Parse a number as an integer.
 *
 * Parse an integer constant from the source code.
 */
static void integer();

/** @brief Parse a number as a real.
 *
 * Parse a real constant from the source code.
 */
static void real();

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

/** @brief The rules array holds all of the ParseRule structures.
 *
 * Holds the ParseRule structures that define what to do for a given
 * Token value. Each ParseRule index coincides with a TokenType value.
 * Look up the current token type value in the rules array to fine
 * the ParseFn for parsing that token.
 */
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
  { unary,    NULL,    PREC_NONE },       // TOKEN_BANG
  { NULL,     binary,  PREC_EQUALITY },   // TOKEN_BANG_EQUAL
  { NULL,     NULL,    PREC_NONE },       // TOKEN_EQUAL
  { NULL,     binary,  PREC_EQUALITY },   // TOKEN_EQUAL_EQUAL
  { NULL,     binary,  PREC_COMPARISON }, // TOKEN_GREATER
  { NULL,     binary,  PREC_COMPARISON }, // TOKEN_GREATER_EQUAL
  { NULL,     binary,  PREC_COMPARISON }, // TOKEN_LESS
  { NULL,     binary,  PREC_COMPARISON }, // TOKEN_LESS_EQUAL
  { NULL,     NULL,    PREC_NONE },       // TOKEN_IDENTIFIER
  { NULL,     NULL,    PREC_NONE },       // TOKEN_STRING
  { integer,  NULL,    PREC_NONE },       // TOKEN_INTEGER
  { real,     NULL,    PREC_NONE },       // TOKEN_REAL
  { NULL,     NULL,    PREC_NONE },       // TOKEN_BEGIN
  { NULL,     NULL,    PREC_NONE },       // TOKEN_BREAK
  { NULL,     NULL,    PREC_NONE },       // TOKEN_CASE
  { NULL,     NULL,    PREC_NONE },       // TOKEN_CLASS
  { NULL,     NULL,    PREC_NONE },       // TOKEN_DEF
  { NULL,     NULL,    PREC_NONE },       // TOKEN_DO
  { NULL,     NULL,    PREC_NONE },       // TOKEN_ELSE
  { NULL,     NULL,    PREC_NONE },       // TOKEN_END
  { NULL,     NULL,    PREC_NONE },       // TOKEN_ENSURE
  { literal,  NULL,    PREC_NONE },       // TOKEN_FALSE
  { NULL,     NULL,    PREC_NONE },       // TOKEN_IF
  { NULL,     NULL,    PREC_NONE },       // TOKEN_NEXT
  { literal,  NULL,    PREC_NONE },       // TOKEN_NIL
  { NULL,     NULL,    PREC_NONE },       // TOKEN_RESCUE
  { NULL,     NULL,    PREC_NONE },       // TOKEN_RETURN
  { NULL,     NULL,    PREC_NONE },       // TOKEN_SUPER
  { NULL,     NULL,    PREC_NONE },       // TOKEN_SWITCH
  { NULL,     NULL,    PREC_NONE },       // TOKEN_THIS
  { literal,  NULL,    PREC_NONE },       // TOKEN_TRUE
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

    if(parser.current.type == TOKEN_EOF)
    {
      if(source_buffers_remain())
        activate_next_buffer();
      else
      {
        remove_current_buffer();
        break;
      }
    }

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
    case TOKEN_BANG_EQUAL:
      emit_bytes(OP_EQUAL, OP_NOT);
      break;
    case TOKEN_EQUAL_EQUAL:
      emit_byte(OP_EQUAL);
      break;
    case TOKEN_GREATER:
      emit_byte(OP_GREATER);
      break;
    case TOKEN_GREATER_EQUAL:
      emit_bytes(OP_LESS, OP_NOT);
      break;
    case TOKEN_LESS:
      emit_byte(OP_LESS);
      break;
    case TOKEN_LESS_EQUAL:
      emit_bytes(OP_GREATER, OP_NOT);
      break;
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

/** @brief Parse a literal value such as `true` or `false`.
 *
 * Look for literal values and convert them to the correct OpCode.
 */
static void literal()
{
  switch(parser.previous.type)
  {
    case TOKEN_FALSE:
      emit_byte(OP_FALSE);
      break;
    case TOKEN_NIL:
      emit_byte(OP_NIL);
      break;
    case TOKEN_TRUE:
      emit_byte(OP_TRUE);
      break;
    default:
      return;
  }
}

/** @brief Parse an integer number.
 *
 * Parse an integer constant from the source code.
 */
static void integer()
{
  long value = strtol(parser.previous.lexeme, NULL, 10);
  emit_constant(INTEGER_VAL(value));
}

/** @brief Parse an real number.
 *
 * Parse a real constant from the source code.
 */
static void real()
{
  double value = strtod(parser.previous.lexeme, NULL);
  emit_constant(REAL_VAL(value));
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
    case TOKEN_BANG:
      emit_byte(OP_NOT);
      break;
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
