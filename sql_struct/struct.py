from typing import List

from header import Header
from sql_types import SqlType, SqlColumn
from function import FunctionTemplate, FunctionArgument, MacroDefinition


class Prop:
	def __init__(self, name: str, proptype: SqlColumn):
		self.name = name
		self.proptype = proptype


class Struct:
	def __init__(self, name: str, columns: List[SqlColumn]):
		assert columns[0].proptype == SqlType.PK_LONG

		self.members = columns
		self.name = name
		self.typedef_name = name.upper()
		self.enum_name = name.upper() + "_E"

	def __getitem__(self, key) -> SqlColumn:
		return self.members[key]

	def __setitem__(self, key, value):
		self.members[key] = value

	def __repr__(self) -> str:
		out = ""
		for prop in self.members:
			out += "{:10} : {}\n".format(prop.name, prop.proptype)
		return out

	def col_count(self):
		"""
		Returns the number of columns.
		"""
		return len(self.members)

	def param_count(self):
		"""
		Returns the number of columns excluding Primary Key.
		"""
		return len(self.members) - 1

	def get_col_buffer_definitions(self):
		"""
		Defines buffers which are used for SQL Result set binding.
		"""
		out = "/* Generated using get_col_buffer_definitions()*/\n"
		out += """unsigned long lengths[RES_COL_COUNT];
		my_bool is_null[RES_COL_COUNT];
		my_bool error[RES_COL_COUNT];\n"""
		for prop in self.members:
			out += "\t"
			if prop.proptype == SqlType.VARCHAR:
				out += "char {col}_buffer[BUFFER_SIZE];\n".format(col=prop.name)
			elif prop.proptype in [SqlType.LONG, SqlType.FK_LONG, SqlType.PK_LONG]:
				out += "uint {col}_buffer;\n".format(col=prop.name)
		return out

	def get_buffer_bindings(self):
		"""
		Generates binding for every struct member in the query. Buffers to which members are bound
		must be defined with get_col_buffer_definitions().
		"""
		out = "/* Generated using get_buffer_bindings()*/\n"
		out += "MYSQL_BIND param[RES_COL_COUNT];\n"
		out += "memset(param, 0, sizeof(param));\n"
		for i, prop in enumerate(self.members):
			out += self.bind_prop_buffer(i, prop)
		return out

	@staticmethod
	def bind_prop_buffer(index, prop):
		"""
		Generates code that binds parameter structs memembers to predefined stack allocated buffers.
		Buffers must be defined with get_col_buffer_definitions().
		"""
		out = """
				/* {type} COLUMN */
				param[{index}].buffer_type = {mysql_type};
				param[{index}].buffer = &{name}_buffer;
				param[{index}].is_null = &is_null[{index}];
				param[{index}].length = &lengths[{index}];
				param[{index}].error = &error[{index}];
				"""
		reg_type = None
		mysql_type = None
		if prop.proptype == SqlType.VARCHAR:
			reg_type = "STRING"
			mysql_type = "MYSQL_TYPE_STRING"
			out += "param[{index}].buffer_length = {len};\n".format(index=index, len=prop.size)

		elif prop.proptype in [SqlType.LONG, SqlType.FK_LONG, SqlType.PK_LONG]:
			reg_type = "INTEGER"
			mysql_type = "MYSQL_TYPE_LONG"

		assert reg_type is not None
		assert mysql_type is not None

		return out.format(index=index, type=reg_type, mysql_type=mysql_type, name=prop.name)

	def col_fetch(self):
		"""
		Generates part of the code responsible for looping over results fetched from
		executing the SQL statement. Respective buffers must be defined and bound.
		"""
		cols = ""

		for i, prop in enumerate(self.members):
			if prop.proptype == SqlType.VARCHAR:
				cols += """
					if (is_null[{index}]) {{
						strcpy((({name}*) row->data)->{col}, "NULL");
					}} else {{
						strncpy((({name}*) row->data)->{col}, {col}_buffer, lengths[{index}]);
					}}""".format(index=i, col=prop.name, name=self.typedef_name)
			elif prop.proptype in [SqlType.LONG, SqlType.PK_LONG]:
				cols += """
					if (is_null[{index}]) {{
						(({name}*) row->data)->{col} = 0;
					}} else {{
						(({name}*) row->data)->{col} = {col}_buffer;
					}}""".format(index=i, col=prop.name, name=self.typedef_name)
			elif prop.proptype == SqlType.FK_LONG:
				cols += """
					if (is_null[{index}]) {{
						(({name}*) row->data)->{col_name} = NULL;
					}} else {{
						(({name}*) row->data)->{col_name} = {col_name}_find_by_id({col}_buffer);
					}}""".format(index=i, col=prop.name, col_name=prop.name[3:], name=self.typedef_name)

		return """
			/* Generated using col_fetch()*/
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

	def get_col_param_buffers(self):
		"""
		Generates buffers for all available struct members.
		"""
		out = "/* Generated using  get_col_param_buffers() */\n"
		for i, prop in enumerate(self.members):
			if i == 0:
				continue
			out += self.col_param_from_prop(i - 1, prop, self.name)
		return out

	def get_col_param_buffer(self, props: List[str]):
		"""
		Generates buffers for the selected struct members.
		"""
		members = []
		for mem in self.members:
			if mem.name in props:
				members.append(mem)

		out = "/* Generated using  get_col_param_buffers() */\n"
		out += "MYSQL_BIND param[PARAM_COUNT];\n"
		out += "memset(param, 0, sizeof(param));\n"
		for i, prop in enumerate(members):
			out += self.col_param_from_prop(i, prop, self.name)
		return out

	@staticmethod
	def col_param_from_prop(num, prop, name):
		"""
		Allocates respective param buffers that store struct data used in the SQL Query.
		Buffers are used in queries such as 'UPDATE' or 'INSERT'

		Example:

		/* STRING PARAM */
		param[1].buffer = malloc(name_len);
		param[1].buffer_type = MYSQL_TYPE_STRING;
		param[1].buffer_length = name_len;
		strncpy(param[1].buffer, libraryT->name, name_len);

		Make sure to free buffers using col_param_buffer_free().
		"""
		out = """
				/* {type} PARAM */
				param[{index}].buffer = malloc({buffer_size});
				param[{index}].buffer_type = {mysql_type};
				"""
		reg_type = None
		mysql_type = None
		buffer_size = None
		if prop.proptype == SqlType.VARCHAR:
			reg_type = "STRING"
			mysql_type = "MYSQL_TYPE_STRING"
			buffer_size = "{col}_len".format(col=prop.name)
			out += """param[{index}].buffer_length = {col}_len;
				strncpy(param[{index}].buffer, {name}T->{col}, {col}_len);"""
		elif prop.proptype in [SqlType.LONG, SqlType.PK_LONG]:
			reg_type = "INTEGER"
			mysql_type = "MYSQL_TYPE_LONG"
			buffer_size = "sizeof(uint)"
			out += "memcpy(param[{index}].buffer, &{name}T->{col}, {buffer_size});"

		elif prop.proptype == SqlType.FK_LONG:
			reg_type = "INTEGER"
			mysql_type = "MYSQL_TYPE_LONG"
			buffer_size = "sizeof(uint)"
			out += "memcpy(param[{index}].buffer, &{name}T->{col_fk}->{col}, {buffer_size});"

		assert reg_type is not None
		assert mysql_type is not None
		assert buffer_size is not None

		return out.format(index=num, type=reg_type, mysql_type=mysql_type, col=prop.name, name=name,
		                  buffer_size=buffer_size, col_fk=prop.name[3:])

	def col_param_buffer_free(self, num: int = None):
		"""
		Frees allocated param buffers after use.
		"""
		out = "/* Generated using col_param_buffer_free() */\n"
		if num is None:
			for i in range(self.param_count()):
				out += "free(param[{index}].buffer);\n".format(index=i)
		else:
			for i in range(num):
				out += "free(param[{index}].buffer);\n".format(index=i)
		return out

	def col_buffer_free(self, num: int = None):
		"""
		Frees allocated param buffers after use.
		"""
		out = "/* Generated using col_buffer_free() */\n"
		if num is None:
			for i in range(self.col_count()):
				out += "free(param[{index}].buffer);\n".format(index=i)
		else:
			for i in range(num):
				out += "free(param[{index}].buffer);\n".format(index=i)
		return out

	def get_update_fk(self):
		"""
		Updates Foreign Key references in 'UPDATE' or 'INSERT' methods.
		"""
		out = "/* Generated using get_update_fk() */\n"
		for prop in self.members:
			if prop.proptype == SqlType.FK_LONG:
				out += """			
				if ({name}T->{fk_name}->{fk_id} == 0) {{
					{fk_name}_insert({name}T->{fk_name});
				}} else {{
					{fk_name}_update({name}T->{fk_name});
				}}""".format(name=self.name, fk_name=prop.name[3:], fk_id=prop.name)
		return out

	def col_param_lengths(self, func_ref: FunctionTemplate):
		"""
		Defines '#define' preprocessor directives and stack variables specifying sizes of column fields.
		"""
		out = "/* Generated using col_param_lengths() */\n"
		out += "MYSQL_BIND param[PARAM_COUNT];\n"
		out += "memset(&param, 0, sizeof(param));\n"
		for prop in self.members:
			if prop.proptype == SqlType.VARCHAR:
				out += self.col_param_length(prop, func_ref)
		return out

	def col_param_length(self, prop, func_ref: FunctionTemplate):
		"""
		Defines '#define' preprocessor directive and stack variable specifying size of the column field.
		"""
		func_ref.add_macro_def(
			MacroDefinition("{name_upper}_SIZE".format(name_upper=prop.name.upper()), str(prop.size)))
		return """
			unsigned long {name}_len;
			{name}_len = strnlen({struct}T->{name}, {name_upper}_SIZE);
			""".format(name=prop.name, name_upper=prop.name.upper(), struct=self.name)

	def col_update_params(self, func_ref: FunctionTemplate):
		"""
		Allocates and bind param buffers used in execute_update methods.
		Be sure to free allocated buffers with col_param_buffer_free().
		"""
		out = "/* Generated using col_update_params() */\n"
		out += "MYSQL_BIND param[PARAM_COUNT];\n"
		out += "memset(&param, 0, sizeof(param));\n"
		memb = self.members.copy()
		memb.append(memb.pop(0))
		for prop in memb:
			if prop.proptype == SqlType.VARCHAR:
				out += self.col_param_length(prop, func_ref)

		for i, prop in enumerate(memb):
			out += self.col_param_from_prop(i, prop, self.name)
		return out

	def get_insert_assertions(self):
		"""
		Generates basic assertions for 'INSERT' query.
		"""
		out = "/* Generated using get_insert_assertions() */\n"
		for prop in self.members:
			if prop.proptype == SqlType.PK_LONG:
				out += "assert({name}T->id_{name} == 0);\n".format(name=self.name)
			elif prop.proptype == SqlType.VARCHAR:
				out += "assert(strnlen({name}T->{prop_name}, STRING_SIZE) > 1);\n".format(name=self.name,
				                                                                          prop_name=prop.name)
		return out

	def get_params(self):
		return [prop for prop in self.members[1:]]

	def get_pk(self):
		return self.members[0]

	def insert_sql_string(self):
		"""
		Generates 'INSERT' SQL command for respective struct.
		"""
		return f'insert into {self.name} ({", ".join([param.name for param in self.get_params()])}) values ({", ".join(("?" * self.param_count()))});'

	def update_sql_string(self):
		"""
		Generates 'UPDATE' SQL command for respective struct.
		"""
		# noinspection SqlResolve
		return f'update {self.name} set {", ".join([param.name + " = ?" for param in self.get_params()])} where id_{self.name} = ?;'

	def find_by_id_sql_string(self):
		"""
		Generates 'SELECT' SQL command for respective struct that finds by ID.
		"""
		# noinspection SqlResolve
		return f'select * from {self.name} where id_{self.name} = ?;'

	def genereate_h(self):
		return str(Header(self))

	def save_to_file(self):
		"""
		Saves all structs methods to file '{name}.c'.
		"""
		insert_func = self.template_insert()
		execute_find_func = self.template_execute_find()
		find_by_id_func = self.template_find_by_id()
		update_func = self.template_update()
		execute_update_func = self.template_execute_update()

		with open("out/{name}.c".format(name=self.name), "w") as file:
			file.write(str(insert_func))
			file.write(str(execute_find_func))
			file.write(str(find_by_id_func))
			file.write(str(update_func))
			file.write(str(execute_update_func))
		with open("out/{name}.h".format(name=self.name), "w") as file:
			file.write(self.genereate_h())

	def declaration_execute_find(self):
		arg_list = [FunctionArgument("char const*", "query"),
		            FunctionArgument("MYSQL_BIND*", "params"),
		            FunctionArgument("uint", "param_count")]
		func = FunctionTemplate("{name}_execute_find".format(name=self.name), "SQL_RESULT*", arg_list)
		return func

	def template_execute_find(self):
		func = self.declaration_execute_find()

		func.add_macro_def(MacroDefinition("QUERY_SIZE", "512"))
		func.add_macro_def(MacroDefinition("RES_COL_COUNT", str(self.col_count())))
		func.add_macro_def(MacroDefinition("BUFFER_SIZE", "255"))
		func.add_block("""MYSQL* __attribute__((cleanup(mysql_con_cleanup))) conn;
		SQL_RESULT* res;
		MYSQL_RES* prepare_meta_result;
		MYSQL_STMT* stmt;""")
		func.add_block(self.get_col_buffer_definitions())
		func.add_block("""conn = db_init();
			stmt = mysql_stmt_init(conn);
		""")
		func.add_block("""if (mysql_stmt_prepare(stmt, query, strnlen(query, QUERY_SIZE))) {
				fprintf(stderr, " mysql_stmt_prepare(), SELECT failed\\n");
				fprintf(stderr, " %s\\n", mysql_stmt_error(stmt));
				return NULL;
			}
			mysql_stmt_bind_param(stmt, params);
			assert(param_count == mysql_stmt_param_count(stmt));""")
		func.add_block("""/* Fetch result set meta information */
			prepare_meta_result = mysql_stmt_result_metadata(stmt);
			if (!prepare_meta_result) {
				fprintf(stderr, " mysql_stmt_result_metadata(), returned no meta information\\n");
				fprintf(stderr, " %s\\n", mysql_stmt_error(stmt));
				return NULL;
			}
			assert(RES_COL_COUNT == mysql_num_fields(prepare_meta_result));""")
		func.add_block("""/* Execute the SELECT query */
		if (mysql_stmt_execute(stmt)) {
			fprintf(stderr, " mysql_stmt_execute(), failed\\n");
			fprintf(stderr, " %s\\n", mysql_stmt_error(stmt));
			return NULL;
		}""")
		func.add_block(self.get_buffer_bindings())
		func.add_block("""/* Bind the result buffers */
		if (mysql_stmt_bind_result(stmt, param)) {
			fprintf(stderr, " mysql_stmt_bind_result() failed\\n");
			fprintf(stderr, " %s\\n", mysql_stmt_error(stmt));
			return NULL;
		}""")
		func.add_block("""/* Now buffer all results to client (optional step) */
		if (mysql_stmt_store_result(stmt)) {
			fprintf(stderr, " mysql_stmt_store_result() failed\\n");
			fprintf(stderr, " %s\\n", mysql_stmt_error(stmt));
			return NULL;
		}""")
		func.add_block("""/* Fetch all rows */
			struct sql_result_row* row = NULL;
			struct sql_result_row* curr = NULL;
		
			res = calloc(1, sizeof(SQL_RESULT));
			res->results = NULL;
			res->type = {enum_name};
			res->count = 0;""".format(enum_name=self.enum_name))
		func.add_block(self.col_fetch())
		func.add_block("""/* Free the prepared result metadata */
			mysql_free_result(prepare_meta_result);
			if (mysql_stmt_close(stmt)) {
				fprintf(stderr, " failed while closing the statement\\n");
				fprintf(stderr, " %s\\n", mysql_error(conn));
				mysql_res_free(&res);
				return NULL;
			}""")
		func.add_block("return res;")
		return func

	def declaration_find_by_id(self):
		arg_list = [FunctionArgument("uint", "id")]
		return FunctionTemplate("{}_find_by_id".format(self.name), "{}*".format(self.typedef_name), arg_list)

	def template_find_by_id(self):
		func = self.declaration_find_by_id()
		func.add_macro_def(MacroDefinition("QUERY", '"{}"'.format(self.find_by_id_sql_string())))
		func.add_macro_def(MacroDefinition("PARAM_COUNT", "1"))
		func.add_block("{}* out;".format(self.typedef_name))
		func.add_block("""
			SQL_RESULT* res;
			struct {name} {name};
			{name}.id_{name} = id;
			struct {name}* {name}T = &{name};
			""".format(name=self.name))
		func.add_block(self.get_col_param_buffer(["id_{}".format(self.name)]))
		func.add_block("res = {}_execute_find(QUERY, param, PARAM_COUNT);".format(self.name))
		func.add_block(self.col_param_buffer_free(1))
		func.add_block("""out = res->results->data;
			if (res->count == 1) {{
				free(res->results);
				free(res);
				return out;
			}} else {{
				fprintf(stderr, "{name}_execute_find(), failed - multiple results (%d)\\n", res->count);
				mysql_res_free(&res);
				return NULL;
			}}""".format(name=self.name))
		return func

	def declaration_insert(self):
		arg_list = [FunctionArgument("{}*".format(self.typedef_name), "{}T".format(self.name))]
		return FunctionTemplate("{}_insert".format(self.name), "uint", arg_list)

	def template_insert(self):
		func = self.declaration_insert()
		func.add_macro_def(MacroDefinition("QUERY_LENGTH", "512"))
		func.add_macro_def(MacroDefinition("STRING_SIZE", "255"))
		func.add_macro_def(MacroDefinition("QUERY", '"{}"'.format(self.insert_sql_string())))
		func.add_macro_def(MacroDefinition("PARAM_COUNT", "{}".format(self.param_count())))
		func.add_block(self.get_insert_assertions())
		func.add_block("""MYSQL* __attribute__((cleanup(mysql_con_cleanup))) conn;
				MYSQL_STMT* __attribute__((cleanup(mysql_stmt_cleanup))) stmt;
				uint retval;
				""")
		func.add_block("""conn = db_init();
				stmt = mysql_stmt_init(conn);""")
		func.add_block(self.col_param_lengths(func))
		func.add_block(self.get_col_param_buffers())
		func.add_block(self.get_update_fk())
		func.add_block("""if (mysql_stmt_prepare(stmt, QUERY, QUERY_LENGTH)) {
					fprintf(stderr, " mysql_stmt_prepare(), failed\\n");
					fprintf(stderr, " %s\\n", mysql_stmt_error(stmt));
					return 0U;
					}""")
		func.add_block("""if (mysql_stmt_bind_param(stmt, param)) {
					fprintf(stderr, " mysql_stmt_bind_param(), failed\\n");
					fprintf(stderr, " %s\\n", mysql_stmt_error(stmt));
					return 0U;
				}""")
		func.add_block("""if (mysql_stmt_execute(stmt)) {
					fprintf(stderr, " mysql_stmt_execute(), failed\\n");
					fprintf(stderr, " %s\\n", mysql_stmt_error(stmt));
					return 0U;
				}""")
		func.add_block("""retval = (uint) mysql_stmt_insert_id(stmt);
				// update id after insertion;""")
		func.add_block("{name}T->{pk} = retval;".format(name=self.name, pk="id_" + self.name))
		func.add_block(self.col_param_buffer_free())
		func.add_block("return retval;")
		return func

	def declaration_update(self):
		arg_list = [FunctionArgument("{}*".format(self.typedef_name), "{}T".format(self.name))]
		return FunctionTemplate("{}_update".format(self.name), "int", arg_list)

	def template_update(self):
		func = self.declaration_update()
		func.add_block("assert({name}T->id_{name} != 0);".format(name=self.name))
		func.add_macro_def(MacroDefinition("QUERY", '"{}"'.format(self.update_sql_string())))
		func.add_macro_def(MacroDefinition("PARAM_COUNT", str(self.col_count())))
		func.add_macro_def(MacroDefinition("STRING_SIZE", "255"))
		func.add_block("""int retval;""")
		func.add_block(self.col_update_params(func))
		func.add_block("retval = {}_execute_update(QUERY, param, PARAM_COUNT);".format(self.name))
		func.add_block(self.col_buffer_free())
		func.add_block("return retval;")
		return func

	def declaration_execute_update(self):
		arg_list = [FunctionArgument("char const*", "query"),
		            FunctionArgument("MYSQL_BIND*", "params"),
		            FunctionArgument("uint", "param_count")]
		return FunctionTemplate("{name}_execute_update".format(name=self.name), "int", arg_list)

	def template_execute_update(self):
		func = self.declaration_execute_update()
		func.add_macro_def(MacroDefinition("QUERY_LENGTH", '512'))
		func.add_block('\tMYSQL_STMT* stmt;\t\nMYSQL* __attribute__((cleanup(mysql_con_cleanup))) conn;\t\nint retval;')
		func.add_block('\tconn = db_init();\t\nstmt = mysql_stmt_init(conn);')
		func.add_block("""if ((retval = mysql_stmt_prepare(stmt, query, strnlen(query, QUERY_LENGTH)))) {
					fprintf(stderr, "mysql_stmt_prepare(), failed\\n");
					fprintf(stderr, "%s\\n", mysql_error(conn));
					return retval;
				}""")
		func.add_block("""if ((retval = mysql_stmt_bind_param(stmt, params))) {
					fprintf(stderr, "mysql_stmt_bind_param(), failed\\n");
					fprintf(stderr, "%s\\n", mysql_error(conn));
					return retval;
				}""")
		func.add_block("""if ((retval = mysql_stmt_execute(stmt))) {
					fprintf(stderr, "mysql_stmt_execute(), failed\\n");
					fprintf(stderr, "%s\\n", mysql_error(conn));
					return retval;
				}""")
		func.add_block("""if ((retval = mysql_stmt_close(stmt))) {
					fprintf(stderr, " failed while closing the statement\\n");
					fprintf(stderr, " %s\\n", mysql_error(conn));
					return retval;
				}""")
		return func


region_props = [SqlColumn("id_region", SqlType.PK_LONG, 4),
                SqlColumn("name", SqlType.VARCHAR, 255)]

region = Struct("region", region_props)

address_props = [SqlColumn("id_address", SqlType.PK_LONG, 4), SqlColumn("id_municipality", SqlType.FK_LONG, 4),
                 SqlColumn("street", SqlType.VARCHAR, 255), SqlColumn("number", SqlType.VARCHAR, 8)]
address = Struct("address", address_props)

library_props = [SqlColumn("id_library", SqlType.PK_LONG, 4),
                 SqlColumn("id_address", SqlType.FK_LONG, 4),
                 SqlColumn("name", SqlType.VARCHAR, 255)]
library = Struct("library", library_props)

# print(library.__repr__())

region.save_to_file()
address.save_to_file()
library.save_to_file()

f1 = FunctionTemplate("region_execute_find", "SQL_RESULT*",
                      [FunctionArgument("char const*", "query"),
                       FunctionArgument("MYSQL_BIND*", "params"),
                       FunctionArgument("uint", "param_count")])
f1.add_macro_def(MacroDefinition("QUERY", '"select * from library;"'))
# print(library.genereate_h())
# print(library.template_execute_update())
