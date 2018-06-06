#include <stdio.h>
#include <stdlib.h>
#include "error.h"

void die(const char *message, const char *file, int line)
{
  fprintf(stderr, "Cube error");
  if(message)
    fprintf(stderr, " %s", message);

  if(file && line > 0)
    fprintf(stderr, " at %s:%d\n", file, line);

  fprintf(stderr, "Aborting...\n");
  fflush(stderr);
  abort();
}
