#include "output.h"
#include "node_list.h"
#include <ncurses.h>
#include "util.h"
#include "math.h"

void output_node(const Node *n, int x, int y) {
  mvprintw(y, x, "[Node #%d acc=%d bak=%d ov=%d]\n", n->number, n->acc, n->bak, n->output_value);

  for (int j=0; j<n->instruction_count; j++) {
    Instruction i = n->instructions[j];

    int row = y + j + 1;
    if (j == n->ip) {
      mvprintw(row, x, "-->");
    }
    mvprintw(row, x+4, "[%X] ", j);

    switch(i.operation) {
      case MOV:
        printw("MOV ");
        output_location(i.src_type, i.src);
        printw(" ");
        output_location(i.dest_type, i.dest);
        break;
      case ADD:
        printw("ADD ");
        output_location(i.src_type, i.src);
        break;
      case SUB:
        printw("SUB ");
        output_location(i.src_type, i.src);
        break;
      case JEZ:
        printw("JEZ ");
        output_location(i.src_type, i.src);
        break;
      case JMP:
        printw("JMP ");
        output_location(i.src_type, i.src);
        break;
      case JNZ:
        printw("JNZ ");
        output_location(i.src_type, i.src);
        break;
      case JGZ:
        printw("JGZ ");
        output_location(i.src_type, i.src);
        break;
      case JLZ:
        printw("JLZ ");
        output_location(i.src_type, i.src);
        break;
      case JRO:
        printw("JRO ");
        output_location(i.src_type, i.src);
        break;
      case SAV:
        printw("SAV");
        break;
      case SWP:
        printw("SWP");
        break;
      case NEG:
        printw("NEG");
        break;
      case NOP:
        printw("NOP");
        break;
      case OUT:
        printw("OUT");
        break;
      default:
        raise_error("dunno about %d", i.operation);
    }
    printw("\n");
  }
}

void output_location(LocationType type, union Location loc) {
  if (type == ADDRESS) {
    switch(loc.direction) {
      case NIL:   printw("NIL"); break;
      case UP:    printw("UP"); break;
      case DOWN:  printw("DOWN"); break;
      case LEFT:  printw("LEFT"); break;
      case RIGHT: printw("RIGHT"); break;
      case ACC:   printw("ACC"); break;
      case ANY:   printw("ANY"); break;
      case LAST:  printw("LAST"); break;
    }
  } else if (type == NUMBER) {
    printw("%d", loc.number);
  }
}

void output_program(const Program * p) {

  int width = COLS;
  int nw = width / 4;
  int nh = 15;

  for_each_list(l, p->active_nodes) {
    Node *n = l->node;
    if (n->visible) {
      int x = (n->number % 4) * nw;
      int y = floor(n->number / 4) * nh;
      output_node(n, x, y);
    }
  }
}
