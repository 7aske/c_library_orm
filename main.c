#include <ncurses.h>
#include <stdbool.h>
#include <form.h>

#include "db/dbc.h"
#include "ui/state.h"
#include "ui/layout.h"
#include "ui/util.h"

volatile static int running = true;

int handle_input(state_t* state);

int main() {
	system("env | grep -i TERM");

	atexit((void (*)(void)) endwin);

	state_t state;
	init_state(&state, ROOT_CTX);

	state.win = initscr();

	cbreak();
	noecho();
	start_color();
	keypad(stdscr, TRUE);
	wresize(state.win, APP_ROW, APP_COL);


	change_list(&state, 0);

	while (running) {
		display(&state);
		refresh();
		handle_input(&state);

		if (state.win == NULL) {
			running = false;
		}
	}
	list_free_noref(&state.curr_list, state.list_type);

	return 0;
}

int handle_input(state_t* state) {
	state_t* curr = state;
	state_t* par = NULL;
	int input;

	while (curr->child != NULL) {
		par = curr;
		curr = curr->child;
	}

	if (curr->ctx == ROOT_CTX) {
		input = wgetch(stdscr);
	} else {
		input = wgetch(curr->win);
	}

	if (curr->ctx == WINDOW_CTX || curr->ctx == ROOT_CTX) {
		switch (input) {
			case 'q':
				delwin(curr->win);
				curr->win = NULL;
				if (curr->ctx != ROOT_CTX) {
					if (curr->curr_list != NULL) {
						list_free_noref(&state->curr_list, state->list_type);
					}
				} else {
					running = false;
				}
				if (par != NULL){
					free(par->child);
					par->child = NULL;
				}
				break;
			case KEY_UP:
				if (curr->curr_line_pos - 1 > 0) {
					curr->curr_line_pos--;
				}
				if (curr->curr_sel_idx > 0) {
					curr->curr_sel_idx--;
				}
				break;
			case KEY_DOWN:
				if (curr->curr_line_pos + 1 < APP_ROW - 2) {
					curr->curr_line_pos++;
				}
				if (curr->curr_sel_idx < alist_size(curr->curr_list) - 1) {
					curr->curr_sel_idx++;
				}
				break;
			case KEY_LEFT:
				if (curr->list_type >= 0) {
					change_list(curr, -1);
					curr->curr_line_pos = 0;
					curr->curr_sel_idx = 0;
				}
				clear();
				break;
			case KEY_RIGHT:
				if (curr->list_type < ETYPE_LEN) {
					change_list(curr, 1);
					curr->curr_line_pos = 0;
					curr->curr_sel_idx = 0;
				}
				clear();
				break;
			case 'l':
				create_popup(curr);
				change_list(curr, 0);
				break;
			default:
				break;
		}
	}
}
