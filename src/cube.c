/** @file cube.c
 *
 * @brief The entry point for Cube.
 *
 * This is the starting point for all Cube programs. This file sets up
 * the virtual machine, handles command line parsing, and starts a REPL or
 * prcesses the source files as necessary.
 *
 * @author David J. Lains (dlains)
 * @bug No known bugs.
 */
#include "config.h"
#include <stdio.h>
#include <stdlib.h>
#include "common.h"
#include "chunk.h"
#include "debug.h"
#include "vm.h"
#include "scanner.h"
#include "options.h"

#include "readline/readline.h"
#include "readline/history.h"

/** @brief Start a read / eval / print loop.
 *
 * The repl function is called if there is no supplied source file.
 * It reads input from a command line prompt, evaluates the source
 * code and then prints any results.
 *
 * This currently uses the GNU readline library to handle history and
 * editing. The Makefile is linking directly with the Homebrew readline
 * library on my machine. This needs to be made portable in some way.
 * There is a story for handling this issue here: [Link](https://www.pivotaltracker.com/story/show/159615558).
 *
 * @return The result of the source code interpretation.
 */
static int repl();

/** @brief Process the source code file at `file_path`.
 *
 * Add the file at `file_path` to the source list and
 * begin processing the source code.
 *
 * @return The result of the source code interpretation.
 */
static int run_file(const char *file_path);

/** @brief The main function for Cube.
 *
 * Read the command line options, initialize the virtual machine and then
 * process the source code as needed.
 *
 * @param argc The count of command line arguments.
 * @param argv The array of command line arguments.
 * @return The result of processing the Cube source code.
 */
int main(int argc, char *argv[])
{
  Options options = options_init();
  options_parse(options, argc, argv);

  int result = 0;

  vm_init(options);

  if(options_get_script(options) == NULL)
  {
    result = repl();
  }
  else
  {
    result = run_file(options_get_script(options));
  }

  options_free(options);
  vm_free();

  return result;
}

/** @brief Start a read / eval / print loop.
 *
 * The repl function is called if there is no supplied source file.
 * It reads input from a command line prompt, evaluates the source
 * code and then prints any results.
 *
 * This currently uses the GNU readline library to handle history and
 * editing. The Makefile is linking directly with the Homebrew readline
 * library on my machine. This needs to be made portable in some way.
 * There is a story for handling this issue here: [Link](https://www.pivotaltracker.com/story/show/159615558).
 *
 * @return The result of the source code interpretation.
 */
static int repl()
{
  InterpretResult result = INTERPRET_OK;

  static char *line = (char *)NULL;
  for(;;)
  {
    if(line)
    {
      free(line);
      line = (char *)NULL;
    }

    line = readline("> ");

    if(line && *line)
      add_history(line);

    add_source_line(line);
    result = interpret();
  }

  return result;
}

/** @brief Process the source code file at `file_path`.
 *
 * Add the file at `file_path` to the source list and
 * begin processing the source code.
 *
 * @return The result of the source code interpretation.
 */
static int run_file(const char *file_path)
{
  add_source_file(file_path);
  return interpret();
}
