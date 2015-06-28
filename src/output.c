#include "output.h"
#include "node_list.h"
#include <ncurses.h>

void output_node(const Node *n) {
  printw("[Node #%d acc=%d bak=%d ov=%d]\n", n->number, n->acc, n->bak, n->output_value);

  for (int j=0; j<n->instruction_count; j++) {
    Instruction i = n->instructions[j];

    if (j == n->ip) {
      printw("-->");
    } else {
      printw("   ");
    }
    printw(" [%X] ", j);
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
      default:
        printw("dunno about %d", i.operation);
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
  for_each_list(l, p->active_nodes) {
    output_node(l->node);
  }
}
