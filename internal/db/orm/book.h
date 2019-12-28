/**
 * Generated by nik on 28/12/2019
 */
#ifndef __nik_LIBRARY_DB_ENTITY_STRUCT_BOOK_H
#define __nik_LIBRARY_DB_ENTITY_STRUCT_BOOK_H

#pragma once


#include "db/orm/entity.h"

struct book {
	uint id_book;
	char isbn[32];
	char name[255];
	struct tm publish_date;
};

typedef struct book BOOK;

uint book_insert(BOOK* bookT);

SQL_RESULT* book_execute_find(char const* query, MYSQL_BIND* params, uint param_count);

BOOK* book_find_by_id(uint id);

int book_update(BOOK* bookT);

int book_execute(char const* query, MYSQL_BIND* params, uint param_count);

int book_delete(BOOK* bookT);

SQL_RESULT* book_find_all();

void book_free(BOOK** ptr);

#endif
