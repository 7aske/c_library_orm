//
// Created by nik on 12/28/19.
//

#include "ui/util/util.h"


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



char* _fmt_date(struct tm* ts) {
	#define DATE_FMT "%02d-%02d-%04d"
	static char fmt[11];
	sprintf(fmt, DATE_FMT, ts->tm_mday, ts->tm_mon, ts->tm_year);
	return fmt;
	#undef DATE_FMT
}

