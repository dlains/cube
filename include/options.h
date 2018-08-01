/** @file options.h
 *
 * @brief Define command line options structures.
 *
 * The Options structure holds the command line options and
 * makes them available to the rest of the program.
 *
 * @author David J. Lains (dlains)
 * @bug No known bugs
 */
#ifndef OPTIONS_H
#define OPTIONS_H

typedef struct options *Options;

/** @brief Initialize a new Options structure.
 *
 * Create a new Options structure with default values
 * and return it to the caller.
 *
 * @return A newly initialized Options structure.
 */
Options options_init(void);

/** @brief Relese the memory for the given options structure.
 *
 * Release any memory associated with the given options
 * structure.
 *
 * @param options The options structure to free.
 */
void options_free(Options options);

/** @brief Parse the supplied command line options.
 *
 * Get the command line options, parse them and fill
 * in the Options structure with the relevant options.
 *
 * @param argc The count of arguments to to program.
 * @param argv The array of argument values to parse.
 * @param options The Options structure to fill in.
 */
void options_parse(Options options, int argc, char *argv[]);

/** @brief Get the script file name supplied on the command line.
 *
 * Get the script file name that was supplied on the command
 * line if any. Returns NULL if there was no script file
 * given. In that case the REPL will be started.
 *
 * @param options The options structure to operate on.
 */
const char *options_get_script(Options options);

/** @brief Get the show tokens option.
 *
 * Get the show tokens option from the Options structure.
 *
 * @return The show tokens option.
 */
bool options_get_show_tokens(Options options);

#endif // OPTIONS_H
