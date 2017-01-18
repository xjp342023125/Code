#pragma once
#include <assert.h>
#include <map>
#include <vector>
#include <string>
#include "windows.h"
using namespace std;

typedef unsigned long   u32;

class CppCSV
{
public:
	map<u32, map<u32, string>> m_stringMap;
	string m_CSVName;
	map<u32, string> stringMap;
public:
	CppCSV(){}
	CppCSV(const char *path)
	{
		LoadCSV(path);
	}
	~CppCSV(){}

	bool LoadCSV(const char *path)
	{
		FILE *pFile = fopen(path, "r");

		if (pFile)
		{
			fseek(pFile, 0, SEEK_END);
			u32 uSize = ftell(pFile);
			rewind(pFile);

			char *fileBuffer = new char[uSize];
			fread(fileBuffer, 1, uSize, pFile);

			
			u32 uiIndex = 1;
			char *pBegin = fileBuffer;
			char *pEnd = strchr(pBegin, '\n');


			pBegin = pEnd + 1;
			pEnd = strchr(pBegin, '\n');

			while (pEnd)
			{
				string strTemp;
				strTemp.insert(0, pBegin, pEnd-pBegin);
				
				stringMap[uiIndex++] = strTemp;
				pBegin = pEnd + 1;
				pEnd = strchr(pBegin, '\n');
			}
			delete []fileBuffer;
			fileBuffer = NULL;
			pBegin = NULL;
			pEnd = NULL;

			map<u32, string>::iterator iter = stringMap.begin();
			for (; iter != stringMap.end(); ++iter)
			{
				vector<string> stringVec;
				map<u32, string> stringMapTemp;
				GetParamFromString(iter->second, stringVec) ;

				vector<string>::size_type idx = 0;
				for (; idx != stringVec.size(); ++idx)
				{
					stringMapTemp[idx + 1] = stringVec[idx];
				}

				m_stringMap[iter->first] = stringMapTemp;
			}

			fclose(pFile);
			m_CSVName = path;
			return true;
		} 
		else
		{
			return false;
		}
	}

	bool CppCSV::SaveCSV(const char *path /* = NULL */)
	{
		if (path != NULL)
		{
			m_CSVName = path;
		}

		FILE *pFile = fopen(m_CSVName.c_str(), "w");
		if (pFile)
		{
			map<u32, map<u32, string>>::iterator iter = m_stringMap.begin();
			for (; iter != m_stringMap.end(); ++iter)
			{
				map<u32, string> &rStringMap = iter->second;
				map<u32, string>::iterator it = rStringMap.begin();
				for (; it != rStringMap.end(); ++it)
				{
					string strTemp = it->second;
					strTemp += ',';
					fwrite(strTemp.c_str(), 1, 1, pFile);
				}

				char delim = '\n';
				fwrite(&delim, 1, 1, pFile);
			}

			fclose(pFile);
			return true;
		} 
		else
		{
			return false;
		}
	}


	bool GetIntValue(u32 uiRow, u32 uiCol, int &riValue)
	{
		string *pStr = GetStringValue(uiRow, uiCol);
		if (pStr)
		{
			riValue = atoi(pStr->c_str());
			return true;
		} 
		else
		{
			return false;
		}
	}


	bool CppCSV::GetFloatValue(u32 uiRow, u32 uiCol, float &rfValue)
	{
		string *pStr = GetStringValue(uiRow, uiCol);
		if (pStr)
		{
			rfValue = atof(pStr->c_str());
			return true;
		} 
		else
		{
			return false;
		}
	}
	string* CppCSV::GetStringValue(u32 uiRow, u32 uiCol)
	{
		map<u32, map<u32, string>>::iterator iter = m_stringMap.find(uiRow);
		if (iter != m_stringMap.end())
		{
			map<u32, string> &rStrMap = iter->second;
			map<u32, string>::iterator it = rStrMap.find(uiCol);
			if (it != rStrMap.end())
			{
				return &(it->second);
			} 
			else
			{
				return NULL;
			}
		} 
		else
		{
			return NULL;
		}
	}



	//用于分割字符串，将CSV表格中的一行按照规则解析成一组字符串，存储在一个vector中
	//根据CSV表格中所存储的数据的不同，重载各函数
	int GetParamFromString(string str, vector<string> &stringVec, char delim=',')
	{
		char *token = strtok(const_cast<char *>(str.c_str()), &delim);
		while (token)
		{
			string strTemp = token;
			stringVec.push_back(strTemp);
			token = strtok(NULL, &delim);
		}


		return stringVec.size();
	}


	map<u32, map<u32, string>>& GetCSVMap()
	{
		return m_stringMap;
	}

};
