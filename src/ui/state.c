#include "ui/state.h"

void init_state(state_t* state) {
	state->win = NULL;
	state->curr_list = NULL;
	state->curr_sel_idx = 0;
	state->curr_line_pos = 1;
	state->list_type = REGION_TYPE;
	state->ctx = WINDOW_CTX;
	state->child = NULL;
}

