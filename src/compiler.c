/** @file compiler.c
 *
 * @brief Implementation of the compiler module.
 *
 * @author David J. Lains (dlains)
 * @bug No known bugs.
 */
#include "config.h"
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include "common.h"
#include "object.h"
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
typedef void (*ParseFn)(bool can_assign);

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

/** @struct Local
 *
 * Data for a single local variable.
 */
typedef struct {
  Token name;            /**< The local variable name. */
  int depth;             /**< The scope depth this variable is in. */
} Local;

/** @struct Compiler
 *
 * Data that the compiler needs to store in order for the VM to be
 * able to access it later.
 */
typedef struct Compiler {
  Local locals[LOCALS_MAX]; /**< Array of Local structs to hold the local variables. */
  int local_count;          /**< The current count of local variables. */
  int scope_depth;          /**< The current scope depth. Zero is the global scope. */
} Compiler;

Compiler *current = NULL;   /** Global Compiler struct to hold data for the VM to use. */

/** @brief Initialize the compiler data.
 *
 * Set the Compiler data structure fields to their initial values.
 *
 * @param compiler The pointer to the Compiler structure to initialize.
 */
static void init_compiler(Compiler *compiler);

/** @brief Begin a new environment scope.
 *
 * Increase the scope depth in the compiler because we have entered a block.
 */
static void begin_scope(void);

/** @brief End the current environment scope.
 *
 * Decrease the scope depth in the compiler because we have exited a block.
 */
static void end_scope(void);

/** @brief Check if two identifiers are equivalent.
 *
 * Check if the two tokens are the same identifier.
 *
 * @param a The first Token to check.
 * @param b The second Token to check.
 * @return True if the tokens are equal, false otherwise.
 */
static bool identifiers_equal(Token *a, Token *b);

/** @brief Find the specified local variable.
 *
 * Look through the local variables stored in the compiler and return the index
 * of the variable in the locals array if found.
 *
 * @param compiler A pointer to the compiler data struct.
 * @param name The name of the local variable to find.
 * @param in_function Is the current parsing step inside a function.
 * @return The index of the found local variable or -1 if it was not found.
 */
static int resolve_local(Compiler *compiler, Token *name, bool in_function);

/** @brief Add a local variable to the comiler data.
 *
 * Add a local variable to the locals array.
 *
 * @param name The Token containing the variable identifier.
 */
static void add_local(Token name);

/** @brief Declare a new local variable.
 *
 * Declare a new local variable as long as we are not at the global scope
 * and the identifier does not already exist.
 */
static void declare_variable(void);

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

/** @brief Check the token type.
 *
 * Check to see if the current token type matches the supplied token type.
 *
 * @param type The TokenType to check for.
 * @return True if the parsers current token type matches the type, false otherwise.
 */
static bool check(TokenType type);

/** @brief Match and advance the parser if the TokenType matches.
 *
 * Check the parsers current token type against the supplied token type. If they
 * match advance to the next token.
 *
 * @param type The TokenType to check against.
 * @return True if the token types match, false otherwise.
 */
static bool match(TokenType type);

/** @brief Parse an and expression.
 *
 * Parse the and expression and emit a JUMP if necessary.
 */
static void and_(bool can_assign);

/** @brief Parse an or expression.
 *
 * Parse the or expression and emit a JUMP if necessary.
 */
static void or_(bool can_assign);

/** @brief Parse a binary expression.
 *
 * Parse binary expressions and write the byte code to the Chunk array.
 */
static void binary(bool can_assign);

/** @brief Parse a literal value such as `true` or `false`.
 *
 * Look for literal values and convert them to the correct OpCode.
 *
 * @param can_assign
 */
static void literal(bool can_assign);

/** @brief Parse a number as an integer.
 *
 * Parse an integer constant from the source code.
 *
 * @param can_assign
 */
static void integer(bool can_assign);

/** @brief Parse a number as a real.
 *
 * Parse a real constant from the source code.
 *
 * @param can_assign
 */
