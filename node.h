#ifndef _NODE_H
#define _NODE_H

#include "constants.h"
#include "input_code.h"

typedef enum {
  MOV,
  SAV,
  SWP,
  SUB,
  ADD,
  NOP,
  NEG,
  JEZ,
  JMP,
  JNZ,
  JGZ,
  JLZ,
  JRO
} Operation;

typedef enum {
  NUMBER,
  ADDRESS,
} LocationType;

typedef enum {
  NIL,
  UP,
  RIGHT,
  DOWN,
  LEFT,
  ACC,
  ANY,
  LAST
} LocationDirection;

union Location {
  signed short number;
  LocationDirection direction;
} _Location;

typedef struct _Instruction {
  Operation operation;
  LocationType src_type;
  union Location src;

  LocationType dest_type;
  union Location dest;
} Instruction;

typedef struct _Node {
  unsigned char ip;
  unsigned char number;
  unsigned char instruction_count;
  Instruction instructions[MAX_INSTRUCTIONS];
} Node;

void node_init(Node *n, int number);
void node_output(const Node *n);
void node_parse_code(Node *n, InputCode *ic);
void node_parse_line(Node *n, InputCode *ic, const char *line);
void node_tick(Node *n);

#endif
