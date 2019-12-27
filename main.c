#include <ncurses.h>
#include <stdbool.h>
#include <form.h>

#include "db/dbc.h"
#include "ui/state.h"

volatile static int running = true;

WINDOW* win;

void print_list(state_t* state);

int handle_input(state_t* state);

void change_list(state_t* state, int inc);


int main() {
	atexit((void (*)(void)) endwin);
	win = initscr();
	cbreak();
	noecho();
	start_color();
	keypad(stdscr, TRUE);
	wresize(win, APP_ROW, APP_COL);

	state_t state;
	state.curr_sel_idx = 0;
	state.curr_line_pos = 1;
	state.curr_list = region_find_all();
	state.list_types[0] = REGION_E;
	state.list_types[1] = MUNICIPALITY_E;
	state.list_types[2] = ADDRESS_E;
	state.list_types[3] = LIBRARY_E;
	state.list_type = 0;

	while (running) {
		wborder(win, ACS_VLINE, ACS_VLINE, ACS_HLINE, ACS_HLINE, ACS_ULCORNER, ACS_URCORNER, ACS_LLCORNER,
				ACS_LRCORNER);
		print_list(&state);
		refresh();
		handle_input(&state);
	}

	return 0;
}

void quit() {
	running = false;
}


void print_list(state_t* state) {
	#define USED_LINES 2
	move(0, 0);
	init_pair(1, COLOR_RED, COLOR_WHITE);
	init_pair(2, COLOR_MAGENTA, COLOR_BLACK);
	init_pair(3, COLOR_BLACK, COLOR_WHITE);

	int curr_idx = 0;
	int offset;
	if (state->curr_line_pos > APP_ROW / 2) {
		offset = state->curr_line_pos - APP_ROW / 2 - 1;
	} else {
		offset = -USED_LINES;
	}

	struct sql_result_row* curr = state->curr_list->results;

	while (curr != NULL && curr_idx - offset + USED_LINES < APP_ROW - 1) {
		if (curr_idx >= offset) {
			if (state->curr_sel_idx == curr_idx) {
				attron(COLOR_PAIR(1));
			}
			switch (state->curr_list->type) {
				case MUNICIPALITY_E:
					mvprintw(curr_idx + 1 - offset, 1, "%2d %3d %.8s %.8s", curr_idx + 1,
							 ((MUNICIPALITY*) curr->data)->id_municipality,
							 ((MUNICIPALITY*) curr->data)->name,
							 ((MUNICIPALITY*) curr->data)->region->name);
					break;
				case REGION_E:
					mvprintw(curr_idx + 1 - offset, 1, "%2d %3d %.8s", curr_idx + 1,
							 ((REGION*) curr->data)->id_region,
							 ((REGION*) curr->data)->name);
					break;
				case ADDRESS_E:
					mvprintw(curr_idx + 1 - offset, 1, "%2d %3d %.8s %.8s %.8s", curr_idx + 1,
							 ((ADDRESS*) curr->data)->id_address,
							 ((ADDRESS*) curr->data)->street,
							 ((ADDRESS*) curr->data)->number,
							 ((ADDRESS*) curr->data)->municipality->name);
					break;
			}
			attroff(COLOR_PAIR(1));
		}
		curr = curr->next;
		curr_idx++;
	}
	move(1, 1);
	clrtoeol();
	move(2, 1);
	clrtoeol();
	/*HEADER*/
	// mvprintw(1, 1, "%2d %4s %16s", state->curr_sel_idx + 1, "ID", "NAME");
	int len = sizeof(state->list_types) / sizeof(state->list_types[0]);
	for (int i = 0; i < len; ++i) {
		if (i == state->list_type) {
			attron(COLOR_PAIR(3));
		} else {
			attron(COLOR_PAIR(2));
		}
		mvprintw(1, 1 + i * (APP_COL / len), list_type_str(state->list_types[i]));
		attroff(COLOR_PAIR(3));
		attroff(COLOR_PAIR(2));
	}

	/*HEADER*/
	attron(COLOR_PAIR(2));
	switch (state->curr_list->type) {
		case ADDRESS_E:
			mvprintw(2, 1, "%2s %3s %.8s %.8s", "N", "ID", "NAME", "MUNICIP");
			break;
		case AUTHOR_E:
			break;
		case AUTHOR_BOOK_E:
			break;
		case BOOK_E:
			break;
		case BOOK_SPECIMEN_E:
			break;
		case EMPLOYEE_E:
			break;
		case LIBRARY_E:
			break;
		case MUNICIPALITY_E:
			mvprintw(2, 1, "%2s %3s %.8s %.8s", "N", "ID", "NAME", "REGION");
			break;
		case PERSON_E:
			break;
		case READER_E:
			break;
		case REGION_E:
			mvprintw(2, 1, "%2s %3s %.8s", "N", "ID", "NAME");
			break;
		case RENT_E:
			break;
		default:
			mvprintw(2, 1, "ERROR");
	}
	attroff(COLOR_PAIR(2));
	move(0, 0);
	#undef USED_LINES
}

int handle_input(state_t* state) {
	int input;
	input = wgetch(win);

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
			if (state->curr_sel_idx < state->curr_list->count - 1) {
				state->curr_sel_idx++;
			}
			break;
		case KEY_LEFT:
			if (state->list_type > 0) {
				change_list(state, -1);
				state->curr_line_pos = 0;
				state->curr_sel_idx = 0;
			}
			clear();
			break;
		case KEY_RIGHT:
			if (state->list_type < sizeof(state->list_types) / sizeof(state->list_types[0])) {
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

void change_list(state_t* state, int inc) {
	state->list_type =
			(state->list_type + inc) % ((int) sizeof(state->list_types) / (int) sizeof(state->list_types[0]));
	mysql_res_free(&state->curr_list);
	switch (state->list_types[state->list_type]) {
		case MUNICIPALITY_E:
			state->curr_list = municipality_find_all();
			break;
		case ADDRESS_E:
			state->curr_list = address_find_all();
			break;
		case REGION_E:
			state->curr_list = region_find_all();
			break;
		case LIBRARY_E:
			state->curr_list = library_find_all();
			break;
	}
}
