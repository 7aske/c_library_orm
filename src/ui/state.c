
#include "ui/state.h"

void init_state(state_t* state, ctx_e ctx) {
	static const char* title = "APP";
	strncpy(state->title, title, 16);
	state->win = NULL;
	state->curr_list = NULL;
	state->curr_sel_idx = 0;
	state->curr_line_pos = 1;
	state->list_type = REGION_TYPE;
	state->ctx = ctx;
	state->child = NULL;
}

void delete_win(state_t* state, state_t* par, WINDOW* win) {
	delwin(win);
	state->win = NULL;
	window_count--;
	if (state->curr_list != NULL) {
		list_free_noref(&state->curr_list, state->list_type);
	}
	if (par && par->child != NULL) {
		free(par->child);
		par->child = NULL;
	}
}

state_t* create_win(state_t* state) {
	#define BUF_LEN 16
	char buf[BUF_LEN];
	state_t* newstate = calloc(1, sizeof(state_t));
	int row, col;
	getmaxyx(state->win, row, col);

	snprintf(buf, BUF_LEN, "WINDOW%d", ++window_count);
	strncpy(newstate->title, buf, BUF_LEN);

	newstate->win = newwin(row, col, 0, 0);
	keypad(newstate->win, TRUE);
	newstate->ctx = WINDOW_CTX;
	newstate->list_type = REGION_TYPE;
	newstate->curr_list = NULL;
	newstate->curr_line_pos = 1;
	newstate->curr_sel_idx = 0;
	newstate->child = NULL;
	// state->child = newstate;
	return newstate;
}