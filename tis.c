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

  Node *n = &p->nodes[6];
  node_output(n);
  printf("------------------------------------------------------\n");
  node_tick(n);
  node_output(n);
  printf("------------------------------------------------------\n");
  node_tick(n);
  node_output(n);
  printf("------------------------------------------------------\n");

  /* program_output(p); */
}
