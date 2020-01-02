//
// Created by nik on 1/1/20.
//

#include "ui/forms/municipality_form.h"


void municipality_form_construct(state_t* state) {
	#define FIELDS 3
	assert(state->ctx == FORM_CTX);
	FIELD* field[FIELDS];
	FORM* my_form;
	WINDOW* form_win;
	int ch;
	char buf[12];

	state->win = newwin(LINES, COLS, 0, 0);
	keypad(state->win, TRUE);

	field[0] = new_field(1, 20, 4, 18, 0, 0);
	field[1] = new_field(1, 20, 5, 18, 0, 0);
	field[2] = NULL;

	for (int i = 0; i < FIELDS; i++) {
		set_field_back(field[i], A_UNDERLINE);
		field_opts_off(field[i], O_AUTOSKIP);
	}

	if (state->fs.ftype == FORM_UPDATE) {
		set_field_buffer(field[0], 0, ((MUNICIPALITY*) state->fs.data)->name);
		snprintf(buf, 11, "%d", ((MUNICIPALITY*) state->fs.data)->region->id_region);
		set_field_buffer(field[1], 0, buf);
	}

	form_win = derwin(state->win, LINES, COLS, 0, 0);

	my_form = new_form(field);
	set_form_win(my_form, state->win);
	set_form_sub(my_form, form_win);

	post_form(my_form);


	DBORDER(state->win);
	DBORDER(form_win);

	mvwprintw(state->win, 4, 10, "Name   :");
	mvwprintw(state->win, 5, 10, "Region :");

	if (state->fs.ftype == FORM_UPDATE) {
		mvwprintw(state->win, 0, 4, "Update %s ID = %d", list_type_str(state->fs.type),
				  ((MUNICIPALITY*) state->fs.data)->id_municipality);
	} else {
		mvwprintw(state->win, 0, 4, "Add a new %s", list_type_str(state->fs.type));
	}

	print_form_footer(state->win);

	form_driver(my_form, REQ_FIRST_FIELD);
	form_driver(my_form, REQ_END_LINE);
	wrefresh(state->win);
	long id;
	REGION* regionptr;
	while ((ch = wgetch(state->win))) {
		switch (ch) {
			case ctrl('l'):
				state->child = create_state_ctx(state, LIST_CTX);
				change_list(state->child, 0, state->conn);
				break;
			case ctrl('d'):
				if (state->fs.ftype == FORM_UPDATE) {
					((MUNICIPALITY*) state->fs.data)->id_municipality = 0;
				} else {
					((MUNICIPALITY*) state->fs.data)->id_municipality = INT_MAX;
				}
				goto end;
			case ctrl('x'):
				form_driver(my_form, REQ_PREV_FIELD);
				form_driver(my_form, REQ_NEXT_FIELD);
				if (state->fs.ftype == FORM_CREATE) {
					((MUNICIPALITY*) state->fs.data)->id_municipality = 0;
				}
				strncpy(((MUNICIPALITY*) state->fs.data)->name, trimws(field_buffer(field[0], 0)), 255);
				id = strtol(trimws(field_buffer(field[1], 0)), NULL, 10);
				regionptr = region_find_by_id(state->conn, id);
				((MUNICIPALITY*) state->fs.data)->region = regionptr;
				goto end;
			case KEY_STAB:
			case KEY_DOWN:
				form_driver(my_form, REQ_NEXT_FIELD);
				form_driver(my_form, REQ_END_LINE);
				break;
			case KEY_UP:
				form_driver(my_form, REQ_PREV_FIELD);
				form_driver(my_form, REQ_END_LINE);
				break;
			case KEY_LEFT:
				form_driver(my_form, REQ_PREV_CHAR);
				break;
			case KEY_RIGHT:
				form_driver(my_form, REQ_NEXT_CHAR);
				break;
			case KEY_BACKSPACE:
				form_driver(my_form, REQ_DEL_PREV);
			case KEY_DC:
				form_driver(my_form, REQ_DEL_CHAR);
				break;
			default:
				form_driver(my_form, ch);
				break;
		}
	}
	end:;

	unpost_form(my_form);
	free_form(my_form);
	for (int j = 0; j < FIELDS; ++j) {
		free_field(field[j]);
	}
	delwin(form_win);
	#undef FIELDS
}