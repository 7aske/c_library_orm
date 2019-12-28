//
// Created by nik on 12/28/19.
//

#ifndef IT350_PZ_APP_UTIL_H
#define IT350_PZ_APP_UTIL_H

#pragma once

#include <time.h>
#include <stdio.h>

#include "db/sql_result.h"

char const* list_type_str(ENTITY_TYPE type);

char* _fmt_date(struct tm* ts);

#endif //IT350_PZ_APP_UTIL_H
