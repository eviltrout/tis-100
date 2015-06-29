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
  JRO,
  OUT
} Operation;

typedef enum {
  NUMBER,
  ADDRESS,
} LocationType;

typedef enum {
  UP,
  RIGHT,
  DOWN,
  LEFT,
  NIL,
  ACC,
  ANY,
  LAST
} LocationDirection;

union Location {
  short number;
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
  unsigned visible:1;
  unsigned char ip;
  unsigned char number;
  unsigned char instruction_count;
  Instruction *instructions;

  short acc;
  short bak;

  struct _Node *output_port;
  short output_value;

  struct _Node *ports[4];
} Node;

typedef struct _ReadResult {
  int blocked;
  short value;
} ReadResult;

void node_init(Node *n);
void node_clean(Node *n);

void node_parse_code(Node *n, InputCode *ic);
void node_parse_line(Node *n, InputCode *ic, const char *line);
void node_tick(Node *n);
ReadResult node_read(Node *n, LocationType lt, union Location where);
int node_write(Node *n, LocationDirection dir, short value);
void node_advance(Node *n);
Instruction *node_create_instruction(Node *n, Operation op);

#endif
