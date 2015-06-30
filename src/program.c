#define _POSIX_C_SOURCE 200809L
#include <stdio.h>
#include <stdlib.h>
#include <assert.h>
#include <string.h>
#include <unistd.h>

#include "constants.h"
#include "input_code.h"
#include "program.h"
#include "util.h"
#include "curses.h"


static Node *create_node(Program *p) {
  Node *n = (Node *) malloc(sizeof(Node));
  node_init(n);
  p->nodes = node_list_append(p->nodes, n);
  return n;
}

void program_init(Program * p) {
  p->nodes = NULL;
  p->active_nodes = NULL;

  for (int i=0; i<PROGRAM_NODES; i++) {
    Node *n = create_node(p);
    n->visible = TRUE;
    n->number = i;
    p->nodes_by_index[i] = n;
  }

  // Link all the nodes up
  Node **nodes = p->nodes_by_index;
  for (int i=0; i<4; i++) {
    if (i < 3) {
      nodes[i]->ports[RIGHT] = nodes[i+1];
      nodes[i+4]->ports[RIGHT] = nodes[i+5];
      nodes[i+8]->ports[RIGHT] = nodes[i+9];
      nodes[i+1]->ports[LEFT] = nodes[i];
      nodes[i+5]->ports[LEFT] = nodes[i+4];
      nodes[i+9]->ports[LEFT] = nodes[i+8];
    }
    nodes[i]->ports[DOWN] = nodes[i+4];
    nodes[i+4]->ports[DOWN] = nodes[i+8];
    nodes[i+4]->ports[UP] = nodes[i];
    nodes[i+8]->ports[UP] = nodes[i+4];
  }
}

void program_clean(Program *p) {
  node_list_clean(p->active_nodes, FALSE);
  node_list_clean(p->nodes, TRUE);
  p->nodes = NULL;
}

int program_tick(const Program *p) {
  int all_blocked = TRUE;
  for_each_list(l, p->active_nodes) {
    Node *n = l->node;
    node_tick(n);
    all_blocked &= n->blocked;
  }
  return all_blocked;
}

static char *read_line(FILE *fp, char *line) {
  size_t len = 0;
  ssize_t read = getline(&line, &len, fp);

  if (read == -1) {
    if (line) { free(line); }
    return NULL;
  }
  return line;
}


static Node *create_input_node(Program *p, FILE *fp) {
  Node *n = create_node(p);

  char *line = NULL;
  line = read_line(fp, line);
  Node *below = p->nodes_by_index[atoi(line)];

  n->ports[DOWN] = below;
  below->ports[UP] = n;

  line = read_line(fp, line);
  while (line[0] != '*') {
    Instruction *i = node_create_instruction(n, MOV);
    i->src_type = NUMBER;
    i->src.number = atoi(line);
    i->dest_type = ADDRESS;
    i->dest.direction = DOWN;
    line = read_line(fp, line);
  }

  Instruction *i = node_create_instruction(n, JRO);
  i->src_type = NUMBER;
  i->src.number = 0;

  if (line) { free(line); }
  return n;
}

static Node *create_output_node(Program *p, FILE *fp) {
  Node *n = create_node(p);

  char *line = NULL;
  line = read_line(fp, line);
  Node *above = p->nodes_by_index[atoi(line)];

  Instruction *i = node_create_instruction(n, MOV);
  i->src_type = ADDRESS;
  i->src.direction = UP;
  i->dest_type = ADDRESS;
  i->dest.direction = ACC;

  node_create_instruction(n, OUT);

  n->ports[UP] = above;
  above->ports[DOWN] = n;

  if (line) { free(line); }
  return n;
}

void program_load_system(Program *p, const char *filename) {
  assert(filename);
  FILE *fp = fopen(filename, "r");
  if (fp == NULL) {
    raise_error("Couldn't load %s", filename);
  }

  char * line = NULL;
  while ((line = read_line(fp, line))) {
    Node *n = NULL;
    if (strncmp(line, "input-top", 9) == 0) {
      n = create_input_node(p, fp);
    } else if (strncmp(line, "output-bottom", 12) == 0) {
      n = create_output_node(p, fp);
    }
    if (n) {
      p->active_nodes = node_list_append(p->active_nodes, n);
    }
  }
  if (line) { free(line); }

  fclose(fp);
}

void program_load_code(Program *p, const char *filename) {
  assert(filename);

  FILE *fp = fopen(filename, "r");
  if (fp == NULL) {
    raise_error("Couldn't load %s", filename);
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

  for (int i=0; i<PROGRAM_NODES; i++) {
    Node *n = p->nodes_by_index[i];
    node_parse_code(n, &all_input[n->number]);
    input_code_clean(&all_input[n->number]);

    if (n->instruction_count > 0) {
      p->active_nodes = node_list_append(p->active_nodes, n);
    }
  }

  fclose(fp);
  if (line) { free(line); }
}
