#ifndef _PROGRAM_H
#define _PROGRAM_H

#include "node.h"

typedef struct _Program {
  Node *nodes;
} Program;

void program_init(Program *p);
void program_load(const Program *p, const char *filename);
void program_tick(const Program *p);
void program_clean(Program * p);
void program_output(const Program *p);

#endif
