//
// Created by nik on 12/21/19.
//

#ifndef IT350_PZ_APP_ADDRESS_H
#define IT350_PZ_APP_ADDRESS_H

#pragma once

struct address {
	uint id_address;
	struct municipality* municipality;
	char street[255];
	char number[8];
};

typedef struct address ADDRESS;

extern uint address_insert(ADDRESS* addressT);

extern int address_update(ADDRESS* addressT);

extern int address_execute_update(char const* query, MYSQL_BIND* params, uint param_count);

extern SQL_RESULT* address_find_by_street(char const* street);

extern SQL_RESULT* address_find_by_street_and_number(char const* street, char const* number);

extern ADDRESS* address_find_by_id(uint id);

extern SQL_RESULT* address_execute_find(char const* query, MYSQL_BIND* params, uint param_count);

#endif //IT350_PZ_APP_ADDRESS_H
