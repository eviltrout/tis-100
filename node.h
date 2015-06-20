#ifndef _NODE_H
#define _NODE_H

#define MAX_INSTRUCTIONS 15

typedef enum {
  MOV,
  SAV,
  SWP,
  SUB,
  ADD,
  NOP,
  NEG
} Operation;

typedef enum {
  LITERAL,
  ADDRESS
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
} Location;

typedef struct _Instruction {
  Operation operation;
  LocationType src_type;
  Location src;

  LocationType dest_type;
  Location dest;
} Instruction;

typedef struct _InputCode {
  int line_count;
  char * lines[MAX_INSTRUCTIONS];
} InputCode;

typedef struct _Node {
  int ip;
  int number;
  int instruction_count;
  Instruction instructions[MAX_INSTRUCTIONS];
} Node;

void init_input_code(InputCode *ic);
void input_code_addline(InputCode *ic, const char *line);
void free_input_code(InputCode *ic);

void init_node(Node *n, int number);
void node_output(const Node *n);
void node_parse_code(Node *n, InputCode *ic);

#endif
