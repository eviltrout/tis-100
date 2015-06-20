#include <stdio.h>
#include <stdlib.h>
#include <assert.h>
#include <string.h>

#include "program.h"
#include "util.h"

Program *create_program(void) {
  Program *p = (Program *) malloc(sizeof(Program));
  p->nodes = (Node *) malloc(sizeof(Node) * PROGRAM_NODES);

  Node *n = p->nodes;
  for (int i=0; i<PROGRAM_NODES; i++, ++n) {
    init_node(n, i);
  }
  return p;
}

void tick_program(Program *p) {
  Node *n = p->nodes;
  for (int i=0; i<PROGRAM_NODES; i++, ++n) {
  }
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

  InputCode all_input[PROGRAM_NODES];
  for (int i=0; i<PROGRAM_NODES; i++) {
    init_input_code(&all_input[i]);
  }

  for (int index = 0; (read = getline(&line, &len, fp)) != -1;) {

    // ignore after comment, ignore debug
    char *c = line;
    while (*c != '\0') {
      if (*c == '#') {
        *c = '\0';
        break;
      } else if (*c == '!') {
        *c = ' ';
      }
      c++;
    }
    char *trimmed = trim_whitespace(line);

    if (strlen(trimmed) > 0) {
      if (line[0] == '@') {
        index = atoi(trimmed+1);
      } else {
        input_code_addline(&all_input[index], trimmed);
      }
    }
  }

  n = p->nodes;
  for (int i=0; i<PROGRAM_NODES; i++, ++n) {
    node_parse_code(n, &all_input[i]);
    free_input_code(&all_input[i]);
  }

  n = p->nodes;
  for (int i=0; i<PROGRAM_NODES; i++, ++n) {
    node_output(n);
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

