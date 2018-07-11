#include "common.h"
#include "chunk.h"
#include "debug.h"
#include "vm.h"
#include "scanner.h"

static int repl();
static int run_file(const char *file_path);

int main(int argc, char *argv[])
{
  int result = 0;

  vm_init();

  if(argc == 1)
  {
    result = repl();
  }
  else if(argc == 2)
  {
    result = run_file(argv[1]);
  }
  else
  {
    fprintf(stderr, "Usage: cube [path]\n");
    exit(EXIT_FAILURE);
  }
  
  vm_free();

  return result;
}

static int repl()
{
  InterpretResult result = INTERPRET_OK;

  char line[1024];
  for(;;)
  {
    printf("> ");

    if(!fgets(line, sizeof(line), stdin))
    {
      printf("\n");
      break;
    }

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
