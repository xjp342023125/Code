#pragma once
#include "../ComAll/XPe.hpp"
#include "CFunc.hpp"
#include "VMProtectSDK.h"
static int xjpmemcmp(
	const void * buf1,
	const void * buf2,
	size_t count
	)
{
	const char* p1 = (const char*)buf1;
	const char* p2 = (const char*)buf2;
	for (size_t i = 0; i<count; ++i)
	{
		if (p1[i] != p2[i])
		{
			return i;
		}
	}
	return 0;
}


template<int n>
static BOOL CheckModule(const char *pName)
{
	VMProtectBegin(__FUNCTION__);

	static CPEFile peFile;
	static CPEMem peMem;
	static bool bInit = false;
	if (!bInit)
	{
		bInit = true;
		HMODULE h = GetModuleHandleA(pName);
		peMem.InitWithMem((addr_t)h);
		MODULEINFO info;
		GetModuleInformation(GetCurrentProcess(),h,&info,sizeof(info));

		char szName[300]={0};
		GetModuleFileNameA(h,szName,300);
		peFile.InitWithFile(szName);
		peFile.DoImport();
		peFile.Reloc((DWORD)h);
		
	}
	int nSecCnt = peMem.m_pNtHead->FileHeader.NumberOfSections;
	for (int i = 0;i < nSecCnt;++i)
	{
		if (peMem.m_pSection[i].Characteristics &IMAGE_SCN_CNT_CODE)
		{
			int nSize = peMem.m_pSection[i].SizeOfRawData;
			if (nSize > peMem.m_pSection[i].Misc.VirtualSize)
			{
				nSize = peMem.m_pSection[i].Misc.VirtualSize;
			}
			const char *pMem = 
				(const char*)peMem.m_pSection[i].VirtualAddress+ \
				peMem.m_pNtHead->OptionalHeader.ImageBase;
			const char *pFile = 
				peFile.GetP(peFile.m_pSection[i].VirtualAddress);
			int nRet = xjpmemcmp(pMem,pFile,nSize);

			return (0==nRet);

		}
		
	}
	VMProtectEnd();

	return TRUE;
}