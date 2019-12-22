//
// Created by nik on 12/20/19.
//

#ifndef IT350_PZ_APP_DBC_H
#define IT350_PZ_APP_DBC_H

#define SQL_ADDR "127.0.0.1"
// #define SQL_ADDR "192.168.1.202"
#define SQL_USER "nik"
#define SQL_PASS "nik"
#define SQL_DB   "library"
#define SQL_PORT 3306

#pragma once

#include <stdlib.h>
#include <assert.h>
#include <stdio.h>

#include <mysql/mysql.h>

#include "db/orm/entity.h"
#include "db/sql_result.h"

extern MYSQL*
db_init();

extern void __attribute__((used))
mysql_con_cleanup(MYSQL** conn);

extern void __attribute__((used))
mysql_res_cleanup(MYSQL_RES** res);

extern void __attribute__((used))
mysql_stmt_cleanup(MYSQL_STMT** stmt);

extern void __attribute__((noreturn))
mysql_panic(MYSQL* c);

extern void __attribute__((used))
mysql_bind_free(MYSQL_BIND* bind);

extern void __attribute__((used))
mysql_bind_cleanup(MYSQL_BIND** bind);

extern void __attribute__((used))
mysql_res_free(SQL_RESULT** res);

#endif //IT350_PZ_APP_DBC_H
