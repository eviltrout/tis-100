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

typedef struct _InputCode {
  unsigned char line_count;
  char * lines[MAX_INSTRUCTIONS];

  unsigned char label_count;
  char * labels[MAX_INSTRUCTIONS];
  unsigned char label_address[MAX_INSTRUCTIONS];
} InputCode;

typedef struct _Node {
  unsigned char ip;
  unsigned char number;
  unsigned char instruction_count;
  Instruction instructions[MAX_INSTRUCTIONS];
} Node;

void init_input_code(InputCode *ic);
void input_code_addline(InputCode *ic, const char *line);
void free_input_code(InputCode *ic);

void init_node(Node *n, int number);
void node_output(const Node *n);
void node_parse_code(Node *n, InputCode *ic);
void node_parse_line(Node *n, InputCode *ic, const char *line);

#endif
