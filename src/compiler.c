/** @file compiler.c
 *
 * @brief Implementation of the compiler module. 
 *
 * @author David J. Lains (dlains)
 * @bug No known bugs.
 */

#include "common.h"
#include "compiler.h"
#include "scanner.h"

/** @brief Compile the source code available in the scanner.
 *
 * Compile the source code available in the scanner.
 */
void compile()
{
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
}
