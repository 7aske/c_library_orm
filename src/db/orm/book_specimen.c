/**
 * Generated by nik on 04/01/2020
 */

#include "db/orm/book_specimen.h"


/* Generated function */
uint book_specimen_insert(MYSQL* conn, BOOK_SPECIMEN* book_specimenT) {
	#define QUERY_LENGTH 512
	#define STRING_SIZE 255
	#define QUERY "insert into book_specimen (id_library, id_book, book_serial) values (?, ?, ?);"
	#define PARAM_COUNT 3
	#define BOOK_SERIAL_SIZE 17
	/* Generated using get_insert_assertions() */
	assert(book_specimenT->id_book_specimen == 0);
	assert(strnlen(book_specimenT->book_serial, STRING_SIZE) > 1);
	

	
	MYSQL_STMT* __attribute__((cleanup(mysql_stmt_cleanup))) stmt;
	uint retval;
	

	if (conn == NULL) {
		conn = db_init();
	}
	

	stmt = mysql_stmt_init(conn);
	

	/* Generated using get_update_fk() */
	
	if (book_specimenT->library == NULL){
		fprintf(stderr, "%s->%s is NULL\n", "book_specimen", "library");
		return 0U;
	} else if (book_specimenT->library->id_library == 0) {
		library_insert(conn, book_specimenT->library);
	} else {
		library_update(conn, book_specimenT->library);
	}
	if (book_specimenT->book == NULL){
		fprintf(stderr, "%s->%s is NULL\n", "book_specimen", "book");
		return 0U;
	} else if (book_specimenT->book->id_book == 0) {
		book_insert(conn, book_specimenT->book);
	} else {
		book_update(conn, book_specimenT->book);
	}

	/* Generated using col_param_lengths() */
	MYSQL_BIND param[PARAM_COUNT];
	memset(&param, 0, sizeof(param));
	
	unsigned long book_serial_len;
	book_serial_len = strnlen(book_specimenT->book_serial, BOOK_SERIAL_SIZE);
	

	/* Generated using get_col_param_buffers() */
	
	/* INTEGER PARAM */
	param[0].buffer = malloc(sizeof(uint));
	param[0].buffer_type = MYSQL_TYPE_LONG;
	memcpy(param[0].buffer, &book_specimenT->library->id_library, sizeof(uint));
	/* INTEGER PARAM */
	param[1].buffer = malloc(sizeof(uint));
	param[1].buffer_type = MYSQL_TYPE_LONG;
	memcpy(param[1].buffer, &book_specimenT->book->id_book, sizeof(uint));
	/* STRING PARAM */
	param[2].buffer = malloc(book_serial_len);
	param[2].buffer_type = MYSQL_TYPE_STRING;
	param[2].buffer_length = book_serial_len;
	strncpy(param[2].buffer, book_specimenT->book_serial, book_serial_len);

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
	book_specimenT->id_book_specimen = retval;
	

	/* Generated using col_param_buffer_free() */
	free(param[0].buffer);
	free(param[1].buffer);
	free(param[2].buffer);
	

	return retval;

	#undef QUERY_LENGTH
	#undef STRING_SIZE
	#undef QUERY
	#undef PARAM_COUNT
	#undef BOOK_SERIAL_SIZE
}


