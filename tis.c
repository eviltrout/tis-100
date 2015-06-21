#include <stdio.h>
#include <stdlib.h>
#include <assert.h>

#include "util.h"
#include "program.h"

int main() {
  Program program;
  Program *p = &program;

  program_init(p);
  program_load(p, "./programs/divide.tis");
  program_tick(p);
  program_output(p);

  program_clean(p);
}
