#include <stdio.h>
#include <assert.h>
#include <string.h>
#include <stdlib.h>

#include "util.h"
#include "node.h"

void init_node(Node *n, int number) {
  n->number = number;
  n->instruction_count = 0;
  n->ip = 0;
}

void location_output(LocationType type, Location loc) {
  if (type == ADDRESS) {
    switch(loc) {
      case NIL:   printf("NIL"); break;
      case UP:    printf("UP"); break;
      case DOWN:  printf("DOWN"); break;
      case LEFT:  printf("LEFT"); break;
      case RIGHT: printf("RIGHT"); break;
      case ACC:   printf("ACC"); break;
      case ANY:   printf("ANY"); break;
      case LAST:  printf("LAST"); break;
    }
  } else {
    printf("%d", loc);
  }
}

void node_output(const Node *n) {
  printf("[Node #%d count=%d ip=%d]\n", n->number, n->instruction_count, n->ip);

  for (int j=0; j<n->instruction_count; j++) {
    Instruction i = n->instructions[j];

    printf("  ");
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

void parse_location(const char *s, Location *loc, LocationType *type) {
  if (!s) { raise_error("no source was found"); }

  if (strcmp(s, "UP") == 0) {
    *type = ADDRESS;
    *loc = UP;
  } else if (strcmp(s, "DOWN") == 0) {
    *type = ADDRESS;
    *loc = DOWN;
  } else if (strcmp(s, "LEFT") == 0) {
    *type = ADDRESS;
    *loc = LEFT;
  } else if (strcmp(s, "RIGHT") == 0) {
    *type = ADDRESS;
    *loc = RIGHT;
  } else if (strcmp(s, "ACC") == 0) {
    *type = ADDRESS;
    *loc = ACC;
  } else if (strcmp(s, "NIL") == 0) {
    *type = ADDRESS;
    *loc = NIL;
  } else if (strcmp(s, "ANY") == 0) {
    *type = ADDRESS;
    *loc = ANY;
  } else if (strcmp(s, "LAST") == 0) {
    *type = ADDRESS;
    *loc = LAST;
  } else {
    *type = LITERAL;
    *loc = atoi(s);
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

void parse_onearg(Node *n, const char *s, Operation op) {
  const int len = strlen(s+4);
  char *rem = (char *) malloc(sizeof(char) * len);
  strcpy(rem, s+4);

  Instruction *i = node_create_instruction(n, op);
  parse_location(strtok(rem, "\n"), &i->src, &i->src_type);

  free(rem);
}

void node_parse_instruction(Node *n, const char *s) {
  assert(n);
  assert(s);
  assert(strlen(s) > 3);

  char ins[5];
  strncpy(ins, s, 3);

  if (strcmp(ins, "MOV") == 0) {
    parse_mov(n, s);
  } else if (strcmp(ins, "SUB") == 0) {
    parse_onearg(n, s, SUB);
  } else if (strcmp(ins, "ADD") == 0) {
    parse_onearg(n, s, ADD);
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
