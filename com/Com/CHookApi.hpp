#pragma once
#include "../ComAll/XHook.hpp"

namespace HookLoadLibraryW
{
static HookEnv			env;

HMODULE WINAPI MyLoad(
							 __in        const WCHAR* lpFileName,
							 __reserved  HANDLE hFile,
							 __in        DWORD dwFlags
							 )
{
	wcout<<lpFileName<<endl;
	return NULL;
}



static int32 Hook(){
	InitHookEnv(env);
	E_OSVersion os = CGetOSVersion();
	addr_t old=NULL;
	if (E_Win7 == os){
		old = GetProcAddress(GetModuleHandleA("KernelBase.dll"),"LoadLibraryExW") ;
	}else if (E_XP == os)
	{
		old = GetProcAddress(GetModuleHandleA("kernel32.dll"),"LoadLibraryExW") ;
	}
	
	Hook(env,old,MyLoad);
	return 0;
}

static int32 UnHook(){
	UnHook(env);
	return 0;
}

}
