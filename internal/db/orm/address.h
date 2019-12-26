/**
 * Generated by nik on 26/12/2019
 */
#ifndef __nik_LIBRARY_DB_ENTITY_STRUCT_ADDRESS_H
#define __nik_LIBRARY_DB_ENTITY_STRUCT_ADDRESS_H

#pragma once


#include "db/orm/entity.h"

struct address {
	uint id_address;
	struct municipality* municipality;
	char street[255];
	char number[8];
};

typedef struct address ADDRESS;

uint address_insert(ADDRESS* addressT);

SQL_RESULT* address_execute_find(char const* query, MYSQL_BIND* params, uint param_count);

ADDRESS* address_find_by_id(uint id);

int address_update(ADDRESS* addressT);

int address_execute(char const* query, MYSQL_BIND* params, uint param_count);

int address_delete(ADDRESS* addressT);

SQL_RESULT* address_find_all();

#endif
