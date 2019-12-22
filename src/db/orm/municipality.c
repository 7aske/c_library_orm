//
// Created by nik on 12/21/19.
//

#include "db/orm/entity.h"

uint municipality_insert(MUNICIPALITY* municipalityT) {
	#define STRING_SIZE 255
	#define QUERY_SIZE 512
	#define QUERY "insert into municipality (name, id_region) values (?, ?);"
	#define PARAM_COUNT 2
	assert(strnlen(municipalityT->name, STRING_SIZE) > 1);
	assert(municipalityT->region != NULL);

	MYSQL* __attribute__((cleanup(mysql_con_cleanup))) conn;
	MYSQL_STMT* __attribute__((cleanup(mysql_stmt_cleanup))) stmt;
	MYSQL_BIND bind[PARAM_COUNT];
	unsigned long len;
	uint region_fk;
	int retval;

	conn = db_init();
	stmt = mysql_stmt_init(conn);
	len = strnlen(municipalityT->name, STRING_SIZE);

	if (municipalityT->region->id_region == 0) {
		region_insert(municipalityT->region);
	}

	region_fk = municipalityT->region->id_region;


	memset(&bind, 0, sizeof(bind));

	bind[0].buffer = malloc(len);
	bind[0].buffer_type = MYSQL_TYPE_STRING;
	bind[0].buffer_length = len;
	strncpy(bind[0].buffer, municipalityT->name, len);

	bind[1].buffer = &region_fk;
	bind[1].buffer_type = MYSQL_TYPE_LONG;

	mysql_stmt_prepare(stmt, QUERY, QUERY_SIZE);
	mysql_stmt_bind_param(stmt, bind);

	mysql_stmt_execute(stmt);

	retval = (uint) mysql_stmt_insert_id(stmt);
	// update id after insertion;
	municipalityT->id_municipality = retval;

	free(bind[0].buffer);
	return retval;
	#undef STRING_SIZE
	#undef PARAM_COUNT
	#undef QUERY_SIZE
	#undef QUERY
}

