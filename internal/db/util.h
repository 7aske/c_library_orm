//
// Created by nik on 12/26/19.
//

#ifndef IT350_PZ_APP_UTIL_H
#define IT350_PZ_APP_UTIL_H

#pragma once

#include <mysql/mysql.h>
#include <time.h>
#include <stdio.h>

extern void mysql_timecpy(MYSQL_TIME* mysqlTime, struct tm* ts);

extern void mysql_timecpystr(struct tm* ts, char* sqlstr);

#endif //IT350_PZ_APP_UTIL_H
