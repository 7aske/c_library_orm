import enum


class SqlType(enum.Enum):
	LONG = 1,
	VARCHAR = 2,
	FK_LONG = 3


class PropType:
	def __init__(self, name: str, proptype: SqlType, size: int):
		self.name = name
		self.proptype = proptype
		self.size = size

	def __repr__(self):
		return "<PropType name: {:10} proptype: {:16} size: {:4}>".format(self.name, self.proptype, self.size)


class Prop:
	def __init__(self, name: str, proptype: PropType):
		self.name = name
		self.proptype = proptype


class Struct:
	def __init__(self, dictionary, name):
		self.name = name
		self.enum_name = name.upper() + "_E"
		self.struct = dictionary

	def __getitem__(self, key) -> PropType:
		return self.struct[key]

	def __setitem__(self, key, value):
		self.struct[key] = value

	def __repr__(self) -> str:
		out = ""
		for key, value in self.struct.items():
			out += "{:10} : {}\n".format(key, value)
		return out

	def col_buffer(self):
		out = ""
		for _, prop in self.struct.items():
			out += "\t"
			if prop.proptype == SqlType.VARCHAR:
				out += "char {col}_buffer[BUFFER_SIZE];\n".format(col=prop.name)
			elif prop.proptype == SqlType.LONG:
				out += "uint {col};\n".format(col=prop.name)
			elif prop.proptype == SqlType.FK_LONG:
				out += "uint {col};\n".format(col=prop.name)
		return out

	def col_count(self):
		return len(self.struct.keys())

	def col_bind(self):
		out = ""
		for i, kv in enumerate(self.struct.items()):
			out += self.col_bind_from_prop(i, kv[1])
		return out

	def col_bind_from_prop(self, num, prop):
		if prop.proptype == SqlType.VARCHAR:
			reg_type = "STRING"
			mysql_type = "MYSQL_TYPE_STRING"
			return """
				/* {TYPE} COLUMN */
				param[{INDEX}].buffer_type = {MYSQL_TYPE};
				param[{INDEX}].buffer = {COL}_buffer;
				param[{INDEX}].buffer_length = {LENGTH};
				param[{INDEX}].is_null = &is_null[{INDEX}];
				param[{INDEX}].length = &lengths[{INDEX}];
				param[{INDEX}].error = &error[{INDEX}];
				""".format(INDEX=num, TYPE=reg_type, MYSQL_TYPE=mysql_type, COL=prop.name, LENGTH=prop.size)
		elif prop.proptype == SqlType.LONG or prop.proptype == SqlType.FK_LONG:
			reg_type = "INTEGER"
			mysql_type = "MYSQL_TYPE_LONG"
			return """
				/* {TYPE} COLUMN */
				param[{INDEX}].buffer_type = {MYSQL_TYPE};
				param[{INDEX}].buffer = &{COL};
				param[{INDEX}].is_null = &is_null[{INDEX}];
				param[{INDEX}].length = &lengths[{INDEX}];
				param[{INDEX}].error = &error[{INDEX}];
				""".format(INDEX=num, TYPE=reg_type, MYSQL_TYPE=mysql_type, COL=prop.name)

	def col_fetch(self):
		ind = 0
		cols = ""

		for _, prop in self.struct.items():
			if prop.proptype == SqlType.VARCHAR:
				cols += """
					if (is_null[{ind}]) {{
						strcpy((({name}*) row->data)->{col}, "NULL");
					}} else {{
						strncpy((({name}*) row->data)->{col}, {col}_buffer, lengths[{ind}]);
					}}""".format(ind=ind, col=prop.name, name=self.name.upper())
			elif prop.proptype == SqlType.LONG:
				cols += """
					if (is_null[{ind}]) {{
						(({name}*) row->data)->{col} = 0;
					}} else {{
						(({name}*) row->data)->{col} = {col};
					}}""".format(ind=ind, col=prop.name, name=self.name.upper())
			elif prop.proptype == SqlType.FK_LONG:
				cols += """
					if (is_null[{ind}]) {{
						(({name}*) row->data)->{col_name} = NULL;
					}} else {{
						(({name}*) row->data)->{col_name} = {col_name}_find_by_id({col});
					}}""".format(ind=ind, col=prop.name, col_name=prop.name[3:], name=self.name.upper())
			ind += 1

		return """
			while (!mysql_stmt_fetch(stmt)) {{
				res->count++;
				row = calloc(1, sizeof(struct sql_result_row));
				if (res->results == NULL) {{
					res->results = row;
				}} else {{
					curr = res->results;
					while (curr->next != NULL) {{
						curr = curr->next;
					}}
					curr->next = row;
				}}
				row->data = calloc(1, sizeof({struct_name}));
				{cols}
			}}""".format(cols=cols, struct_name=self.name.upper())

	def param_count(self):
		return len(self.struct.keys()) - 1

	def col_param(self):
		out = ""
		for i, kv in enumerate(self.struct.items()):
			if i == 0:
				continue
			out += self.col_param_from_prop(i - 1, kv[1])
		return out

	def col_param_buffer_free(self):
		out = ""
		for i, kv in enumerate(self.struct.items()):
			if i == 0:
				continue
			out += "free(param[{INDEX}].buffer);\n".format(INDEX=i - 1)
		return out

	def fk_links(self):
		out = ""
		for key, prop in self.struct.items():
			if prop.proptype == SqlType.FK_LONG:
				out += """			
				if ({name}T->{fk_name}->{fk_id} == 0) {{
					{fk_name}_insert({name}T->{fk_name});
				}} else {{
					{fk_name}_update({name}T->{fk_name});
				}}""".format(name=self.name, fk_name=prop.name[3:], fk_id=prop.name)
		return out

	def col_param_lengths(self):
		out = ""
		for k, prop in self.struct.items():
			if prop.proptype == SqlType.VARCHAR:
				out += self.col_param_length(prop)
		return out

	def col_param_length(self, prop):
		return """
			#define {name_upper}_SIZE {size}
			unsigned long {name}_len;
			{name}_len = strnlen({struct}T->{name}, {name_upper}_SIZE);
			""".format(name=prop.name, name_upper=prop.name.upper(), size=prop.size, struct=self.name)

	def col_update_params(self):
		out = ""
		l = list(self.struct.items())
		l.append(l.pop(0))
		for prop in l:
			if prop[1].proptype == SqlType.VARCHAR:
				out += self.col_param_length(prop[1])

		for i, prop in enumerate(l):
			out += self.col_param_from_prop(i, prop[1])
		return out

	def insert_assertions(self):
		out = ""

		for k, prop in self.struct.items():
			if prop.name == "id_" + self.name:
				out += "assert({name}T->id_{name} == 0);\n".format(name=self.name)
			elif prop.proptype == SqlType.VARCHAR:
				out += "assert(strnlen({name}T->{prop_name}, STRING_SIZE) > 1);\n".format(name=self.name,
				                                                                          prop_name=prop.name)
		return out

	def get_params(self):
		return [prop[1] for prop in list(self.struct.items())[1:]]

	def get_pl(self):
		return list(self.struct.items())[1][1]

	def insert_sql_string(self):
		return f'insert into {self.name} ({", ".join([param.name for param in self.get_params()])}) values ({", ".join(("?" * self.param_count()))});'

	def update_sql_string(self):
		sql = "update region set name = ? WHERE id_region = ?;"
		return f'update {self.name} set {", ".join([param.name+" = ?" for param in self.get_params()])} where id_{self.name} = ?;'

	def find_by_id_sql_string(self):
		return f'select * from {self.name} where id_{self.name} = ?;'

	def col_param_from_prop(self, num, prop):
		buf_len = "4"
		malloc_size = "malloc({COL}_len)"
		if prop.proptype == SqlType.VARCHAR:
			reg_type = "STRING"
			mysql_type = "MYSQL_TYPE_STRING"
			buf_len = "{COL}_len".format(COL=prop.name)
			return """
					/* {TYPE} PARAM */
					param[{INDEX}].buffer = malloc({BUF_LEN});
					param[{INDEX}].buffer_type = {MYSQL_TYPE};
					param[{INDEX}].buffer_length = {BUF_LEN};
					strncpy(param[{INDEX}].buffer, {NAME}T->{COL}, {COL}_len);
					""".format(INDEX=num, TYPE=reg_type, MYSQL_TYPE=mysql_type, COL=prop.name, NAME=self.name,
			                   BUF_LEN=buf_len, MALLOC_SIZE=malloc_size)
		elif prop.proptype == SqlType.LONG:
			reg_type = "INTEGER"
			mysql_type = "MYSQL_TYPE_LONG"
			malloc_size = "sizeof(uint)"
			return """
					/* {TYPE} PARAM */
					param[{INDEX}].buffer = malloc({MALLOC_SIZE});
					param[{INDEX}].buffer_type = {MYSQL_TYPE};
					memcpy(param[{INDEX}].buffer, &{NAME}T->{COL}, {MALLOC_SIZE});
					""".format(INDEX=num, TYPE=reg_type, MYSQL_TYPE=mysql_type, COL=prop.name, NAME=self.name,
			                   BUF_LEN=buf_len, MALLOC_SIZE=malloc_size)
		elif prop.proptype == SqlType.FK_LONG:
			reg_type = "INTEGER"
			mysql_type = "MYSQL_TYPE_LONG"
			malloc_size = "sizeof(uint)"
			return """
					/* {TYPE} PARAM */
					param[{INDEX}].buffer = malloc({MALLOC_SIZE});
					param[{INDEX}].buffer_type = {MYSQL_TYPE};
					memcpy(param[{INDEX}].buffer, &{NAME}T->{COL_FK}->{COL}, {MALLOC_SIZE});
					""".format(INDEX=num, TYPE=reg_type, MYSQL_TYPE=mysql_type, COL=prop.name, COL_FK=prop.name[3:],
			                   NAME=self.name,
			                   BUF_LEN=buf_len, MALLOC_SIZE=malloc_size)

	def save_to_file(self):
		with open("{name}.c".format(name=self.name), "w") as file:
			file.write(self.template_insert())
			file.write(self.template_execute_find())
			file.write(self.template_find_by_id())
			file.write(self.template_update())
			file.write(self.template_execute_update())

	def template_execute_find(self):
		return """
	SQL_RESULT* {name}_execute_find(char const* query, MYSQL_BIND* params, uint param_count) {{
		#define QUERY_SIZE 512
		#define RES_COL_COUNT {col_count}
		#define BUFFER_SIZE 255
	
		MYSQL* __attribute__((cleanup(mysql_con_cleanup))) conn;
		SQL_RESULT* res;
		MYSQL_BIND param[RES_COL_COUNT];
		MYSQL_RES* prepare_meta_result;
		MYSQL_STMT* stmt;
	
		{buffer_declarations}
	
		unsigned long lengths[RES_COL_COUNT];
		my_bool is_null[RES_COL_COUNT];
		my_bool error[RES_COL_COUNT];
	
		conn = db_init();
		stmt = mysql_stmt_init(conn);
	
		if (mysql_stmt_prepare(stmt, query, strnlen(query, QUERY_SIZE))) {{
			fprintf(stderr, " mysql_stmt_prepare(), SELECT failed\\n");
			fprintf(stderr, " %s\\n", mysql_stmt_error(stmt));
			return NULL;
		}}
		mysql_stmt_bind_param(stmt, params);
		assert(param_count == mysql_stmt_param_count(stmt));
	
		/* Fetch result set meta information */
		prepare_meta_result = mysql_stmt_result_metadata(stmt);
		if (!prepare_meta_result) {{
			fprintf(stderr, " mysql_stmt_result_metadata(), returned no meta information\\n");
			fprintf(stderr, " %s\\n", mysql_stmt_error(stmt));
			return NULL;
		}}
		assert(RES_COL_COUNT == mysql_num_fields(prepare_meta_result));
	
		/* Execute the SELECT query */
		if (mysql_stmt_execute(stmt)) {{
			fprintf(stderr, " mysql_stmt_execute(), failed\\n");
			fprintf(stderr, " %s\\n", mysql_stmt_error(stmt));
			return NULL;
		}}
	
		/* Bind the result buffers for all RES_COL_COUNT columns before fetching them */
		memset(param, 0, sizeof(param));
		{column_declarations}
	
		/* Bind the result buffers */
		if (mysql_stmt_bind_result(stmt, param)) {{
			fprintf(stderr, " mysql_stmt_bind_result() failed\\n");
			fprintf(stderr, " %s\\n", mysql_stmt_error(stmt));
			return NULL;
		}}
	
		/* Now buffer all results to client (optional step) */
		if (mysql_stmt_store_result(stmt)) {{
			fprintf(stderr, " mysql_stmt_store_result() failed\\n");
			fprintf(stderr, " %s\\n", mysql_stmt_error(stmt));
			return NULL;
		}}
	
		/* Fetch all rows */
		struct sql_result_row* row = NULL;
		struct sql_result_row* curr = NULL;
	
		res = calloc(1, sizeof(SQL_RESULT));
		res->results = NULL;
		res->type = {enum_name};
		res->count = 0;
	
		{fetch_loop}
	
		/* Free the prepared result metadata */
		mysql_free_result(prepare_meta_result);
		if (mysql_stmt_close(stmt)) {{
			fprintf(stderr, " failed while closing the statement\\n");
			fprintf(stderr, " %s\\n", mysql_error(conn));
			mysql_res_free(&res);
			return NULL;
		}}
		return res;
		#undef QUERY_SIZE
		#undef RES_COL_COUNT
		#undef PARAM_COUNT
		#undef BUFFER_SIZE
	}}
	""".format(name=self.name, buffer_declarations=self.col_buffer(), fetch_loop=self.col_fetch(),
		       col_count=self.col_count(), column_declarations=self.col_bind(), enum_name=self.enum_name)

	def template_find_by_id(self):
		return """
		{struct_name}* {name}_find_by_id(uint id) {{
			#define QUERY "{sql_query}"
			#define PARAM_COUNT 1
			MYSQL_BIND param;
			{struct_name}* out;
			SQL_RESULT* res;
		
			memset(&param, 0, sizeof(param));
		
			param.buffer = malloc(sizeof(uint));
			param.buffer_type = MYSQL_TYPE_LONG;
			memcpy(param.buffer, &id, sizeof(uint));
		
			res = {name}_execute_find(QUERY, &param, PARAM_COUNT);
			out = res->results->data;
			free(param.buffer);
			if (res->count == 1) {{
				free(res->results);
				free(res);
				return out;
			}} else {{
				fprintf(stderr, "{name}_execute_find(), failed - multiple results (%d)\\n", res->count);
				mysql_res_free(&res);
				return NULL;
			}}
			#undef QUERY
			#undef PARAM_COUNT
		}}
		""".format(name=self.name, struct_name=self.name.upper(), sql_query=self.find_by_id_sql_string())

	def template_insert(self):
		return """
			uint {name}_insert({struct_name}* {name}T) {{
				#define QUERY_LENGTH 512
				#define STRING_SIZE 255
				#define QUERY "{sql_query}"
				#define PARAM_COUNT {param_count}
			
				{assertions}
			
				MYSQL* __attribute__((cleanup(mysql_con_cleanup))) conn;
				MYSQL_STMT* __attribute__((cleanup(mysql_stmt_cleanup))) stmt;
				MYSQL_BIND param[PARAM_COUNT];
				int retval;
				{length_definitions}
			
				conn = db_init();
				stmt = mysql_stmt_init(conn);
		
				{fk_linking}
							
				memset(&param, 0, sizeof(param));
			
				{param_setup}
			
				mysql_stmt_prepare(stmt, QUERY, QUERY_LENGTH);
				mysql_stmt_bind_param(stmt, param);
			
				mysql_stmt_execute(stmt);
			
				retval = (uint) mysql_stmt_insert_id(stmt);
				// update id after insertion;
				{name}T->{pk} = retval;
			
				{free_buffers}
				return retval;
				#undef QUERY_LENGTH
				#undef QUERY
				#undef STRING_SIZE
				#undef NUMBER_SIZE
				#undef PARAM_COUNT
			}}""".format(name=self.name, assertions=self.insert_assertions(), struct_name=self.name.upper(),
		                 length_definitions=self.col_param_lengths(), fk_linking=self.fk_links(),
		                 param_setup=self.col_param(),
		                 sql_query=self.insert_sql_string(),
		                 free_buffers=self.col_param_buffer_free(), param_count=self.param_count(),
		                 pk="id_" + self.name)

	def template_update(self):
		return """
			int {name}_update({struct_name}* {name}T) {{
				assert({name}T->id_{name} != 0);
				#define QUERY "{sql_query}"
				#define PARAM_COUNT {param_count}
				#define STRING_SIZE 255
				MYSQL_BIND param[PARAM_COUNT];
				ulong len;
				int retval;
			
				memset(&param, 0, sizeof(param));
				
				{update_params}
				
				retval = {name}_execute_update(QUERY, param, PARAM_COUNT);
				{free_buffers}
				return retval;
			}}""".format(name=self.name, struct_name=self.name.upper(), sql_query=self.update_sql_string(), param_count=self.col_count(),
		                 update_params=self.col_update_params(), free_buffers=self.col_param_buffer_free())

	def template_execute_update(self):
		return """
			int {name}_execute_update(char const* query, MYSQL_BIND* params, uint param_count) {{
				#define QUERY_LENGTH 512
				MYSQL_STMT* stmt;
				MYSQL* __attribute__((cleanup(mysql_con_cleanup))) conn;
				int retval;
			
				conn = db_init();
				stmt = mysql_stmt_init(conn);
			
				if ((retval = mysql_stmt_prepare(stmt, query, strnlen(query, QUERY_LENGTH)))) {{
					fprintf(stderr, "mysql_stmt_prepare(), failed\\n");
					fprintf(stderr, "%s\\n", mysql_error(conn));
					return retval;
				}}
			
				if ((retval = mysql_stmt_bind_param(stmt, params))) {{
					fprintf(stderr, "mysql_stmt_bind_param(), failed\\n");
					fprintf(stderr, "%s\\n", mysql_error(conn));
					return retval;
				}}
			
				if ((retval = mysql_stmt_execute(stmt))) {{
					fprintf(stderr, "mysql_stmt_execute(), failed\\n");
					fprintf(stderr, "%s\\n", mysql_error(conn));
					return retval;
				}}
			
				if ((retval = mysql_stmt_close(stmt))) {{
					fprintf(stderr, " failed while closing the statement\\n");
					fprintf(stderr, " %s\\n", mysql_error(conn));
					return retval;
				}}
				#undef QUERY_LENGTH
			}}""".format(name=self.name)


region = Struct({
	"id_region": PropType("id_region", SqlType.LONG, 4),
	"name"     : PropType("name", SqlType.VARCHAR, 255)
}, "region")

address = Struct({
	"id_address"     : PropType("id_address", SqlType.LONG, 4),
	"id_municipality": PropType("id_municipality", SqlType.FK_LONG, 4),
	"street"         : PropType("street", SqlType.VARCHAR, 255),
	"number"         : PropType("number", SqlType.VARCHAR, 8)
}, "address")

library = Struct({
	"id_library": PropType("id_library", SqlType.LONG, 4),
	"id_address": PropType("id_address", SqlType.FK_LONG, 4),
	"name"      : PropType("name", SqlType.VARCHAR, 255),
}, "library")

region.save_to_file()
address.save_to_file()
library.save_to_file()
