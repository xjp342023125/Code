#pragma once
#include "CStr.hpp"
#include "cfile.hpp"
#include <sstream>
#include <set>

enum op {
	DB_CREATE,
	DB_ADD,
	DB_MODIFY,
	DB_DEL
};
struct CCodeField
{
	bool init_with_str(string s)
	{
		if (string::npos == s.find("="))
		{
			return false;
		}
		stringstream ss;
		ss << s;

		string _fix, type, name;
		ss >> _fix >> type >> name;
		if (name == "")
		{
			return false;
		}

		unsigned int min = string::npos;
		unsigned int idx_space = name.find_first_of(' ');
		unsigned int idx_equal = name.find_first_of('=');
		min = (min<idx_space) ? min : idx_space;
		min = (min<idx_equal) ? min : idx_equal;
		if (min != string::npos)
		{
			name = name.substr(0, min);
		}

		this->name = name;
		this->type = type;
		this->mysql_auto_inc = (string::npos != s.find("mysql_auto_inc"));
		this->mysql_primary_key = (string::npos != s.find("mysql_primary_key"));;
		this->mysql_unique_key = (string::npos != s.find("mysql_unique_key"));;
		this->mysql_key_ = (string::npos != s.find("mysql_key"));;

		this->mysql_int8 = (string::npos != s.find("mysql_int8"));;
		this->mysql_int16 = (string::npos != s.find("mysql_int16"));;
		this->mysql_uint8 = (string::npos != s.find("mysql_uint8"));;
		this->mysql_uint16 = (string::npos != s.find("mysql_uint16"));;

		return true;
	}
	bool mysql_key()
	{
		return mysql_primary_key || mysql_unique_key || mysql_key_;
	}

	string get_ctype()
	{
		if (mysql_int8)
			return "short";
		if (mysql_int16)
			return "short";
		if (mysql_uint8)
			return "unsigned short";
		if (mysql_uint16)
			return "unsigned short";


#define PROTO2C(proto,c)	if(type==(proto))	{return (c);}
		PROTO2C("bool", "bool");
		PROTO2C("double", "double");
		PROTO2C("float", "float");
		PROTO2C("int32", "int");
		PROTO2C("uint32", "unsigned int");
		PROTO2C("int64", "long long");
		PROTO2C("uint64", "unsigned long long");
		PROTO2C("string", "string");

		return "unkown type!";
	}
	
	string get_sql_type()
	{
		if (mysql_int8)
			return "tinyint(4) NOT NULL";
		if (mysql_int16)
			return "smallint(6) NOT NULL";
		if (mysql_uint8)
			return "tinyint(3) unsigned NOT NULL";
		if (mysql_uint16)
			return "smallint(5) unsigned NOT NULL";

#define PROTOSQL(proto,c)	if(type==(proto))	{return (c);}
		PROTOSQL("int32", "int(11) NOT NULL");
		PROTOSQL("uint32", "int(10) unsigned NOT NULL");
		PROTOSQL("int64", "bigint(20) NOT NULL");
		PROTOSQL("uint64", "bigint(20) unsigned NOT NULL");
		PROTOSQL("string", " varchar(255) NOT NULL ");

		return "unkown type!";
	}
	string get_sql(int opt)
	{
		string sql;
		switch (opt) 
		{
			case DB_CREATE:
				//`roleid` int(10) unsigned NOT NULL AUTO_INCREMENT
				sql = "  `" + name + "` " + get_sql_type() + get_sql_fix() + ",\r\n";
				break;
			case DB_ADD:
				sql = "ADD " + name + " " + get_sql_type() + get_sql_fix() + ";\r\n";
				break;
			case DB_MODIFY:
				sql = "MODIFY " + name + " " + get_sql_type() + get_sql_fix() + ";\r\n";
				break;
			case DB_DEL:
				sql = "DROP COLUMN " + name+ ";\r\n";
				break;
		}

		return sql;
	}
	string get_index_sql()
	{
		if (mysql_primary_key)
		{
			return  "  PRIMARY KEY(`" + name + "`),\r\n";
		}
		if (mysql_unique_key)
		{
			return "  UNIQUE KEY `" + name + "_key` (`" + name + "`),\r\n";
		}
		if (mysql_key_)
		{
			return "  KEY `" + name + "_key` (`" + name + "`),\r\n";
		}
		return "";
	}
	bool is_str()
	{
		return get_ctype() == "string";
	}
private:
	