static void real(bool can_assign);

/** @brief Parse a string object.
 *
 * Parse a string literal into a ObjectString.
 *
 * @param can_assign
 */
static void string(bool can_assign);

/** @brief Parge a grouped expression.
 *
 * Parse an expression inside parentheses.
 *
 * @param can_assign
 */
static void grouping(bool can_assign);

/** @brief Parse a unary negation expression.
 *
 * Parse a prefix negation expression.
 *
 * @param can_assign
 */
static void unary(bool can_assign);

/** @brief Parse a statement.
 *
 * Parse a high level statement.
 */
static void statement();

/** @brief Parse a declaration.
 *
 * Parse a variable declaration.
 */
static void declaration();

/** @brief Parse a brace block.
 *
 * Parse a { ... } block.
 */
static void brace_block();

/** @brief Parse a block.
 *
 * Parse a do...end or a {..} block.
 */
static void do_block();

/** @brief Parse a variable.
 *
 * Parse a variable.
 *
 * @param can_assign
 */
static void variable(bool can_assign);

/** @brief Parse a variable declaration.
 *
 * Parse a variable declaration.
 */
static void var_declaration();

/** @brief Parse an expression statement.
 *
 * Parse an expression statement.
 */
static void expression_statement();

/** @brief Parse an if statement.
 *
 * Parse and create the operations for an if statement.
 */
static void if_statement();

/** @brief Parse a while statement.
 *
 * Parse and create the operations for a while statement.
 */
static void while_statement();

/** @brief Parse a print statement.
 *
 * Parse a print statement.
 */
static void print_statement();

/** @brief Set the precedence level for parsing.
 *
 * Set the current precedence level to the given precedence.
 *
 * @param precedence The Precedence level to set.
 */
static void parse_precedence(Precedence precedence);

/** @brief Parse a variable.
 *
 * Parse a variable and create an identifier.
 *
 * @param error_message The error message to display if the parse fails.
 * @return The index of the constant for the identifier.
 */
static Byte parse_variable(const char *error_message);

/** @brief Define a variable.
 *
 * Define a global variable and emit the necessary op codes.
 *
 * @param global The constant index to the global identifier.
 */
static void define_variable(Byte global);

/** @brief Compile a reference to a variable.
 *
 * Compile a reference to a variable.
 *
 * @param name The variable name.
 * @param can_assign
 */
static void named_variable(Token name, bool can_assign);

/** @brief Create a constant for the identifier token.
 *
 * Create a constant for the identifier and return the index
 * to the constant.
 *
 * @param token The identifier token to turn into a constant.
 * @return The constant index for the identifier.
 */
static Byte identifier_constant(Token *token);

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

/** @brief Create a loop operation.
 *
 * Loops jump backwards in the Chunk array of operation codes.
 * The offset to loop to is stored as two consecutive byte values.
 *
 * @param loop_start The offset of the start of the loop.
 */
static void emit_loop(int loop_start);

/** @brief Create a jump operation.
 *
 * Adds the instruction followed by a placeholder for the
 * jump offset. The placeholder can be patched by calling
 * jump_patch.
 *
 * @param instruction The bytecode corresponding to one of the Jump instructions.
 * @return The offset of the jump placeholder.
 */
static int emit_jump(Byte instruction);

/** @brief Patch a jump offset.
 *
 * Replace the placeholder for a previous JUMP instruction with the
 * correct offset.
 *
 * @param offset The offset to patch into the jump instruction.
 */
static void patch_jump(int offset);

/** @brief End compilation and emit a return operation.
 *
 * Finalizes the compilation process and emits a return
 * bytecode.
 */
static void end_compiler();

/** @brief Emit an object constant operation.
 *
 * Add the byte codes to access an object to the Chunk array.
 *
 * @param object The object constant to reference.
 */
static void emit_constant(Object *object);

/** @brief Make a new object.
 *
 * Add an object to the list of constants in the Chunk.
 *
 * @param object The object to make.
 * @return The index to the object in the Chunk.
 */
