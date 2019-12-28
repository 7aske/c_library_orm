/**
 * Generated by nik on 28/12/2019
 */

#include "db/orm/book.h"


/* Generated function */
uint book_insert(BOOK* bookT) {
	#define QUERY_LENGTH 512
	#define STRING_SIZE 255
	#define QUERY "insert into book (isbn, name, publish_date) values (?, ?, ?);"
	#define PARAM_COUNT 3
	#define ISBN_SIZE 32
	#define NAME_SIZE 255
	/* Generated using get_insert_assertions() */
	assert(bookT->id_book == 0);
	assert(strnlen(bookT->isbn, STRING_SIZE) > 1);
	assert(strnlen(bookT->name, STRING_SIZE) > 1);
	

	MYSQL* __attribute__((cleanup(mysql_con_cleanup))) conn;
	MYSQL_STMT* __attribute__((cleanup(mysql_stmt_cleanup))) stmt;
	uint retval;
	

	conn = db_init();
	stmt = mysql_stmt_init(conn);

	/* Generated using get_update_fk() */
	

	/* Generated using col_param_lengths() */
	MYSQL_BIND param[PARAM_COUNT];
	memset(&param, 0, sizeof(param));
	
	unsigned long isbn_len;
	isbn_len = strnlen(bookT->isbn, ISBN_SIZE);
	
	unsigned long name_len;
	name_len = strnlen(bookT->name, NAME_SIZE);
	

	/* Generated using get_col_param_buffers() */
	
	/* STRING PARAM */
	param[0].buffer = malloc(isbn_len);
	param[0].buffer_type = MYSQL_TYPE_STRING;
	param[0].buffer_length = isbn_len;
	strncpy(param[0].buffer, bookT->isbn, isbn_len);
	/* STRING PARAM */
	param[1].buffer = malloc(name_len);
	param[1].buffer_type = MYSQL_TYPE_STRING;
	param[1].buffer_length = name_len;
	strncpy(param[1].buffer, bookT->name, name_len);
	/* DATE PARAM */
	param[2].buffer = malloc(56);
	param[2].buffer_type = MYSQL_TYPE_DATE;
	mysql_timecpy(param[2].buffer, &bookT->publish_date);

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
	bookT->id_book = retval;
	

	/* Generated using col_param_buffer_free() */
	free(param[0].buffer);
	free(param[1].buffer);
	free(param[2].buffer);
	

	return retval;

	#undef QUERY_LENGTH
	#undef STRING_SIZE
	#undef QUERY
	#undef PARAM_COUNT
	#undef ISBN_SIZE
	#undef NAME_SIZE
}


