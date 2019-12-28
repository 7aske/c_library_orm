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
	system("env | grep -i TERM");
	atexit((void (*)(void)) endwin);
	win = initscr();
	cbreak();
	noecho();
	start_color();
	keypad(stdscr, TRUE);
	wresize(win, APP_ROW, APP_COL);

	state_t state;
	state.curr_list = NULL;
	state.curr_sel_idx = 0;
	state.curr_line_pos = 1;
	state.list_type = 0;
	state.list_types[0] = REGION_E;
	state.list_types[1] = MUNICIPALITY_E;
	state.list_types[2] = ADDRESS_E;
	state.list_types[3] = LIBRARY_E;

	state.list_types[4] = AUTHOR_E;
	state.list_types[5] = AUTHOR_BOOK_E;
	state.list_types[6] = BOOK_E;
	state.list_types[7] = BOOK_SPECIMEN_E;
	state.list_types[8] = EMPLOYEE_E;
	state.list_types[9] = PERSON_E;
	state.list_types[10] = READER_E;
	state.list_types[11] = RENT_E;

	change_list(&state, 0);

	while (running) {
		print_list(&state);
		wborder(win, ACS_VLINE, ACS_VLINE, ACS_HLINE, ACS_HLINE, ACS_ULCORNER, ACS_URCORNER, ACS_LLCORNER,
				ACS_LRCORNER);
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
	init_pair(3, COLOR_WHITE, COLOR_GREEN);
	int col, row;
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
			row = curr_idx + 1 - offset;
			col = 1;
			move(row, col);
			clrtoeol();
			if (state->curr_sel_idx == curr_idx) {
				attron(COLOR_PAIR(1));
			}
			switch (state->curr_list->type) {
				case MUNICIPALITY_E:
					printw("%2d %3d %16.16s %16.16s", curr_idx + 1,
						   ((MUNICIPALITY*) curr->data)->id_municipality,
						   ((MUNICIPALITY*) curr->data)->name,
						   ((MUNICIPALITY*) curr->data)->region->name);
					break;
				case REGION_E:
					printw("%2d %3d %16.16s", curr_idx + 1,
						   ((REGION*) curr->data)->id_region,
						   ((REGION*) curr->data)->name);
					break;
				case ADDRESS_E:
					printw("%2d %3d %16.16s %8.8s %16.16s", curr_idx + 1,
						   ((ADDRESS*) curr->data)->id_address,
						   ((ADDRESS*) curr->data)->street,
						   ((ADDRESS*) curr->data)->number,
						   ((ADDRESS*) curr->data)->municipality->name);
					break;
				case LIBRARY_E:
					printw("%2d %3d %16.16s %8.8s %8.8s %8.8s", curr_idx + 1,
						   ((LIBRARY*) curr->data)->id_library,
						   ((LIBRARY*) curr->data)->name,
						   ((LIBRARY*) curr->data)->address->street,
						   ((LIBRARY*) curr->data)->address->number,
						   ((LIBRARY*) curr->data)->address->municipality->name);
					break;
				case AUTHOR_E:
					printw("%2s %3s %8.8s %8.8s %55.55s", curr_idx + 1,
						   ((AUTHOR*) curr->data)->id_author,
						   ((AUTHOR*) curr->data)->person->first_name,
						   ((AUTHOR*) curr->data)->person->last_name,
						   ((AUTHOR*) curr->data)->description);
					break;
				case AUTHOR_BOOK_E:
					break;
				case BOOK_E:
					break;
				case BOOK_SPECIMEN_E:
					break;
				case EMPLOYEE_E:
					printw("%2s %3s %8.8s %8.8s %16.16s %8.8", curr_idx + 1,
						   ((EMPLOYEE*) curr->data)->id_employee,
						   ((EMPLOYEE*) curr->data)->person->first_name,
						   ((EMPLOYEE*) curr->data)->person->last_name,
						   ((EMPLOYEE*) curr->data)->library->name,
						   ((EMPLOYEE*) curr->data)->position);
					break;
				case PERSON_E:
					printw("%2s %3s %8.8s %8.8s %13.13s", curr_idx + 1,
						   ((PERSON*) curr->data)->id_person,
						   ((PERSON*) curr->data)->first_name,
						   ((PERSON*) curr->data)->last_name,
						   ((PERSON*) curr->data)->jmbg);
					break;
				case READER_E:
					break;
				case RENT_E:
					break;
			}
			attroff(COLOR_PAIR(1));
		}
		curr = curr->next;
		curr_idx++;
	}
	move(1, 1);
	clrtoeol();
	for (int j = 0; j < 79; ++j) {
		attron(COLOR_PAIR(3));
		addch(' ');
		attroff(COLOR_PAIR(3));
	}
	/*HEADER*/
	int len = sizeof(state->list_types) / sizeof(state->list_types[0]);
	for (int i = 0; i < len; ++i) {
		if (i == state->list_type) {
			attron(COLOR_PAIR(1));
		} else {
			attron(COLOR_PAIR(3));
		}
		mvprintw(1, 1 + i * (APP_COL / len), "%s", list_type_str(state->list_types[i]));
		attroff(COLOR_PAIR(1));
		attroff(COLOR_PAIR(3));
	}
	move(2, 1);
	clrtoeol();

	/*HEADER*/
	attron(COLOR_PAIR(2));
	switch (state->curr_list->type) {
		case ADDRESS_E:
			printw("%2s %3s %16.16s %8.8s %16.16s", "N", "ID", "STREET", "NUMBER", "MUNICIP");
			break;
		case AUTHOR_E:
			printw("%2s %3s %8.8s %8.8s %55.55s", "N", "ID", "FNAME", "LNAME", "DESCRIPTION");
			break;
		case AUTHOR_BOOK_E:
			break;
		case BOOK_E:
			break;
		case BOOK_SPECIMEN_E:
			break;
		case EMPLOYEE_E:
			printw("%2s %3s %8.8s %8.8s %16.16s %8.8s", "N", "ID", "FNAME", "LNAME", "LIBRARY", "POSITION");
			break;
		case LIBRARY_E:
			printw("%2s %3s %16.16s %8.8s %8.8s %8.8s", "N", "ID", "NAME", "STREET", "NUMBER", "MUNICIP");
			break;
		case MUNICIPALITY_E:
			printw("%2s %3s %16.16s %16.16s", "N", "ID", "NAME", "REGION");
			break;
		case PERSON_E:
			printw("%2s %3s %8.8s %8.8s %13.13s", "N", "ID", "FNAME", "LNAME", "JMBG");
			break;
		case READER_E:
			break;
		case REGION_E:
			printw("%2s %3s %16.16s", "N", "ID", "NAME");
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
	int len = ((int) sizeof(state->list_types) / (int) sizeof(state->list_types[0]));
	if (state->list_type == 0 && inc == -1) {
		state->list_type = len - 1;
	} else {
		state->list_type = (state->list_type + inc) % len;
	}
	if (state->curr_list != NULL) {
		mysql_res_free(&state->curr_list);
	}
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
		case AUTHOR_E:
			state->curr_list = author_find_all();
			break;
		case AUTHOR_BOOK_E:
			state->curr_list = author_book_find_all();
			break;
		case BOOK_E:
			state->curr_list = book_find_all();
			break;
		case BOOK_SPECIMEN_E:
			state->curr_list = book_specimen_find_all();
			break;
		case EMPLOYEE_E:
			state->curr_list = employee_find_all();
			break;
		case PERSON_E:
			state->curr_list = person_find_all();
			break;
		case READER_E:
			state->curr_list = reader_find_all();
			break;
		case RENT_E:
			state->curr_list = rent_find_all();
			break;
	}
}
