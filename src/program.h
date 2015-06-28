#ifndef _PROGRAM_H
#define _PROGRAM_H

#include "node.h"
#include "node_list.h"

typedef struct _Program {
  Node * nodes_by_index[PROGRAM_NODES];

  NodeList *nodes;
  NodeList *active_nodes;
} Program;

void program_init(Program *p);
void program_load_system(Program *p, const char *filename);
void program_load_code(Program *p, const char *filename);
void program_tick(const Program *p);
void program_output(const Program *p);
void program_clean(Program *p);

#endif
