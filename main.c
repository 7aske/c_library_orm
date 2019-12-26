#include <ncurses.h>

#include "db/dbc.h"


int main() {
	initscr();
	cbreak();
	noecho();

	clear();

	refresh();
	printw("Hello World!");


	getch();
	endwin();

	return 0;
}
