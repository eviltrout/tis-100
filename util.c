#include <stdio.h>
#include <stdlib.h>
#include "util.h"

void raise_error(const char *msg) {
  fprintf(stderr, "ERROR: %s", msg);
  exit(0);
}
