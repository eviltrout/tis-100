#ifndef _PROGRAM_H
#define _PROGRAM_H

#include "node.h"

#define PROGRAM_NODES 9

typedef struct _Program {
  Node *nodes;
} Program;

Program *create_program(void);
int load_program(Program *p, const char *filename);
void free_program(Program ** pp);


#endif
