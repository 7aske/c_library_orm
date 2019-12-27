//
// Created by nik on 12/26/19.
//

#ifndef IT350_PZ_APP_STATE_H
#define IT350_PZ_APP_STATE_H

#pragma once

#define APP_COL 80
#define APP_ROW 24

struct state {
	int curr_sel_idx;
	int curr_line_pos;
	SQL_RESULT* curr_list;
	ENTITY_TYPE list_types[4];
	int list_type;
};

typedef struct state state_t;


char const* list_type_str(ENTITY_TYPE type) {
	switch (type) {
		case ADDRESS_E:
			return "ADDRESS";
		case AUTHOR_E:
			return "AUTHOR";
		case AUTHOR_BOOK_E:
			return "AUTH BOOK";
		case BOOK_E:
			return "BOOK";
		case BOOK_SPECIMEN_E:
			return "BOOK SPEC";
		case EMPLOYEE_E:
			return "EMPLOYEE";
		case LIBRARY_E:
			return "LIBRARY";
		case MUNICIPALITY_E:
			return "MUNICIP";
		case PERSON_E:
			return "PERSON";
		case READER_E:
			return "READER";
		case REGION_E:
			return "REGION";
		case RENT_E:
			return "RENT";
	}
}


#endif //IT350_PZ_APP_STATE_H
