#include <stdio.h>
#include <stdlib.h>
#include <assert.h>

#include "program.h"

Program *create_program(void) {
  Program *p = (Program *) malloc(sizeof(Program));
  p->nodes = (Node *) malloc(sizeof(Node) * PROGRAM_NODES);
  return p;
}

void free_program(Program ** pp) {
  Program *p = *pp;
  free(p->nodes);
  free(p);
  *pp = NULL;
}

int load_program(Program *p, const char *filename) {
  assert(filename);
  assert(p);

  FILE *fp = fopen(filename, "r");
  if (fp == NULL) {
    return 0;
  }

  fclose(fp);
  return 1;
}

