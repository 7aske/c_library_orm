//
// Created by nik on 12/26/19.
//

#ifndef IT350_PZ_APP_STATE_H
#define IT350_PZ_APP_STATE_H

#pragma once

#include <ncurses.h>

#include "structs/arraylist.h"

#include "util.h"
#include "db/dbc.h"

static unsigned char APP_COL = 80;
static unsigned char APP_ROW = 24;

#define ETYPE_LEN 12

static const ENTITY_TYPE list_types[ETYPE_LEN] = {
		REGION_E,
		MUNICIPALITY_E,
		ADDRESS_E,
		LIBRARY_E,
		EMPLOYEE_E,
		PERSON_E,
		AUTHOR_E,
		AUTHOR_BOOK_E,
		BOOK_E,
		BOOK_SPECIMEN_E,
		READER_E,
		RENT_E
};

enum context {
	WINDOW_CTX,
	POPUP_CTX
};

struct state {
	WINDOW* win;
	int curr_sel_idx;
	int curr_line_pos;
	alist_t* curr_list;
	ENTITY_TYPE list_type;
	enum context ctx;
	struct state* child;
};

typedef struct state state_t;
typedef enum context ctx_e;

void init_state(state_t* state);

#endif //IT350_PZ_APP_STATE_H
