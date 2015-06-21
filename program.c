#include <stdio.h>
#include <stdlib.h>
#include <assert.h>
#include <string.h>

#include "constants.h"
#include "input_code.h"
#include "program.h"
#include "util.h"

void program_init(Program * p) {
  p->nodes = (Node *) malloc(sizeof(Node) * PROGRAM_NODES);

  Node *n = p->nodes;
  for (int i=0; i<PROGRAM_NODES; i++, ++n) {
    node_init(n, i);
  }
}

void program_tick(const Program *p) {
  Node *n = p->nodes;
  for (int i=0; i<PROGRAM_NODES; i++, ++n) {
    node_tick(n);
  }
}

void program_load(const Program *p, const char *filename) {
  assert(filename);

  FILE *fp = fopen(filename, "r");
  if (fp == NULL) {
    raise_error("Couldn't load file");
  }

  char * line = NULL;
  size_t len = 0;
  ssize_t read;

  Node *n = NULL;

  InputCode all_input[PROGRAM_NODES];
  for (int i=0; i<PROGRAM_NODES; i++) {
    input_code_init(&all_input[i]);
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
    input_code_clean(&all_input[i]);
  }

  fclose(fp);
  if (line) { free(line); }
}

void program_output(const Program * p) {
  Node *n = p->nodes;
  for (int i=0; i<PROGRAM_NODES; i++, ++n) {
    node_output(n);
  }
}

void program_clean(Program * p) {
  free(p->nodes);
}