/* Generated function */
SQL_RESULT* book_specimen_execute_find(MYSQL* conn, char const* query, MYSQL_BIND* params, uint param_count) {
	#define QUERY_SIZE 512
	#define RES_COL_COUNT 4
	#define BUFFER_SIZE 255
	
	SQL_RESULT* res;
	MYSQL_RES* prepare_meta_result;
	MYSQL_STMT* stmt;

	if (conn == NULL) {
		conn = db_init();
	}
	

	/* Generated using get_col_buffer_definitions()*/
	unsigned long lengths[RES_COL_COUNT];
	my_bool is_null[RES_COL_COUNT];
	my_bool error[RES_COL_COUNT];
	uint id_book_specimen_buffer;
	uint id_library_buffer;
	uint id_book_buffer;
	char book_serial_buffer[17];
	

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
	param[0].buffer = &id_book_specimen_buffer;
	param[0].is_null = &is_null[0];
	param[0].length = &lengths[0];
	param[0].error = &error[0];
	
	/* INTEGER COLUMN */
	param[1].buffer_type = MYSQL_TYPE_LONG;
	param[1].buffer = &id_library_buffer;
	param[1].is_null = &is_null[1];
	param[1].length = &lengths[1];
	param[1].error = &error[1];
	
	/* INTEGER COLUMN */
	param[2].buffer_type = MYSQL_TYPE_LONG;
	param[2].buffer = &id_book_buffer;
	param[2].is_null = &is_null[2];
	param[2].length = &lengths[2];
	param[2].error = &error[2];
	
	/* STRING COLUMN */
	param[3].buffer_type = MYSQL_TYPE_STRING;
	param[3].buffer = &book_serial_buffer;
	param[3].is_null = &is_null[3];
	param[3].length = &lengths[3];
	param[3].error = &error[3];
	param[3].buffer_length = 17;
	

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
	res->type = BOOK_SPECIMEN_E;
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
		row->data = calloc(1, sizeof(BOOK_SPECIMEN));
		
		if (is_null[0]) {
			((BOOK_SPECIMEN*) row->data)->id_book_specimen = 0;
		} else {
			((BOOK_SPECIMEN*) row->data)->id_book_specimen = id_book_specimen_buffer;
		}
		if (is_null[1]) {
			((BOOK_SPECIMEN*) row->data)->library = NULL;
		} else {
			((BOOK_SPECIMEN*) row->data)->library = library_find_by_id(conn, id_library_buffer);
		}
		if (is_null[2]) {
			((BOOK_SPECIMEN*) row->data)->book = NULL;
		} else {
			((BOOK_SPECIMEN*) row->data)->book = book_find_by_id(conn, id_book_buffer);
		}
		if (is_null[3]) {
			strcpy(((BOOK_SPECIMEN*) row->data)->book_serial, "NULL");
		} else {
			strncpy(((BOOK_SPECIMEN*) row->data)->book_serial, book_serial_buffer, lengths[3]);
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
BOOK_SPECIMEN* book_specimen_find_by_id(MYSQL* conn, uint id) {
	#define QUERY "select * from book_specimen where id_book_specimen = ?;"
	#define PARAM_COUNT 1
	if (conn == NULL) {
		conn = db_init();
	}
	

	BOOK_SPECIMEN* out;

	
	SQL_RESULT* res;
	struct book_specimen book_specimen;
	book_specimen.id_book_specimen = id;
	struct book_specimen* book_specimenT = &book_specimen;
	

	/* Generated using  get_col_param_buffers() */
	MYSQL_BIND param[PARAM_COUNT];
	memset(param, 0, sizeof(param));
	
	/* INTEGER PARAM */
	param[0].buffer = malloc(sizeof(uint));
	param[0].buffer_type = MYSQL_TYPE_LONG;
	memcpy(param[0].buffer, &book_specimenT->id_book_specimen, sizeof(uint));

	res = book_specimen_execute_find(conn, QUERY, param, PARAM_COUNT);

	/* Generated using col_param_buffer_free() */
	free(param[0].buffer);
	

if (res->results == NULL) { return NULL; }

	out = res->results->data;
	if (res->count == 1) {
		free(res->results);
		free(res);
		return out;
	} else {
		fprintf(stderr, "book_specimen_execute_find(), failed - multiple results (%d)\n", res->count);
		mysql_res_free(&res);
		return NULL;
	}

	#undef QUERY
	#undef PARAM_COUNT
}


/* Generated function */
int book_specimen_update(MYSQL* conn, BOOK_SPECIMEN* book_specimenT) {
	#define QUERY "update book_specimen set id_library = ?, id_book = ?, book_serial = ? where id_book_specimen = ?;"
	#define PARAM_COUNT 4
	#define STRING_SIZE 255
	#define BOOK_SERIAL_SIZE 17
	assert(book_specimenT->id_book_specimen != 0);

	if (conn == NULL) {
		conn = db_init();
	}
	

	int retval;

	/* Generated using col_update_params() */
	MYSQL_BIND param[PARAM_COUNT];
	memset(&param, 0, sizeof(param));
	
	unsigned long book_serial_len;
	book_serial_len = strnlen(book_specimenT->book_serial, BOOK_SERIAL_SIZE);
	
	/* INTEGER PARAM */
	param[0].buffer = malloc(sizeof(uint));
	param[0].buffer_type = MYSQL_TYPE_LONG;
	memcpy(param[0].buffer, &book_specimenT->library->id_library, sizeof(uint));
	/* INTEGER PARAM */
	param[1].buffer = malloc(sizeof(uint));
	param[1].buffer_type = MYSQL_TYPE_LONG;
	memcpy(param[1].buffer, &book_specimenT->book->id_book, sizeof(uint));
	/* STRING PARAM */
	param[2].buffer = malloc(book_serial_len);
	param[2].buffer_type = MYSQL_TYPE_STRING;
	param[2].buffer_length = book_serial_len;
	strncpy(param[2].buffer, book_specimenT->book_serial, book_serial_len);
	/* INTEGER PARAM */
	param[3].buffer = malloc(sizeof(uint));
	param[3].buffer_type = MYSQL_TYPE_LONG;
	memcpy(param[3].buffer, &book_specimenT->id_book_specimen, sizeof(uint));

	retval = book_specimen_execute(conn, QUERY, param, PARAM_COUNT);

	/* Generated using col_buffer_free() */
	free(param[0].buffer);
	free(param[1].buffer);
	free(param[2].buffer);
	free(param[3].buffer);
	

	return retval;

	#undef QUERY
	#undef PARAM_COUNT
	#undef STRING_SIZE
	#undef BOOK_SERIAL_SIZE
}


/* Generated function */
int book_specimen_execute(MYSQL* conn, char const* query, MYSQL_BIND* params, uint param_count) {
	#define QUERY_LENGTH 512
	/* Generated by body_execute */

	MYSQL_STMT* stmt;
	int retval;

	if (conn == NULL) {
		conn = db_init();
	}
	

	
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
int book_specimen_delete(MYSQL* conn, BOOK_SPECIMEN* book_specimenT) {
	#define QUERY "delete from book_specimen where id_book_specimen = ?;"
	#define PARAM_COUNT 1
	assert(book_specimenT->id_book_specimen != 0);

	if (conn == NULL) {
		conn = db_init();
	}
	

	int retval;

	/* Generated using  get_col_param_buffers() */
	MYSQL_BIND param[PARAM_COUNT];
	memset(param, 0, sizeof(param));
	
	/* INTEGER PARAM */
	param[0].buffer = malloc(sizeof(uint));
	param[0].buffer_type = MYSQL_TYPE_LONG;
	memcpy(param[0].buffer, &book_specimenT->id_book_specimen, sizeof(uint));

	retval = book_specimen_execute(conn, QUERY, param, PARAM_COUNT);

	/* Generated using col_param_buffer_free() */
	free(param[0].buffer);
	

	return retval;

	#undef QUERY
	#undef PARAM_COUNT
}


/* Generated function */
SQL_RESULT* book_specimen_find_all(MYSQL* conn) {
	#define QUERY "select * from book_specimen;"
	#define PARAM_COUNT 0
	SQL_RESULT* res;

	MYSQL_BIND param[1];

	if (conn == NULL) {
		conn = db_init();
	}
	

	res = book_specimen_execute_find(conn, QUERY, param, PARAM_COUNT);

	return res;

	#undef QUERY
	#undef PARAM_COUNT
}


/* Generated function */
void book_specimen_free(BOOK_SPECIMEN** ptr) {
	/* Generated by get_free_members() */
	assert(ptr != NULL);
	
	if (*ptr != NULL){
		library_free(&((*ptr)->library));
		book_free(&((*ptr)->book));
		free(*ptr);
	}

}
