//
// Created by nik on 1/1/20.
//

#include "ui/forms/forms.h"


void municipality_form_construct(state_t* state) {
	#define FIELDS 2
	#define BUFLEN 16
	assert(state->ctx == FORM_CTX);
	FIELD* field[FIELDS + 1];
	FORM* my_form;
	WINDOW* form_win;
	int ch, i, id;
	char buf[BUFLEN];
	int (* action)(MYSQL*, void*) = NULL;

	MUNICIPALITY* ptr = (MUNICIPALITY*) state->fs.data;

	state->win = newwin(LINES, COLS, 0, 0);
	keypad(state->win, TRUE);

	field[0] = new_field(1, 20, 4, 24, 0, 0);
	field[1] = new_field(1, 20, 5, 24, 0, 0);
	field[2] = NULL;

	for (i = 0; i < FIELDS; i++) {
		set_field_back(field[i], A_UNDERLINE);
		field_opts_off(field[i], O_AUTOSKIP);
	}

	if (state->fs.ftype == FORM_UPDATE) {
		set_field_buffer(field[0], 0, ptr->name);
		snprintf(buf, BUFLEN, "%d", ptr->region->id_region);
		set_field_buffer(field[1], 0, buf);
	}

	form_win = derwin(state->win, LINES, COLS, 0, 0);

	my_form = new_form(field);
	set_form_win(my_form, state->win);
	set_form_sub(my_form, form_win);

	post_form(my_form);


	DBORDER(state->win);
	DBORDER(form_win);

	mvwprintw(state->win, 4, 10, "Name         :");
	mvwprintw(state->win, 5, 10, "Region       :");

	if (state->fs.ftype == FORM_UPDATE) {
		mvwprintw(state->win, 0, 4, "Update %s ID = %d", list_type_str(state->fs.type),
				  type_get_id(ptr, state->fs.type));
	} else {
		mvwprintw(state->win, 0, 4, "Add a new %s", list_type_str(state->fs.type));
	}

	print_form_footer(state->win);

	form_driver(my_form, REQ_FIRST_FIELD);
	form_driver(my_form, REQ_END_LINE);
	wrefresh(state->win);


	while ((ch = wgetch(state->win))) {
		switch (ch) {
			case ctrl('d'):
				goto end;
			case ctrl('x'):
				form_driver(my_form, REQ_PREV_FIELD);
				form_driver(my_form, REQ_NEXT_FIELD);

				strncpy(ptr->name, trimws(field_buffer(field[0], 0), sizeof(ptr->name)), sizeof(ptr->name));
				id = (int) strtol(trimws(field_buffer(field[1], 0), BUFLEN), NULL, 10);

				if (state->fs.ftype == FORM_CREATE) {
					action = type_insert_action(state->fs.type);
					ptr->id_municipality = 0;
				} else if (state->fs.ftype == FORM_UPDATE) {
					action = type_update_action(state->fs.type);
					type_free_ref(ptr, state->fs.type);
				}

				ptr->region = region_find_by_id(state->conn, id);

				if (action != NULL) {
					action(state->conn, ptr);
				}
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
	for (i = 0; i < FIELDS; ++i) {
		free_field(field[i]);
	}
	delwin(form_win);
	#undef FIELDS
	#undef BUFLEN
}