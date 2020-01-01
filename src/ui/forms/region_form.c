//
// Created by nik on 12/30/19.
//

#include "ui/forms/region_form.h"

#define ctrl(x)           ((x) & 0x1f)

void region_form_construct(state_t* state) {
	FIELD* field[2];
	FORM* my_form;
	WINDOW* form_win;
	int ch;

	state->win = newwin(LINES, COLS, 0, 0);
	keypad(state->win, TRUE);

	field[0] = new_field(1, 10, 4, 18, 0, 0);
	field[1] = NULL;

	set_field_back(field[0], A_UNDERLINE);
	field_opts_off(field[0], O_AUTOSKIP);

	form_win = derwin(state->win, LINES, COLS, 0, 0);

	my_form = new_form(field);
	set_form_win(my_form, state->win);
	set_form_sub(my_form, form_win);

	post_form(my_form);


	DBORDER(state->win);
	DBORDER(form_win);

	mvwprintw(state->win, 4, 12, "Name :");
	mvwprintw(state->win, 0, 4, "Add a new %s", "Region");

	mvwprintw(state->win, LINES - 2, 2, "^X - Save and add");
	mvwprintw(state->win, LINES - 3, 2, "^D - Cancel");


	form_driver(my_form, REQ_NEXT_FIELD);
	form_driver(my_form, REQ_PREV_FIELD);
	wrefresh(state->win);

	while ((ch = wgetch(state->win))) {
		switch (ch) {
			case ctrl('d'):
				((REGION*) state->fs.data)->id_region = INT_MAX;
				goto end;
			case ctrl('x'):
				form_driver(my_form, REQ_NEXT_FIELD);
				form_driver(my_form, REQ_PREV_FIELD);
				((REGION*) state->fs.data)->id_region = 0;
				strncpy(((REGION*) state->fs.data)->name, trimws(field_buffer(field[0], 0)), 255);
				// strncpy(((REGION*) state->fs.data)->name, "trimws(field_buffer(field[0], 0))", 255);
				fprintf(stderr, "%s\n",((REGION*) state->fs.data)->name);
				goto end;
			case KEY_DOWN:
				form_driver(my_form, REQ_NEXT_FIELD);
				form_driver(my_form, REQ_END_LINE);
				break;
			case KEY_UP:
				form_driver(my_form, REQ_PREV_FIELD);
				form_driver(my_form, REQ_END_LINE);
				break;
			case KEY_BACKSPACE:
				form_driver(my_form, REQ_DEL_PREV);
			default:
				form_driver(my_form, ch);
				break;
		}
	}
	end:;

	unpost_form(my_form);
	free_form(my_form);
	free_field(field[0]);
	delwin(form_win);
}