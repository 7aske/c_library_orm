//
// Created by nik on 12/26/19.
//

#ifndef IT350_PZ_APP_STATE_H
#define IT350_PZ_APP_STATE_H

#pragma once

#include <ncurses.h>

#include "structs/arraylist.h"
#include "db/dbc.h"


static unsigned short APP_COL = 80;
static unsigned short APP_ROW = 24;

static short window_count = 0;

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
	ROOT_CTX = 0,
	WINDOW_CTX = 1,
	LIST_CTX = 2,
	FORM_CTX = 3,
	POPUP_CTX = 4
};

enum payload {
	NONE_PLOAD,
	FORM_PLOAD,
	POPUP_PLOAD
};

struct list_state {
	int sel_idx;
	int line_pos;
	alist_t* list;
	enum list_type type;
};

struct popup_state {
	char question[16];
	unsigned short answer;
};

struct state {
	char title[16];
	WINDOW* win;
	enum context ctx;

	union {
		struct list_state ls;
		struct popup_state ps;
	};
	struct state* parent;
	struct state* child;

	void* payload;
	enum payload ptype;
};

typedef struct state state_t;

typedef enum context ctx_e;

typedef enum list_type list_type_e;

#include "ui/util.h"

void init_state(state_t* state);

void delete_win(state_t* state);

state_t* create_win(state_t* parent, ctx_e ctx);

#endif //IT350_PZ_APP_STATE_H
