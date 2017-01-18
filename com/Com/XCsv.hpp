#pragma once

#include "../ComAll/XType.h"
#include "CFile.hpp"
#include "CStr.hpp"
#include <vector>
#include <string>
using namespace std;

class XCsv
{
public:
	int load(cstr_t path,char c = ',')
	{

		bool_t bRet = false;
		ifstream in_file;
		string strTemp;

		in_file.open(path, ios::in);
		if (!in_file.is_open())
		{
			return -1;
		}
		while (getline(in_file, strTemp))
		{
			m.push_back(xstrtokex(strTemp, c));
		}
		if (m.size() > 2)
		{
			int nSize = m[0].size();
			for (size_t i = 0; i < m.size(); ++i)
			{
				if (m[i].size() != nSize)
				{
					cout << "size err,i=" << i << endl;

					in_file.close();
					return -2;
				}
			}
		}
			
		return 0;
	}
	string get_str(int32 row_idx, int32 col_idx)
	{
		if (row_idx < 0 || row_idx > m.size() - 1)
		{
			cout << "get_str,row_idx err" << endl;
			return "";
		}
		if (col_idx < 0 || col_idx > m[row_idx].size() - 1)
		{
			cout << "get_str,col_idx err" << endl;
			return "";
		}

		return m[row_idx][col_idx];
	}
	int32 get(int32 row_idx, int32 col_idx)
	{
		if (row_idx < 0 || row_idx > m.size() -1 )
		{
			cout << "get,row_idx err" << endl;
			return -1;
		}
		if (col_idx < 0 || col_idx > m[row_idx].size() - 1)
		{
			cout << "get,col_idx err" << endl;
			return -2;
		}

		return atoi(m[row_idx][col_idx].c_str());
	}
public:
	vector<vector<string>>	m;
};