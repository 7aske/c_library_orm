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
	ENTITY_TYPE list_types[12];
	int list_type;
};

typedef struct state state_t;


char const* list_type_str(ENTITY_TYPE type) {
	switch (type) {
		case ADDRESS_E:
			return "ADDR";
		case AUTHOR_E:
			return "ATHR";
		case AUTHOR_BOOK_E:
			return "ATBK";
		case BOOK_E:
			return "BOOK";
		case BOOK_SPECIMEN_E:
			return "BKSP";
		case EMPLOYEE_E:
			return "EMPL";
		case LIBRARY_E:
			return "LIBR";
		case MUNICIPALITY_E:
			return "MUNI";
		case PERSON_E:
			return "PERS";
		case READER_E:
			return "READ";
		case REGION_E:
			return "REGI";
		case RENT_E:
			return "RENT";
	}
}


#endif //IT350_PZ_APP_STATE_H