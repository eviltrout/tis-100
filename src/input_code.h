#ifndef _INPUT_CODE_H
#define _INPUT_CODE_H

#include "constants.h"

typedef struct _InputCode {
  unsigned char line_count;
  char * lines[MAX_INSTRUCTIONS];

  unsigned char label_count;
  char * labels[MAX_INSTRUCTIONS];
  unsigned char label_address[MAX_INSTRUCTIONS];
} InputCode;

void input_code_init(InputCode *ic);
void input_code_addline(InputCode *ic, const char *line);
void input_code_clean(InputCode *ic);

#endif
