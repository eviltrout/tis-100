#include <stdio.h>
#include <stdlib.h>
#include <assert.h>
#include <ncurses.h>

#include "util.h"
#include "program.h"

int main() {
  Program program;
  Program *p = &program;

  program_init(p);
  program_load_system(p, "./programs/divide.sys");
  program_load_code(p, "./programs/divide.tis");

  Node *n = &p->nodes[6];

  initscr();
	raw();
	keypad(stdscr, TRUE);
	noecho();

  node_output(n);
  node_tick(n);

  refresh();
  int code = getch();
  while (code != 'q') {
    node_tick(n);
    clear();
    node_output(n);
    refresh();
    code = getch();
  }

  endwin();			/* End curses mode		  */

  /* program_output(p); */
}
