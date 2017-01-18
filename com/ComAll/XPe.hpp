#pragma once
#include "XSTL.hpp"

extern  Buf XReadFile(cstr_t pPath);
extern addr_t XLoadModule(cstr_t sName);
extern addr_t XGetFunc(addr_t pModule,cstr_t sName);
extern void XFreeModule(addr_t pModule);


class CPE
{
public:
	virtual char* GetP(DWORD rva) = 0;
	void Update()
	{

		m_pNtHead=(PIMAGE_NT_HEADERS)
			(buf.m_addr + ((IMAGE_DOS_HEADER*)(buf.m_addr))->e_lfanew) ;
		m_pSection=IMAGE_FIRST_SECTION(m_pNtHead) ;
	}
	int32 InitWithFile(const char *pPath)
	{

		buf = XReadFile(pPath);
		Update();
		return 0;
	}
	int InitWithMem(addr_t addr)
	{
		buf.m_addr = (int8*)addr;
		Update();
		return 0;
	}

	bool ToFile(cstr_t pPath)
	{
		XWriteFile(pPath, buf.m_addr, buf.m_nLen);
		return true;

	}
	WORD& SecCnt()
	{
		return m_pNtHead->FileHeader.NumberOfSections;
	}
	IMAGE_IMPORT_DESCRIPTOR* GetImportDes()
	{
		IMAGE_DATA_DIRECTORY *pImport = m_pNtHead->OptionalHeader.DataDirectory + IMAGE_DIRECTORY_ENTRY_IMPORT;
		return (IMAGE_IMPORT_DESCRIPTOR*)GetP(pImport->VirtualAddress);
	}
	IMAGE_EXPORT_DIRECTORY * GetExportDes()
	{
		IMAGE_DATA_DIRECTORY *pExport = m_pNtHead->OptionalHeader.DataDirectory + IMAGE_DIRECTORY_ENTRY_EXPORT;
		return (IMAGE_EXPORT_DIRECTORY*)GetP(pExport->VirtualAddress);
	}

	DWORD GetRva(DWORD f_off)
	{

		for (int i = 0; i < SecCnt(); ++i)
		{
			if (m_pSection[i].PointerToRawData <= f_off
				&& f_off < m_pSection[i].PointerToRawData + m_pSection[i].SizeOfRawData)
			{
				return f_off - m_pSection[i].PointerToRawData + m_pSection[i].VirtualAddress;
			}
		}
		return 0;
	}

	int32 Reloc(DWORD CodeBase)
	{

		IMAGE_DATA_DIRECTORY *pReloc = 
			m_pNtHead->OptionalHeader.DataDirectory + IMAGE_DIRECTORY_ENTRY_BASERELOC;
		IMAGE_BASE_RELOCATION *m_pReloc = (IMAGE_BASE_RELOCATION*) GetP(pReloc->VirtualAddress);
		if(CodeBase == m_pNtHead->OptionalHeader.ImageBase)
		{
			return 0;
		}
		DWORD nDiff = CodeBase - m_pNtHead->OptionalHeader.ImageBase;
		IMAGE_BASE_RELOCATION *pRelocTemp = m_pReloc;
		for (;0 != pRelocTemp->VirtualAddress;)
		{
			WORD *pW = (WORD*)(pRelocTemp+1);
			int nCount = (pRelocTemp->SizeOfBlock - sizeof(IMAGE_BASE_RELOCATION)) / sizeof(WORD);
			for (int i = 0;i < nCount;++i)
			{
				if (IMAGE_REL_BASED_HIGHLOW == pW[i]>>12)
				{
					DWORD RvaToFix = pRelocTemp->VirtualAddress + (pW[i]&0xfff) ;
					*(DWORD*)GetP(RvaToFix) += nDiff;
				}
			}
			pRelocTemp = (IMAGE_BASE_RELOCATION*)((char*)pRelocTemp+(pRelocTemp->SizeOfBlock));
		}
		return 0;
	}
	int32 DoImport(bool bFree = true)
	{

		IMAGE_DATA_DIRECTORY *pImportDir = m_pNtHead->OptionalHeader.DataDirectory + IMAGE_DIRECTORY_ENTRY_IMPORT;
		if (0 == pImportDir->VirtualAddress)
		{
			return 0;
		}

		IMAGE_IMPORT_DESCRIPTOR *pImport = (IMAGE_IMPORT_DESCRIPTOR*) GetP(pImportDir->VirtualAddress);
		addr_t fpFun = NULL;
		IMAGE_IMPORT_BY_NAME *pImportName = NULL;
		for (;pImport->Name;++pImport)
		{
			char *pDllName = GetP(pImport->Name);
			CHECK_RET(pDllName);
			addr_t hDll = XLoadModule(pDllName);
			//CHECK_RET(NULL != hDll);
			if (NULL == hDll)
			{
				//cout<<"load err  "<<pDllName<<endl;
				//return -10;
				continue;
			}
			IMAGE_THUNK_DATA *pOri = (IMAGE_THUNK_DATA*)GetP(pImport->OriginalFirstThunk);
			IMAGE_THUNK_DATA *pFirst = (IMAGE_THUNK_DATA*)GetP(pImport->FirstThunk);
			CHECK_RET(NULL != pOri);
			CHECK_RET(NULL != pFirst);
			for (;pOri->u1.Function != NULL;++pOri,++pFirst)
			{
				if (pOri->u1.Ordinal & IMAGE_ORDINAL_FLAG32) 
				{
					fpFun = XGetFunc(hDll, (LPCSTR)(pOri->u1.Function & 0x0000ffff));
				}
				else 
				{
					pImportName = (PIMAGE_IMPORT_BY_NAME)GetP(pOri->u1.AddressOfData);
					fpFun = XGetFunc(hDll, (LPCSTR)pImportName->Name);
				}
				pFirst->u1.Function = (DWORD)fpFun;
			}
			if (bFree)
			{
				XFreeModule(hDll);
			}
		}
		return 0;
	}

