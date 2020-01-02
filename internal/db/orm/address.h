/**
 * Generated by nik on 02/01/2020
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

uint address_insert(MYSQL* conn, ADDRESS* addressT);

SQL_RESULT* address_execute_find(MYSQL* conn, char const* query, MYSQL_BIND* params, uint param_count);

ADDRESS* address_find_by_id(MYSQL* conn, uint id);

int address_update(MYSQL* conn, ADDRESS* addressT);

int address_execute(MYSQL* conn, char const* query, MYSQL_BIND* params, uint param_count);

int address_delete(MYSQL* conn, ADDRESS* addressT);

SQL_RESULT* address_find_all(MYSQL* conn);

void address_free(ADDRESS** ptr);

#endif
