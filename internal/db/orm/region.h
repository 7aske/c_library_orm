/**
 * Generated by nik on 28/12/2019
 */
#ifndef __nik_LIBRARY_DB_ENTITY_STRUCT_REGION_H
#define __nik_LIBRARY_DB_ENTITY_STRUCT_REGION_H

#pragma once


#include "db/orm/entity.h"

struct region {
	uint id_region;
	char name[255];
};

typedef struct region REGION;

uint region_insert(REGION* regionT);

SQL_RESULT* region_execute_find(char const* query, MYSQL_BIND* params, uint param_count);

REGION* region_find_by_id(uint id);

int region_update(REGION* regionT);

int region_execute(char const* query, MYSQL_BIND* params, uint param_count);

int region_delete(REGION* regionT);

SQL_RESULT* region_find_all();

void region_free(REGION** ptr);

#endif