static Byte make_constant(Object *object);

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

/** @brief Synchronize the compiler after an error.
 *
 * Clears out the parser panic mode and moves to the next
 * viable expression or statement.
 */
static void synchronize();

/** @brief Compile the source code available in the scanner.
 *
 * Compile the source code available in the scanner.
 *
 * @param chunk Store the generated bytecode in the chunk array.
 * @return True if the compilation step had no errors, false otherwise.
 */
bool compile(Chunk *chunk)
{
  Compiler main_compiler;
  init_compiler(&main_compiler);

  compiling_chunk = chunk;

  parser.had_error = false;
  parser.panic_mode = false;

  advance();

  if(!match(TOKEN_EOF))
  {
    do {
      declaration();
    } while(!check(TOKEN_EOF));
  }

  end_compiler();
  return !parser.had_error;
}

/** @brief Initialize the compiler data.
 *
 * Set the Compiler data structure fields to their initial values.
 *
 * @param compiler The pointer to the Compiler structure to initialize.
 */
static void init_compiler(Compiler *compiler)
{
  compiler->local_count = 0;
  compiler->scope_depth = 0;

  current = compiler;
}

/** @brief Begin a new environment scope.
 *
 * Increase the scope depth in the compiler because we have entered a block.
 */
static void begin_scope(void)
{
  current->scope_depth++;
}

/** @brief End the current environment scope.
 *
 * Decrease the scope depth in the compiler because we have exited a block.
 */
static void end_scope(void)
{
  current->scope_depth--;

  // Remove local variables from the stack.
  while(current->local_count > 0 && current->locals[current->local_count - 1].depth > current->scope_depth)
  {
    emit_byte(OP_POP);
    token_free(current->locals[current->local_count - 1].name);
    current->local_count--;
  }
}

/** @brief Check if two identifiers are equivalent.
 *
 * Check if the two tokens are the same identifier.
 *
 * @param a The first Token to check.
 * @param b The second Token to check.
 * @return True if the tokens are equal, false otherwise.
 */
static bool identifiers_equal(Token *a, Token *b)
{
  if(a->type != b->type)
    return false;

  return strcmp(a->lexeme, b->lexeme) == 0;
}

/** @brief Find the specified local variable.
 *
 * Look through the local variables stored in the compiler and return the index
 * of the variable in the locals array if found.
 *
 * @param compiler A pointer to the compiler data struct.
 * @param name The name of the local variable to find.
 * @param in_function Is the current parsing step inside a function.
 * @return The index of the found local variable or -1 if it was not found.
 */
static int resolve_local(Compiler *compiler, Token *name, bool in_function)
{
  for(int i = compiler->local_count - 1; i >= 0; i--)
  {
    Local *local = &compiler->locals[i];
    if(identifiers_equal(name, &local->name))
    {
      if(!in_function && local->depth == -1)
      {
        error(name, "Connot read local variable in its own initializer.");
      }
      return i;
    }
  }

  return -1;
}

/** @brief Add a local variable to the comiler data.
 *
 * Add a local variable to the locals array.
 *
 * @param name The Token containing the variable identifier.
 */
static void add_local(Token name)
{
  if(current->local_count == LOCALS_MAX)
  {
    error(&name, "Too many local variables in function.");
    return;
  }

  Local *local = &current->locals[current->local_count];
  local->name = token_duplicate(name);
  local->depth = -1;

  current->local_count++;
}

/** @brief Declare a new local variable.
 *
 * Declare a new local variable as long as we are not at the global scope
 * and the identifier does not already exist.
 */
