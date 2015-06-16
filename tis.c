#include <stdio.h>
#include <stdlib.h>
#include <assert.h>

#include "util.h"
#include "program.h"

int main() {
  Program *p = create_program();
  assert(p);
  const int success = load_program(p, "./programs/swap.tis");

  if (!success) {
    raise_error("ERROR LOADING FILE");
  }

  free_program(&p);
  assert(!p);
}
