#include <stdio.h>
#include <assert.h>
#include <string.h>
#include <stdlib.h>

#include "util.h"
#include "node.h"

void node_parse_instruction(const Node *n, const char *s) {
  assert(n);
  assert(s);
  assert(strlen(s) > 3);

  char ins[5];
  strncpy(ins, s, 4);

  if (strcmp(ins, "MOV ") == 0) {
    const int len = strlen(s+4);
    char *rem = (char *) malloc(sizeof(char) * len);
    strcpy(rem, s+4);

    Instruction i;
    i.operation = MOV;

    const char *first = strtok(rem, " ,");
    if (!first) {
      raise_error("MOV HAS NO SOURCE");
    }

    if (strcmp(first, "UP") == 0) {
      i.src_type = ADDRESS;
      i.src = UP;
    }
    if (strcmp(first, "DOWN") == 0) {
      i.src_type = ADDRESS;
      i.src = DOWN;
    }
    if (strcmp(first, "LEFT") == 0) {
      i.src_type = ADDRESS;
      i.src = LEFT;
    }
    if (strcmp(first, "RIGHT") == 0) {
      i.src_type = ADDRESS;
      i.src = RIGHT;
    }
    if (strcmp(first, "ACC") == 0) {
      i.src_type = ADDRESS;
      i.src = ACC;
    }

    const char *second = strtok(NULL, " ,");
    if (second == NULL) {
      raise_error("MOV HAS NO DESTINATION");
    }

    free(rem);
  }

}