SQL_RESULT* municipality_execute_find(char const* query, MYSQL_BIND* params, uint param_count) {
	#define BUFFER_SIZE 255
	#define QUERY_SIZE 512
	#define RES_COL_COUNT 3

	MYSQL* __attribute__((cleanup(mysql_con_cleanup))) conn;
	SQL_RESULT* res;

	MYSQL_BIND bind[RES_COL_COUNT];
	MYSQL_RES* prepare_meta_result;
	MYSQL_STMT* stmt;

	uint id_municipality;
	uint id_region;
	char buffer[BUFFER_SIZE];

	unsigned long lengths[RES_COL_COUNT];
	my_bool is_null[RES_COL_COUNT];
	my_bool error[RES_COL_COUNT];

	conn = db_init();
	stmt = mysql_stmt_init(conn);

	if (mysql_stmt_prepare(stmt, query, strnlen(query, QUERY_SIZE))) {
		fprintf(stderr, "mysql_stmt_prepare(), SELECT failed\n");
		fprintf(stderr, "%s\n", mysql_error(conn));
		return NULL;
	}

	mysql_stmt_bind_param(stmt, params);
	assert(param_count == mysql_stmt_param_count(stmt));

	prepare_meta_result = mysql_stmt_result_metadata(stmt);
	if (!prepare_meta_result) {
		fprintf(stderr, " mysql_stmt_result_metadata(), returned no meta information\n");
		fprintf(stderr, " %s\n", mysql_stmt_error(stmt));
		return NULL;
	}
	assert(RES_COL_COUNT == mysql_num_fields(prepare_meta_result));

	if (mysql_stmt_execute(stmt)) {
		fprintf(stderr, " mysql_stmt_execute(), failed\n");
		fprintf(stderr, " %s\n", mysql_stmt_error(stmt));
		return NULL;
	}

	memset(bind, 0, sizeof(bind));

	/* INTEGER COLUMN */
	bind[0].buffer_type = MYSQL_TYPE_LONG;
	bind[0].buffer = &id_municipality;
	bind[0].is_null = &is_null[0];
	bind[0].length = &lengths[0];
	bind[0].error = &error[0];
	/* INTEGER COLUMN */
	bind[1].buffer_type = MYSQL_TYPE_LONG;
	bind[1].buffer = &id_region;
	bind[1].is_null = &is_null[1];
	bind[1].length = &lengths[1];
	bind[1].error = &error[1];
	/* STRING COLUMN */
	bind[2].buffer_type = MYSQL_TYPE_STRING;
	bind[2].buffer = buffer;
	bind[2].buffer_length = BUFFER_SIZE;
	bind[2].is_null = &is_null[2];
	bind[2].length = &lengths[2];
	bind[2].error = &error[2];

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

	struct sql_result_row* row = NULL;
	struct sql_result_row* curr = NULL;

	res = calloc(1, sizeof(SQL_RESULT));
	res->results = NULL;
	res->type = MUNICIPALITY_E;
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
		row->data = calloc(1, sizeof(MUNICIPALITY));

		if (is_null[0]) {
			((MUNICIPALITY*) row->data)->id_municipality = 0;
		} else {
			((MUNICIPALITY*) row->data)->id_municipality = id_municipality;
		}

		if (is_null[1]) {
			((MUNICIPALITY*) row->data)->region = NULL;
		} else {
			((MUNICIPALITY*) row->data)->region = region_find_by_id(id_region);
		}

		if (is_null[2]) {
			strcpy(((MUNICIPALITY*) row->data)->name, "NULL");
		} else {
			strncpy(((MUNICIPALITY*) row->data)->name, buffer, lengths[2]);
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
	#undef BUFFER_SIZE
	#undef QUERY_SIZE
	#undef RES_COL_COUNT
}

SQL_RESULT* municipality_find_by_name(char const* name) {
	#define QUERY "select * from municipality where name like ?;"
	#define BUFFER_SIZE 255

	char buffer[BUFFER_SIZE];
	SQL_RESULT* res;
	MYSQL_BIND param;
	ulong len;

	snprintf(buffer, BUFFER_SIZE, "%%%s%%", name);

	memset(&param, 0, sizeof(param));
	len = strnlen(buffer, BUFFER_SIZE);
	param.buffer_type = MYSQL_TYPE_STRING;
	param.buffer = malloc(len);
	param.buffer_length = len;
	strncpy(param.buffer, buffer, len);

	res = municipality_execute_find(QUERY, &param, 1);

	free(param.buffer);
	return res;
	#undef QUERY
	#undef BUFFER_SIZE
}

MUNICIPALITY* municipality_find_by_id(uint id) {
	#define QUERY "select * from municipality where id_municipality = ?;"
	#define PARAM_COUNT 1
	MYSQL_BIND param;
	MUNICIPALITY* out;
	SQL_RESULT* res;

	memset(&param, 0, sizeof(param));
	param.buffer_type = MYSQL_TYPE_LONG;
	param.buffer = malloc(sizeof(uint));
	memcpy(param.buffer, &id, sizeof(uint));

	res = municipality_execute_find(QUERY, &param, PARAM_COUNT);
	out = res->results->data;
	free(param.buffer);
	if (res->count == 1) {
		free(res->results);
		free(res);
		return out;
	} else {
		fprintf(stderr, "municipality_execute_find(), failed - multiple results (%d)\n", res->count);
		mysql_res_free(&res);
		return NULL;
	}
	#undef QUERY
	#undef PARAM_COUNT
}


int municipality_update(MUNICIPALITY* municipalityT) {
	assert(municipalityT->id_municipality != 0);
	assert(municipalityT->region != NULL);
	#define QUERY "update municipality set name = ?, id_region = ? where id_municipality = ?;"
	#define PARAM_COUNT 3
	#define BUFFER_LENGTH 255

	ulong len;
	int retval;
	MYSQL_BIND params[PARAM_COUNT];

	if (municipalityT->region->id_region == 0) {
		region_insert(municipalityT->region);
	} else {
		region_update(municipalityT->region);
	}

	len = strnlen(municipalityT->name, BUFFER_LENGTH);

	// STRING PARAM
	params[0].buffer_type = MYSQL_TYPE_STRING;
	params[0].buffer = malloc(len);
	params[0].buffer_length = len;
	strncpy(params[0].buffer, municipalityT->name, len);
	// INTEGER PARAM
	params[1].buffer_type = MYSQL_TYPE_LONG;
	params[1].buffer = malloc(sizeof(uint));
	memcpy(params[1].buffer, &municipalityT->region->id_region, sizeof(uint));
	//INTEGER PARAM
	params[2].buffer_type = MYSQL_TYPE_LONG;
	params[2].buffer = malloc(sizeof(uint));
	memcpy(params[2].buffer, &municipalityT->id_municipality, sizeof(uint));

	retval = municipality_execute_update(QUERY, params, PARAM_COUNT);

	free(params[0].buffer);
	free(params[1].buffer);
	free(params[2].buffer);
	return retval;
	#undef QUERY
	#undef PARAM_COUNT
}

int municipality_execute_update(char const* query, MYSQL_BIND* params, uint param_count) {
	#define QUERY_LENGTH 512

	int retval;
	MYSQL_STMT* stmt;
	MYSQL* __attribute__((cleanup(mysql_con_cleanup))) conn;

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
