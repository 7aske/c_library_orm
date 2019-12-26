/**
 * Generated by nik on 26/12/2019
 */
#ifndef __nik_DB_ENTITY_STRUCT_EMPLOYEE_H
#define __nik_DB_ENTITY_STRUCT_EMPLOYEE_H

#pragma once


#include "db/orm/entity.h"

struct employee {
	uint id_employee;
	struct person* person;
	struct library* library;
	char position[32];
};

typedef struct employee EMPLOYEE;

uint employee_insert(EMPLOYEE* employeeT);

SQL_RESULT* employee_execute_find(char const* query, MYSQL_BIND* params, uint param_count);

EMPLOYEE* employee_find_by_id(uint id);

int employee_update(EMPLOYEE* employeeT);

int employee_execute(char const* query, MYSQL_BIND* params, uint param_count);

int employee_delete(EMPLOYEE* employeeT);

#endif
