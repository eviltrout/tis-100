#ifndef _OUTPUT_H
#define _OUTPUT_H

#include <ncurses.h>
#include "node.h"
#include "program.h"

void output_node(WINDOW *win, const Node *n);
void output_location(WINDOW *win, LocationType type, union Location loc);
void output_program(WINDOW *win, const Program *p);

#endif
