#include "rts2option.h"

void
Rts2Option::getOptionChar (char **end_opt)
{
  **end_opt = short_option;
  (*end_opt)++;
  if (has_arg)
    {
      **end_opt = ':';
      (*end_opt)++;
    }
}
