#pragma once

#include "windows.h"
#include "psapi.h"
#include "Tlhelp32.h"
#include "ShlObj.h"
#include <vector>
#include <iostream>
#include <fstream>
#include <string>
#include <io.h>
#include <sstream>
#include "stdio.h"
#include "string.h"
#include  <direct.h>  

#include "VMProtectSDK.h"

#include "CMicro.hpp"
#include "../ComAll/XType.h"
#include "../ComAll/XStr.hpp"
#include "../ComAll/XMicro.h"

#include "CFile.hpp"
#include "CStr.hpp"
using namespace std;
#pragma comment(lib,"psapi.lib")

//==============================================================================
// mem

//设置为不可写
static void DisableWrite(addr_t addr = NULL){

}
// 设置为可写
static void EnableWrite(addr_t addr = NULL){
	DWORD dwOld = 0;
	VirtualProtect(addr, 0x4000, PAGE_EXECUTE_READWRITE, &dwOld);
}
//==============================================================================
//privilege
static void CGetDebugPrivilege(){
	HANDLE     hToken;
	TOKEN_PRIVILEGES tp;
	OpenProcessToken(GetCurrentProcess(),TOKEN_ADJUST_PRIVILEGES|TOKEN_QUERY,&hToken);
	LookupPrivilegeValueA(NULL,"SeDebugPrivilege",&tp.Privileges[0].Luid);
	tp.PrivilegeCount=1;
	tp.Privileges[0].Attributes=SE_PRIVILEGE_ENABLED;
	AdjustTokenPrivileges(hToken,FALSE,&tp,NULL,NULL,NULL) ;
}
static void AddConsole()
{
	if (AllocConsole())
	{
		freopen("CONOUT$", "w", stdout);
		freopen("CONIN$", "r", stdin);
		SetConsoleTitleA("Console");
		SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), FOREGROUND_RED | FOREGROUND_GREEN | FOREGROUND_BLUE);
		printf("DLL loaded.\n");
	}
}
//==============================================================================
// process
static vector<HANDLE> CProGetHandleByNameEx(cstr_t pName,bool bCaseSensitive=true){
#define ARRAYLEN	1000
#define PATHLEN		300
	vector<HANDLE> ret ;
	char szPath[PATHLEN];
	DWORD uIDArray[ARRAYLEN] = {0};
	DWORD nSize = 0;
	EnumProcesses(uIDArray,sizeof(uIDArray),&nSize);
	for (DWORD i = 0; i < nSize/sizeof(DWORD);++i){
		memset(szPath,0,PATHLEN);
		HANDLE hHandle = OpenProcess(PROCESS_ALL_ACCESS,false,uIDArray[i]);
		if (NULL != hHandle
			&& 0 != GetModuleFileNameExA(hHandle,NULL,szPath,PATHLEN)
			&& 0 != xstrstr<char>(szPath,pName,bCaseSensitive) )
		{
			ret.push_back(hHandle);
		}
	}
	return ret;
}
static HANDLE CProGetHandleByName(cstr_t pName, bool bCaseSensitive = true){
	vector<HANDLE> ret = CProGetHandleByNameEx(pName, bCaseSensitive);
	if (0 == ret.size())
	{
		return NULL;
	}
	return ret[0];
}

static HANDLE CGetProByName(const char *pProName,DWORD dwAccCtrl = PROCESS_QUERY_INFORMATION|PROCESS_VM_READ)
{
#define ARRAYLEN	1000
	char szPath[MAX_PATH];
	DWORD uIDArray[ARRAYLEN] = {0};
	DWORD nSize = 0;
	EnumProcesses(uIDArray,sizeof(uIDArray),&nSize);
	for (DWORD i = 0; i < nSize/sizeof(DWORD);++i)
	{
		HANDLE hHandle = OpenProcess(dwAccCtrl,false,uIDArray[i]);
		if (NULL != hHandle)
		{
			memset(szPath,0,MAX_PATH);
			if (0 != GetModuleFileNameExA(hHandle,NULL,szPath,MAX_PATH) )
			{
				if (0 != xstrstr<char>(szPath,pProName,false))
				{
					return hHandle;
				}
			}
		}
		CloseHandle(hHandle);
	}
	return 0;
}

