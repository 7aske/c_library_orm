//
// Created by nik on 12/21/19.
//

#include "db/orm/entity.h"


uint address_insert(ADDRESS* addressT) {
	#define QUERY_LENGTH 512
	#define QUERY "insert into address (id_municipality, street, number) values (?, ?, ?);"
	#define PARAM_COUNT 3
	#define STRING_SIZE 255
	#define NUMBER_SIZE 8

	assert(addressT->id_address == 0);
	assert(strnlen(addressT->street, STRING_SIZE) > 1);
	assert(strnlen(addressT->number, STRING_SIZE) > 1);

	MYSQL* __attribute__((cleanup(mysql_con_cleanup))) conn;
	MYSQL_STMT* __attribute__((cleanup(mysql_stmt_cleanup))) stmt;
	MYSQL_BIND bind[PARAM_COUNT];
	unsigned long street_len;
	unsigned long num_len;
	int retval;

	conn = db_init();
	stmt = mysql_stmt_init(conn);

	street_len = strnlen(addressT->street, STRING_SIZE);
	num_len = strnlen(addressT->street, NUMBER_SIZE);

	if (addressT->municipality->id_municipality == 0) {
		municipality_insert(addressT->municipality);
	} else {
		municipality_update(addressT->municipality);
	}

	memset(&bind, 0, sizeof(bind));

	// INTEGER PARAM
	bind[0].buffer = malloc(sizeof(uint));
	bind[0].buffer_type = MYSQL_TYPE_LONG;
	memcpy(bind[0].buffer, &addressT->municipality->id_municipality, sizeof(uint));
	// STRING PARAM
	bind[1].buffer = malloc(street_len);
	bind[1].buffer_type = MYSQL_TYPE_STRING;
	bind[1].buffer_length = street_len;
	strncpy(bind[1].buffer, addressT->street, street_len);
	// STRING PARAM
	bind[2].buffer = malloc(num_len);
	bind[2].buffer_type = MYSQL_TYPE_STRING;
	bind[2].buffer_length = num_len;
	strncpy(bind[2].buffer, addressT->number, num_len);


	mysql_stmt_prepare(stmt, QUERY, QUERY_LENGTH);
	mysql_stmt_bind_param(stmt, bind);

	mysql_stmt_execute(stmt);

	retval = (uint) mysql_stmt_insert_id(stmt);
	// update id after insertion;
	addressT->id_address = retval;

	free(bind[0].buffer);
	free(bind[1].buffer);
	free(bind[2].buffer);
	return retval;
	#undef QUERY_LENGTH
	#undef QUERY
	#undef STRING_SIZE
	#undef NUMBER_SIZE
	#undef PARAM_COUNT
}

SQL_RESULT* address_find_by_street(char const* street) {
	#define QUERY "select * from address where street like ?;"
	#define PARAM_COUNT 1
	#define BUFFER_SIZE 255

	char buffer[BUFFER_SIZE];
	SQL_RESULT* res;
	MYSQL_BIND param;
	ulong len;

	snprintf(buffer, BUFFER_SIZE, "%%%s%%", street);

	memset(&param, 0, sizeof(param));
	len = strnlen(buffer, BUFFER_SIZE);
	param.buffer_type = MYSQL_TYPE_STRING;
	param.buffer = malloc(len);
	param.buffer_length = len;
	strncpy(param.buffer, buffer, len);

	res = municipality_execute_find(QUERY, &param, PARAM_COUNT);

	free(param.buffer);
	return res;
	#undef QUERY
	#undef PARAM_COUNT
	#undef BUFFER_SIZE
}

SQL_RESULT* address_find_by_street_and_number(char const* street, char const* number) {
	return NULL;
}

ADDRESS* address_find_by_id(uint id) {
	#define QUERY "select * from address where id_address = ?;"
	#define PARAM_COUNT 1
	MYSQL_BIND param;
	ADDRESS* out;
	SQL_RESULT* res;

	memset(&param, 0, sizeof(param));

	param.buffer = malloc(sizeof(uint));
	param.buffer_type = MYSQL_TYPE_LONG;
	memcpy(param.buffer, &id, sizeof(uint));

	res = address_execute_find(QUERY, &param, PARAM_COUNT);
	out = res->results->data;
	free(param.buffer);
	if (res->count == 1) {
		free(res->results);
		free(res);
		return out;
	} else {
		fprintf(stderr, "address_execute_find(), failed - multiple results (%d)\n", res->count);
		mysql_res_free(&res);
		return NULL;
	}
	#undef QUERY
	#undef PARAM_COUNT
}

SQL_RESULT* address_execute_find(char const* query, MYSQL_BIND* params, uint param_count) {
	#define BUFFER_SIZE 255
	#define QUERY_SIZE 512
	#define RES_COL_COUNT 4

	MYSQL* __attribute__((cleanup(mysql_con_cleanup))) conn;
	SQL_RESULT* res;

	MYSQL_BIND bind[RES_COL_COUNT];
	MYSQL_RES* prepare_meta_result;
	MYSQL_STMT* stmt;

	uint id_municipality;
	uint id_address;
	char street_buffer[BUFFER_SIZE];
	char number_buffer[BUFFER_SIZE];

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
	bind[0].buffer = &id_address;
	bind[0].is_null = &is_null[0];
	bind[0].length = &lengths[0];
	bind[0].error = &error[0];
	/* INTEGER COLUMN */
	bind[1].buffer_type = MYSQL_TYPE_LONG;
	bind[1].buffer = &id_municipality;
	bind[1].is_null = &is_null[1];
	bind[1].length = &lengths[1];
	bind[1].error = &error[1];
	/* STRING COLUMN */
	bind[2].buffer_type = MYSQL_TYPE_STRING;
	bind[2].buffer = street_buffer;
	bind[2].buffer_length = BUFFER_SIZE;
	bind[2].is_null = &is_null[2];
	bind[2].length = &lengths[2];
	bind[2].error = &error[2];
	/* STRING COLUMN */
	bind[3].buffer_type = MYSQL_TYPE_STRING;
	bind[3].buffer = number_buffer;
	bind[3].buffer_length = BUFFER_SIZE;
	bind[3].is_null = &is_null[3];
	bind[3].length = &lengths[3];
	bind[3].error = &error[3];

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
	res->type = ADDRESS_E;
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
		row->data = calloc(1, sizeof(ADDRESS));

		if (is_null[0]) {
			((ADDRESS*) row->data)->id_address = 0;
		} else {
			((ADDRESS*) row->data)->id_address = id_address;
		}

		if (is_null[1]) {
			((ADDRESS*) row->data)->municipality = NULL;
		} else {
			((ADDRESS*) row->data)->municipality = municipality_find_by_id(id_municipality);
		}

		if (is_null[2]) {
			strcpy(((ADDRESS*) row->data)->street, "NULL");
		} else {
			strncpy(((ADDRESS*) row->data)->street, street_buffer, lengths[2]);
		}

		if (is_null[3]) {
			strcpy(((ADDRESS*) row->data)->number, "NULL");
		} else {
			strncpy(((ADDRESS*) row->data)->number, number_buffer, lengths[3]);
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

int address_execute_update(char const* query, MYSQL_BIND* params, uint param_count) {
	return 0;
}

int address_update(ADDRESS* addressT) {
	return 0;
}
