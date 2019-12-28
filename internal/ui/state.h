//
// Created by nik on 12/26/19.
//

#ifndef IT350_PZ_APP_STATE_H
#define IT350_PZ_APP_STATE_H

#pragma once

#include <ncurses.h>

#include "structs/arraylist.h"

#include "ui/util/util.h"
#include "db/dbc.h"

static unsigned char APP_COL =  80;
static unsigned char APP_ROW =  24;

struct state {
	int curr_sel_idx;
	int curr_line_pos;
	SQL_RESULT* curr_list;
	ENTITY_TYPE list_types[12];
	int list_type;
	WINDOW* pwin;
};

typedef struct state state_t;


void init_state(state_t* state);

#endif //IT350_PZ_APP_STATE_H
