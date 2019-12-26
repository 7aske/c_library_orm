from sql_types import SqlColumn, SqlType
import os


class Header:
	def __init__(self, struct):
		self.struct = struct

	def format_prop(self, prop: SqlColumn):

		if prop.proptype == SqlType.VARCHAR:
			return "char {name}[{size}];".format(name=prop.name, size=prop.size)
		elif prop.proptype == SqlType.TEXT:
			return "char {name}[{size}];".format(name=prop.name, size=prop.size)
		elif prop.proptype == SqlType.PK_LONG:
			return "uint {name};".format(name=prop.name)
		elif prop.proptype == SqlType.FK_LONG:
			return "struct {name}* {name};".format(name=prop.name.replace("_id", "").replace("id_", ""))
		elif prop.proptype == SqlType.LONG:
			return "uint {name};".format(name=prop.name)
		elif prop.proptype == SqlType.DATE:
			return "struct tm {name};".format(name=prop.name)
		else:
			msg = f"SQL type not handled '{prop}'"
			assert False, msg

	def __str__(self):
		assert len(self.struct.methods) > 0
		out = ""
		out += "#ifndef __{uname}_DB_STRUCT_{struct}_H\n".format(struct=self.struct.name.upper(), uname=os.getlogin())
		out += "#define __{uname}_DB_STRUCT_{struct}_H\n\n".format(struct=self.struct.name.upper(), uname=os.getlogin())

		out += "struct {name} {{\n".format(name=self.struct.name)
		out += "".join(["\t" + self.format_prop(prop) + "\n" for prop in self.struct.members])
		out += "};\n\n"
		out += "typedef struct {name} {typedef};\n\n".format(name=self.struct.name, typedef=self.struct.typedef_name)
		out += "\n\n".join([func.get_declaration() for func in self.struct.methods])
		out += "\n\n#endif\n"

		return out
