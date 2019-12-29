#include "ui/state.h"

void init_state(state_t* state, ctx_e ctx) {
	state->win = NULL;
	state->curr_list = NULL;
	state->curr_sel_idx = 0;
	state->curr_line_pos = 1;
	state->list_type = REGION_TYPE;
	state->ctx = ctx;
	state->child = NULL;
}


state_t* create_popup(state_t* state) {
	state_t* newstate = calloc(1, sizeof(state_t));
	int row, col;
	getmaxyx(state->win, row, col);
	newstate->win = newwin(row - 3, col - 2, 2, 1);
	newstate->ctx = WINDOW_CTX;
	newstate->list_type = REGION_TYPE;
	newstate->curr_list = NULL;
	newstate->curr_line_pos = 1;
	newstate->curr_sel_idx = 0;
	newstate->child = NULL;
	state->child = newstate;
	return newstate;
}