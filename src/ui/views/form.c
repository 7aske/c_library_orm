
#include "ui/views/form.h"

void form_add_data(state_t* state, void* data, size_t size) {
	assert(state->ctx == FORM_CTX);
	assert(state->fs.ftype == FORM_UPDATE);
	memcpy(state->fs.data, data, size);
}

state_t* form_set_type(state_t* state, form_type_e ftype) {
	assert(state->parent->ctx == LIST_CTX);
	state->fs.ftype = ftype;
	unsigned long size = type_get_size(state->fs.type);
	state->fs.data = calloc(1, size);
	if (ftype == FORM_UPDATE) {
		form_add_data(state, alist_get(state->parent->ls.list, state->parent->ls.sel_idx), size);
	}
	switch (state->fs.type) {
		case REGION_TYPE:
			region_form_construct(state);
			break;
		case MUNICIPALITY_TYPE:
			municipality_form_construct(state);
			break;
		case ADDRESS_TYPE:
			free(state->fs.data);
			state->fs.data = NULL;
			break;
		case LIBRARY_TYPE:
			free(state->fs.data);
			state->fs.data = NULL;
			break;
		case EMPLOYEE_TYPE:
			free(state->fs.data);
			state->fs.data = NULL;
			break;
		case PERSON_TYPE:
			person_form_construct(state);
			break;
		case AUTHOR_TYPE:
			free(state->fs.data);
			state->fs.data = NULL;
			break;
		case AUTHOR_BOOK_TYPE:
			free(state->fs.data);
			state->fs.data = NULL;
			break;
		case BOOK_TYPE:
			free(state->fs.data);
			state->fs.data = NULL;
			break;
		case BOOK_SPECIMEN_TYPE:
			free(state->fs.data);
			state->fs.data = NULL;
			break;
		case READER_TYPE:
			free(state->fs.data);
			state->fs.data = NULL;
			break;
		case RENT_TYPE:
			free(state->fs.data);
			state->fs.data = NULL;
			break;
	}
	return state;
}

void print_form_footer(WINDOW* win) {
	int y, x;
	getmaxyx(win, y, x);
	wmove(win, y - 2, 1);
	wattron(win, COLOR_PAIR(3));
	for (int j = 0; j < x - 2; ++j) {
		waddch(win, ' ');
	}
	mvwprintw(win, y - 2, 2, "^X - Commit | ^D Discard");
	wattroff(win, COLOR_PAIR(3));
}

