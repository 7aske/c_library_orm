//
// Created by nik on 12/21/19.
//

#include "db/orm/entity.h"

uint region_insert(REGION* regionT) {
	#define QUERY_LENGTH 512
	#define QUERY "insert into region (name) values (?);"
	#define STRING_SIZE 255
	assert(strnlen(regionT->name, STRING_SIZE) > 1);

	int retval;
	MYSQL* __attribute__((cleanup(mysql_con_cleanup))) conn;
	MYSQL_STMT* __attribute__((cleanup(mysql_stmt_cleanup))) stmt;
	unsigned long len;
	MYSQL_BIND bind;

	conn = db_init();
	stmt = mysql_stmt_init(conn);
	len = strnlen(regionT->name, STRING_SIZE);

	memset(&bind, 0, sizeof(bind));

	bind.buffer = malloc(len);
	bind.buffer_type = MYSQL_TYPE_STRING;
	bind.buffer_length = len;
	strncpy(bind.buffer, regionT->name, len);

	mysql_stmt_prepare(stmt, QUERY, QUERY_LENGTH);
	mysql_stmt_bind_param(stmt, &bind);

	mysql_stmt_execute(stmt);

	retval = (uint) mysql_stmt_insert_id(stmt);
	// update id after insertion;
	regionT->id_region = retval;

	free(bind.buffer);
	return retval;
	#undef QUERY
	#undef QUERY_LENGTH
	#undef STRING_SIZE
}

SQL_RESULT* region_execute_find(char const* query, MYSQL_BIND* params, uint param_count) {
	#define QUERY_SIZE 512
	#define RES_COL_COUNT 2
	#define BUFFER_SIZE 255

	MYSQL* __attribute__((cleanup(mysql_con_cleanup))) conn;
	SQL_RESULT* res;
	MYSQL_BIND bind[RES_COL_COUNT];
	MYSQL_RES* prepare_meta_result;
	MYSQL_STMT* stmt;

	uint num_data;
	char buffer[BUFFER_SIZE];

	unsigned long lengths[RES_COL_COUNT];
	my_bool is_null[RES_COL_COUNT];
	my_bool error[RES_COL_COUNT];

	conn = db_init();
	stmt = mysql_stmt_init(conn);

	if (mysql_stmt_prepare(stmt, query, strnlen(query, QUERY_SIZE))) {
		fprintf(stderr, " mysql_stmt_prepare(), SELECT failed\n");
		fprintf(stderr, " %s\n", mysql_stmt_error(stmt));
		return NULL;
	}
	mysql_stmt_bind_param(stmt, params);
	assert(param_count == mysql_stmt_param_count(stmt));

	/* Fetch result set meta information */
	prepare_meta_result = mysql_stmt_result_metadata(stmt);
	if (!prepare_meta_result) {
		fprintf(stderr, " mysql_stmt_result_metadata(), returned no meta information\n");
		fprintf(stderr, " %s\n", mysql_stmt_error(stmt));
		return NULL;
	}
	assert(RES_COL_COUNT == mysql_num_fields(prepare_meta_result));

	/* Execute the SELECT query */
	if (mysql_stmt_execute(stmt)) {
		fprintf(stderr, " mysql_stmt_execute(), failed\n");
		fprintf(stderr, " %s\n", mysql_stmt_error(stmt));
		return NULL;
	}

	/* Bind the result buffers for all RES_COL_COUNT columns before fetching them */
	memset(bind, 0, sizeof(bind));
	/* INTEGER COLUMN */
	bind[0].buffer_type = MYSQL_TYPE_LONG;
	bind[0].buffer = &num_data;
	bind[0].is_null = &is_null[0];
	bind[0].length = &lengths[0];
	bind[0].error = &error[0];
	/* STRING COLUMN */
	bind[1].buffer_type = MYSQL_TYPE_STRING;
	bind[1].buffer = buffer;
	bind[1].buffer_length = BUFFER_SIZE;
	bind[1].is_null = &is_null[1];
	bind[1].length = &lengths[1];
	bind[1].error = &error[1];

	/* Bind the result buffers */
	if (mysql_stmt_bind_result(stmt, bind)) {
		fprintf(stderr, " mysql_stmt_bind_result() failed\n");
		fprintf(stderr, " %s\n", mysql_stmt_error(stmt));
		return NULL;
	}

	/* Now buffer all results to client (optional step) */
	if (mysql_stmt_store_result(stmt)) {
		fprintf(stderr, " mysql_stmt_store_result() failed\n");
		fprintf(stderr, " %s\n", mysql_stmt_error(stmt));
		return NULL;
	}

	/* Fetch all rows */
	struct sql_result_row* row = NULL;
	struct sql_result_row* curr = NULL;

	res = calloc(1, sizeof(SQL_RESULT));
	res->results = NULL;
	res->type = REGION_E;
	res->count = 0;

	while (!mysql_stmt_fetch(stmt)) {
		res->count++;
		row = calloc(1, sizeof(struct sql_result_row));
		if (res->results == NULL) {
			res->results = row;
		} else {
			curr = res->results;
			while (curr->next != NULL) {
				curr = curr->next;
			}
			curr->next = row;
		}
		row->data = calloc(1, sizeof(REGION));

		if (is_null[0]) {
			((REGION*) row->data)->id_region = 0;
		} else {
			((REGION*) row->data)->id_region = num_data;
		}

		if (is_null[1]) {
			strcpy(((REGION*) row->data)->name, "NULL");
		} else {
			strncpy(((REGION*) row->data)->name, buffer, lengths[1]);
		}
	}

	/* Free the prepared result metadata */
	mysql_free_result(prepare_meta_result);
	if (mysql_stmt_close(stmt)) {
		fprintf(stderr, " failed while closing the statement\n");
		fprintf(stderr, " %s\n", mysql_error(conn));
		mysql_res_free(&res);
		return NULL;
	}
	return res;
	#undef QUERY_SIZE
	#undef RES_COL_COUNT
	#undef PARAM_COUNT
	#undef BUFFER_SIZE
}

