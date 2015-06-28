#include <stdio.h>
#include <stdlib.h>
#include <assert.h>
#include <ncurses.h>

#include "util.h"
#include "program.h"
#include "output.h"

int main() {
  Program program;
  Program *p = &program;

  initscr();
	raw();
	keypad(stdscr, TRUE);
	noecho();

  program_init(p);
  program_load_system(p, "./programs/divide.sys");
  program_load_code(p, "./programs/divide.tis");

  output_program(p);
  int code = getch();
  while (code != 'q') {
    program_tick(p);
    clear();
    output_program(p);
    code = getch();
  }

  endwin();
  program_clean(p);
}
