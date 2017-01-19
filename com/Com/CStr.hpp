#pragma once

#include <string>
using namespace std;
#include "CMicro.hpp"
#include "../ComAll/XType.h"
#include "../ComAll/XStr.hpp"
#include "../ComAll/XMicro.h"
#include "windows.h"
//utf-8תunicode
static wchar_t *Utf8_Unicode(const char* szU8) {
	int wcsLen = ::MultiByteToWideChar(CP_UTF8, NULL, szU8, strlen(szU8), NULL, 0);
	wchar_t* wszString = (wchar_t*)XGetMem((wcsLen + 1)*sizeof(wchar_t)) ;
	::MultiByteToWideChar(CP_UTF8, NULL, szU8, strlen(szU8), wszString, wcsLen);
	wszString[wcsLen] = '\0';
	return wszString;
}
//unicodeתutf-8
static char* Unicode_Utf8(const wchar_t *str){
	int u8Len =WideCharToMultiByte(CP_UTF8, NULL,str,wcslen(str), NULL, 0, NULL, NULL);
	char* szU8 = (char*)XGetMem((u8Len + 1)*sizeof(char)) ;
	WideCharToMultiByte(CP_UTF8, NULL, str,wcslen(str), szU8, u8Len, NULL, NULL);
	szU8[u8Len] = '\0';
	return szU8;  
}


static char *Unicode_Ansi(const wchar_t *str){
	int u8Len =WideCharToMultiByte(CP_ACP, NULL,str,wcslen(str), NULL, 0, NULL, NULL);
	char* szU8 = (char*)XGetMem((u8Len + 1)*sizeof(char)) ;
	WideCharToMultiByte(CP_ACP, NULL, str,wcslen(str), szU8, u8Len, NULL, NULL);
	szU8[u8Len] = '\0';
	return szU8;  
}

static wchar_t *Ansi_Unicode(const char *str){
	int wcsLen = ::MultiByteToWideChar(CP_ACP, NULL, str, strlen(str), NULL, 0);
	wchar_t* wszString = (wchar_t*)XGetMem((wcsLen + 1)*sizeof(wchar_t));
	::MultiByteToWideChar(CP_ACP, NULL, str, strlen(str), wszString, wcsLen);
	wszString[wcsLen] = '\0';
	return wszString;
}

static char* Utf8_Ansi(char *str){
	wchar_t *p2 = Utf8_Unicode(str);
	char *pRet = Unicode_Ansi(p2);
	XFreeMem(p2);
	return pRet;
}

static char* Ansi_Utf8(const char *str)
{
	wchar_t *p=Ansi_Unicode(str);
	char *pp = Unicode_Utf8(p);
	XFreeMem(p);
	return pp;
}



static string GetRandomStr(int nLen)
{
	string ret;
	const char *p="qazsxedcrfvtgbyhnujmikolp0987612345QAZSXEDCRFVTGBYHNUJMIKOLP";
	
	int nCnt = strlen(p);
	for (int i = 0;i < nLen;++i)
	{
		ret+=p[rand()%nCnt];
	}
	return ret;
}

#include <regex>
static bool have_eamil(const std::string& email)
{
	int at = 0,dian = 0;

	for (auto i:email)
	{
		if (0 != at && '.' == i)
		{
			++dian;
		}
		if ('@' == i)
		{
			++at;
		}
	}
	return at>0 && dian>0;
}
static bool is_url(const std::string& url)
{
	if (NULL != strstr(url.c_str(),"http"))
	{
		return true;
	}

	int dian = 0, xie = 0;
	for (auto i : url)
	{
		if (0 == xie && '.' == i)
		{
			++dian;
		}
		if ('/' == i)
		{
			++xie;
		}
	}
	return xie>0 && dian>0;
}

static string URLFormat(string url)
{
	const std::regex pattern("&amp;");

	std::string replace = "&";
	std::string newtext = std::regex_replace(url, pattern, replace);
	const char *p = strrchr(newtext.c_str(), '#');
	if (!p)
	{
		return newtext;
	}
	else
	{
		return string(newtext.c_str(), p);
	}
	
}

// http://www.a.com/aaaa  -> http://www.a.com
static string GetURLBase(string url)
{
	const char *p = strstr(url.c_str(), "//");
	if (p)
	{
		const char *pRet = strstr(p + 2, "/");
		if (pRet)
		{
			return string(url.c_str(), pRet);
		}
		
	}
	return "";
}



static vector<string> xstrtokex(string in, int8 cSplit = ' ')
{
	vector<string> ret;
	cstr_t pBegin = in.c_str();
	cstr_t pEnd = in.c_str() + in.size();
	
	while (pBegin && pBegin < pEnd)
	{
		pBegin = xskipchar(pBegin, pEnd, cSplit);
		if (pBegin<pEnd){
			cstr_t pNewBegin = strchr(pBegin, cSplit);
			if (pNewBegin)
			{
				ret.push_back(string(pBegin, pNewBegin));
			}
			else
			{
				ret.push_back(string(pBegin));
			}
			pBegin = pNewBegin;
			
		}
	}
	return ret;
}

static string XGetNumStr(int32 v)
{
	char sztemp[20] = { 0 };
	sprintf(sztemp, "%d", v);
	return sztemp;
}

static string CGetFileName(string fullpath)
{
	cstr_t  p = strrchr(fullpath.c_str(), '\\');
	if (p)
	{
		return string(p + 1);
	}
	else
	{
		return fullpath;
	}
}
static string CGetFilePath(string fullpath)
{
	cstr_t  p = strrchr(fullpath.c_str(), '\\');
	if (p)
	{
		return string(fullpath.c_str(), p);
	}
	else
	{
		return "";
	}
}

static cstr_t CGetCharPos(cstr_t path, int cnt, char ctofind)
{
	for (int i = strlen(path); i >= 0; --i)
	{
		if (path[i] == ctofind)
		{
			--cnt;
			if (0 == cnt)
			{
				return &path[i];
			}
		}
	}
	return NULL;
}

static bool CPathHaveXieGang(cstr_t path)
{
	return path[strlen(path) - 1] == '\\';
}
static void CPathAddXieGang(string &s)
{
	if (!CPathHaveXieGang(s.c_str()))
		s += "\\";
}