//
// Created by nik on 12/28/19.
//

#ifndef IT350_PZ_APP_UTIL_H
#define IT350_PZ_APP_UTIL_H

#pragma once

#include <time.h>
#include <stdio.h>

#include "db/dbc.h"
#include "ui/state.h"
#include "structs/arraylist.h"

const char* list_type_str(list_type_e type);

void res_to_list(SQL_RESULT* res, alist_t* list);

const char* _fmt_date(struct tm* ts);

void type_free(void* elem, list_type_e list_type);

void type_free_ref(void* elem, list_type_e list_type);

void list_free_noref(alist_t** list, list_type_e type);

#endif //IT350_PZ_APP_UTIL_H
