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

struct options {
  char *script;
  bool show_tokens;
  bool show_bytecode;
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
  o->show_tokens   = false;
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
  FREE(char, options->script);
  FREE(Options, options);
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
  while(true)
  {
    static struct option long_opts[] = {
      { "dump",    required_argument, 0, 'd'},
      { "help",    no_argument,       0, 'h'},
      { "version", no_argument,       0, 'v'},
      { 0,         0,                 0,  0 }
    };

    int opt_index = 0;
    int c = getopt_long(argc, argv, "d:hv", long_opts, &opt_index);

    if(c == -1)
      break;

    switch(c)
    {
      case 'd':
        if(strcmp(optarg, "tokens") == 0)
          options->show_tokens = true;
        else if(strcmp(optarg, "code") == 0)
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
  return options->script;
}

/** @brief Get the show tokens option.
 *
 * Get the show tokens option from the Options structure.
 *
 * @return The show tokens option.
 */
bool options_get_show_tokens(Options options)
{
  return options->show_tokens;
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
