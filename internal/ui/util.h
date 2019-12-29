//
// Created by nik on 12/28/19.
//

#ifndef IT350_PZ_APP_UTIL_H
#define IT350_PZ_APP_UTIL_H

#pragma once

#include <time.h>
#include <stdio.h>

#include "db/dbc.h"
#include "structs/arraylist.h"

const char* list_type_str(ENTITY_TYPE type);

void res_to_list(SQL_RESULT* res, alist_t* list);

const char* _fmt_date(struct tm* ts);

void type_free(void* elem, ENTITY_TYPE list_type);

#endif //IT350_PZ_APP_UTIL_H
