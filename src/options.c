/** @file options.c
 *
 * @brief Implementation of the options module.
 *
 * @author David J. Lains (dlains)
 * @bug No known bugs.
 */
#include <getopt.h>
#include "common.h"
#include "memory.h"
#include "options.h"

/** @struct options
 *
 * Store any command line options parsed from the command line
 * for use later in the code.
 */
struct options {
  char *script;         /**< The source file provided on the command line. */
  bool show_bytecode;   /**< Set if the -d / --dump flag is given. */
};

/** @brief Show command line help.
 *
 * Prints a simple command line help message to stdout.
 */
static void print_usage(void);

/** @brief Show the program version string.
 *
 * Prints the Cube version string to stdout.
 */
static void print_version(void);

/** @brief Validate the options handle passed to many functions.
 *
 * Check that the Options handle is not null. There is no way
 * to recover from that error, so the program is ended.
 *
 * @param options The Options structure handle to validate.
 * @param file The current file name where the validate call is called.
 * @param line The current line number where validate is called.
 */
static void options_validate(Options options, const char *file, int line);

/** @def VALIDATE is used to simplify calling `options_validate` with the
 * current file and line number.
 */
#define VALIDATE(o) (options_validate((o), __FILE__, __LINE__))

/** @brief Initialize a new Options structure.
 *
 * Create a new Options structure with default values
 * and return it to the caller.
 *
 * @return A newly initialized Options structure.
 */
Options options_init(void)
{
  Options o        = NEW(o);
  o->script        = NULL;
  o->show_bytecode = false;

  return o;
}

/** @brief Relese the memory for the given options structure.
 *
 * Release any memory associated with the given options
 * structure.
 *
 * @param options The options structure to free.
 */
void options_free(Options options)
{
  VALIDATE(options);
  FREE(char, options->script);
  FREE(Options, options);
  options = NULL;
}

/** @brief Parse the supplied command line options.
 *
 * Get the command line options, parse them and fill
 * in the Options structure with the relevant options.
 *
 * @param argc The count of arguments to to program.
 * @param argv The array of argument values to parse.
 * @param options The Options structure to fill in.
 */
void options_parse(Options options, int argc, char *argv[])
{
  VALIDATE(options);
  while(true)
  {
    static struct option long_opts[] = {
      { "dump",    no_argument,       0, 'd'},
      { "help",    no_argument,       0, 'h'},
      { "version", no_argument,       0, 'v'},
      { 0,         0,                 0,  0 }
    };

    int opt_index = 0;
    int c = getopt_long(argc, argv, "dhv", long_opts, &opt_index);

    if(c == -1)
      break;

    switch(c)
    {
      case 'd':
        options->show_bytecode = true;
        break;
      case 'v':
        print_version();
        break;
      case 'h':
      case '?':
      default:
        print_usage();
    }
  }

  if(optind < argc)
  {
    // Copy a single argument at argv[optind] to options->script. Ignore anything
    // else on the command line.
    options->script = ALLOC(char, strlen(argv[optind] + 1));
    strcpy(options->script, argv[optind]);
  }
}

/** @brief Get the script file name supplied on the command line.
 *
 * Get the script file name that was supplied on the command
 * line if any. Returns NULL if there was no script file
 * given. In that case the REPL will be started.
 *
 * @param options The options structure to operate on.
 */
const char *options_get_script(Options options)
{
  VALIDATE(options);
  return options->script;
}

/** @brief Get the show code option.
 *
 * Get the show code option from the Options structure.
 *
 * @return The show code option.
 */
bool options_get_show_code(Options options)
{
  VALIDATE(options);
  return options->show_bytecode;
}

/** @brief Show command line help.
 *
 * Prints a simple command line help message to stdout.
 */
static void print_usage(void)
{
  printf("Usage: cube [arguments] [script]\n");
  printf("-d --dump      [tokens|code]              Dump token or bytecode information for the compiled code.\n");
  printf("-h --help                                 Show this help message.\n");
  printf("-v --version                              Show the program version string.\n");
  printf("\n");
  exit(EXIT_SUCCESS);
}

/** @brief Show the program version string.
 *
 * Prints the Cube version string to stdout.
 */
static void print_version(void)
{
  printf("Cube v0.1.0\n");
  exit(EXIT_SUCCESS);
}

/** @brief Validate the options handle passed to many functions.
 *
 * Check that the Options handle is not null. There is no way
 * to recover from that error, so the program is ended.
 *
 * @param options The Options structure handle to validate.
 */
static void options_validate(Options options, const char *file, int line)
{
  if(!options)
    die("Invalid Options handle used.", file, line);
}
