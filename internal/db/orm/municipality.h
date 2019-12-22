//
// Created by nik on 12/21/19.
//

#ifndef IT350_PZ_APP_MUNICIPALITY_H
#define IT350_PZ_APP_MUNICIPALITY_H

#pragma once

struct municipality {
	uint id_municipality;
	char name[255];
	struct region* region;
};

typedef struct municipality MUNICIPALITY;

extern uint municipality_insert(MUNICIPALITY* municipalityT);

extern int municipality_update(MUNICIPALITY* municipalityT);

extern int municipality_execute_update(char const* query, MYSQL_BIND* params, uint param_count);

extern SQL_RESULT* municipality_execute_find(char const* query, MYSQL_BIND* params, uint param_count);

extern SQL_RESULT* municipality_find_by_name(char const* name);

extern MUNICIPALITY* municipality_find_by_id(uint id);

#endif //IT350_PZ_APP_MUNICIPALITY_H
