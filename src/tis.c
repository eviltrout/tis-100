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

  WINDOW * win =initscr();
	raw();
	keypad(stdscr, TRUE);
	noecho();

  program_init(p);
  program_load_system(p, "./programs/divide.sys");
  program_load_code(p, "./programs/divide.tis");

  output_program(p);
  /* nodelay(stdscr, TRUE); */
  /* int code = -1; */
  /* while (code != 'q') { */
  /*   program_tick(p); */
  /*   code = getch(); */
  /* } */

  int code = getch();
  while (code != 'q') {
    program_tick(p);
    clear();
    output_program(p);
    wrefresh(win);
    code = getch();
  }

  endwin();
  program_clean(p);
}