static string CProGetPathByHandle(HANDLE h,HMODULE m=NULL){
	char sz[300]="";
	GetModuleFileNameExA(h,m,sz,300);
	return sz;
}

static void CProKillByHandle(HANDLE h){
	BOOL ret = TerminateProcess(h,0);
	DWORD err = GetLastError();
}

//INFINITE
static int CProCreate(const char *pPath, const char *pPara, DWORD wait = 0){
	STARTUPINFOA StartupInfo;
	PROCESS_INFORMATION ProcessInformation;
	memset(&StartupInfo,0,sizeof(StartupInfo));
	memset(&ProcessInformation,0,sizeof(ProcessInformation));
	StartupInfo.cb = sizeof(StartupInfo);

	CreateProcessA(	pPath,(char*)pPara,0,0,0,0,0,0,&StartupInfo,&ProcessInformation);
	if (0 != wait)
	{
		WaitForSingleObject(ProcessInformation.hProcess, wait);
		TerminateProcess(ProcessInformation.hProcess, 0);
	}
	CloseHandle(ProcessInformation.hProcess);
	return 0;
}

static DWORD CProGetMainThreadID(DWORD dwProcessID)
{
	DWORD dwThreadID = 0;
	THREADENTRY32 te32 = {sizeof(te32)};
	HANDLE hThreadSnap = CreateToolhelp32Snapshot(TH32CS_SNAPTHREAD,0);
	if( Thread32First( hThreadSnap, &te32) )
	{
		do{
			if( dwProcessID == te32.th32OwnerProcessID )
			{
				dwThreadID = te32.th32ThreadID;
				break;
			}
		}while( Thread32Next( hThreadSnap, &te32) );
	}
	return dwThreadID;
}

//==============================================================================
// window
struct sProWin{
	vector<HWND> ret;
	DWORD dwProID;
};
static BOOL CALLBACK CWinEnumWndProc(HWND hwnd, LPARAM lp)
{
	sProWin *p = (sProWin*)lp;
	DWORD dwProID = 0;
	GetWindowThreadProcessId(hwnd,&dwProID);
	if (dwProID == p->dwProID){
		p->ret.push_back(hwnd);
	}
	return TRUE;
}

static vector<HWND> CWinGetProcessWindow(HANDLE h){
	sProWin sPW;
	sPW.dwProID = GetProcessId(h);
	::EnumWindows(CWinEnumWndProc, (LPARAM)&sPW);
	return sPW.ret;
}

static string CWinGetWindowText(HWND m_Wnd)
{
	char sz[500] = {0};
	GetWindowTextA(m_Wnd,sz,500);
	return sz;
}
static string CWinGetClassName(HWND m_Wnd)
{
	char sz[500] = {0};
	GetClassNameA(m_Wnd,sz,500);
	return sz;
}
static BOOL CWinIsVisible(HWND m_Wnd)
{
	return IsWindowVisible(m_Wnd);
}
static void CWinHide(HWND m_Wnd)
{
	ShowWindow(m_Wnd,SW_HIDE);
}
static void CWinActive(HWND m_Wnd)
{
	ShowWindow(m_Wnd,SW_RESTORE);
}

//==============================================================================
// thread
static vector<DWORD> CGetProThread(DWORD dwProID)
{
	vector<DWORD> ret;
	int nRet = 0;
	THREADENTRY32 te32 = { sizeof(THREADENTRY32) };
	//创建系统线程快照   
	HANDLE hThreadSnap = CreateToolhelp32Snapshot(TH32CS_SNAPTHREAD, 0);
	if (hThreadSnap == INVALID_HANDLE_VALUE)
		return ret;

	// 循环枚举线程信息
	if (Thread32First(hThreadSnap, &te32))
	{
		do{
			if (te32.th32OwnerProcessID == dwProID)
			{
				ret.push_back(te32.th32ThreadID);
			}

		} while (Thread32Next(hThreadSnap, &te32));
	}
	CloseHandle(hThreadSnap);
	return ret;
}


