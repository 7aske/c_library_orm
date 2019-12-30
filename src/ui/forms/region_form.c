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

	set_field_back(field[0], A_UNDERLINE);    /* Print a line for the option 	*/
	field_opts_off(field[0], O_AUTOSKIP);    /* Don't go to next field when this */

	form_win = derwin(state->win, LINES, COLS, 0, 0);

	my_form = new_form(field);
	set_form_win(my_form, state->win);
	set_form_sub(my_form, form_win);

	post_form(my_form);
	wrefresh(state->win);

	mvwprintw(state->win, 4, 12, "Name :");
	wrefresh(state->win);

	while ((ch = wgetch(state->win))) {
		switch (ch) {
			case ctrl('x'):
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
	form_driver(my_form, REQ_NEXT_FIELD);
	form_driver(my_form, REQ_PREV_FIELD);
	strncpy(state->fs.region.name, field_buffer(field[0], 0), 255);

	unpost_form(my_form);
	free_form(my_form);
	free_field(field[0]);
	delwin(form_win);
}