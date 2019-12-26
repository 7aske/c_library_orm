/**
 * Generated by nik on 26/12/2019
 */

#include "db/orm/author.h"


/* Generated function */
uint author_insert(AUTHOR* authorT) {
	#define QUERY_LENGTH 512
	#define STRING_SIZE 255
	#define QUERY "insert into author (id_person, description) values (?, ?);"
	#define PARAM_COUNT 2
	#define DESCRIPTION_SIZE 4096
	/* Generated using get_insert_assertions() */
	assert(authorT->id_author == 0);
	assert(strnlen(authorT->description, STRING_SIZE) > 1);
	

	MYSQL* __attribute__((cleanup(mysql_con_cleanup))) conn;
	MYSQL_STMT* __attribute__((cleanup(mysql_stmt_cleanup))) stmt;
	uint retval;
	

	conn = db_init();
	stmt = mysql_stmt_init(conn);

	/* Generated using get_update_fk() */
	
	if (authorT->person == NULL){
		fprintf(stderr, "%s->%s is NULL\n", "author", "person");
		return 0U;
	} else if (authorT->person->id_person == 0) {
		person_insert(authorT->person);
	} else {
		person_update(authorT->person);
	}

	/* Generated using col_param_lengths() */
	MYSQL_BIND param[PARAM_COUNT];
	memset(&param, 0, sizeof(param));
	
	unsigned long description_len;
	description_len = strnlen(authorT->description, DESCRIPTION_SIZE);
	

	/* Generated using get_col_param_buffers() */
	
	/* INTEGER PARAM */
	param[0].buffer = malloc(sizeof(uint));
	param[0].buffer_type = MYSQL_TYPE_LONG;
	memcpy(param[0].buffer, &authorT->person->id_person, sizeof(uint));
	/* STRING PARAM */
	param[1].buffer = malloc(description_len);
	param[1].buffer_type = MYSQL_TYPE_STRING;
	param[1].buffer_length = description_len;
	strncpy(param[1].buffer, authorT->description, description_len);

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
	authorT->id_author = retval;
	

	/* Generated using col_param_buffer_free() */
	free(param[0].buffer);
	free(param[1].buffer);
	

	return retval;

	#undef QUERY_LENGTH
	#undef STRING_SIZE
	#undef QUERY
	#undef PARAM_COUNT
	#undef DESCRIPTION_SIZE
}


