//
// Created by nik on 12/22/19.
//

#include "db/orm/entity.h"


uint library_insert(LIBRARY* libraryT) {
	#define QUERY_LENGTH 512
	#define STRING_SIZE 255
	#define QUERY "insert into library (id_address, name) values (?, ?);"
	#define PARAM_COUNT 2

	assert(libraryT->id_library == 0);
	assert(strnlen(libraryT->name, STRING_SIZE) > 1);


	MYSQL* __attribute__((cleanup(mysql_con_cleanup))) conn;
	MYSQL_STMT* __attribute__((cleanup(mysql_stmt_cleanup))) stmt;
	MYSQL_BIND param[PARAM_COUNT];
	int retval;

	#define NAME_SIZE 255
	unsigned long name_len;
	name_len = strnlen(libraryT->name, NAME_SIZE);


	conn = db_init();
	stmt = mysql_stmt_init(conn);


	if (libraryT->address->id_address == 0) {
		address_insert(libraryT->address);
	} else {
		address_update(libraryT->address);
	}

	memset(&param, 0, sizeof(param));


	/* INTEGER PARAM */
	param[0].buffer = malloc(sizeof(uint));
	param[0].buffer_type = MYSQL_TYPE_LONG;
	memcpy(param[0].buffer, &libraryT->address->id_address, sizeof(uint));

	/* STRING PARAM */
	param[1].buffer = malloc(name_len);
	param[1].buffer_type = MYSQL_TYPE_STRING;
	param[1].buffer_length = name_len;
	strncpy(param[1].buffer, libraryT->name, name_len);


	mysql_stmt_prepare(stmt, QUERY, QUERY_LENGTH);
	mysql_stmt_bind_param(stmt, param);

	mysql_stmt_execute(stmt);

	retval = (uint) mysql_stmt_insert_id(stmt);
	// update id after insertion;
	libraryT->id_library = retval;

	free(param[0].buffer);
	free(param[1].buffer);

	return retval;
	#undef QUERY_LENGTH
	#undef QUERY
	#undef STRING_SIZE
	#undef NUMBER_SIZE
	#undef PARAM_COUNT
}
SQL_RESULT* library_execute_find(char const* query, MYSQL_BIND* params, uint param_count) {
	#define QUERY_SIZE 512
	#define RES_COL_COUNT 3
	#define BUFFER_SIZE 255

	MYSQL* __attribute__((cleanup(mysql_con_cleanup))) conn;
	SQL_RESULT* res;
	MYSQL_BIND param[RES_COL_COUNT];
	MYSQL_RES* prepare_meta_result;
	MYSQL_STMT* stmt;

	uint id_library;
	uint id_address;
	char name_buffer[BUFFER_SIZE];


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
	memset(param, 0, sizeof(param));

	/* INTEGER COLUMN */
	param[0].buffer_type = MYSQL_TYPE_LONG;
	param[0].buffer = &id_library;
	param[0].is_null = &is_null[0];
	param[0].length = &lengths[0];
	param[0].error = &error[0];

	/* INTEGER COLUMN */
	param[1].buffer_type = MYSQL_TYPE_LONG;
	param[1].buffer = &id_address;
	param[1].is_null = &is_null[1];
	param[1].length = &lengths[1];
	param[1].error = &error[1];

	/* STRING COLUMN */
	param[2].buffer_type = MYSQL_TYPE_STRING;
	param[2].buffer = name_buffer;
	param[2].buffer_length = 255;
	param[2].is_null = &is_null[2];
	param[2].length = &lengths[2];
	param[2].error = &error[2];


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
	res->type = LIBRARY_E;
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
		row->data = calloc(1, sizeof(LIBRARY));

		if (is_null[0]) {
			((LIBRARY*) row->data)->id_library = 0;
		} else {
			((LIBRARY*) row->data)->id_library = id_library;
		}
		if (is_null[1]) {
			((LIBRARY*) row->data)->address = NULL;
		} else {
			((LIBRARY*) row->data)->address = address_find_by_id(id_address);
		}
		if (is_null[2]) {
			strcpy(((LIBRARY*) row->data)->name, "NULL");
		} else {
			strncpy(((LIBRARY*) row->data)->name, name_buffer, lengths[2]);
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

LIBRARY* library_find_by_id(uint id) {
	#define QUERY "select * from library where id_library = ?;"
	#define PARAM_COUNT 1
	MYSQL_BIND param;
	LIBRARY* out;
	SQL_RESULT* res;

	memset(&param, 0, sizeof(param));

	param.buffer = malloc(sizeof(uint));
	param.buffer_type = MYSQL_TYPE_LONG;
	memcpy(param.buffer, &id, sizeof(uint));

	res = library_execute_find(QUERY, &param, PARAM_COUNT);
	out = res->results->data;
	free(param.buffer);
	if (res->count == 1) {
		free(res->results);
		free(res);
		return out;
	} else {
		fprintf(stderr, "library_execute_find(), failed - multiple results (%d)\n", res->count);
		mysql_res_free(&res);
		return NULL;
	}
	#undef QUERY
	#undef PARAM_COUNT
}

int library_update(LIBRARY* libraryT) {
	assert(libraryT->id_library != 0);
	#define QUERY "update library set id_address = ?, name = ? where id_library = ?;"
	#define PARAM_COUNT 3
	#define STRING_SIZE 255
	MYSQL_BIND param[PARAM_COUNT];
	ulong len;
	int retval;

	memset(&param, 0, sizeof(param));


	#define NAME_SIZE 255
	unsigned long name_len;
	name_len = strnlen(libraryT->name, NAME_SIZE);

	/* INTEGER PARAM */
	param[0].buffer = malloc(sizeof(uint));
	param[0].buffer_type = MYSQL_TYPE_LONG;
	memcpy(param[0].buffer, &libraryT->address->id_address, sizeof(uint));

	/* STRING PARAM */
	param[1].buffer = malloc(name_len);
	param[1].buffer_type = MYSQL_TYPE_STRING;
	param[1].buffer_length = name_len;
	strncpy(param[1].buffer, libraryT->name, name_len);

	/* INTEGER PARAM */
	param[2].buffer = malloc(sizeof(uint));
	param[2].buffer_type = MYSQL_TYPE_LONG;
	memcpy(param[2].buffer, &libraryT->id_library, sizeof(uint));


	retval = library_execute_update(QUERY, param, PARAM_COUNT);
	free(param[0].buffer);
	free(param[1].buffer);

	return retval;
}
int library_execute_update(char const* query, MYSQL_BIND* params, uint param_count) {
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