//
// Created by nik on 12/29/19.
//
#include "ui/layout.h"


void change_list(state_t* state, int inc) {
	void* elem;
	if (state->curr_list != NULL) {
		for (int i = 0; i < alist_size(state->curr_list); ++i) {
			elem = alist_get(state->curr_list, i);
			type_free_ref(elem, state->list_type);
		}
		alist_destroy(&state->curr_list);
	}

	if (state->list_type == list_types[0] && inc == -1) {
		state->list_type = ETYPE_LEN - 1;
	} else {
		state->list_type = (state->list_type + inc) % ETYPE_LEN;
	}

	switch (state->list_type) {
		case MUNICIPALITY_E:
			state->curr_list = alist_new(sizeof(MUNICIPALITY));
			res_to_list(municipality_find_all(), state->curr_list);
			break;
		case ADDRESS_E:
			state->curr_list = alist_new(sizeof(ADDRESS));
			res_to_list(address_find_all(), state->curr_list);
			break;
		case REGION_E:
			state->curr_list = alist_new(sizeof(REGION));
			res_to_list(region_find_all(), state->curr_list);
			break;
		case LIBRARY_E:
			state->curr_list = alist_new(sizeof(LIBRARY));
			res_to_list(library_find_all(), state->curr_list);
			break;
		case AUTHOR_E:
			state->curr_list = alist_new(sizeof(AUTHOR));
			res_to_list(author_find_all(), state->curr_list);
			break;
		case AUTHOR_BOOK_E:
			state->curr_list = alist_new(sizeof(AUTHOR_BOOK));
			res_to_list(author_book_find_all(), state->curr_list);
			break;
		case BOOK_E:
			state->curr_list = alist_new(sizeof(BOOK));
			res_to_list(book_find_all(), state->curr_list);
			break;
		case BOOK_SPECIMEN_E:
			state->curr_list = alist_new(sizeof(BOOK_SPECIMEN));
			res_to_list(book_specimen_find_all(), state->curr_list);
			break;
		case EMPLOYEE_E:
			state->curr_list = alist_new(sizeof(EMPLOYEE));
			res_to_list(employee_find_all(), state->curr_list);
			break;
		case PERSON_E:
			state->curr_list = alist_new(sizeof(PERSON));
			res_to_list(person_find_all(), state->curr_list);
			break;
		case READER_E:
			state->curr_list = alist_new(sizeof(READER));
			res_to_list(reader_find_all(), state->curr_list);
			break;
		case RENT_E:
			state->curr_list = alist_new(sizeof(RENT));
			res_to_list(rent_find_all(), state->curr_list);
			break;
	}
}