	string get_sql_fix()
	{
		if (mysql_auto_inc)
		{
			return " AUTO_INCREMENT";
		}
		else
		{
			if (is_str())
			{
				return " DEFAULT ''";
			}
			else
			{
				return " DEFAULT '0'";
			}
		}
	}
public:
	string name;
	string type;
	bool mysql_auto_inc;
	bool mysql_primary_key;
	bool mysql_unique_key;
	bool mysql_key_;
public:
	bool mysql_int8;
	bool mysql_int16;
	bool mysql_uint8;
	bool mysql_uint16;
};

class CProtoPara
{
public:

	int para_with_str(string s)
	{
#define PARA_TEMP		"proto_temp.txt"
		XDelFile(PARA_TEMP);
		XWriteFile(PARA_TEMP, s.c_str(), s.size());

		linevec_t lines;
		XReadFileLine(PARA_TEMP, lines);

		for (size_t i = 0; i < lines.size(); i++)
		{
			if (m_table_name == "")
			{
				get_db_name(lines[i]);
				continue;
			}
			CCodeField filed;
			if (filed.init_with_str(lines[i]))
			{
				fields.push_back(filed);
			}
		}
		return 0;
	}
	string general_create_sql()
	{
		string sql;
		sql += "[------>New<------]\r\n";
		sql += "CREATE TABLE `" + m_table_name + "` (" + "\r\n";
		for (size_t i = 0; i < fields.size(); i++)
		{
			sql += fields[i].get_sql(DB_CREATE) ;
		}
		for (size_t i = 0; i < fields.size(); i++)
		{
			sql += fields[i].get_index_sql();
		}
		sql += ") ENGINE=InnoDB  DEFAULT CHARSET=utf8;\r\n";


		sql += "\r\n";
		sql += "\r\n";
		sql += "[------>Add<------]\r\n";
		for (size_t i = 0; i < fields.size(); i++)
		{
			sql += "ALTER TABLE " + m_table_name + " ";
			sql += fields[i].get_sql(DB_ADD);
		}

		sql += "\r\n";
		sql += "\r\n";
		sql += "[------>Mod<------]\r\n";
		for (size_t i = 0; i < fields.size(); i++)
		{
			sql += "ALTER TABLE " + m_table_name + " ";
			sql += fields[i].get_sql(DB_MODIFY);
		}

		sql += "\r\n";
		sql += "\r\n";
		sql += "[------>Del<------]\r\n";
		for (size_t i = 0; i < fields.size(); i++)
		{
			sql += "ALTER TABLE " + m_table_name + " ";
			sql += fields[i].get_sql(DB_DEL);
		}
		return sql;
	}
	string general_code()
	{
		string code;
		code += "//mysql table name:" + m_table_name + "\r\n";
		code += general_insert_code();
		code += general_get_code();
		code += general_update_code();
		code += general_del_code();

		return code;
	}
	string general_real_code()
	{
		string code;
		code += "//mysql table name:" + m_table_name + "\r\n";
		
		code += general_get_real_code();
		code += general_insert_real_code();
		code += general_update_real_code();
		code += general_del_real_code();
		return code;

	}

private:
	string general_del_real_code(CCodeField key)
	{
		string ret;
		char sz[5000] = { 0 };
		if (!key.is_str())
		{
			sprintf(sz, R"(
int MainLogic::del_%s_by_%s(%s %s)
{
	stringstream ss;
	ss << "delete from %s where %s=" << %s << ";";
	int sql_ret = db.do_write(ss.str());
	CHECK_RETURNV(0 == sql_ret, -1);
	return 0;
}
)",m_table_name.c_str(), key.name.c_str(), key.get_ctype().c_str(), key.name.c_str(), m_table_name.c_str(), key.name.c_str(), key.name.c_str());
		}
		else
		{
			sprintf(sz, R"(
int MainLogic::del_%s_by_%s(%s %s)
{
	stringstream ss;
	ss << "delete from %s where %s='" << db.escape_str(%s) << "';";
	int sql_ret = db.do_write(ss.str());
	CHECK_RETURNV(0 == sql_ret, -1);
	return 0;
}
)", m_table_name.c_str(), key.name.c_str(), key.get_ctype().c_str(), key.name.c_str(), m_table_name.c_str(), key.name.c_str(), key.name.c_str());
		}
		return sz;
	}
	string general_del_real_code()
	{
		string ret;
		for (size_t i = 0; i < fields.size(); i++)
		{
			if (fields[i].mysql_key())
			{
				ret += general_del_real_code(fields[i]);
			}
		}
		
		return ret;
	}
	string general_del_code()
	{
		string ret;
		for (size_t i = 0; i < fields.size(); i++)
		{
			if (fields[i].mysql_key())
			{
				ret += general_del_code_one(fields[i]);
			}
		}
		return ret;
	}
	string general_del_code_one(CCodeField key)
	{
		char sz[5000] = { 0 };
		sprintf(sz, "int del_%s_by_%s(%s %s);\r\n", m_table_name.c_str(), key.name.c_str(), key.get_ctype().c_str(), key.name.c_str());
		return sz;
	}


