
#pragma once

#include <mysql.h>
#include <mysqld_error.h>
#include <errmsg.h>
#include <map>
#include <string>
#include <iostream>
#include "stdlib.h"
#include "CLog.hpp"
#include "CFile.hpp"
#pragma comment(lib,"libmysql.lib")
using namespace std;

#ifdef _WIN32
#define ATOLL(x) _atoi64((x))
#else
#define ATOLL(x) atoll((x))
#endif

//#define USE_LOG
#ifdef USE_LOG
extern CLog gSqlLog;
static void init_mysql_log()
{
	gSqlLog.InitDevice("sql");
}

#define HINFO	gSqlLog.l()
#define HERROR	gSqlLog.l()<<"Error,Line"<<__LINE__<<","
#else

#define HINFO	cout
#define HERROR	cout
#endif


using namespace std;
class mysql_helper
{
public:
	mysql_helper(MYSQL_RES *p)
	{
		init_with(p);
	}
	void init_with(MYSQL_RES *p)
	{
		res = p;
		if (res)
		{
			int id = 0;
			while (1)
			{
				MYSQL_FIELD *filed = mysql_fetch_field(res);
				if (NULL == filed)
				{
					break;
				}
				if (filed->name)
				{
					col_id_map[filed->name] = id;
				}
				++id;
			}
		}
	}
	~mysql_helper()
	{
		free_mysql();
	}
	void free_mysql()
	{
		if(res)
		{
			mysql_free_result(res);
			//HINFO<<"free";
			res = NULL;
		}
		
	}
	my_ulonglong get_row_cnt()
	{
		return mysql_num_rows(res);
	}
	MYSQL_ROW get_row()
	{
		return mysql_fetch_row(res);
	}

	long long get_row_ll(const char *p_name,MYSQL_ROW p)
	{
		int idx = get_col_id_by_name(p_name);
		if (idx<0)
		{
			return 0;
		}
		if (!p[idx])
		{
			return 0;
		}
		return ATOLL((const char *)p[idx]);
	}
	// Î´²âÊÔ
	string get_row_str(const char *p_name,MYSQL_ROW p)
	{
		int idx = get_col_id_by_name(p_name);
		if (idx<0)
		{
			return "";
		}
		if (!p[idx])
		{
			return "";
		}
		return p[idx];
	}
private:
	int get_col_id_by_name(const char *p_name)
	{
		if (NULL == p_name)
		{
			return -1;
		}
		map<string,int>::iterator it = col_id_map.find(p_name);
		if (col_id_map.end() == it)
		{
			return -1;
		}
		return it->second;
	}
	MYSQL_RES *res;
	map<string,int>		col_id_map;
};

struct mysql_col
{
	mysql_col()
	{
		is_auto_inc = false;
		is_pri_key = false;
		is_unique = false;
		is_numl = false;
	}
	string name;
	bool is_auto_inc;
	bool is_pri_key;
	bool is_unique;
	bool is_numl;
};

class Table
{
public:
	map<int,mysql_col>		col_id_map;
	map<string,mysql_col>	col_str_map;
};
static ostream &operator<<(ostream &output, const mysql_col &thiz) 
{
	output << thiz.name;
	return output;
}
class MysqlDB
{
public:
	int init_connect(
		const char *pIP,
		const char *user,
		const char *pass,
		const char *db,
		int port = 3306,
		const char *sock=NULL)
	{
		MYSQL *pInit = mysql_init(NULL);
		mysql = mysql_real_connect(
			pInit,
			pIP,
			user,
			pass,
			db,
			port,
			sock,
			0);
		if (NULL != mysql )
		{
			return 0;
		}else
		{
			cout<<"err="<<mysql_error(pInit)<<endl;
			cout<<"err="<<mysql_errno(pInit)<<endl;
			return -1;
		}
	}
	~MysqlDB()
	{
		close_mysql();
	}
	void close_mysql()
	{
		if (mysql)
		{
			mysql_close(mysql);
		}
	}

	int init_tables()
	{
		tables.clear();
		MYSQL_RES *pTableSet = mysql_list_tables(mysql,NULL);

		MYSQL_ROW row = NULL;
		while (row = mysql_fetch_row(pTableSet))
		{
			string table_name=row[0];
			MYSQL_RES *pFieldSet = mysql_list_fields(mysql,table_name.c_str(),NULL);
			if (!pFieldSet)
			{
				return -1;
			}

			Table table;
			int id = 0;
			while (1)
			{
				MYSQL_FIELD *filed = mysql_fetch_field(pFieldSet);
				if (NULL == filed)
				{
					break;
				}
				if (filed->name)
				{
					mysql_col temp;
					temp.name = filed->name;
					temp.is_auto_inc = (0 != (filed->flags & AUTO_INCREMENT_FLAG));
					temp.is_pri_key = (0 != (filed->flags & PRI_KEY_FLAG));
					temp.is_unique = (0 != (filed->flags & UNIQUE_KEY_FLAG));
					temp.is_numl = (0 != (filed->flags & NUM_FLAG));
					table.col_id_map[id] = temp;
					table.col_str_map[temp.name] = temp;
				}
				++id;
			}
			tables.insert(make_pair(table_name,table));
		}
		return 0;
	}

	int select_db(cstr_t db)
	{
		int ret = mysql_select_db(mysql,db);
		if (0 != ret)
		{
			HINFO<<"slelct dberr  "<<db;
			return -1;
		}
		return 0;
	}	
	MYSQL_RES* do_sql(cstr_t sql,int *res = NULL,bool blog = true,int nLen = 0)
	{
		if (res)
			*res = false;
		if (0 == nLen)
		{
			nLen = strlen(sql);
		}

		//DWORD start = GetTickCount();
		int ret = mysql_real_query(mysql,sql,nLen);
		if (0 != ret)
		{
			if (blog)
			{
				HINFO<<"mysql err = "<<mysql_errno(mysql)<<",sql="<<sql;
				HINFO<<"mysql err = "<<mysql_error(mysql)<<",sql="<<sql;
			}
			
			
			
			return NULL;	
		}
		//HINFO<<"cost="<<(GetTickCount() - start)<<",affect="<<mysql_affected_rows(mysql)<<",sql="<<sql;

		if (res)
			*res = true;;
		return mysql_use_result(mysql);
	}
	void do_sql_file(string path)
	{
		linevec_t sqls;
		XReadFileLine(path.c_str(),sqls);
		for (int i = 0; i < sqls.size();++i)
		{
			if (sqls[i].size() == 0)
			{
				continue;
			}
			if (sqls[i][0] == '#')
			{
				HINFO<<"ignore sql="<<sqls[i];
				continue;
			}
			MYSQL_RES* res = do_sql(sqls[i].c_str());
			mysql_free_result(res);
		}	
	}
	int get_table_row_cnt(string name)
	{
		string s;
		s = "select count(*) from "+name+";";
		MYSQL_RES* p= do_sql(s.c_str());

		mysql_helper help(p);
		return atoi(help.get_row()[0]);

	}

	bool set_char_set(cstr_t p)
	{
		//
		return 0 == mysql_set_character_set(mysql,p);
	}
public:
	map<string,Table>	tables;
	MYSQL*				mysql;
};

static string str_del_char(string s,char c)
{
	string ret;
	for (int i = 0; i < s.size();++i)
	{
		if (s[i]!=c)
		{
			ret +=s[i];
		}
		
	}
	
	return ret;
};

static void  test_str_del_char()
{
	string a = "12345";
	a +=5;
	a+=6;
	a+="ccc";
	a = str_del_char(a,5);
	cout<<a<<endl;;
	
}