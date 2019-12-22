//
// Created by nik on 12/21/19.
//

#ifndef IT350_PZ_APP_REGION_H
#define IT350_PZ_APP_REGION_H

#pragma once

#include "db/sql_result.h"

struct region {
	uint id_region;
	char name[255];
};

typedef struct region REGION;

extern uint region_insert(REGION* regionT);

extern int region_update(REGION* regionT);

extern int region_execute_update(char const* query, MYSQL_BIND* params, uint param_count);

extern SQL_RESULT* region_find_by_name(char const* name);

extern REGION* region_find_by_id(uint id);

extern SQL_RESULT* region_execute_find(char const* query, MYSQL_BIND* params, uint param_count);

#endif //IT350_PZ_APP_REGION_H
