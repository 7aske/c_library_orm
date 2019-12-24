from sql_types import SqlColumn, SqlType


class Header:
	def __init__(self, struct):
		self.struct = struct

	def format_prop(self, prop: SqlColumn):
		if prop.proptype == SqlType.VARCHAR:
			return "char {name}[{size}];".format(name=prop.name, size=prop.size)
		elif prop.proptype == SqlType.PK_LONG:
			return "uint id_{name};".format(name=self.struct.name)
		elif prop.proptype == SqlType.FK_LONG:
			return "struct {name}* {name};".format(name=prop.name[3:])
		elif prop.proptype == SqlType.LONG:
			return "uint id_{name};".format(name=prop.name)

	def __str__(self):
		out = ""
		out += "#ifndef STRUCT_{struct}_H\n".format(struct=self.struct.name.upper())
		out += "#define STRUCT_{struct}_H\n\n".format(struct=self.struct.name.upper())

		out += "struct {name} {{\n".format(name=self.struct.name)
		out += "".join(["\t" + self.format_prop(prop) + "\n" for prop in self.struct.members])
		out += "};\n\n"
		out += "typedef struct {name} {typedef};\n\n".format(name=self.struct.name, typedef=self.struct.typedef_name)
		out += self.struct.declaration_insert().get_declaration()+"\n\n"
		out += self.struct.declaration_find_by_id().get_declaration()+"\n\n"
		out += self.struct.declaration_execute_find().get_declaration()+"\n\n"
		out += self.struct.declaration_update().get_declaration()+"\n\n"
		out += self.struct.declaration_execute_update().get_declaration()+"\n\n"
		out += self.struct.declaration_delete().get_declaration()+"\n\n"

		out += "#endif\n"

		return out
