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
}

void location_output(LocationType type, union Location loc) {
  if (type == ADDRESS) {
    switch(loc.direction) {
      case NIL:   printf("NIL"); break;
      case UP:    printf("UP"); break;
      case DOWN:  printf("DOWN"); break;
      case LEFT:  printf("LEFT"); break;
      case RIGHT: printf("RIGHT"); break;
      case ACC:   printf("ACC"); break;
      case ANY:   printf("ANY"); break;
      case LAST:  printf("LAST"); break;
    }
  } else if (type == NUMBER) {
    printf("%d", loc.number);
  }
}

void node_output(const Node *n) {
  printf("[Node #%d count=%d ip=%d]\n", n->number, n->instruction_count, n->ip);

  for (int j=0; j<n->instruction_count; j++) {
    Instruction i = n->instructions[j];

    printf("  [%d] ", j);
    switch(i.operation) {
      case MOV:
        printf("MOV ");
        location_output(i.src_type, i.src);
        printf(" ");
        location_output(i.dest_type, i.dest);
        break;
      case ADD:
        printf("ADD ");
        location_output(i.src_type, i.src);
        break;
      case SUB:
        printf("SUB ");
        location_output(i.src_type, i.src);
        break;
      case JEZ:
        printf("JEZ ");
        location_output(i.src_type, i.src);
        break;
      case JMP:
        printf("JMP ");
        location_output(i.src_type, i.src);
        break;
      case JNZ:
        printf("JNZ ");
        location_output(i.src_type, i.src);
        break;
      case JGZ:
        printf("JGZ ");
        location_output(i.src_type, i.src);
        break;
      case JLZ:
        printf("JLZ ");
        location_output(i.src_type, i.src);
        break;
      case JRO:
        printf("JRO ");
        location_output(i.src_type, i.src);
        break;
      case SAV:
        printf("SAV");
        break;
      case SWP:
        printf("SWP");
        break;
      case NEG:
        printf("NEG");
        break;
      case NOP:
        printf("NOP");
        break;
      default:
        printf("dunno about %d", i.operation);
    }
    printf("\n");
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

void node_tick(Node *n) {
  
}