/* Generated function */
SQL_RESULT* book_execute_find(char const* query, MYSQL_BIND* params, uint param_count) {
	#define QUERY_SIZE 512
	#define RES_COL_COUNT 4
	#define BUFFER_SIZE 255
	MYSQL* __attribute__((cleanup(mysql_con_cleanup))) conn;
	SQL_RESULT* res;
	MYSQL_RES* prepare_meta_result;
	MYSQL_STMT* stmt;

	/* Generated using get_col_buffer_definitions()*/
	unsigned long lengths[RES_COL_COUNT];
	my_bool is_null[RES_COL_COUNT];
	my_bool error[RES_COL_COUNT];
	uint id_book_buffer;
	char isbn_buffer[32];
	char name_buffer[255];
	char publish_date_buffer[56];
	

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
	param[0].buffer = &id_book_buffer;
	param[0].is_null = &is_null[0];
	param[0].length = &lengths[0];
	param[0].error = &error[0];
	
	/* STRING COLUMN */
	param[1].buffer_type = MYSQL_TYPE_STRING;
	param[1].buffer = &isbn_buffer;
	param[1].is_null = &is_null[1];
	param[1].length = &lengths[1];
	param[1].error = &error[1];
	param[1].buffer_length = 32;
	
	/* STRING COLUMN */
	param[2].buffer_type = MYSQL_TYPE_STRING;
	param[2].buffer = &name_buffer;
	param[2].is_null = &is_null[2];
	param[2].length = &lengths[2];
	param[2].error = &error[2];
	param[2].buffer_length = 255;
	
	/* DATE COLUMN */
	param[3].buffer_type = MYSQL_TYPE_STRING;
	param[3].buffer = &publish_date_buffer;
	param[3].is_null = &is_null[3];
	param[3].length = &lengths[3];
	param[3].error = &error[3];
	param[3].buffer_length = BUFFER_SIZE;
	

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
	res->type = BOOK_E;
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
		row->data = calloc(1, sizeof(BOOK));
		
		if (is_null[0]) {
			((BOOK*) row->data)->id_book = 0;
		} else {
			((BOOK*) row->data)->id_book = id_book_buffer;
		}
		if (is_null[1]) {
			strcpy(((BOOK*) row->data)->isbn, "NULL");
		} else {
			strncpy(((BOOK*) row->data)->isbn, isbn_buffer, lengths[1]);
		}
		if (is_null[2]) {
			strcpy(((BOOK*) row->data)->name, "NULL");
		} else {
			strncpy(((BOOK*) row->data)->name, name_buffer, lengths[2]);
		}
		if (is_null[3]) {
			// strcpy(((BOOK*) row->data)->publish_date, "NULL");
		} else {
			mysql_timecpystr(&((BOOK*) row->data)->publish_date, publish_date_buffer);
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
BOOK* book_find_by_id(uint id) {
	#define QUERY "select * from book where id_book = ?;"
	#define PARAM_COUNT 1
	BOOK* out;

	
	SQL_RESULT* res;
	struct book book;
	book.id_book = id;
	struct book* bookT = &book;
	

	/* Generated using  get_col_param_buffers() */
	MYSQL_BIND param[PARAM_COUNT];
	memset(param, 0, sizeof(param));
	
	/* INTEGER PARAM */
	param[0].buffer = malloc(sizeof(uint));
	param[0].buffer_type = MYSQL_TYPE_LONG;
	memcpy(param[0].buffer, &bookT->id_book, sizeof(uint));

	res = book_execute_find(QUERY, param, PARAM_COUNT);

	/* Generated using col_param_buffer_free() */
	free(param[0].buffer);
	

if (res->results == NULL) { return NULL; }

	out = res->results->data;
	if (res->count == 1) {
		free(res->results);
		free(res);
		return out;
	} else {
		fprintf(stderr, "book_execute_find(), failed - multiple results (%d)\n", res->count);
		mysql_res_free(&res);
		return NULL;
	}

	#undef QUERY
	#undef PARAM_COUNT
}


/* Generated function */
int book_update(BOOK* bookT) {
	#define QUERY "update book set isbn = ?, name = ?, publish_date = ? where id_book = ?;"
	#define PARAM_COUNT 4
	#define STRING_SIZE 255
	#define ISBN_SIZE 32
	#define NAME_SIZE 255
	assert(bookT->id_book != 0);

	int retval;

	/* Generated using col_update_params() */
	MYSQL_BIND param[PARAM_COUNT];
	memset(&param, 0, sizeof(param));
	
	unsigned long isbn_len;
	isbn_len = strnlen(bookT->isbn, ISBN_SIZE);
	
	unsigned long name_len;
	name_len = strnlen(bookT->name, NAME_SIZE);
	
	/* STRING PARAM */
	param[0].buffer = malloc(isbn_len);
	param[0].buffer_type = MYSQL_TYPE_STRING;
	param[0].buffer_length = isbn_len;
	strncpy(param[0].buffer, bookT->isbn, isbn_len);
	/* STRING PARAM */
	param[1].buffer = malloc(name_len);
	param[1].buffer_type = MYSQL_TYPE_STRING;
	param[1].buffer_length = name_len;
	strncpy(param[1].buffer, bookT->name, name_len);
	/* DATE PARAM */
	param[2].buffer = malloc(56);
	param[2].buffer_type = MYSQL_TYPE_DATE;
	mysql_timecpy(param[2].buffer, &bookT->publish_date);
	/* INTEGER PARAM */
	param[3].buffer = malloc(sizeof(uint));
	param[3].buffer_type = MYSQL_TYPE_LONG;
	memcpy(param[3].buffer, &bookT->id_book, sizeof(uint));

	retval = book_execute(QUERY, param, PARAM_COUNT);

	/* Generated using col_buffer_free() */
	free(param[0].buffer);
	free(param[1].buffer);
	free(param[2].buffer);
	free(param[3].buffer);
	

	return retval;

	#undef QUERY
	#undef PARAM_COUNT
	#undef STRING_SIZE
	#undef ISBN_SIZE
	#undef NAME_SIZE
}


/* Generated function */
int book_execute(char const* query, MYSQL_BIND* params, uint param_count) {
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
int book_delete(BOOK* bookT) {
	#define QUERY "delete from book where id_book = ?;"
	#define PARAM_COUNT 1
	assert(bookT->id_book != 0);

	int retval;

	/* Generated using  get_col_param_buffers() */
	MYSQL_BIND param[PARAM_COUNT];
	memset(param, 0, sizeof(param));
	
	/* INTEGER PARAM */
	param[0].buffer = malloc(sizeof(uint));
	param[0].buffer_type = MYSQL_TYPE_LONG;
	memcpy(param[0].buffer, &bookT->id_book, sizeof(uint));

	retval = book_execute(QUERY, param, PARAM_COUNT);

	/* Generated using col_param_buffer_free() */
	free(param[0].buffer);
	

	return retval;

	#undef QUERY
	#undef PARAM_COUNT
}


/* Generated function */
SQL_RESULT* book_find_all() {
	#define QUERY "select * from book;"
	#define PARAM_COUNT 0
	SQL_RESULT* res;

	MYSQL_BIND param[1];

	res = book_execute_find(QUERY, param, PARAM_COUNT);

	return res;

	#undef QUERY
	#undef PARAM_COUNT
}


/* Generated function */
void book_free(BOOK** ptr) {
	/* Generated by get_free_members() */
	assert(ptr != NULL);
	
	if (*ptr != NULL){
		free(*ptr);
	}

}
