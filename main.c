#include <ncurses.h>
#include <stdbool.h>
#include <form.h>

#include "db/dbc.h"
#include "ui/state.h"
#include "ui/format.h"

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
	init_state(&state);

	change_list(&state, 0);

	state.pwin = newwin(12, 40, 1, 1);

	while (running) {
		print_list(&state);
		wborder(win, ACS_VLINE, ACS_VLINE, ACS_HLINE, ACS_HLINE, ACS_ULCORNER, ACS_URCORNER, ACS_LLCORNER,
				ACS_LRCORNER);
		refresh();
		if (state.pwin != NULL) {
			// wborder(state.pwin, ACS_VLINE, ACS_VLINE, ACS_HLINE, ACS_HLINE, ACS_ULCORNER, ACS_URCORNER, ACS_LLCORNER,
			// 		ACS_LRCORNER);
			mvwprintw(state.pwin, 10, 10, "HELLO");
			box(state.pwin, 0, 0);
			wrefresh(state.pwin);
		}
		handle_input(&state);
	}
	mysql_res_free(&state.curr_list);
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
	if (state->curr_sel_idx > APP_ROW / 2) {
		if (state->curr_sel_idx > state->curr_list->count - APP_ROW / 2 - 1) {
			offset = (int) state->curr_list->count - APP_ROW + USED_LINES;
		} else {
			offset = state->curr_sel_idx - APP_ROW / 2 - 1;
		}
	} else {
		offset = -USED_LINES;
	}

	struct sql_result_row* curr = state->curr_list->results;

	while (curr != NULL && curr_idx - offset + USED_LINES <= APP_ROW - 1) {
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
					printw(MUNICIPALITY_FMT, curr_idx + 1,
						   ((MUNICIPALITY*) curr->data)->id_municipality,
						   ((MUNICIPALITY*) curr->data)->name,
						   ((MUNICIPALITY*) curr->data)->region->name);
					break;
				case REGION_E:
					printw(REGION_FMT, curr_idx + 1,
						   ((REGION*) curr->data)->id_region,
						   ((REGION*) curr->data)->name);
					break;
				case ADDRESS_E:
					printw(ADDRESS_FMT, curr_idx + 1,
						   ((ADDRESS*) curr->data)->id_address,
						   ((ADDRESS*) curr->data)->street,
						   ((ADDRESS*) curr->data)->number,
						   ((ADDRESS*) curr->data)->municipality->name);
					break;
				case LIBRARY_E:
					printw(LIBRARY_FMT, curr_idx + 1,
						   ((LIBRARY*) curr->data)->id_library,
						   ((LIBRARY*) curr->data)->name,
						   ((LIBRARY*) curr->data)->address->street,
						   ((LIBRARY*) curr->data)->address->number,
						   ((LIBRARY*) curr->data)->address->municipality->name);
					break;
				case AUTHOR_E:
					printw(AUTHOR_FMT, curr_idx + 1,
						   ((AUTHOR*) curr->data)->id_author,
						   ((AUTHOR*) curr->data)->person->first_name,
						   ((AUTHOR*) curr->data)->person->last_name,
						   ((AUTHOR*) curr->data)->description);
					break;
				case AUTHOR_BOOK_E:
					printw(AUTHOR_BOOK_FMT, curr_idx + 1,
						   ((AUTHOR_BOOK*) curr->data)->id_author_book,
						   ((AUTHOR_BOOK*) curr->data)->author->person->first_name,
						   ((AUTHOR_BOOK*) curr->data)->author->person->last_name,
						   ((AUTHOR_BOOK*) curr->data)->book->name,
						   ((AUTHOR_BOOK*) curr->data)->book->isbn);
					break;
				case BOOK_E:
					printw(BOOK_FMT, curr_idx + 1,
						   ((BOOK*) curr->data)->id_book,
						   ((BOOK*) curr->data)->name,
						   ((BOOK*) curr->data)->isbn,
						   _fmt_date(&((BOOK*) curr->data)->publish_date));
					break;
				case BOOK_SPECIMEN_E:
					printw(BOOK_SPECIMEN_FMT, curr_idx + 1,
						   ((BOOK_SPECIMEN*) curr->data)->id_book_specimen,
						   ((BOOK_SPECIMEN*) curr->data)->book->name,
						   ((BOOK_SPECIMEN*) curr->data)->book->isbn,
						   _fmt_date(&((BOOK_SPECIMEN*) curr->data)->book->publish_date),
						   ((BOOK_SPECIMEN*) curr->data)->library->name);
					break;
				case EMPLOYEE_E:
					printw(EMPLOYEE_FMT, curr_idx + 1,
						   ((EMPLOYEE*) curr->data)->id_employee,
						   ((EMPLOYEE*) curr->data)->person->first_name,
						   ((EMPLOYEE*) curr->data)->person->last_name,
						   ((EMPLOYEE*) curr->data)->library->name,
						   ((EMPLOYEE*) curr->data)->position);
					break;
				case PERSON_E:
					printw(PERSON_FMT, curr_idx + 1,
						   ((PERSON*) curr->data)->id_person,
						   ((PERSON*) curr->data)->first_name,
						   ((PERSON*) curr->data)->last_name,
						   ((PERSON*) curr->data)->jmbg);
					break;
				case READER_E:
					printw(READER_FMT, curr_idx + 1,
						   ((READER*) curr->data)->id_reader,
						   ((READER*) curr->data)->username,
							// ((READER*) curr->data)->password);
						   "******************");
					break;
				case RENT_E:
					printw(RENT_FMT, curr_idx + 1,
						   ((RENT*) curr->data)->id_rent,
						   ((RENT*) curr->data)->reader->username,
						   ((RENT*) curr->data)->book_specimen->book_serial,
						   ((RENT*) curr->data)->book_specimen->book->name,
						   _fmt_date(&((RENT*) curr->data)->due_date));
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
		mvprintw(1, 5 + i * (APP_COL / len), "%s", list_type_str(state->list_types[i]));
		attroff(COLOR_PAIR(1));
		attroff(COLOR_PAIR(3));
	}
	move(2, 1);
	clrtoeol();

	/*HEADER*/
	attron(COLOR_PAIR(2));
	switch (state->curr_list->type) {
		case ADDRESS_E:
			printw(ADDRESS_FMTH, "N", "ID", "STREET", "NUMBER", "MUNICIPALITY");
			break;
		case AUTHOR_E:
			printw(AUTHOR_FMTH, "N", "ID", "FIRST NAME", "LAST NAME", "DESCRIPTION");
			break;
		case AUTHOR_BOOK_E:
			printw(AUTHOR_BOOK_FMTH, "N", "ID", "FIRST NAME", "LAST NAME", "BOOK NAME", "ISBN");
			break;
		case BOOK_E:
			printw(BOOK_FMTH, "N", "ID", "BOOK NAME", "ISBN", "PUB DATE");
			break;
		case BOOK_SPECIMEN_E:
			printw(BOOK_SPECIMEN_FMTH, "N", "ID", "NAME", "ISBN", "PUB DATE", "LIBRARY");
			break;
		case EMPLOYEE_E:
			printw(EMPLOYEE_FMTH, "N", "ID", "FIRST NAME", "LAST NAME", "LIBRARY", "POSITION");
			break;
		case LIBRARY_E:
			printw(LIBRARY_FMTH, "N", "ID", "LIBRARY NAME", "STREET", "NUMBER", "MUNICIPALITY");
			break;
		case MUNICIPALITY_E:
			printw(MUNICIPALITY_FMTH, "N", "ID", "NAME", "REGION");
			break;
		case PERSON_E:
			printw(PERSON_FMTH, "N", "ID", "FIRST NAME", "LAST NAME", "JMBG");
			break;
		case READER_E:
			printw(READER_FMTH, "N", "ID", "USERNAME", "PASSWORD");
			break;
		case REGION_E:
			printw(REGION_FMTH, "N", "ID", "NAME");
			break;
		case RENT_E:
			printw(RENT_FMTH, "N", "ID", "USERNAME", "BOOKID", "BOOK NAME", "DUE DATE");
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
			if (state->list_type >= 0) {
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
		case 'a':
			if (state->pwin == NULL) {
				state->pwin = newwin(24, 40, 0, 0);
				wrefresh(state->pwin);
			}
			break;
		case 's':
			if (state->pwin != NULL) {
				delwin(state->pwin);
			}
			break;
		default:
			break;
	}
}
