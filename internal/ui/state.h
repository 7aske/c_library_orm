//
// Created by nik on 12/26/19.
//

#ifndef IT350_PZ_APP_STATE_H
#define IT350_PZ_APP_STATE_H

#pragma once

#include <ncurses.h>

#include "structs/arraylist.h"
#include "db/dbc.h"

static unsigned char APP_COL = 80;
static unsigned char APP_ROW = 24;

#define ETYPE_LEN 12

enum list_type {
	REGION_TYPE = 0,
	MUNICIPALITY_TYPE = 1,
	ADDRESS_TYPE = 2,
	LIBRARY_TYPE = 3,
	EMPLOYEE_TYPE = 4,
	PERSON_TYPE = 5,
	AUTHOR_TYPE = 6,
	AUTHOR_BOOK_TYPE = 7,
	BOOK_TYPE = 8,
	BOOK_SPECIMEN_TYPE = 9,
	READER_TYPE = 10,
	RENT_TYPE = 11,
};

static const enum list_type list_types[ETYPE_LEN] = {
		REGION_TYPE,
		MUNICIPALITY_TYPE,
		ADDRESS_TYPE,
		LIBRARY_TYPE,
		EMPLOYEE_TYPE,
		PERSON_TYPE,
		AUTHOR_TYPE,
		AUTHOR_BOOK_TYPE,
		BOOK_TYPE,
		BOOK_SPECIMEN_TYPE,
		READER_TYPE,
		RENT_TYPE
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
	enum list_type list_type;
	enum context ctx;
	struct state* child;
};

typedef struct state state_t;

typedef enum context ctx_e;

typedef enum list_type list_type_e;

void init_state(state_t* state);

#endif //IT350_PZ_APP_STATE_H