void print_list(state_t* state) {
	#define HEAD_LINES 2
	#define FOOT_LINES 1
	#define PER_SCR (APP_ROW - 2 - HEAD_LINES - FOOT_LINES)
	move(0, 0);
	init_pair(1, COLOR_RED, COLOR_WHITE);
	init_pair(2, COLOR_MAGENTA, COLOR_BLACK);
	init_pair(3, COLOR_WHITE, COLOR_GREEN);
	int col, row, k;
	int count = alist_size(state->curr_list);

	WINDOW* win = state->win;

	void* curr;
	int start = state->curr_sel_idx / PER_SCR * PER_SCR;
	int end = start + PER_SCR;

	for (k = start, row = HEAD_LINES + 1, col = 1; k < end && k < count; ++k, row++) {
		curr = alist_get(state->curr_list, k);
		move(row, col);
		clrtoeol();

		if (state->curr_sel_idx == k) {
			attron(COLOR_PAIR(1));
		}

		switch (state->list_type) {
			case MUNICIPALITY_E:
				wprintw(win, MUNICIPALITY_FMT, k + 1,
						((MUNICIPALITY*) curr)->id_municipality,
						((MUNICIPALITY*) curr)->name,
						((MUNICIPALITY*) curr)->region->name);
				break;
			case REGION_E:
				wprintw(win, REGION_FMT, k + 1,
						((REGION*) curr)->id_region,
						((REGION*) curr)->name);
				break;
			case ADDRESS_E:
				wprintw(win, ADDRESS_FMT, k + 1,
						((ADDRESS*) curr)->id_address,
						((ADDRESS*) curr)->street,
						((ADDRESS*) curr)->number,
						((ADDRESS*) curr)->municipality->name);
				break;
			case LIBRARY_E:
				wprintw(win, LIBRARY_FMT, k + 1,
						((LIBRARY*) curr)->id_library,
						((LIBRARY*) curr)->name,
						((LIBRARY*) curr)->address->street,
						((LIBRARY*) curr)->address->number,
						((LIBRARY*) curr)->address->municipality->name);
				break;
			case AUTHOR_E:
				wprintw(win, AUTHOR_FMT, k + 1,
						((AUTHOR*) curr)->id_author,
						((AUTHOR*) curr)->person->first_name,
						((AUTHOR*) curr)->person->last_name,
						((AUTHOR*) curr)->description);
				break;
			case AUTHOR_BOOK_E:
				wprintw(win, AUTHOR_BOOK_FMT, k + 1,
						((AUTHOR_BOOK*) curr)->id_author_book,
						((AUTHOR_BOOK*) curr)->author->person->first_name,
						((AUTHOR_BOOK*) curr)->author->person->last_name,
						((AUTHOR_BOOK*) curr)->book->name,
						((AUTHOR_BOOK*) curr)->book->isbn);
				break;
			case BOOK_E:
				wprintw(win, BOOK_FMT, k + 1,
						((BOOK*) curr)->id_book,
						((BOOK*) curr)->name,
						((BOOK*) curr)->isbn,
						_fmt_date(&((BOOK*) curr)->publish_date));
				break;
			case BOOK_SPECIMEN_E:
				wprintw(win, BOOK_SPECIMEN_FMT, k + 1,
						((BOOK_SPECIMEN*) curr)->id_book_specimen,
						((BOOK_SPECIMEN*) curr)->book->name,
						((BOOK_SPECIMEN*) curr)->book->isbn,
						_fmt_date(&((BOOK_SPECIMEN*) curr)->book->publish_date),
						((BOOK_SPECIMEN*) curr)->library->name);
				break;
			case EMPLOYEE_E:
				wprintw(win, EMPLOYEE_FMT, k + 1,
						((EMPLOYEE*) curr)->id_employee,
						((EMPLOYEE*) curr)->person->first_name,
						((EMPLOYEE*) curr)->person->last_name,
						((EMPLOYEE*) curr)->library->name,
						((EMPLOYEE*) curr)->position);
				break;
			case PERSON_E:
				wprintw(win, PERSON_FMT, k + 1,
						((PERSON*) curr)->id_person,
						((PERSON*) curr)->first_name,
						((PERSON*) curr)->last_name,
						((PERSON*) curr)->jmbg);
				break;
			case READER_E:
				wprintw(win, READER_FMT, k + 1,
						((READER*) curr)->id_reader,
						((READER*) curr)->username,
						// ((READER*) curr)->password);
						"******************");
				break;
			case RENT_E:
				wprintw(win, RENT_FMT, k + 1,
						((RENT*) curr)->id_rent,
						((RENT*) curr)->reader->username,
						((RENT*) curr)->book_specimen->book_serial,
						((RENT*) curr)->book_specimen->book->name,
						_fmt_date(&((RENT*) curr)->due_date));
				break;
		}
		attroff(COLOR_PAIR(1));
	}

	move(1, 1);
	clrtoeol();
	for (int j = 0; j < 79; ++j) {
		attron(COLOR_PAIR(3));
		addch(' ');
		attroff(COLOR_PAIR(3));
	}

	/*HEADER*/
	for (int i = 0; i < ETYPE_LEN; ++i) {
		if (i == state->list_type) {
			attron(COLOR_PAIR(1));
		} else {
			attron(COLOR_PAIR(3));
		}
		mvprintw(1, 5 + i * (APP_COL / ETYPE_LEN), "%s", list_type_str(list_types[i]));
		attroff(COLOR_PAIR(1));
		attroff(COLOR_PAIR(3));
	}
	move(2, 1);
	clrtoeol();

	/*HEADER*/
	attron(COLOR_PAIR(2));
	switch (state->list_type) {
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

	print_footer(state);
	move(0, 0);
	#undef HEAD_LINES
}

void print_footer(state_t* state) {
	WINDOW* win = state->win;
	mvwprintw(win, APP_ROW - 2, 1, "CREATE REMOVE UPDATE DELETE");
}
