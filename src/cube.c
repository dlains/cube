#include "common.h"
#include "chunk.h"
#include "debug.h"
#include "vm.h"
#include "scanner.h"
#include "options.h"

#include "readline/readline.h"
#include "readline/history.h"

static int repl();
static int run_file(const char *file_path);

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

static int run_file(const char *file_path)
{
  add_source_file(file_path);
  return interpret();
}
