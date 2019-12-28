#include <ncurses.h>
#include <stdbool.h>
#include <form.h>

#include "db/dbc.h"
#include "ui/state.h"

#define ADDRESS_FMTH       "%3s %4s %29.29s %10.10s %28.28s"
#define ADDRESS_FMT        "%3d %4d %29.29s %10.10s %28.28s"

#define AUTHOR_BOOK_FMTH   "%3s %4s %8.8s %8.8s %16.16s %13.13s"
#define AUTHOR_BOOK_FMT    "%3d %4d %8.8s %8.8s %16.16s %13.13s"

#define AUTHOR_FMTH        "%3s %4s %8.8s %8.8s %51.51s"
#define AUTHOR_FMT         "%3d %4d %8.8s %8.8s %51.51s"

#define BOOK_FMTH          "%3s %4s %-41.41s %14.14s %12.12s"
#define BOOK_FMT           "%3d %4d %-41.41s %14.14s %12.12s"

#define BOOK_SPECIMEN_FMTH "%3s %4s %32.32s %13.13s %10.10s %8.8s"
#define BOOK_SPECIMEN_FMT  "%3d %4d %32.32s %13.13s %10.10s %8.8s"

#define EMPLOYEE_FMTH      "%3s %4s %8.8s %8.8s %16.16s %8.8s"
#define EMPLOYEE_FMT       "%3d %4d %8.8s %8.8s %16.16s %8.8s"

#define LIBRARY_FMTH       "%3s %4s %26.26s %16.16s %8.8s %16.16s"
#define LIBRARY_FMT        "%3d %4d %26.26s %16.16s %8.8s %16.16s"

#define MUNICIPALITY_FMTH  "%3s %4s %34.34s %34.34s"
#define MUNICIPALITY_FMT   "%3d %4d %34.34s %34.34s"

#define PERSON_FMTH        "%3s %4s %27.27s %27.27s %13.14s"
#define PERSON_FMT         "%3d %4d %27.27s %27.27s %13.14s"

#define READER_FMTH        "%3s %4s %16.16s %16.16s"
#define READER_FMT         "%3d %4d %16.16s %16.16s"

#define RENT_FMTH          "%3s %4s %16.16s %10.10s %10.10s"
#define RENT_FMT           "%3d %4d %16.16s %10.10s %10.10s"

#define REGION_FMTH        "%3s %4s %69.69s"
#define REGION_FMT         "%3d %4d %69.69s"

volatile static int running = true;

WINDOW* win;

void print_list(state_t* state);

int handle_input(state_t* state);

void change_list(state_t* state, int inc);

char* _fmt_date(struct tm* ts){
	#define DATE_FMT "%02d-%02d-%04d"
	static char fmt[11];
	sprintf(fmt, DATE_FMT, ts->tm_mday, ts->tm_mon, ts->tm_year);
	return fmt;
	#undef DATE_FMT
}

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
	if (state->curr_sel_idx > APP_ROW / 2) {
		if (state->curr_sel_idx > state->curr_list->count - APP_ROW / 2 - 1) {
			offset = (int)state->curr_list->count - APP_ROW + USED_LINES;
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
						   ((READER*) curr->data)->password);
					break;
				case RENT_E:
					printw(RENT_FMT, curr_idx + 1,
						   ((RENT*) curr->data)->id_rent,
						   ((RENT*) curr->data)->reader->username,
						   ((RENT*) curr->data)->book_specimen->book_serial,
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
			printw(ADDRESS_FMTH, "N", "ID", "STREET", "NUMBER", "MUNICIPALITY");
			break;
		case AUTHOR_E:
			printw(AUTHOR_FMTH, "N", "ID", "FNAME", "LNAME", "DESCRIPTION");
			break;
		case AUTHOR_BOOK_E:
			printw(AUTHOR_BOOK_FMTH, "N", "ID", "AFNAME", "ALNAME", "BKNAME", "ISBN");
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
			printw(RENT_FMTH, "N", "ID", "USERNAME", "BOOKID", "DUE DATE");
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
