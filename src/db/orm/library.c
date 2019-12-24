//
// Created by nik on 12/22/19.
//

#include "db/orm/entity.h"


/* Generated function */
uint region_insert(REGION* regionT) {
	#define QUERY_LENGTH 512
	#define STRING_SIZE 255
	#define QUERY "insert into region (name) values (?);"
	#define PARAM_COUNT 1
	#define NAME_SIZE 255
	/* Generated using get_insert_assertions() */
	assert(regionT->id_region == 0);
	assert(strnlen(regionT->name, STRING_SIZE) > 1);


	MYSQL* __attribute__((cleanup(mysql_con_cleanup))) conn;
	MYSQL_STMT* __attribute__((cleanup(mysql_stmt_cleanup))) stmt;
	uint retval;


	conn = db_init();
	stmt = mysql_stmt_init(conn);

	/* Generated using col_param_lengths() */
	MYSQL_BIND param[PARAM_COUNT];
	memset(&param, 0, sizeof(param));

	unsigned long name_len;
	name_len = strnlen(regionT->name, NAME_SIZE);


	/* Generated using  get_col_param_buffers() */

	/* STRING PARAM */
	param[0].buffer = malloc(name_len);
	param[0].buffer_type = MYSQL_TYPE_STRING;
	param[0].buffer_length = name_len;
	strncpy(param[0].buffer, regionT->name, name_len);

	/* Generated using get_update_fk() */


	if (mysql_stmt_prepare(stmt, QUERY, QUERY_LENGTH)) {
		fprintf(stderr, " mysql_stmt_prepare(), failed\n");
		fprintf(stderr, " %s\n", mysql_stmt_error(stmt));
		return 0U;
	}

	if (mysql_stmt_bind_param(stmt, param)) {
		fprintf(stderr, " mysql_stmt_bind_param(), failed\n");
		fprintf(stderr, " %s\n", mysql_stmt_error(stmt));
		return 0U;
	}

	if (mysql_stmt_execute(stmt)) {
		fprintf(stderr, " mysql_stmt_execute(), failed\n");
		fprintf(stderr, " %s\n", mysql_stmt_error(stmt));
		return 0U;
	}

	retval = (uint) mysql_stmt_insert_id(stmt);
	// update id after insertion;

	regionT->id_region = retval;

	/* Generated using col_param_buffer_free() */
	free(param[0].buffer);


	return retval;

	#undef QUERY_LENGTH
	#undef STRING_SIZE
	#undef QUERY
	#undef PARAM_COUNT
	#undef NAME_SIZE
}


/* Generated function */
SQL_RESULT* region_execute_find(char const* query, MYSQL_BIND* params, uint param_count) {
	#define QUERY_SIZE 512
	#define RES_COL_COUNT 2
	#define BUFFER_SIZE 255
	MYSQL* __attribute__((cleanup(mysql_con_cleanup))) conn;
	SQL_RESULT* res;
	MYSQL_RES* prepare_meta_result;
	MYSQL_STMT* stmt;

	/* Generated using get_col_buffer_definitions()*/
	unsigned long lengths[RES_COL_COUNT];
	my_bool is_null[RES_COL_COUNT];
	my_bool error[RES_COL_COUNT];
	uint id_region_buffer;
	char name_buffer[BUFFER_SIZE];


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

	/* Generated using get_buffer_bindings()*/
	MYSQL_BIND param[RES_COL_COUNT];
	memset(param, 0, sizeof(param));

	/* INTEGER COLUMN */
	param[0].buffer_type = MYSQL_TYPE_LONG;
	param[0].buffer = &id_region_buffer;
	param[0].is_null = &is_null[0];
	param[0].length = &lengths[0];
	param[0].error = &error[0];

	/* STRING COLUMN */
	param[1].buffer_type = MYSQL_TYPE_STRING;
	param[1].buffer = &name_buffer;
	param[1].is_null = &is_null[1];
	param[1].length = &lengths[1];
	param[1].error = &error[1];
	param[1].buffer_length = 255;


	/* Bind the result buffers */
	if (mysql_stmt_bind_result(stmt, param)) {
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


	/* Generated using col_fetch()*/
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
			((REGION*) row->data)->id_region = id_region_buffer;
		}
		if (is_null[1]) {
			strcpy(((REGION*) row->data)->name, "NULL");
		} else {
			strncpy(((REGION*) row->data)->name, name_buffer, lengths[1]);
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
	#undef BUFFER_SIZE
}


/* Generated function */
REGION* region_find_by_id(uint id) {
	#define QUERY "select * from region where id_region = ?;"
	#define PARAM_COUNT 1
	REGION* out;


	SQL_RESULT* res;
	struct region region;
	region.id_region = id;
	struct region* regionT = &region;


	/* Generated using  get_col_param_buffers() */
	MYSQL_BIND param[PARAM_COUNT];
	memset(param, 0, sizeof(param));

	/* INTEGER PARAM */
	param[0].buffer = malloc(sizeof(uint));
	param[0].buffer_type = MYSQL_TYPE_LONG;
	memcpy(param[0].buffer, &regionT->id_region, sizeof(uint));

	res = region_execute_find(QUERY, param, PARAM_COUNT);

	/* Generated using col_param_buffer_free() */
	free(param[0].buffer);


	out = res->results->data;
	if (res->count == 1) {
		free(res->results);
		free(res);
		return out;
	} else {
		fprintf(stderr, "region_execute_find(), failed - multiple results (%d)\n", res->count);
		mysql_res_free(&res);
		return NULL;
	}

	#undef QUERY
	#undef PARAM_COUNT
}


/* Generated function */
int region_update(REGION* regionT) {
	#define QUERY "update region set name = ? where id_region = ?;"
	#define PARAM_COUNT 2
	#define STRING_SIZE 255
	#define NAME_SIZE 255
	assert(regionT->id_region != 0);

	int retval;

	/* Generated using col_update_params() */
	MYSQL_BIND param[PARAM_COUNT];
	memset(&param, 0, sizeof(param));

	unsigned long name_len;
	name_len = strnlen(regionT->name, NAME_SIZE);

	/* STRING PARAM */
	param[0].buffer = malloc(name_len);
	param[0].buffer_type = MYSQL_TYPE_STRING;
	param[0].buffer_length = name_len;
	strncpy(param[0].buffer, regionT->name, name_len);
	/* INTEGER PARAM */
	param[1].buffer = malloc(sizeof(uint));
	param[1].buffer_type = MYSQL_TYPE_LONG;
	memcpy(param[1].buffer, &regionT->id_region, sizeof(uint));

	retval = region_execute_update(QUERY, param, PARAM_COUNT);

	/* Generated using col_buffer_free() */
	free(param[0].buffer);
	free(param[1].buffer);


	return retval;

	#undef QUERY
	#undef PARAM_COUNT
	#undef STRING_SIZE
	#undef NAME_SIZE
}


/* Generated function */
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
