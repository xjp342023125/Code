#pragma once

#include "../ComAll/XPe.hpp"


static int CAddSec(cstr_t pPath, cstr_t pSecname, int nSecSize, char *pNewSec = NULL)
{
	CPEFile pe;
	pe.InitWithFile(pPath);
	
	IMAGE_OPTIONAL_HEADER32 &opHeader = pe.m_pNtHead->OptionalHeader;
	IMAGE_SECTION_HEADER &secLast = pe.m_pSection[pe.SecCnt() - 1];
	IMAGE_SECTION_HEADER	sec;
	memset(&sec, 0, sizeof(sec));
	strcpy((str_t)sec.Name, pSecname);
	sec.Misc.VirtualSize = nSecSize;
	sec.SizeOfRawData = XPEGetAlignSize(nSecSize, opHeader.FileAlignment);
	sec.Characteristics = IMAGE_SCN_MEM_READ | IMAGE_SCN_MEM_WRITE;
	sec.VirtualAddress = XPEGetAlignSize(secLast.VirtualAddress + secLast.Misc.VirtualSize, opHeader.SectionAlignment);
	sec.PointerToRawData = XPEGetAlignSize(pe.buf.m_nCapacity, opHeader.FileAlignment) + XPEGetAlignSize(nSecSize, opHeader.FileAlignment);



	++pe.SecCnt();
	pe.m_pNtHead->OptionalHeader.SizeOfCode += nSecSize;
	pe.m_pNtHead->OptionalHeader.SizeOfImage += nSecSize;
	pe.m_pNtHead->OptionalHeader.SizeOfInitializedData += nSecSize;
	pe.m_pSection[pe.SecCnt() - 1] = sec;

	pe.buf.ReCapacity(sec.PointerToRawData + sec.SizeOfRawData);
	if (pNewSec)
	{
		memcpy(&pe.buf.m_addr[sec.PointerToRawData], &nSecSize, 4);
		memcpy(&pe.buf.m_addr[sec.PointerToRawData + 4], pNewSec, nSecSize);
	}
	const char *p = strchr(pPath, '.');
	string sName = string(pPath,p);
	string sEnd = string(p);
	string sOut = sName + "_addsec" + sEnd;
	pe.buf.m_nLen = pe.buf.m_nCapacity;
	DeleteFileA(sOut.c_str());
	pe.ToFile(sOut.c_str());
	return 0;
}

template <int n>
struct ONE_IMPORT_DESCRIPTOR
{
	ONE_IMPORT_DESCRIPTOR()
	{
		memset(this, 0, sizeof(ONE_IMPORT_DESCRIPTOR));
	}
	void Init()
	{
		memset(this, 0, sizeof(ONE_IMPORT_DESCRIPTOR));
	}
	struct OneImportName
	{
		OneImportName()
		{
			memset(&iName, 0, sizeof(iName));
			memset(szFuncName, 0, 32);
		}
		IMAGE_IMPORT_BY_NAME iName;
		char szFuncName[32];
	};
	char szDllName[32];
	IMAGE_THUNK_DATA Ori[n + 1];
	IMAGE_THUNK_DATA First[n + 1];
	OneImportName m_OneName[n];
};

static int CAddImportDll(cstr_t pPath, cstr_t pDllName, cstr_t pFuncName)
{
	const char *p = strchr(pPath, '.');
	string sName = string(pPath, p);
	string sEnd = string(p);

	int nNewSize = 1000;
	CAddSec(pPath, "NewImpo", nNewSize);
	string sTemp = sName + "_addsec" + sEnd;
	CPEFile pe;
	pe.InitWithFile(sTemp.c_str());
	DeleteFileA(sTemp.c_str());
	

	char *pLastSec = pe.GetP(pe.m_pSection[pe.SecCnt() - 1].VirtualAddress);

	IMAGE_IMPORT_DESCRIPTOR *pImportSrc = pe.GetImportDes();
	IMAGE_IMPORT_DESCRIPTOR *pImportDes = (IMAGE_IMPORT_DESCRIPTOR*)pLastSec;

	// 先复制原来的
	while (pImportSrc->Name)
	{
		memcpy(pImportDes, pImportSrc, sizeof(IMAGE_IMPORT_DESCRIPTOR));
		++pImportSrc;
		++pImportDes;
	}
	// 此时指向最后一个后面的空的,下一个是空的要赋值，再下一个是空
	ONE_IMPORT_DESCRIPTOR<1> *pOneDesCri = (ONE_IMPORT_DESCRIPTOR<1>*)(pImportDes + 2);
	pOneDesCri->Init();

	strncpy(pOneDesCri->szDllName, pDllName, 31);
	pImportDes->Name = pe.GetRva(pOneDesCri->szDllName - pe.buf.m_addr);

	pOneDesCri->m_OneName[0].iName.Hint = 1;
	strncpy((char*)pOneDesCri->m_OneName->iName.Name, pFuncName, 31);
	pOneDesCri->Ori[0].u1.AddressOfData = pe.GetRva((char*)&pOneDesCri->m_OneName[0].iName - pe.buf.m_addr);
	pOneDesCri->First[0].u1.AddressOfData = pe.GetRva((char*)&pOneDesCri->m_OneName[0].iName - pe.buf.m_addr);
	pImportDes->FirstThunk = pe.GetRva((char*)&pOneDesCri->First[0] - pe.buf.m_addr);
	pImportDes->OriginalFirstThunk = pe.GetRva((char*)&pOneDesCri->Ori[0] - pe.buf.m_addr);
	memset(pImportDes + 1, 0, sizeof(IMAGE_IMPORT_DESCRIPTOR));


	// modofy pe mem
	IMAGE_DATA_DIRECTORY *pImportDir = pe.m_pNtHead->OptionalHeader.DataDirectory + IMAGE_DIRECTORY_ENTRY_IMPORT;
	pImportDir->Size = nNewSize;
	pImportDir->VirtualAddress = pe.GetRva(pLastSec - pe.buf.m_addr);

	sTemp = sName + "_addimport" + sEnd;
	DeleteFileA(sTemp.c_str());
	pe.ToFile(sTemp.c_str());

	return 0;
}