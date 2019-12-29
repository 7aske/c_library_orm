
#include "ui/state.h"

void init_state(state_t* state) {
	static const char* title = "APP";
	strncpy(state->title, title, 16);
	state->win = NULL;
	state->child = NULL;
	state->parent = NULL;
	state->ctx = ROOT_CTX;
}

void delete_state_ctx(state_t* state) {
	assert(state != NULL);
	delwin(state->win);
	state->win = NULL;
	window_count--;
	switch (state->ctx) {

		case ROOT_CTX:
			break;
		case WINDOW_CTX:
			break;
		case LIST_CTX:
			if (state->ls.list)
				list_free_noref(&state->ls.list, state->ls.type);
			break;
		case FORM_CTX:
			break;
		case POPUP_CTX:
			break;
	}
	if (state->parent) {
		free(state);
		state->parent->child = NULL;
		wclear(state->parent->win);
	}
}

state_t* append_list_ctx(state_t* state) {

	state->ctx = LIST_CTX;
	state->ls.type = REGION_TYPE;
	state->ls.list = NULL;
	state->ls.line_pos = 1;
	state->ls.sel_idx = 0;

	return state;
}

state_t* append_form_ctx(state_t* state) {
	return NULL;
}

state_t* append_popup_ctx(state_t* state) {
	return NULL;
}

state_t* create_state_ctx(state_t* parent, ctx_e ctx) {
	#define BUF_LEN 16
	char buf[BUF_LEN];
	int row, col;
	getmaxyx(parent->win, row, col);

	state_t* newstate = calloc(1, sizeof(state_t));

	snprintf(buf, BUF_LEN, "WINDOW %02d", ++window_count);
	strncpy(newstate->title, buf, BUF_LEN);

	newstate->win = newwin(row, col, 0, 0);
	keypad(newstate->win, TRUE);

	newstate->ctx = WINDOW_CTX;
	newstate->child = NULL;
	newstate->parent = parent;

	switch (ctx) {
		case ROOT_CTX:
		case WINDOW_CTX:
			return newstate;
		case LIST_CTX:
			return append_list_ctx(newstate);
		case FORM_CTX:
			return append_form_ctx(newstate);
		case POPUP_CTX:
			return append_popup_ctx(newstate);
	}


}