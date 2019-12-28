/**
 * Generated by nik on 28/12/2019
 */

#include "db/orm/person.h"


/* Generated function */
uint person_insert(PERSON* personT) {
	#define QUERY_LENGTH 512
	#define STRING_SIZE 255
	#define QUERY "insert into person (first_name, last_name, jmbg) values (?, ?, ?);"
	#define PARAM_COUNT 3
	#define FIRST_NAME_SIZE 255
	#define LAST_NAME_SIZE 255
	#define JMBG_SIZE 13
	/* Generated using get_insert_assertions() */
	assert(personT->id_person == 0);
	assert(strnlen(personT->first_name, STRING_SIZE) > 1);
	assert(strnlen(personT->last_name, STRING_SIZE) > 1);
	assert(strnlen(personT->jmbg, STRING_SIZE) > 1);
	

	MYSQL* __attribute__((cleanup(mysql_con_cleanup))) conn;
	MYSQL_STMT* __attribute__((cleanup(mysql_stmt_cleanup))) stmt;
	uint retval;
	

	conn = db_init();
	stmt = mysql_stmt_init(conn);

	/* Generated using get_update_fk() */
	

	/* Generated using col_param_lengths() */
	MYSQL_BIND param[PARAM_COUNT];
	memset(&param, 0, sizeof(param));
	
	unsigned long first_name_len;
	first_name_len = strnlen(personT->first_name, FIRST_NAME_SIZE);
	
	unsigned long last_name_len;
	last_name_len = strnlen(personT->last_name, LAST_NAME_SIZE);
	
	unsigned long jmbg_len;
	jmbg_len = strnlen(personT->jmbg, JMBG_SIZE);
	

	/* Generated using get_col_param_buffers() */
	
	/* STRING PARAM */
	param[0].buffer = malloc(first_name_len);
	param[0].buffer_type = MYSQL_TYPE_STRING;
	param[0].buffer_length = first_name_len;
	strncpy(param[0].buffer, personT->first_name, first_name_len);
	/* STRING PARAM */
	param[1].buffer = malloc(last_name_len);
	param[1].buffer_type = MYSQL_TYPE_STRING;
	param[1].buffer_length = last_name_len;
	strncpy(param[1].buffer, personT->last_name, last_name_len);
	/* STRING PARAM */
	param[2].buffer = malloc(jmbg_len);
	param[2].buffer_type = MYSQL_TYPE_STRING;
	param[2].buffer_length = jmbg_len;
	strncpy(param[2].buffer, personT->jmbg, jmbg_len);

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
	personT->id_person = retval;
	

	/* Generated using col_param_buffer_free() */
	free(param[0].buffer);
	free(param[1].buffer);
	free(param[2].buffer);
	

	return retval;

	#undef QUERY_LENGTH
	#undef STRING_SIZE
	#undef QUERY
	#undef PARAM_COUNT
	#undef FIRST_NAME_SIZE
	#undef LAST_NAME_SIZE
	#undef JMBG_SIZE
}


