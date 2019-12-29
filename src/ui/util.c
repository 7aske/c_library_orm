//
// Created by nik on 12/28/19.
//

#include "ui/util.h"


const char * list_type_str(ENTITY_TYPE type) {
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


const char* _fmt_date(struct tm* ts) {
	#define DATE_FMT "%02d-%02d-%04d"
	static char fmt[11];
	sprintf(fmt, DATE_FMT, ts->tm_mday, ts->tm_mon, ts->tm_year);
	return fmt;
	#undef DATE_FMT
}

void res_to_list(SQL_RESULT* res, alist_t* list) {
	assert(res != NULL);
	assert(list != NULL);
	struct sql_result_row* curr;
	curr = res->results;
	while (curr != NULL) {
		assert(curr->data != NULL);
		alist_add(list, curr->data);
		curr = curr->next;
	}
	// clean memory without freeing references within structures
	// they will be used by newly copied elements in the list
	mysql_res_free_noref(&res);
}

void type_free(void* elem, ENTITY_TYPE list_type){
	switch (list_type) {
		case MUNICIPALITY_E:
			municipality_free((MUNICIPALITY**) &elem);
			break;
		case ADDRESS_E:
			address_free((ADDRESS**) &elem);
			break;
		case REGION_E:
			region_free((REGION**) &elem);
			break;
		case LIBRARY_E:
			library_free((LIBRARY**) &elem);
			break;
		case AUTHOR_E:
			author_free((AUTHOR**) &elem);
			break;
		case AUTHOR_BOOK_E:
			author_book_free((AUTHOR_BOOK**) &elem);
			break;
		case BOOK_E:
			book_free((BOOK**) &elem);
			break;
		case BOOK_SPECIMEN_E:
			book_specimen_free((BOOK_SPECIMEN**) &elem);
			break;
		case EMPLOYEE_E:
			employee_free((EMPLOYEE**) &elem);
			break;
		case PERSON_E:
			person_free((PERSON**) &elem);
			break;
		case READER_E:
			reader_free((READER**) &elem);
			break;
		case RENT_E:
			rent_free((RENT**) &elem);
			break;
	}
}