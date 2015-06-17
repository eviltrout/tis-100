#include <stdio.h>
#include <stdarg.h>
#include <stdlib.h>
#include "util.h"

void raise_error(const char *msg, ...) {
  va_list(args);
  va_start(args, msg);
  vfprintf(stderr, msg, args);
  exit(0);
}
