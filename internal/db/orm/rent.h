/**
 * Generated by nik on 26/12/2019
 */
#ifndef __nik_DB_STRUCT_RENT_H
#define __nik_DB_STRUCT_RENT_H

struct rent {
	uint id_rent;
	struct reader* reader;
	struct book_specimen* book_specimen;
};

typedef struct rent RENT;

uint rent_insert(RENT* rentT);

SQL_RESULT* rent_execute_find(char const* query, MYSQL_BIND* params, uint param_count);

RENT* rent_find_by_id(uint id);

int rent_update(RENT* rentT);

int rent_execute(char const* query, MYSQL_BIND* params, uint param_count);

int rent_delete(RENT* rentT);

#endif