	string general_insert_real_code()
	{
		//"+ m_table_name + "
		string ret = "int MainLogic::insert_" + m_table_name + "(const " + m_table_name + " &val) \r\n";
		ret += "{\r\n";
		ret += "	stringstream ss;\r\n";
		ret += R"(	ss << "insert into )"+ m_table_name+R"(()" ;
	
		bool bfirst = true;
		for (size_t i = 0; i < fields.size(); i++)
		{
			if (fields[i].mysql_auto_inc)
				continue;
			if (bfirst)
				ret += fields[i].name;
			else
				ret += (","+fields[i].name);
			bfirst = false;
		}
		ret += R"() values (";)";
		ret += "\r\n";
		for (size_t i = 0; i < fields.size(); i++)
		{
			if (fields[i].mysql_auto_inc)
				continue;
			if (fields[i].is_str())
			{
				const char *p = NULL;
				if (i != fields.size() - 1)
				{
					p = R"(		ss << "'" << db.escape_str(val.%s()) << "',";)";
				}
				else
				{
					p = R"(		ss << "'" << db.escape_str(val.%s()) << "');";)";
				}
					
				char sz[500] = { 0 };
				sprintf(sz, p, fields[i].name.c_str());
				ret += sz;
			}
			else
			{
				ret += R"(	ss << val.)" + fields[i].name;
				if (i != fields.size()-1)
					ret += R"(() << ",";)";
				else
					ret += R"(() << ");";)";
			}
			ret += "\r\n";
		}
		ret += R"(	int sql_ret = db.do_write(ss.str());)"+ string("\r\n");
		ret += R"(	return sql_ret;)"+ string("\r\n");
		ret += R"(})"+ string("\r\n");

		//============4=================================================
		return ret;
	}
	string general_key_get_real_code(CCodeField key)
	{
		string ret;
		//"int get_role_info_by_roleid(unsigned int roleid, vector<role_info> &ret) { return 0; };";
		ret += "int MainLogic::get_" + m_table_name + "_by_" + key.name + "(" + key.get_ctype() + " " + key.name + ", vector<" + m_table_name + "> &ret) ";
		ret += R"(
{
	ret.clear();
	stringstream ss;
	ss << "select  )";

		bool bfirst = true;
		for (size_t i = 0; i < fields.size(); i++)
		{
			if (bfirst)
				ret += fields[i].name;
			else
				ret += ("," + fields[i].name);
			bfirst = false;
		}
		char sz[500] = { 0 };
		if (key.is_str())
			sprintf(sz, R"( from %s where %s='"<<db.escape_str(%s) << "';";)", m_table_name.c_str(), key.name.c_str(), key.name.c_str());
		else
			sprintf(sz, R"( from %s where %s="<<%s << ";";)", m_table_name.c_str(), key.name.c_str(), key.name.c_str());

		ret += sz;
		ret += R"(
	MYSQL_RES* res = (MYSQL_RES*)db.do_select(ss.str());
	CHECK_RETURNV(0 != res, -1);
	while (true)
	{
		MYSQL_ROW row = mysql_fetch_row(res);
		if (NULL == row)
		{
			break;
		}
		int idx = 0;)";

		ret += "\r\n";
		string obj ="		"+ m_table_name + "_one";
		ret += (m_table_name + " " + obj + ";\r\n");
		for (size_t i = 0; i < fields.size(); i++)
		{
			ret += "		"+obj + ".set_" + fields[i].name;
			if (fields[i].is_str())
			{
				ret += "(row[idx++]);\r\n";
			}
			else
			{
				ret += "(cstr_to_t<"+fields[i].get_ctype()+">(row[idx++]));\r\n";
			}
		}
		ret += R"(
		ret.push_back()"+ obj+R"();
	}
	if (res)
	{
		mysql_free_result(res);
		res = NULL;
	}
	return RET_SUC;
})";
		return ret;
	}
	string general_get_real_code()
	{
		string ret;
		for (size_t i = 0; i < fields.size(); i++)
		{
			if (fields[i].mysql_key())
			{
				ret += general_key_get_real_code(fields[i]);
				ret += "\r\n";
			}
		}
		ret += "\r\n";
		return ret;
	}
	string general_keyupdate_real_code(CCodeField key)
	{
		string ret;

		const char *fmt = R"(
int MainLogic::update_%s_by_%s(const %s &ret)
{
	stringstream ss;
	ss << "update %s ";
	bool have = false;
)";
		char sz[5000] = {0};
		sprintf(sz, fmt, m_table_name.c_str(), key.name.c_str(), m_table_name.c_str(), m_table_name.c_str());
		ret = sz;

		for (size_t i = 0; i < fields.size(); i++)
		{
			if (fields[i].name == key.name)
			{
				continue;
			}
			if (fields[i].is_str())
			{
				fmt = R"(
	if (ret.has_%s())
	{
		if (have)
		{
			ss << ",";
		}
		else
		{
			ss << " set ";
		}
		have = true;
		ss << "%s='" << db.escape_str(ret.%s())<<"'" ;
	}
)";
			}
			else
			{
				fmt = R"(
	if (ret.has_%s())
	{
		if (have)
		{
			ss << ",";
		}
		else
		{
			ss << " set ";
		}
		have = true;
		ss << "%s=" << ret.%s();
	}
)";
			}
			
			sprintf(sz, fmt, fields[i].name.c_str(), fields[i].name.c_str(), fields[i].name.c_str());
			ret += sz;
		}
		if (key.is_str())
		{
			fmt = R"(
		ss << " where %s = '"<<db.escape_str(ret.%s())<< "';";
		int sql_ret = db.do_write(ss.str());

		return 0;
	}
)";
		}
		else
		{
			fmt = R"(
		ss << " where %s = "<<ret.%s()<< ";";
		int sql_ret = db.do_write(ss.str());

		return 0;
	}
)";
		}
		
		sprintf(sz, fmt, key.name.c_str(), key.name.c_str());
		ret += sz;


		return ret;
	}
	string general_update_real_code()
	{
		string ret;
		for (size_t i = 0; i < fields.size(); i++)
		{
			if (fields[i].mysql_key())
			{
				ret += general_keyupdate_real_code(fields[i]);
			}
		}
		return ret;
	}
	//===========================================================
	

	string general_insert_code()
	{
		//"+ m_table_name + "
		string ret = "int insert_" + m_table_name + "(const " + m_table_name + " &val) ;";
		ret += "\r\n";
		//=============================================================
		return ret;
	}
	string general_get_code()
	{
		string ret;
		for (size_t i = 0; i < fields.size(); i++)
		{
			if (fields[i].mysql_key())
			{
				//"int get_role_info_by_roleid(unsigned int roleid, vector<role_info> &ret) { return 0; };";
				ret += "int get_" + m_table_name + "_by_" + fields[i].name + "(" + fields[i].get_ctype() + " " + fields[i].name + ", vector<" + m_table_name + "> &ret) ;"+"\r\n";
			}
		}
		return ret;
	}
	string general_update_code()
	{
		string ret;
		for (size_t i = 0; i < fields.size(); i++)
		{
			if (fields[i].mysql_key())
			{
				//"int get_role_info_by_roleid(unsigned int roleid, vector<role_info> &ret) { return 0; };";
				ret += "int update_" + m_table_name + "_by_" + fields[i].name + "( const " + m_table_name + " &ret) ;" + "\r\n";
			}
		}
		return ret;
	}
	bool get_db_name(string s)
	{
		stringstream ss;
		ss << s;
		string _message, table_name;
		ss >> _message >> table_name;
		if (_message == "message")
		{
			m_table_name = table_name;
			return true;
		}
		return false;
	}


public:
	//table:" + m_table_name + "
	//field_name:" + fields[i].name + "
	//field_type:" + fields[i].type + "
	string m_table_name;//
	vector<CCodeField> fields;
};