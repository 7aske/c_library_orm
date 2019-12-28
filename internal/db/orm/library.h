/**
 * Generated by nik on 28/12/2019
 */
#ifndef __nik_LIBRARY_DB_ENTITY_STRUCT_LIBRARY_H
#define __nik_LIBRARY_DB_ENTITY_STRUCT_LIBRARY_H

#pragma once


#include "db/orm/entity.h"

struct library {
	uint id_library;
	struct address* address;
	char name[255];
};

typedef struct library LIBRARY;

uint library_insert(LIBRARY* libraryT);

SQL_RESULT* library_execute_find(char const* query, MYSQL_BIND* params, uint param_count);

LIBRARY* library_find_by_id(uint id);

int library_update(LIBRARY* libraryT);

int library_execute(char const* query, MYSQL_BIND* params, uint param_count);

int library_delete(LIBRARY* libraryT);

SQL_RESULT* library_find_all();

#endif
