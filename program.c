#include <stdio.h>
#include <stdlib.h>
#include <assert.h>
#include <string.h>

#include "program.h"

Program *create_program(void) {
  Program *p = (Program *) malloc(sizeof(Program));
  p->nodes = (Node *) malloc(sizeof(Node) * PROGRAM_NODES);

  Node *n = p->nodes;
  for (int i=0; i<PROGRAM_NODES; i++, ++n) {
    n->number = i;
  }
  return p;
}

int load_program(const Program *p, const char *filename) {
  assert(filename);
  assert(p);

  FILE *fp = fopen(filename, "r");
  if (fp == NULL) {
    return 0;
  }

  char * line = NULL;
  size_t len = 0;
  ssize_t read;

  Node *n = NULL;
  while ((read = getline(&line, &len, fp)) != -1) {
    if (strlen(line) > 1) {
      if (line[0] == '@') {
        n = &p->nodes[atoi(line+1)];
      } else {
        node_parse_instruction(n, line);
      }
    }
  }

  fclose(fp);
  if (line) { free(line); }

  return 1;
}

void free_program(Program ** pp) {
  Program *p = *pp;
  free(p->nodes);
  free(p);
  *pp = NULL;
}

