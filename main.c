#include <ncurses.h>
#include <stdbool.h>
#include <form.h>

#include "db/dbc.h"
#include "ui/state.h"
#include "ui/layout.h"

volatile static int running = true;

int handle_input(state_t* state);

int main() {
	system("env | grep -i TERM");

	atexit((void (*)(void)) endwin);
	state_t state;
	init_state(&state);

	state.win = initscr();
	cbreak();
	noecho();
	start_color();
	keypad(stdscr, TRUE);
	wresize(state.win, APP_ROW, APP_COL);


	change_list(&state, 0);

	while (running) {
		print_list(&state);
		DBORDER(state.win)
		refresh();
		handle_input(&state);
	}
	void* elem;
	for (int i = 0; i < alist_size(state.curr_list); ++i) {
		elem = alist_get(state.curr_list, i);
		type_free(elem, state.list_type);
	}
	alist_destroy(&state.curr_list);

	return 0;
}

void quit() {
	running = false;
}


int handle_input(state_t* state) {
	int input;
	input = wgetch(state->win);

	if (state->ctx == WINDOW_CTX) {
		switch (input) {
			case 'q':
				quit();
				break;
			case KEY_UP:
				if (state->curr_line_pos - 1 > 0) {
					state->curr_line_pos--;
				}
				if (state->curr_sel_idx > 0) {
					state->curr_sel_idx--;
				}
				break;
			case KEY_DOWN:
				if (state->curr_line_pos + 1 < APP_ROW - 2) {
					state->curr_line_pos++;
				}
				if (state->curr_sel_idx < alist_size(state->curr_list) - 1) {
					state->curr_sel_idx++;
				}
				break;
			case KEY_LEFT:
				if (state->list_type >= 0) {
					change_list(state, -1);
					state->curr_line_pos = 0;
					state->curr_sel_idx = 0;
				}
				clear();
				break;
			case KEY_RIGHT:
				if (state->list_type < ETYPE_LEN) {
					change_list(state, 1);
					state->curr_line_pos = 0;
					state->curr_sel_idx = 0;
				}
				clear();
				break;
			default:
				break;
		}
	}
}
