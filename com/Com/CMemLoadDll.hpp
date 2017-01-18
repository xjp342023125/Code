#pragma once
#include "../ComAll/XPe.hpp"
#include "CFunc.hpp"
#include "VMProtectSDK.h"
typedef BOOL (WINAPI *DllEntryProc)(HINSTANCE hinstDLL, DWORD fdwReason, LPVOID lpReserved);

static void MoveSec(CPE &peMem,CPE &peFile)
{
	VMProtectBegin(__FUNCTION__);

	int align = peMem.m_pNtHead->OptionalHeader.SectionAlignment;
	for (int i=0;i<peMem.SecCnt();++i)
	{
		char *pDst=peMem.buf.m_addr+peMem.m_pSection[i].VirtualAddress;
		int nSize = peFile.m_pSection[i].SizeOfRawData;
		memcpy(pDst,peFile.buf.m_addr + peFile.m_pSection[i].PointerToRawData,nSize);
	
	}
	VMProtectEnd();
}

static addr_t CMemLoadDll(const char *p,int nLen)
{
	VMProtectBegin(__FUNCTION__);
	CPEMem peMem;
	CPEFile peFile;
	__try
	{
		peFile.buf.ReCapacity(nLen);
		memcpy(peFile.buf.m_addr, p, nLen);

		peFile.Update();
		peMem.buf.ReCapacity(peFile.m_pNtHead->OptionalHeader.SizeOfImage);

		memcpy(peMem.buf.m_addr, peFile.buf.m_addr, peFile.m_pNtHead->OptionalHeader.SizeOfHeaders);
		peMem.Update();

		MoveSec(peMem, peFile);
		int32 ret = peMem.DoImport(false);
		ret = peMem.Reloc((DWORD)peMem.buf.m_addr);

		DWORD old;
		VirtualProtect(peMem.buf.m_addr, peMem.buf.m_nCapacity, PAGE_EXECUTE_READWRITE, &old);
		DllEntryProc DllEntry = (DllEntryProc)(peMem.buf.m_addr + peMem.m_pNtHead->OptionalHeader.AddressOfEntryPoint);
		(*DllEntry)((HINSTANCE)peMem.buf.m_addr, DLL_PROCESS_ATTACH, 0);
	}
	__except (1){
		return 0;
	}
	VMProtectEnd();
	return peMem.buf.m_addr;
}
static addr_t CMemLoadDllFromFile(cstr_t p)
{
	Buf bf = XReadFile(p);
	return CMemLoadDll(bf.m_addr,bf.m_nLen);
}
static addr_t CGetProAddrss(    __in addr_t hModule,   __in LPCSTR lpProcName)
{
	CPEMem peMem;
	peMem.InitWithMem((addr_t)hModule);
	peMem.buf.m_addr = (char*)hModule;
	
	return peMem.PEGetProcAddress(lpProcName);
}