SQL_RESULT* region_find_by_name(char const* name) {
	#define BUFFER_SIZE 255
	#define PARAM_COUNT 1
	#define QUERY "select * from region where name like ?;"

	char query2[BUFFER_SIZE];
	SQL_RESULT* res;
	MYSQL_BIND param;
	ulong len;

	snprintf(query2, BUFFER_SIZE, "%%%s%%", name);

	len = strnlen(query2, BUFFER_SIZE);
	memset(&param, 0, sizeof(param));
	param.buffer = malloc(len);
	param.buffer_type = MYSQL_TYPE_STRING;
	param.buffer_length = len;
	strncpy(param.buffer, query2, len);

	res = region_execute_find(QUERY, &param, PARAM_COUNT);

	free(param.buffer);
	return res;
	#undef QUERY
	#undef BUFFER_SIZE
}

REGION* region_find_by_id(uint id) {
	#define QUERY "select * from region where id_region = ?;"
	#define PARAM_COUNT 1
	MYSQL_BIND param;
	REGION* out;
	SQL_RESULT* res;

	memset(&param, 0, sizeof(param));
	param.buffer = malloc(sizeof(uint));
	param.buffer_type = MYSQL_TYPE_LONG;
	memcpy(param.buffer, &id, sizeof(uint));

	res = region_execute_find(QUERY, &param, PARAM_COUNT);
	out = res->results->data;
	free(param.buffer);
	if (res->count == 1) {
		free(res->results);
		free(res);
		return out;
	} else {
		fprintf(stderr, "region_execute_find(), failed - multiple results (%d)\n", res->count);
		mysql_res_free(&res);
		return NULL;
	}
	#undef PARAM_COUNT
	#undef QUERY
}

int region_update(REGION* regionT) {
	assert(regionT->id_region != 0);
	#define QUERY "update region set name = ? WHERE id_region = ?;"
	#define PARAM_COUNT 2
	#define STRING_SIZE 255
	MYSQL_BIND param[PARAM_COUNT];
	ulong len;
	int retval;

	len = strnlen(regionT->name, STRING_SIZE);
	memset(&param, 0, sizeof(param));

	// STRING PARAM
	param[0].buffer = malloc(len);
	param[0].buffer_type = MYSQL_TYPE_STRING;
	param[0].buffer_length = len;
	strncpy(param[0].buffer, regionT->name, len);
	// INTEGER PARAM
	param[1].buffer = malloc(sizeof(uint));
	param[1].buffer_type = MYSQL_TYPE_LONG;
	memcpy(param[1].buffer, &regionT->id_region, sizeof(uint));

	retval = region_execute_update(QUERY, param, PARAM_COUNT);
	free(param[0].buffer);
	free(param[1].buffer);
	return retval;
}

int region_execute_update(char const* query, MYSQL_BIND* params, uint param_count) {
	#define QUERY_LENGTH 512
	MYSQL_STMT* stmt;
	MYSQL* __attribute__((cleanup(mysql_con_cleanup))) conn;
	int retval;

	conn = db_init();
	stmt = mysql_stmt_init(conn);

	if ((retval = mysql_stmt_prepare(stmt, query, strnlen(query, QUERY_LENGTH)))) {
		fprintf(stderr, "mysql_stmt_prepare(), failed\n");
		fprintf(stderr, "%s\n", mysql_error(conn));
		return retval;
	}

	if ((retval = mysql_stmt_bind_param(stmt, params))) {
		fprintf(stderr, "mysql_stmt_bind_param(), failed\n");
		fprintf(stderr, "%s\n", mysql_error(conn));
		return retval;
	}

	if ((retval = mysql_stmt_execute(stmt))) {
		fprintf(stderr, "mysql_stmt_execute(), failed\n");
		fprintf(stderr, "%s\n", mysql_error(conn));
		return retval;
	}

	if ((retval = mysql_stmt_close(stmt))) {
		fprintf(stderr, " failed while closing the statement\n");
		fprintf(stderr, " %s\n", mysql_error(conn));
		return retval;
	}
	#undef QUERY_LENGTH
}


