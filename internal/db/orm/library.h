/**
 * Generated by nik on 04/01/2020
 */
#ifndef __nik_LIBRARY_DB_ENTITY_STRUCT_LIBRARY_H
#define __nik_LIBRARY_DB_ENTITY_STRUCT_LIBRARY_H

#pragma once


#include "db/orm/entity.h"

struct library {
	uint id_library;
	struct address* address;
	char name[256];
};

typedef struct library LIBRARY;

uint library_insert(MYSQL* conn, LIBRARY* libraryT);

SQL_RESULT* library_execute_find(MYSQL* conn, char const* query, MYSQL_BIND* params, uint param_count);

LIBRARY* library_find_by_id(MYSQL* conn, uint id);

int library_update(MYSQL* conn, LIBRARY* libraryT);

int library_execute(MYSQL* conn, char const* query, MYSQL_BIND* params, uint param_count);

int library_delete(MYSQL* conn, LIBRARY* libraryT);

SQL_RESULT* library_find_all(MYSQL* conn);

void library_free(LIBRARY** ptr);

#endif
