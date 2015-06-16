#ifndef _NODE_H
#define _NODE_H

typedef enum {
  MOV,
  SAV
} Operation;

typedef enum {
  LITERAL,
  ADDRESS
} LocationType;

typedef enum {
  UP,
  RIGHT,
  DOWN,
  LEFT,
  ACC
} Location;

typedef struct _Instruction {
  Operation operation;
  LocationType src_type;
  Location src;
  Location dest;
} Instruction;

typedef struct _Node {
  int number;
} Node;

void node_parse_instruction(const Node *n, const char *s);
#endif