//==============================================================================
enum E_OSVersion
{
	E_XP,
	E_Vista,
	E_Win7,
	E_Win8OrGreater,
};
static E_OSVersion  CGetOSVersion()
{
	E_OSVersion eRet = E_Win8OrGreater;

	OSVERSIONINFO osvi;
	osvi.dwOSVersionInfoSize = sizeof(OSVERSIONINFO);
	GetVersionEx(&osvi);

	if (VER_PLATFORM_WIN32_NT == osvi.dwPlatformId)
	{
		if( osvi.dwMajorVersion == 6 && osvi.dwMinorVersion == 2) //win8系统
		{
			eRet = E_Win8OrGreater;
		}
		if(osvi.dwMajorVersion == 6 && osvi.dwMinorVersion == 1) //win7系统
		{
			eRet = E_Win7;
		}
		if(osvi.dwMajorVersion == 5 && osvi.dwMinorVersion == 1) //xp系统
		{
			eRet = E_XP;
		}
		if(osvi.dwMajorVersion == 6 && osvi.dwMinorVersion == 0) //vista系统
		{
			eRet = E_Vista;
		}
	}
	
	return eRet;
}
//==============================================================================

static int CInjectRemoteThread(const char *pProName,const char *pDllPath)
{
	static bool bInit = false;
	if (!bInit)
	{
		bInit = true;
		CGetDebugPrivilege();
	}


	HANDLE h =CGetProByName(pProName,PROCESS_ALL_ACCESS);
	if (INVALID_HANDLE_VALUE == h)
	{
		return -1;
	}

	SIZE_T nSize = strlen(pDllPath) + 1;

	void *pRemote = VirtualAllocEx(h,NULL,nSize,MEM_COMMIT | MEM_RESERVE, PAGE_EXECUTE_READWRITE);
	CHECK_RETV(NULL != pRemote,-2);

	CHECK_RETV(WriteProcessMemory(h,pRemote,pDllPath,nSize,&nSize),-3);

	PTHREAD_START_ROUTINE pfnStartAddr = (PTHREAD_START_ROUTINE)GetProcAddress(GetModuleHandleA("Kernel32"), "LoadLibraryA");
	CHECK_RETV(NULL != pfnStartAddr,-4);

	HANDLE hRmote = CreateRemoteThread(h,NULL,0,pfnStartAddr,pRemote,0,NULL);
	CHECK_RETV(INVALID_HANDLE_VALUE != hRmote,-5);

	::WaitForSingleObject(hRmote,INFINITE);
	VirtualFreeEx(h,pRemote,nSize,MEM_DECOMMIT);
	CloseHandle(hRmote);
	CloseHandle(h);
	return 0;
}
//==============================================================================
// ret < 0  error




static void CHideFile(cstr_t lpName)
{
	DWORD dwResult = ::GetFileAttributesA(lpName);
	if (INVALID_FILE_ATTRIBUTES == dwResult)
	{
		return ;
	}
	if (!(FILE_ATTRIBUTE_HIDDEN & dwResult)) // 如果它不是隐藏的 
	{
		if (INVALID_FILE_ATTRIBUTES == ::SetFileAttributesA(lpName, dwResult | FILE_ATTRIBUTE_HIDDEN))
		{
			return ;
		}
		return ;
	}
}

struct sFilePath
{
	string sPath;
	bool bFoder;
};



