#include "ui/state.h"

void init_state(state_t* state) {
	state->pwin = NULL;
	state->curr_list = NULL;
	state->curr_sel_idx = 0;
	state->curr_line_pos = 1;
	state->list_type = 0;

	state->list_types[0] = REGION_E;
	state->list_types[1] = MUNICIPALITY_E;
	state->list_types[2] = ADDRESS_E;
	state->list_types[3] = LIBRARY_E;
	state->list_types[4] = EMPLOYEE_E;
	state->list_types[5] = PERSON_E;
	state->list_types[6] = AUTHOR_E;
	state->list_types[7] = AUTHOR_BOOK_E;
	state->list_types[8] = BOOK_E;
	state->list_types[9] = BOOK_SPECIMEN_E;
	state->list_types[10] = READER_E;
	state->list_types[11] = RENT_E;
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