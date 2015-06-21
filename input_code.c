#include "input_code.h"
#include <string.h>
#include <stdlib.h>

void input_code_init(InputCode *ic) {
  ic->line_count = 0;
  ic->label_count = 0;
}

void input_code_addline(InputCode *ic, const char *line) {
  char *copy = malloc(sizeof(char) * strlen(line));
  strcpy(copy, line);
  ic->lines[ic->line_count++] = copy;
}

void input_code_clean(InputCode *ic) {
  for (int i=0; i<ic->line_count; i++) {
    free(ic->lines[i]);
  }
  for (int i=0; i<ic->label_count; i++) {
    free(ic->labels[i]);
  }
}