/* Generated function */
SQL_RESULT* person_execute_find(char const* query, MYSQL_BIND* params, uint param_count) {
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
	uint id_person_buffer;
	char first_name_buffer[255];
	char last_name_buffer[255];
	char jmbg_buffer[13];
	

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
	param[0].buffer = &id_person_buffer;
	param[0].is_null = &is_null[0];
	param[0].length = &lengths[0];
	param[0].error = &error[0];
	
	/* STRING COLUMN */
	param[1].buffer_type = MYSQL_TYPE_STRING;
	param[1].buffer = &first_name_buffer;
	param[1].is_null = &is_null[1];
	param[1].length = &lengths[1];
	param[1].error = &error[1];
	param[1].buffer_length = 255;
	
	/* STRING COLUMN */
	param[2].buffer_type = MYSQL_TYPE_STRING;
	param[2].buffer = &last_name_buffer;
	param[2].is_null = &is_null[2];
	param[2].length = &lengths[2];
	param[2].error = &error[2];
	param[2].buffer_length = 255;
	
	/* STRING COLUMN */
	param[3].buffer_type = MYSQL_TYPE_STRING;
	param[3].buffer = &jmbg_buffer;
	param[3].is_null = &is_null[3];
	param[3].length = &lengths[3];
	param[3].error = &error[3];
	param[3].buffer_length = 13;
	

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
	res->type = PERSON_E;
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
		row->data = calloc(1, sizeof(PERSON));
		
		if (is_null[0]) {
			((PERSON*) row->data)->id_person = 0;
		} else {
			((PERSON*) row->data)->id_person = id_person_buffer;
		}
		if (is_null[1]) {
			strcpy(((PERSON*) row->data)->first_name, "NULL");
		} else {
			strncpy(((PERSON*) row->data)->first_name, first_name_buffer, lengths[1]);
		}
		if (is_null[2]) {
			strcpy(((PERSON*) row->data)->last_name, "NULL");
		} else {
			strncpy(((PERSON*) row->data)->last_name, last_name_buffer, lengths[2]);
		}
		if (is_null[3]) {
			strcpy(((PERSON*) row->data)->jmbg, "NULL");
		} else {
			strncpy(((PERSON*) row->data)->jmbg, jmbg_buffer, lengths[3]);
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
PERSON* person_find_by_id(uint id) {
	#define QUERY "select * from person where id_person = ?;"
	#define PARAM_COUNT 1
	PERSON* out;

	
	SQL_RESULT* res;
	struct person person;
	person.id_person = id;
	struct person* personT = &person;
	

	/* Generated using  get_col_param_buffers() */
	MYSQL_BIND param[PARAM_COUNT];
	memset(param, 0, sizeof(param));
	
	/* INTEGER PARAM */
	param[0].buffer = malloc(sizeof(uint));
	param[0].buffer_type = MYSQL_TYPE_LONG;
	memcpy(param[0].buffer, &personT->id_person, sizeof(uint));

	res = person_execute_find(QUERY, param, PARAM_COUNT);

	/* Generated using col_param_buffer_free() */
	free(param[0].buffer);
	

if (res->results == NULL) { return NULL; }

	out = res->results->data;
	if (res->count == 1) {
		free(res->results);
		free(res);
		return out;
	} else {
		fprintf(stderr, "person_execute_find(), failed - multiple results (%d)\n", res->count);
		mysql_res_free(&res);
		return NULL;
	}

	#undef QUERY
	#undef PARAM_COUNT
}


/* Generated function */
int person_update(PERSON* personT) {
	#define QUERY "update person set first_name = ?, last_name = ?, jmbg = ? where id_person = ?;"
	#define PARAM_COUNT 4
	#define STRING_SIZE 255
	#define FIRST_NAME_SIZE 255
	#define LAST_NAME_SIZE 255
	#define JMBG_SIZE 13
	assert(personT->id_person != 0);

	int retval;

	/* Generated using col_update_params() */
	MYSQL_BIND param[PARAM_COUNT];
	memset(&param, 0, sizeof(param));
	
	unsigned long first_name_len;
	first_name_len = strnlen(personT->first_name, FIRST_NAME_SIZE);
	
	unsigned long last_name_len;
	last_name_len = strnlen(personT->last_name, LAST_NAME_SIZE);
	
	unsigned long jmbg_len;
	jmbg_len = strnlen(personT->jmbg, JMBG_SIZE);
	
	/* STRING PARAM */
	param[0].buffer = malloc(first_name_len);
	param[0].buffer_type = MYSQL_TYPE_STRING;
	param[0].buffer_length = first_name_len;
	strncpy(param[0].buffer, personT->first_name, first_name_len);
	/* STRING PARAM */
	param[1].buffer = malloc(last_name_len);
	param[1].buffer_type = MYSQL_TYPE_STRING;
	param[1].buffer_length = last_name_len;
	strncpy(param[1].buffer, personT->last_name, last_name_len);
	/* STRING PARAM */
	param[2].buffer = malloc(jmbg_len);
	param[2].buffer_type = MYSQL_TYPE_STRING;
	param[2].buffer_length = jmbg_len;
	strncpy(param[2].buffer, personT->jmbg, jmbg_len);
	/* INTEGER PARAM */
	param[3].buffer = malloc(sizeof(uint));
	param[3].buffer_type = MYSQL_TYPE_LONG;
	memcpy(param[3].buffer, &personT->id_person, sizeof(uint));

	retval = person_execute(QUERY, param, PARAM_COUNT);

	/* Generated using col_buffer_free() */
	free(param[0].buffer);
	free(param[1].buffer);
	free(param[2].buffer);
	free(param[3].buffer);
	

	return retval;

	#undef QUERY
	#undef PARAM_COUNT
	#undef STRING_SIZE
	#undef FIRST_NAME_SIZE
	#undef LAST_NAME_SIZE
	#undef JMBG_SIZE
}


/* Generated function */
int person_execute(char const* query, MYSQL_BIND* params, uint param_count) {
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
int person_delete(PERSON* personT) {
	#define QUERY "delete from person where id_person = ?;"
	#define PARAM_COUNT 1
	assert(personT->id_person != 0);

	int retval;

	/* Generated using  get_col_param_buffers() */
	MYSQL_BIND param[PARAM_COUNT];
	memset(param, 0, sizeof(param));
	
	/* INTEGER PARAM */
	param[0].buffer = malloc(sizeof(uint));
	param[0].buffer_type = MYSQL_TYPE_LONG;
	memcpy(param[0].buffer, &personT->id_person, sizeof(uint));

	retval = person_execute(QUERY, param, PARAM_COUNT);

	/* Generated using col_param_buffer_free() */
	free(param[0].buffer);
	

	return retval;

	#undef QUERY
	#undef PARAM_COUNT
}


/* Generated function */
SQL_RESULT* person_find_all() {
	#define QUERY "select * from person;"
	#define PARAM_COUNT 0
	SQL_RESULT* res;

	MYSQL_BIND param[1];

	res = person_execute_find(QUERY, param, PARAM_COUNT);

	return res;

	#undef QUERY
	#undef PARAM_COUNT
}


/* Generated function */
void person_free(PERSON** ptr) {
	/* Generated by get_free_members() */
	assert(ptr != NULL);
	
	if (*ptr != NULL){
		free(*ptr);
	}

}