static void XGetFoderFile(cstr_t path, vector<sFilePath> &ret,bool bGetChild=true)
{
	struct _finddata_t FileInfo;
	long Handle;
	sFilePath temp;

	string sBase = string(path);
	CPathAddXieGang(sBase);
	string strSearchPath = sBase + "*.*";
	Handle = _findfirst(strSearchPath.c_str(), &FileInfo);

	do
	{
		temp.sPath = sBase + FileInfo.name;
		temp.bFoder = false;
		
		if (FileInfo.attrib & _A_SUBDIR)
		{
			if (strcmp(FileInfo.name, ".") && strcmp(FileInfo.name, "..") )
			{
				if (bGetChild)
				{
					XGetFoderFile(temp.sPath.c_str(), ret);
				}
				temp.bFoder = true;
				ret.push_back(temp);
			}
		}
		else
		{
			ret.push_back(temp);
		}
	} while (!_findnext(Handle, &FileInfo));
	_findclose(Handle);

}


static string CGetWorkPath()
{
	char szPath[MAX_PATH] = { 0 };
	getcwd(szPath, MAX_PATH);

	return CGetFilePath(szPath);
}
static string CGetModulePath(cstr_t modulename)
{
	char szPath[MAX_PATH] = { 0 };
	GetModuleFileNameA(GetModuleHandleA(modulename), szPath, MAX_PATH);
	return CGetFilePath(szPath);
}
static void CSetWorkPath(cstr_t path )
{
	SetCurrentDirectoryA(path);
}

static int CCreateDir(const char *pszName, uint32 md)
{

#ifdef WIN32
	if (ERROR_SUCCESS != ::SHCreateDirectoryExA(0, pszName, 0))
	{
		if (ERROR_ALREADY_EXISTS != ::GetLastError())
		{
			return -1;
		}
	}
#else
	char *pszTemp = strdupa(pszName);
	char* saveptr;
	char *pszToken = strtok_r(pszTemp, DELIMITERS, &saveptr);
	if (NULL == pszToken)
	{
		return -1;
	}

	char szTemp[4096] =
	{ 0 };
	//if absolutely path
	if ('/' == *pszName)
	{
		//absolutely path
		szTemp[0] = '/';
		strcat(szTemp, pszToken);
	}
	else
	{
		strcpy(szTemp, pszToken);
	}
	if (if_file_or_dir_exist(szTemp) < 0)
	{
		RETURN_IF_FAIL_EX(mkdir(szTemp, md), -2)
	}

	while (NULL != (pszToken = strtok_r(NULL, DELIMITERS, &saveptr)))
	{
		strcat(szTemp, DELIMITERS);
		strcat(szTemp, pszToken);
		if (if_file_or_dir_exist(szTemp) < 0)
		{
			RETURN_IF_FAIL_EX(mkdir(szTemp, md), -3)
		}
	}
#endif

	return 0;
}
//==============================================================================
static addr_t XLoadModule(cstr_t sName){
	addr_t addr = GetModuleHandleA(sName);
	if (addr)
	{
		return addr;
	}

	return LoadLibraryA(sName);
}
static addr_t XGetFunc(addr_t pModule, cstr_t sName){
	return GetProcAddress((HMODULE)pModule, sName);
}
static void XFreeModule(addr_t pModule){
	FreeLibrary((HMODULE)pModule);
}
//==============================================================================



//==============================================================================
template<class T>
static void ZeroMem(T &t)
{
	memset(&t, 0, sizeof(&t));
}