	PVOID PEGetProcAddress(const char* p)
	{
		IMAGE_DATA_DIRECTORY *pExport = m_pNtHead->OptionalHeader.DataDirectory + IMAGE_DIRECTORY_ENTRY_EXPORT;
		IMAGE_EXPORT_DIRECTORY *m_pExportTable = (IMAGE_EXPORT_DIRECTORY*) GetP(pExport->VirtualAddress) ;

		IMAGE_EXPORT_DIRECTORY *pEx = m_pExportTable;
		DWORD dwBase = m_pExportTable->Base;
		DWORD *pFileName = (DWORD*)GetP(m_pExportTable->AddressOfNames);
		DWORD *pFileAddr = (DWORD*)GetP(m_pExportTable->AddressOfFunctions);
		uint16 *pOrd = (uint16*)GetP(m_pExportTable->AddressOfNameOrdinals);

		for (DWORD i = 0;i < m_pExportTable->NumberOfFunctions; ++i)
		{
			DWORD addr = pFileAddr[i];
			DWORD index =  i;
			int name_index = -1;
			const char *pName = NULL;
			for (DWORD j = 0;j < m_pExportTable->NumberOfNames;++j)
			{
				if (pOrd[j] == index)
				{
					name_index = j;
					break;
				}

			}
			if (-1 != name_index)
			{
				pName = GetP(pFileName[name_index]);
				if (0 == strcmp(p,pName))
				{
					return addr+buf.m_addr;
				}
			}

		}

		return 0;
	}
public:
	Buf buf;
	IMAGE_NT_HEADERS* m_pNtHead;
	IMAGE_SECTION_HEADER* m_pSection;
};


class CPEMem : public CPE
{
public:
	virtual char* GetP(DWORD rva)
	{
		return buf.m_addr + rva;
	}
};

class CPEFile : public CPE
{
public:
	virtual char* GetP(DWORD rva)
	{
		IMAGE_SECTION_HEADER* ptempSection = m_pSection;
		for (int i = 0; m_pNtHead->FileHeader.NumberOfSections; ++i)
		{
			if (ptempSection[i].VirtualAddress <= rva
				&& rva < ptempSection[i].VirtualAddress + ptempSection[i].Misc.VirtualSize)
			{
				return buf.m_addr + rva - ptempSection[i].VirtualAddress + ptempSection[i].PointerToRawData;
			}
		}
		return NULL;
	}
};

static int XPEGetAlignSize(int size,int Align)
{
	int i = size % Align;
	if (0 == i)
	{
		return size;
	}else
	{
		return (size / Align + 1) * Align;
	}
}