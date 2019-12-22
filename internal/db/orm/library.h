//
// Created by nik on 12/21/19.
//

#ifndef IT350_PZ_APP_LIBRARY_H
#define IT350_PZ_APP_LIBRARY_H

#pragma once

struct library {
	uint id_library;
	struct address* address;
	char name[255];
};

typedef struct library LIBRARY;

extern uint library_insert(LIBRARY* libraryT);

extern int library_update(LIBRARY* libraryT);

extern int library_execute_update(char const* query, MYSQL_BIND* params, uint param_count);

extern LIBRARY* library_find_by_id(uint id);

extern SQL_RESULT* library_execute_find(char const* query, MYSQL_BIND* params, uint param_count);

#endif //IT350_PZ_APP_LIBRARY_H