static void declare_variable(void)
{
  if(current->scope_depth == 0)
    return;

  Token name = parser.previous;
  for(int i = current->local_count - 1; i >= 0; i--)
  {
    Local *local = &current->locals[i];
    if(local->depth != -1 && local->depth < current->scope_depth)
      break;
    if(identifiers_equal(&name, &local->name))
    {
      error(&name, "Variable with this name already declared in this scope.");
    }
  }

  add_local(name);
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
  { NULL,     and_,    PREC_AND },        // TOKEN_AND
  { NULL,     or_,     PREC_OR },         // TOKEN_OR
  { unary,    NULL,    PREC_NONE },       // TOKEN_BANG
  { NULL,     binary,  PREC_EQUALITY },   // TOKEN_BANG_EQUAL
  { NULL,     NULL,    PREC_NONE },       // TOKEN_EQUAL
  { NULL,     binary,  PREC_EQUALITY },   // TOKEN_EQUAL_EQUAL
  { NULL,     binary,  PREC_COMPARISON }, // TOKEN_GREATER
  { NULL,     binary,  PREC_COMPARISON }, // TOKEN_GREATER_EQUAL
  { NULL,     binary,  PREC_COMPARISON }, // TOKEN_LESS
  { NULL,     binary,  PREC_COMPARISON }, // TOKEN_LESS_EQUAL
  { variable, NULL,    PREC_NONE },       // TOKEN_IDENTIFIER
  { string,   NULL,    PREC_NONE },       // TOKEN_STRING
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
  { NULL,     NULL,    PREC_NONE },       // TOKEN_PRINT
  { NULL,     NULL,    PREC_NONE },       // TOKEN_SUPER
  { NULL,     NULL,    PREC_NONE },       // TOKEN_SWITCH
  { NULL,     NULL,    PREC_NONE },       // TOKEN_THIS
  { literal,  NULL,    PREC_NONE },       // TOKEN_TRUE
  { NULL,     NULL,    PREC_NONE },       // TOKEN_UNLESS
  { NULL,     NULL,    PREC_NONE },       // TOKEN_UNTIL
  { NULL,     NULL,    PREC_NONE },       // TOKEN_VAR
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
  token_free(parser.previous);
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

/** @brief Check the token type.
 *
 * Check to see if the current token type matches the supplied token type.
 *
 * @param type The TokenType to check for.
 * @return True if the parsers current token type matches the type, false otherwise.
 */
static bool check(TokenType type)
{
  return parser.current.type == type;
}

/** @brief Match and advance the parser if the TokenType matches.
 *
 * Check the parsers current token type against the supplied token type. If they
 * match advance to the next token.
 *
 * @param type The TokenType to check against.
 * @return True if the token types match, false otherwise.
 */
static bool match(TokenType type)
{
  if(!check(type))
    return false;

  advance();
  return true;
}

/** @brief Parse an and expression.
 *
 * Parse the and expression and emit a JUMP if necessary.
 */
static void and_(bool can_assign)
{
  // Prevent unused parameter error. Not all ParseFn functions actually use the parameter.
  (void)can_assign;

  // Short circuit if the left operand is false.
  int end_jump = emit_jump(OP_JUMP_IF_FALSE);

  emit_byte(OP_POP);
  parse_precedence(PREC_AND);

  patch_jump(end_jump);
}

/** @brief Parse an or expression.
 *
 * Parse the or expression and emit a JUMP if necessary.
 */
static void or_(bool can_assign)
{
  // Prevent unused parameter error. Not all ParseFn functions actually use the parameter.
  (void)can_assign;

  // If the operand is true we want to keep it, so when it's false
  // jump to the code to evaluate the right operand.
  int else_jump = emit_jump(OP_JUMP_IF_FALSE);

  // If we get here, the operand is true, so jump to the end to keep it.
  int end_jump = emit_jump(OP_JUMP);

  patch_jump(else_jump);
  emit_byte(OP_POP);

  parse_precedence(PREC_OR);
  patch_jump(end_jump);
}

/** @brief Parse a binary expression.
 *
 * Parse binary expressions and write the byte code to the Chunk array.
 *
 * @param can_assign Can the result be assigned to a variable.
 */
static void binary(bool can_assign)
{
  // Prevent unused parameter error. Not all ParseFn functions actually use the parameter.
  (void)can_assign;

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
 *
 * @param can_assign
 */
static void literal(bool can_assign)
{
  // Prevent unused parameter error. Not all ParseFn functions actually use the parameter.
  (void)can_assign;

  switch(parser.previous.type)
  {
    case TOKEN_FALSE:
      emit_constant(AS_OBJECT(create_boolean(false)));
      break;
    case TOKEN_NIL:
      emit_constant(AS_OBJECT(create_nil()));
      break;
    case TOKEN_TRUE:
      emit_constant(AS_OBJECT(create_boolean(true)));
      break;
    default:
      return;
  }
}

/** @brief Parse an integer number.
 *
 * Parse an integer constant from the source code.
 *
 * @param can_assign
 */
static void integer(bool can_assign)
{
  // Prevent unused parameter error. Not all ParseFn functions actually use the parameter.
  (void)can_assign;

  long value = strtol(parser.previous.lexeme, NULL, 10);
  emit_constant(AS_OBJECT(create_integer(value)));
}

/** @brief Parse an real number.
 *
 * Parse a real constant from the source code.
 *
 * @param can_assign
 */
static void real(bool can_assign)
{
  // Prevent unused parameter error. Not all ParseFn functions actually use the parameter.
  (void)can_assign;

  double value = strtod(parser.previous.lexeme, NULL);
  emit_constant(AS_OBJECT(create_real(value)));
}

/** @brief Parse a string object.
 *
 * Parse a string literal into a ObjectString.
 *
 * @param can_assign
 */
static void string(bool can_assign)
{
  // Prevent unused parameter error. Not all ParseFn functions actually use the parameter.
  (void)can_assign;

  emit_constant(AS_OBJECT(copy_string(parser.previous.lexeme, strlen(parser.previous.lexeme))));
}

/** @brief Parse a grouped expression.
 *
 * Parse an expression inside parentheses.
 *
 * @param can_assign
 */
static void grouping(bool can_assign)
{
  // Prevent unused parameter error. Not all ParseFn functions actually use the parameter.
  (void)can_assign;

  expression();
  consume(TOKEN_RIGHT_PAREN, "Expect ')' after grouped expression.");
}

/** @brief Parse a unary negation expression.
 *
 * Parse a prefix negation expression.
 *
 * @param can_assign
 */
static void unary(bool can_assign)
{
  // Prevent unused parameter error. Not all ParseFn functions actually use the parameter.
  (void)can_assign;

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

/** @brief Parse a statement.
 *
 * Parse a high level statement.
 */
static void statement()
{
  if(match(TOKEN_IF))
  {
    if_statement();
  }
  else if(match(TOKEN_PRINT))
  {
    print_statement();
  }
  else if(match(TOKEN_WHILE))
  {
    while_statement();
  }
  else if(match(TOKEN_LEFT_BRACE))
  {
    begin_scope();
    brace_block();
    end_scope();
  }
  else if(match(TOKEN_DO))
  {
    begin_scope();
    do_block();
    end_scope();
  }
  else
  {
    expression_statement();
  }
}

/** @brief Parse a declaration.
 *
 * Parse a variable declaration.
 */
static void declaration()
{
  if(match(TOKEN_VAR))
  {
    var_declaration();
  }
  else
  {
    statement();
  }

  if(parser.panic_mode)
    synchronize();
}

/** @brief Parse a brace block.
 *
 * Parse a { ... } block.
 */
static void brace_block()
{
  while(!check(TOKEN_RIGHT_BRACE) && !check(TOKEN_EOF))
  {
    declaration();
  }

  consume(TOKEN_RIGHT_BRACE, "Expect '}' after block.");
}

/** @brief Parse a do block.
 *
 * Parse a do...end block.
 */
static void do_block()
{
  while(!check(TOKEN_END) && !check(TOKEN_EOF))
  {
    declaration();
  }

  consume(TOKEN_END, "Expect 'end' after do block.");
}

/** @brief Parse a variable.
 *
 * Parse a variable.
 *
 * @param can_assign
 */
static void variable(bool can_assign)
{
  named_variable(parser.previous, can_assign);
}

/** @brief Parse a variable declaration.
 *
 * Parse a variable declaration.
 */
static void var_declaration()
{
  Byte global = parse_variable("Expect variable name.");

  if(match(TOKEN_EQUAL))
  {
    // Compile the initializer.
    expression();
  }
  else
  {
    // Default to nil.
    emit_byte(OP_NIL);
  }

  consume(TOKEN_SEMICOLON, "Expect ';' after variable declaration.");

  define_variable(global);
}

/** @brief Parse an expression statement.
 *
 * Parse an expression statement.
 */
static void expression_statement()
{
  expression();
  emit_byte(OP_POP);
  consume(TOKEN_SEMICOLON, "Expect ';' after expression.");
}

/** @brief Parse an if statement.
 *
 * Parse and create the operations for an if statement.
 */
static void if_statement()
{
  consume(TOKEN_LEFT_PAREN, "Expect '(' after 'if'.");
  expression();
  consume(TOKEN_RIGHT_PAREN, "Expect ')' after condition.");

  // Jump to the else branch of the condition is false.
  int else_jump = emit_jump(OP_JUMP_IF_FALSE);

  // Compile the then branch.
  emit_byte(OP_POP);
  statement();

  // Jump over the else branch when the if branch is taken.
  int end_jump = emit_jump(OP_JUMP);

  // Compile the else branch.
  patch_jump(else_jump);
  emit_byte(OP_POP);

  if(match(TOKEN_ELSE))
    statement();

  patch_jump(end_jump);
}

/** @brief Parse a while statement.
 *
 * Parse and create the operations for a while statement.
 */
static void while_statement()
{
  int loop_start = current_chunk()->count;

  consume(TOKEN_LEFT_PAREN, "Expect '(' after 'while'.");
  expression();
  consume(TOKEN_RIGHT_PAREN, "Expect ')' after condition.");

  // Jump out of the loop if the condition is false.
  int exit_jump = emit_jump(OP_JUMP_IF_FALSE);

  // Compile the body.
  emit_byte(OP_POP);
  statement();

  // Loop back to start.
  emit_loop(loop_start);

  patch_jump(exit_jump);
  emit_byte(OP_POP);
}

/** @brief Parse a print statement.
 *
 * Parse a print statement.
 */
static void print_statement()
{
  expression();
  consume(TOKEN_SEMICOLON, "Expect ';' after value.");
  emit_byte(OP_PRINT);
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

  bool can_assign = precedence <= PREC_ASSIGNMENT;
  prefix_rule(can_assign);

  while(precedence <= get_rule(parser.current.type)->precedence)
  {
    advance();
    ParseFn infix_rule = get_rule(parser.previous.type)->infix;
    infix_rule(can_assign);
  }

  if(can_assign && match(TOKEN_EQUAL))
  {
    // If we get here, we didn't parse the '=' even though we
    // could have, so the LHS must not be a valid lvalue.
    error(&parser.previous, "Invalid assignment target.");
    expression();
  }
}

/** @brief Parse a variable.
 *
 * Parse a variable and create an identifier.
 *
 * @param error_message The error message to display if the parse fails.
 * @return The index of the constant for the identifier.
 */
static Byte parse_variable(const char *error_message)
{
  consume(TOKEN_IDENTIFIER, error_message);

  if(current->scope_depth == 0)
    return identifier_constant(&parser.previous);

  declare_variable();

  return 0;
}

/** @brief Define a variable.
 *
 * Define a global variable and emit the necessary op codes.
 *
 * @param global The constant index to the global identifier.
 */
static void define_variable(Byte global)
{
  if(current->scope_depth == 0)
    emit_bytes(OP_DEFINE_GLOBAL, global);
  else
    current->locals[current->local_count - 1].depth = current->scope_depth;
}

/** @brief Compile a reference to a variable.
 *
 * Compile a reference to a variable.
 *
 * @param name The variable name.
 * @param can_assign
 */
static void named_variable(Token name, bool can_assign)
{
  Byte get_op, set_op;
  int arg = resolve_local(current, &name, false);
  if(arg != -1)
  {
    get_op = OP_GET_LOCAL;
    set_op = OP_SET_LOCAL;
  }
  else
  {
    arg = identifier_constant(&name);
    get_op = OP_GET_GLOBAL;
    set_op = OP_SET_GLOBAL;
  }

  if(can_assign && match(TOKEN_EQUAL))
  {
    expression();
    emit_bytes(set_op, arg);
  }
  else
  {
    emit_bytes(get_op, arg);
  }
}

/** @brief Create a constant for the identifier token.
 *
 * Create a constant for the identifier and return the index
 * to the constant.
 *
 * @param token The identifier token to turn into a constant.
 * @return The constant index for the identifier.
 */
static Byte identifier_constant(Token *token)
{
  return make_constant(AS_OBJECT(copy_string(token->lexeme, strlen(token->lexeme))));
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

/** @brief Create a loop operation.
 *
 * Loops jump backwards in the Chunk array of operation codes.
 * The offset to loop to is stored as two consecutive byte values.
 *
 * @param loop_start The offset of the start of the loop.
 */
static void emit_loop(int loop_start)
{
  emit_byte(OP_LOOP);

  int offset = current_chunk()->count - loop_start + 2;
  if(offset > UINT16_MAX)
    error(&parser.current, "Loop body too large");

  emit_byte((offset >> 8) & 0xff);
  emit_byte(offset & 0xff);
}

/** @brief Create a jump operation.
 *
 * Adds the instruction followed by a placeholder for the
 * jump offset. The placeholder can be patched by calling
 * jump_patch.
 *
 * @param instruction The bytecode corresponding to one of the Jump instructions.
 * @return The offset of the jump placeholder.
 */
static int emit_jump(Byte instruction)
{
  emit_byte(instruction);
  emit_byte(0xff);
  emit_byte(0xff);

  return current_chunk()->count - 2;
}

/** @brief Patch a jump offset.
 *
 * Replace the placeholder for a previous JUMP instruction with the
 * correct offset.
 *
 * @param offset The offset to patch into the jump instruction.
 */
static void patch_jump(int offset)
{
  // Subtract 2 to adjust for the bytecode for the jump offset itself.
  int jump = current_chunk()->count - offset - 2;

  if(jump > UINT16_MAX)
    error(&parser.current, "Too much code to jump over.");

  current_chunk()->code[offset] = (jump >> 8) & 0xff;
  current_chunk()->code[offset + 1] = jump & 0xff;
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

/** @brief Emit an object constant operation.
 *
 * Add the byte codes to access an object to the Chunk array.
 *
 * @param object The object constant to reference.
 */
static void emit_constant(Object *object)
{
  emit_bytes(OP_CONSTANT, make_constant(object));
}

/** @brief Make a new object.
 *
 * Add an object to the list of constants in the Chunk.
 *
 * @param object The object to make.
 * @return The index to the object in the Chunk.
 */
static Byte make_constant(Object *object)
{
  int index = add_constant(current_chunk(), object);
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

/** @brief Synchronize the compiler after an error.
 *
 * Clears out the parser panic mode and moves to the next
 * viable expression or statement.
 */
static void synchronize()
{
  parser.panic_mode = false;

  while(parser.current.type != TOKEN_EOF)
  {
    if(parser.previous.type == TOKEN_SEMICOLON)
      return;

    switch(parser.current.type)
    {
      case TOKEN_CLASS:
      case TOKEN_DEF:
      case TOKEN_IF:
      case TOKEN_WHILE:
      case TOKEN_RETURN:
        return;
      default:
        // Do nothing
        ;
    }

    advance();
  }
}
