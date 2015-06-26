#include <stdio.h>
#include <stdlib.h>
#include <assert.h>
#include <string.h>

#include "constants.h"
#include "input_code.h"
#include "program.h"
#include "util.h"

#define for_each_active(n, p) \
        Node *(n); for(int i=0; (n)=p->active_nodes[i], i < p->active_node_count; i++)

#define for_each_node(n, p) \
        Node *(n) = p->nodes; for(int i=0; i < PROGRAM_NODES; i++, ++n)

void program_init(Program * p) {
  p->active_node_count = 0;
  for_each_node(n, p) {
    node_init(n, i);
  }


  // Link all the nodes up
  Node *nodes = p->nodes;
  for (int i=0; i<4; i++) {
    if (i < 3) {
      nodes[i].ports[RIGHT] = &nodes[i+1];
      nodes[i+4].ports[RIGHT] = &nodes[i+5];
      nodes[i+8].ports[RIGHT] = &nodes[i+9];
      nodes[i+1].ports[LEFT] = &nodes[i];
      nodes[i+5].ports[LEFT] = &nodes[i+4];
      nodes[i+9].ports[LEFT] = &nodes[i+8];
    }
    nodes[i].ports[DOWN] = &nodes[i+4];
    nodes[i+4].ports[DOWN] = &nodes[i+8];
    nodes[i+4].ports[UP] = &nodes[i];
    nodes[i+8].ports[UP] = &nodes[i+4];
  }

}

void program_tick(const Program *p) {
  for_each_active(n, p) {
    node_tick(n);
  }
}

void program_load_system(Program *p, const char *filename) {
  assert(filename);
}

void program_load_code(Program *p, const char *filename) {
  assert(filename);

  FILE *fp = fopen(filename, "r");
  if (fp == NULL) {
    raise_error("Couldn't load file");
  }

  char * line = NULL;
  size_t len = 0;
  ssize_t read;

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

  for_each_node(n, p) {
    node_parse_code(n, &all_input[i]);
    input_code_clean(&all_input[i]);

    if (n->instruction_count > 0) {
      p->active_nodes[p->active_node_count++] = n;
    }
  }

  fclose(fp);
  if (line) { free(line); }
}

void program_output(const Program * p) {
  for_each_active(n, p) {
    node_output(n);
  }
}