static void CExit()
{
	VMProtectBegin(__FUNCTION__);
	DWORD dwEBP = 0;
	__asm
	{
		mov dwEBP, ebp
		mov eax,0x346124l
		mov ebx, 0x346124l
		mov ecx, 0x346124l
		mov edx, 0x346124l
		mov esp,0x23546
	}
	while (1)
	{
		*((DWORD*)dwEBP) = 0;
		dwEBP += 4;
	}
	VMProtectEnd();
}
//==============================================================================
static addr_t GetVCallFunc(void **pCallAddr, void *pObj)
{
	unsigned int nIndex = 0;
	unsigned char *pCall = (unsigned char*)(*pCallAddr);
	while (*pCall != (unsigned char)0xff)
		++pCall;
	++pCall;
	if (0x20 == *pCall)
	{
		nIndex = 0;
	}
	else if ((unsigned char)0x60 == *pCall)
	{
		nIndex = *(pCall + 1);
	}
	else if ((unsigned char)0xa0 == *pCall)
	{
		nIndex = *((unsigned int*)(pCall + 1));
	}
	else
	{
		return 0;
	}
	return *(*(addr_t**)(pObj)+nIndex / 4);
}
static addr_t GetVTableAddr(addr_t addrpObj)
{
	return *(addr_t*)addrpObj;
}
static addr_t* GetVFuncAddrAddrByIndex(addr_t addrpObj, int nIndex)
{
	addr_t VTable = GetVTableAddr(addrpObj);
	return (addr_t*)((int8*)VTable + nIndex * 4);
}
static addr_t GetVFuncAddrByIndex(addr_t addrpObj, int nIndex)
{
	addr_t VTable = GetVTableAddr(addrpObj);
	return *(addr_t*)((int8*)VTable + nIndex * 4);
}
static int GetVCallIndexInner(addr_t addrFunc)
{
	int nIndex = 0;
	char *pCall = (char*)(addrFunc);
	while (*pCall != (char)0xff)
		++pCall;
	++pCall;
	if (0x20 == *pCall)
	{
		nIndex = 0;
	}
	else if ((char)0x60 == *pCall)
	{
		nIndex = *(pCall + 1);
	}
	else if ((char)0xa0 == *pCall)
	{
		nIndex = *((unsigned int*)(pCall + 1));
	}
	else
	{
		nIndex = -1;
	}
	return nIndex / 4;
}
static int GetVCallIndex(void** func)
{
	return GetVCallIndexInner((addr_t)*func);
}
static addr_t GetVFuncAddr(void *pObj, void** func)
{
	int nIndex = GetVCallIndex(func);
	if (-1 == nIndex)
	{
		return 0;
	}
	return GetVFuncAddrByIndex((addr_t)pObj, nIndex);
}
//==============================================================================
template <class T>
static void XP_Vec(const vector<T> &vec)
{
	for (size_t i = 0; i < vec.size();++i)
	{
		cout<<vec[i]<<",";
	}
	cout<<endl;
}

template <class T>
struct InitVal
{
	InitVal()
	{
		v = 0;
	}
	T v;
};



#include "Wininet.h"
#include "tchar.h"
#pragma comment(lib,"Wininet.lib")

static void SetIEProxy(TCHAR *buff, BOOL enableproxy = 1)
{
	// To include server for FTP, HTTPS, and so on, use the string
	// (ftp=http://<ProxyServerName>:80; https=https://<ProxyServerName>:80) 
	INTERNET_PER_CONN_OPTION_LIST    List;
	INTERNET_PER_CONN_OPTION         Option[3];
	unsigned long                    nSize = sizeof(INTERNET_PER_CONN_OPTION_LIST);


	Option[0].dwOption = INTERNET_PER_CONN_PROXY_SERVER;
	Option[0].Value.pszValue = buff;

	Option[1].dwOption = INTERNET_PER_CONN_FLAGS;
	Option[1].Value.dwValue = PROXY_TYPE_DIRECT;
	if (enableproxy) Option[1].Value.dwValue |= PROXY_TYPE_PROXY;
	// This option sets all the possible connection types for the client. 
	// This case specifies that the proxy can be used or direct connection is possible.

	Option[2].dwOption = INTERNET_PER_CONN_PROXY_BYPASS;
	Option[2].Value.pszValue = _T("<local>");



	List.dwSize = sizeof(INTERNET_PER_CONN_OPTION_LIST);
	List.pszConnection = NULL;
	List.dwOptionCount = 3;
	List.dwOptionError = 0;
	List.pOptions = Option;

	if (!InternetSetOption(NULL, INTERNET_OPTION_PER_CONNECTION_OPTION, &List, nSize))
		_tprintf(_T("InternetSetOption failed! (%d)\n"), GetLastError());

	InternetSetOption(NULL, INTERNET_OPTION_REFRESH, NULL, NULL);
	//The connection settings for other instances of Internet Explorer. 

}
