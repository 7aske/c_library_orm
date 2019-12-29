//
// Created by nik on 12/28/19.
//

#include "ui/util.h"


const char* list_type_str(list_type_e type) {
	switch (type) {
		case ADDRESS_TYPE:
			return "ADDR";
		case AUTHOR_TYPE:
			return "ATHR";
		case AUTHOR_BOOK_TYPE:
			return "ATBK";
		case BOOK_TYPE:
			return "BOOK";
		case BOOK_SPECIMEN_TYPE:
			return "BKSP";
		case EMPLOYEE_TYPE:
			return "EMPL";
		case LIBRARY_TYPE:
			return "LIBR";
		case MUNICIPALITY_TYPE:
			return "MUNI";
		case PERSON_TYPE:
			return "PERS";
		case READER_TYPE:
			return "READ";
		case REGION_TYPE:
			return "REGI";
		case RENT_TYPE:
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

void type_free(void* elem, list_type_e list_type) {
	switch (list_type) {
		case MUNICIPALITY_TYPE:
			municipality_free((MUNICIPALITY**) &elem);
			break;
		case ADDRESS_TYPE:
			address_free((ADDRESS**) &elem);
			break;
		case REGION_TYPE:
			region_free((REGION**) &elem);
			break;
		case LIBRARY_TYPE:
			library_free((LIBRARY**) &elem);
			break;
		case AUTHOR_TYPE:
			author_free((AUTHOR**) &elem);
			break;
		case AUTHOR_BOOK_TYPE:
			author_book_free((AUTHOR_BOOK**) &elem);
			break;
		case BOOK_TYPE:
			book_free((BOOK**) &elem);
			break;
		case BOOK_SPECIMEN_TYPE:
			book_specimen_free((BOOK_SPECIMEN**) &elem);
			break;
		case EMPLOYEE_TYPE:
			employee_free((EMPLOYEE**) &elem);
			break;
		case PERSON_TYPE:
			person_free((PERSON**) &elem);
			break;
		case READER_TYPE:
			reader_free((READER**) &elem);
			break;
		case RENT_TYPE:
			rent_free((RENT**) &elem);
			break;
	}
}

void type_free_ref(void* elem, list_type_e list_type) {
	switch (list_type) {
		case MUNICIPALITY_TYPE:
			region_free(&((MUNICIPALITY*) elem)->region);
			break;
		case ADDRESS_TYPE:
			municipality_free(&((ADDRESS*) elem)->municipality);
			break;
		case LIBRARY_TYPE:
			address_free(&((LIBRARY*) elem)->address);
			break;
		case AUTHOR_TYPE:
			person_free(&((AUTHOR*) elem)->person);
			break;
		case AUTHOR_BOOK_TYPE:
			book_free(&((AUTHOR_BOOK*) elem)->book);
			author_free(&((AUTHOR_BOOK*) elem)->author);
			break;
		case BOOK_SPECIMEN_TYPE:
			book_free(&((BOOK_SPECIMEN*) elem)->book);
			library_free(&((BOOK_SPECIMEN*) elem)->library);
			break;
		case EMPLOYEE_TYPE:
			library_free(&((EMPLOYEE*) elem)->library);
			person_free(&((EMPLOYEE*) elem)->person);
			break;
		case RENT_TYPE:
			book_specimen_free(&((RENT*) elem)->book_specimen);
			reader_free(&((RENT*) elem)->reader);
			break;
		default:
			break;
	}
}