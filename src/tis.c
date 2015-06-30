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

#ifdef RICH_OUTPUT
  initscr();
	raw();
	keypad(stdscr, TRUE);
	noecho();
#endif

  program_init(p);
  program_load_system(p, "./programs/divide.sys");
  program_load_code(p, "./programs/divide.tis");

#ifdef RICH_OUTPUT
  output_program(p);
  int code = getch();
  while (code != 'q') {
    program_tick(p);
    clear();
    output_program(p);
    refresh();
    code = getch();
  }
#else
  int all_blocked_count = 0;
  while (all_blocked_count < 5) {
    int all_blocked = program_tick(p);
    if (all_blocked) {
      all_blocked_count++;
    } else {
      all_blocked_count = 0;
    }
  }
#endif

#ifdef RICH_OUTPUT
  endwin();
#endif
  program_clean(p);
}
