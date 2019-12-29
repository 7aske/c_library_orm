//
// Created by nik on 12/29/19.
//

#include "ui/input.h"

void root_ctx_handler(state_t* state, int input, volatile int* running) {
	switch (input) {
		case 'q':
			*running = false;
			break;
		case 'l':
			state->child = create_state_ctx(state, LIST_CTX);
			change_list(state->child, 0);
			break;
	}
}

void list_ctx_handler(state_t* state, int input) {
	switch (input) {
		case 'q':
			delete_state_ctx(state);
			break;
		case KEY_UP:
			if (state->ls.line_pos - 1 > 0) {
				state->ls.line_pos--;
			}
			if (state->ls.sel_idx > 0) {
				state->ls.sel_idx--;
			}
			break;
		case KEY_DOWN:
			if (state->ls.line_pos + 1 < APP_ROW - 2) {
				state->ls.line_pos++;
			}
			if (state->ls.sel_idx < alist_size(state->ls.list) - 1) {
				state->ls.sel_idx++;
			}
			break;
		case KEY_LEFT:
			if (state->ls.type >= 0) {
				change_list(state, -1);
				state->ls.line_pos = 0;
				state->ls.sel_idx = 0;
			}
			clear();
			break;
		case KEY_RIGHT:
			if (state->ls.type < ETYPE_LEN) {
				change_list(state, 1);
				state->ls.line_pos = 0;
				state->ls.sel_idx = 0;
			}
			clear();
			break;
		case 'l':
			state->child = create_state_ctx(state, LIST_CTX);
			change_list(state->child, 0);
			break;
		default:
			break;
	}
}

void form_ctx_handler(state_t* state, int input) {

}
