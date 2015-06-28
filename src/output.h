#ifndef _OUTPUT_H
#define _OUTPUT_H

#include "node.h"
#include "program.h"

void output_node(const Node *n);
void output_location(LocationType type, union Location loc);
void output_program(const Program *p);

#endif
