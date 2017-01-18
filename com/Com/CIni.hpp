#pragma once
#include <map>
#include <vector>
#include "../ComAll/XType.h"
using namespace std;

class CIni
{
public:
	typedef map<string, string>	key_map_t;
	typedef map<string, key_map_t> secmap_t;
public:
	bool IsSecName(string &strTemp)
	{
		return strTemp.size() > 2 && strTemp[0] == '[' && strTemp[strTemp.size() - 1] == ']';
	}
	void Add(cstr_t pSec, cstr_t pKey, cstr_t pVal)
	{
		m[pSec][pKey] = pVal;
	}
	void Clear()
	{
		m.clear();
	}
	int32 FromFile(cstr_t pPath)
	{
		linevec_t lines;
		if (!XReadFileLine(pPath, lines))
		{
			printf("FromFile Error! %s\n", pPath);
			return -1;
		}
		string strSecName, strKey, strValue;
		for (int i = 0; i < lines.size(); ++i)
		{
			string &strTemp = lines[i];
			// 检查注释,去掉空格
			if (';' == strTemp[0])
			{
				continue;
			}
			if ("" == strTemp)
			{
				continue;
			}

			// 处理
			if (IsSecName(strTemp))
			{
				strSecName = strTemp.substr(1, strTemp.size() - 2);
				if (0 == m.count(strSecName))
				{
					key_map_t keys;
					m.insert(make_pair(strSecName, keys));
				}
			}
			else
			{
				if ("" == strSecName)
				{
					return -2;
				}
				int nPos = strTemp.find_first_of('=');
				if (string::npos == nPos)
				{
					return -3;
				}
				strKey = strTemp.substr(0, nPos);
				strValue = strTemp.substr(nPos + 1);
				m[strSecName].insert(make_pair(strKey, strValue));
			}
		}
		return 0;
	}

	int32 ToFile(cstr_t pPath)
	{
		DeleteFileA(pPath);
		for (secmap_t::iterator it = m.begin(); it != m.end();++it)
		{
			key_map_t & s = it->second;
			for (key_map_t::iterator it2 = s.begin(); it2 != s.end();++it2)
			{
				WritePrivateProfileStringA(it->first.c_str(), it2->first.c_str(), it2->second.c_str(), pPath);
			}
		}
		return 0;
	}
public:
	secmap_t m;
};