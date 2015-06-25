#include <stdio.h>
#include <assert.h>
#include <string.h>
#include <stdlib.h>

#include "util.h"
#include "node.h"

void node_init(Node *n, int number) {
  n->number = number;
  n->instruction_count = 0;
  n->ip = 0;
  n->acc = 0;
  n->bak = 0;
  n->left = NULL;
  n->right = NULL;
  n->up = NULL;
  n->down = NULL;
}

void location_output(LocationType type, union Location loc) {
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

void node_output(const Node *n) {
  printw("[Node #%d acc=%d bak=%d ip=%d]\n", n->number, n->acc, n->bak, n->ip);

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
        location_output(i.src_type, i.src);
        printw(" ");
        location_output(i.dest_type, i.dest);
        break;
      case ADD:
        printw("ADD ");
        location_output(i.src_type, i.src);
        break;
      case SUB:
        printw("SUB ");
        location_output(i.src_type, i.src);
        break;
      case JEZ:
        printw("JEZ ");
        location_output(i.src_type, i.src);
        break;
      case JMP:
        printw("JMP ");
        location_output(i.src_type, i.src);
        break;
      case JNZ:
        printw("JNZ ");
        location_output(i.src_type, i.src);
        break;
      case JGZ:
        printw("JGZ ");
        location_output(i.src_type, i.src);
        break;
      case JLZ:
        printw("JLZ ");
        location_output(i.src_type, i.src);
        break;
      case JRO:
        printw("JRO ");
        location_output(i.src_type, i.src);
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

Instruction *node_create_instruction(Node *n, Operation op) {
  assert(n->instruction_count < MAX_INSTRUCTIONS);
  Instruction *i = &n->instructions[n->instruction_count++];
  i->operation = op;
  return i;
}

void parse_location(const char *s, union Location *loc, LocationType *type) {
  if (!s) { raise_error("no source was found"); }

  if (strcmp(s, "UP") == 0) {
    *type = ADDRESS;
    (*loc).direction = UP;
  } else if (strcmp(s, "DOWN") == 0) {
    *type = ADDRESS;
    (*loc).direction = DOWN;
  } else if (strcmp(s, "LEFT") == 0) {
    *type = ADDRESS;
    (*loc).direction = LEFT;
  } else if (strcmp(s, "RIGHT") == 0) {
    *type = ADDRESS;
    (*loc).direction = RIGHT;
  } else if (strcmp(s, "ACC") == 0) {
    *type = ADDRESS;
    (*loc).direction = ACC;
  } else if (strcmp(s, "NIL") == 0) {
    *type = ADDRESS;
    (*loc).direction = NIL;
  } else if (strcmp(s, "ANY") == 0) {
    *type = ADDRESS;
    (*loc).direction = ANY;
  } else if (strcmp(s, "LAST") == 0) {
    *type = ADDRESS;
    (*loc).direction = LAST;
  } else {
    *type = NUMBER;
    (*loc).number = atoi(s);
  }
}

void parse_mov(Node *n, const char *s) {
  const int len = strlen(s+4);
  char *rem = (char *) malloc(sizeof(char) * len);
  strcpy(rem, s+4);

  Instruction *i = node_create_instruction(n, MOV);
  parse_location(strtok(rem, " ,"), &i->src, &i->src_type);
  parse_location(strtok(NULL, " ,\n"), &i->dest, &i->dest_type);

  free(rem);
}

void parse_onearg(Node *n, InputCode *ic, const char *s, Operation op) {
  const int len = strlen(s+4);
  char *rem = (char *) malloc(sizeof(char) * len);
  strcpy(rem, s+4);

  Instruction *ins = node_create_instruction(n, op);

  switch(op) {
    case JEZ:
    case JMP:
    case JNZ:
    case JGZ:
    case JLZ:
      for (int i=0; i<ic->label_count; i++) {
        const char *label = ic->labels[i];
        if (strcmp(label, rem) == 0) {
          ins->src_type = NUMBER;
          ins->src.number = ic->label_address[i];
          goto finally;
        }
      }
    default:
      parse_location(rem, &ins->src, &ins->src_type);
  }
finally:
  free(rem);
}

void node_parse_code(Node *n, InputCode *ic) {

  // First let's find the labels
  for (int i=0; i< ic->line_count; i++) {
    char *line = ic->lines[i];

    // Look for a label
    char *c = line;
    while (*c != '\0') {
      if (*c == ':') {
        int length = (c - line);
        char *label = (char *) malloc(sizeof(char) * (length + 1));
        strncpy(label, line, length);
        label[length] = '\0';

        int idx = ic->label_count;
        ic->labels[idx] = label;
        ic->label_address[idx] = i;
        ic->label_count++;

        // Remove the label from the code
        char *rem = trim_whitespace(c+1);

        // We need something to jump to, so NOP for now
        // TODO: compress empty lines and jump to the next instruction
        if (!strlen(rem)) { rem = "NOP"; }

        char *new_line = (char *) malloc(sizeof(char) * strlen(rem));
        strcpy(new_line, rem);

        free(line);
        line = new_line;
        ic->lines[i] = new_line;
      }
      c++;
    }
  }

  for (int i=0; i< ic->line_count; i++) {
    node_parse_line(n, ic, ic->lines[i]);
  }
}

void node_parse_line(Node *n, InputCode *ic, const char *s) {
  assert(n);
  assert(s);
  assert(strlen(s) > 2);

  char ins[5];
  strncpy(ins, s, 3);

  if (strcmp(ins, "MOV") == 0) {
    parse_mov(n, s);
  } else if (strcmp(ins, "SUB") == 0) {
    parse_onearg(n, ic, s, SUB);
  } else if (strcmp(ins, "ADD") == 0) {
    parse_onearg(n, ic, s, ADD);
  } else if (strcmp(ins, "JEZ") == 0) {
    parse_onearg(n, ic, s, JEZ);
  } else if (strcmp(ins, "JMP") == 0) {
    parse_onearg(n, ic, s, JMP);
  } else if (strcmp(ins, "JNZ") == 0) {
    parse_onearg(n, ic, s, JNZ);
  } else if (strcmp(ins, "JGZ") == 0) {
    parse_onearg(n, ic, s, JGZ);
  } else if (strcmp(ins, "JLZ") == 0) {
    parse_onearg(n, ic, s, JLZ);
  } else if (strcmp(ins, "JRO") == 0) {
    parse_onearg(n, ic, s, JRO);
  } else if (strcmp(ins, "SAV") == 0) {
    node_create_instruction(n, SAV);
  } else if (strcmp(ins, "SWP") == 0) {
    node_create_instruction(n, SWP);
  } else if (strcmp(ins, "NOP") == 0) {
    node_create_instruction(n, NOP);
  } else if (strcmp(ins, "NEG") == 0) {
    node_create_instruction(n, NEG);
  } else {
    raise_error("Don't understand instruction [%s]", ins);
  }
}

static inline void node_set_ip(Node *n, short new_val) {
  if (new_val >= n->instruction_count || new_val < 0) new_val = 0;
  n->ip = new_val;
}

ReadResult node_read(Node *n, LocationType type, union Location where) {
  ReadResult res;
  res.blocked = 1;

  if (type == NUMBER) {
    res.blocked = 0;
    res.value = where.number;
  } else if (where.direction == NIL) {
    res.value = 0;
    res.blocked = 0;
  } else {
    raise_error("wat");
  }

  return res;
}

int node_write(Node *n, LocationDirection dir, short value) {
  switch(dir) {
    case ACC: n->acc = value; break;
    default:
      raise_error("don't know how to write %d", dir);
  }

  // not blocked
  return 0;
}

void node_tick(Node *n) {
  Instruction *i = &n->instructions[n->ip];
  short tmp;
  ReadResult read;

  int blocked;

  switch(i->operation) {
    case MOV:
      read = node_read(n, i->src_type, i->src);
      if (read.blocked) return;
      blocked = node_write(n, i->dest.direction, read.value);
      if (blocked) return;
      break;
    case ADD:
      n->acc += i->src.number;
      if (n->acc > MAX_ACC) n->acc = MAX_ACC;
      if (n->acc < MIN_ACC) n->acc = MIN_ACC;
      break;
    case SUB:
      n->acc -= i->src.number;
      if (n->acc > MAX_ACC) n->acc = MAX_ACC;
      if (n->acc < MIN_ACC) n->acc = MIN_ACC;
      break;
    case JMP: node_set_ip(n, i->src.number); return;
    case JRO: node_set_ip(n, n->ip + i->src.number); return;
    case JEZ:
      if (n->acc == 0) {
        node_set_ip(n, i->src.number);
        return;
      }
      break;
    case JGZ:
      if (n->acc > 0) {
        node_set_ip(n, i->src.number);
        return;
      }
      break;
    case JLZ:
      if (n->acc < 0) {
        node_set_ip(n, i->src.number);
        return;
      }
      break;
    case JNZ:
      if (n->acc != 0) {
        node_set_ip(n, i->src.number);
        return;
      }
      break;
    case SWP:
      tmp = n->bak;
      n->bak = n->acc;
      n->acc = tmp;
      break;
    case SAV: n->bak = n->acc; break;
    case NEG: n->acc = n->acc * -1; break;
    case NOP: break;
    default:
      printw("ERROR: DIDN'T HANDLE op\n");
  }
  refresh();			/* Print it on to the real screen */

  node_set_ip(n, n->ip+1);
}
