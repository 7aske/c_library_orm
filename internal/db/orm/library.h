//
// Created by nik on 12/21/19.
//
#ifndef STRUCT_LIBRARY_H
#define STRUCT_LIBRARY_H

struct library {
	uint id_library;
	struct address* address;
	char name[255];
};

typedef struct library LIBRARY;

uint library_insert(LIBRARY* libraryT);

LIBRARY* library_find_by_id(uint id);

SQL_RESULT* library_execute_find(char const* query, MYSQL_BIND* params, uint param_count);

int library_update(LIBRARY* libraryT);

int library_execute_update(char const* query, MYSQL_BIND* params, uint param_count);

int library_delete(LIBRARY* libraryT);

#endif

