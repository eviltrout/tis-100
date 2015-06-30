#ifndef _OUTPUT_H
#define _OUTPUT_H

#include <ncurses.h>
#include "node.h"
#include "program.h"

void output_node(const Node *n, int x, int y);
void output_location(LocationType type, union Location loc);
void output_program(const Program *p);

#endif
