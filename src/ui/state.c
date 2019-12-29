
#include "ui/state.h"

void init_state(state_t* state) {
	static const char* title = "APP";
	strncpy(state->title, title, 16);
	state->win = NULL;
	state->child = NULL;
	state->parent = NULL;
	state->ctx = ROOT_CTX;
}

void delete_win(state_t* state) {
	assert(state != NULL);
	delwin(state->win);
	state->win = NULL;
	window_count--;
	if (state->ctx == LIST_CTX && state->ls.list != NULL) {
		list_free_noref(&state->ls.list, state->ls.type);
	}
	if (state->parent) {
		free(state);
		state->parent->child = NULL;
		wclear(state->parent->win);
	}
}

state_t* create_win(state_t* parent, ctx_e ctx) {
	#define BUF_LEN 16
	char buf[BUF_LEN];
	int row, col;
	getmaxyx(parent->win, row, col);

	state_t* newstate = calloc(1, sizeof(state_t));

	snprintf(buf, BUF_LEN, "WINDOW%d", ++window_count);
	strncpy(newstate->title, buf, BUF_LEN);

	newstate->win = newwin(row, col, 0, 0);
	keypad(newstate->win, TRUE);

	newstate->ctx = ctx;
	newstate->child = NULL;
	newstate->parent = parent;


	newstate->ls.type = REGION_TYPE;
	newstate->ls.list = NULL;
	newstate->ls.line_pos = 1;
	newstate->ls.sel_idx = 0;

	return newstate;
}