/* Generated function */
SQL_RESULT* author_execute_find(char const* query, MYSQL_BIND* params, uint param_count) {
	#define QUERY_SIZE 512
	#define RES_COL_COUNT 3
	#define BUFFER_SIZE 255
	MYSQL* __attribute__((cleanup(mysql_con_cleanup))) conn;
	SQL_RESULT* res;
	MYSQL_RES* prepare_meta_result;
	MYSQL_STMT* stmt;

	/* Generated using get_col_buffer_definitions()*/
	unsigned long lengths[RES_COL_COUNT];
	my_bool is_null[RES_COL_COUNT];
	my_bool error[RES_COL_COUNT];
	uint id_author_buffer;
	uint id_person_buffer;
	char description_buffer[BUFFER_SIZE];
	

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
	param[0].buffer = &id_author_buffer;
	param[0].is_null = &is_null[0];
	param[0].length = &lengths[0];
	param[0].error = &error[0];
	
	/* INTEGER COLUMN */
	param[1].buffer_type = MYSQL_TYPE_LONG;
	param[1].buffer = &id_person_buffer;
	param[1].is_null = &is_null[1];
	param[1].length = &lengths[1];
	param[1].error = &error[1];
	
	/* STRING COLUMN */
	param[2].buffer_type = MYSQL_TYPE_STRING;
	param[2].buffer = &description_buffer;
	param[2].is_null = &is_null[2];
	param[2].length = &lengths[2];
	param[2].error = &error[2];
	param[2].buffer_length = 4096;
	

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
	res->type = AUTHOR_E;
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
		row->data = calloc(1, sizeof(AUTHOR));
		
		if (is_null[0]) {
			((AUTHOR*) row->data)->id_author = 0;
		} else {
			((AUTHOR*) row->data)->id_author = id_author_buffer;
		}
		if (is_null[1]) {
			((AUTHOR*) row->data)->person = NULL;
		} else {
			((AUTHOR*) row->data)->person = person_find_by_id(id_person_buffer);
		}
		if (is_null[2]) {
			strcpy(((AUTHOR*) row->data)->description, "NULL");
		} else {
			strncpy(((AUTHOR*) row->data)->description, description_buffer, lengths[2]);
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
AUTHOR* author_find_by_id(uint id) {
	#define QUERY "select * from author where id_author = ?;"
	#define PARAM_COUNT 1
	AUTHOR* out;

	
	SQL_RESULT* res;
	struct author author;
	author.id_author = id;
	struct author* authorT = &author;
	

	/* Generated using  get_col_param_buffers() */
	MYSQL_BIND param[PARAM_COUNT];
	memset(param, 0, sizeof(param));
	
	/* INTEGER PARAM */
	param[0].buffer = malloc(sizeof(uint));
	param[0].buffer_type = MYSQL_TYPE_LONG;
	memcpy(param[0].buffer, &authorT->id_author, sizeof(uint));

	res = author_execute_find(QUERY, param, PARAM_COUNT);

	/* Generated using col_param_buffer_free() */
	free(param[0].buffer);
	

if (res->results == NULL) { return NULL; }

	out = res->results->data;
	if (res->count == 1) {
		free(res->results);
		free(res);
		return out;
	} else {
		fprintf(stderr, "author_execute_find(), failed - multiple results (%d)\n", res->count);
		mysql_res_free(&res);
		return NULL;
	}

	#undef QUERY
	#undef PARAM_COUNT
}


/* Generated function */
int author_update(AUTHOR* authorT) {
	#define QUERY "update author set id_person = ?, description = ? where id_author = ?;"
	#define PARAM_COUNT 3
	#define STRING_SIZE 255
	#define DESCRIPTION_SIZE 4096
	assert(authorT->id_author != 0);

	int retval;

	/* Generated using col_update_params() */
	MYSQL_BIND param[PARAM_COUNT];
	memset(&param, 0, sizeof(param));
	
	unsigned long description_len;
	description_len = strnlen(authorT->description, DESCRIPTION_SIZE);
	
	/* INTEGER PARAM */
	param[0].buffer = malloc(sizeof(uint));
	param[0].buffer_type = MYSQL_TYPE_LONG;
	memcpy(param[0].buffer, &authorT->person->id_person, sizeof(uint));
	/* STRING PARAM */
	param[1].buffer = malloc(description_len);
	param[1].buffer_type = MYSQL_TYPE_STRING;
	param[1].buffer_length = description_len;
	strncpy(param[1].buffer, authorT->description, description_len);
	/* INTEGER PARAM */
	param[2].buffer = malloc(sizeof(uint));
	param[2].buffer_type = MYSQL_TYPE_LONG;
	memcpy(param[2].buffer, &authorT->id_author, sizeof(uint));

	retval = author_execute(QUERY, param, PARAM_COUNT);

	/* Generated using col_buffer_free() */
	free(param[0].buffer);
	free(param[1].buffer);
	free(param[2].buffer);
	

	return retval;

	#undef QUERY
	#undef PARAM_COUNT
	#undef STRING_SIZE
	#undef DESCRIPTION_SIZE
}


/* Generated function */
int author_execute(char const* query, MYSQL_BIND* params, uint param_count) {
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


/* Generated function */
int author_delete(AUTHOR* authorT) {
	#define QUERY "delete from author where id_author = ?;"
	#define PARAM_COUNT 1
	assert(authorT->id_author != 0);

	int retval;

	/* Generated using  get_col_param_buffers() */
	MYSQL_BIND param[PARAM_COUNT];
	memset(param, 0, sizeof(param));
	
	/* INTEGER PARAM */
	param[0].buffer = malloc(sizeof(uint));
	param[0].buffer_type = MYSQL_TYPE_LONG;
	memcpy(param[0].buffer, &authorT->id_author, sizeof(uint));

	retval = author_execute(QUERY, param, PARAM_COUNT);

	/* Generated using col_param_buffer_free() */
	free(param[0].buffer);
	

	return retval;

	#undef QUERY
	#undef PARAM_COUNT
}


/* Generated function */
SQL_RESULT* author_find_all() {
	#define QUERY "select * from author;"
	#define PARAM_COUNT 0
	SQL_RESULT* res;

	MYSQL_BIND param[1];

	res = author_execute_find(QUERY, param, PARAM_COUNT);

	return res;

	#undef QUERY
	#undef PARAM_COUNT
}
