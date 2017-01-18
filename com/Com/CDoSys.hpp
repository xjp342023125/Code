#pragma once


static SC_HANDLE CGetSCMgr()
{
	static SC_HANDLE sRet= NULL;
	if (NULL == sRet)
	{
		CGetDebugPrivilege();
		sRet = OpenSCManager(NULL,NULL,SC_MANAGER_ALL_ACCESS);
	}
	return sRet;
}

static int CDoSysIOWithFileHandle(HANDLE hFile,DWORD dwCtrCode,
									LPVOID pIn,DWORD dwInSize,
									LPVOID pOut,DWORD &dwOutSize)
{
	DWORD dwBytesReturned = 0;
	if (!DeviceIoControl(hFile, dwCtrCode, pIn, dwInSize, pOut, dwOutSize, &dwBytesReturned, (LPOVERLAPPED)NULL))
	{
		PLAST_ERR_RET(DeviceIoControl,-2);
	}
	dwOutSize = dwBytesReturned ;
	return 0;
}
static HANDLE COpenSysFileHandle(const char *pSysName)
{
	char szSysName[100]="\\\\.\\";
	strcat_s(szSysName,pSysName);
	HANDLE hRet =::CreateFileA(
		szSysName,
		GENERIC_WRITE | GENERIC_READ,
		NULL,
		NULL,
		OPEN_EXISTING,
		FILE_ATTRIBUTE_NORMAL,
		NULL
		);
	if (INVALID_HANDLE_VALUE == hRet)
	{
		PLAST_ERR(CreateFileA);
	}
	return hRet;
}
//==================================================================================
static SC_HANDLE COpenSys(const char *pName)
{
	SC_HANDLE h = OpenServiceA(CGetSCMgr(), pName,SERVICE_ALL_ACCESS);
	if (NULL==h && ERROR_SERVICE_DOES_NOT_EXIST != GetLastError())
	{
		PLAST_ERR(OpenServiceA);
	}
	return h;
}

static int CStartSys(SC_HANDLE sys)
{
	if ( StartService(sys, 0, NULL) == 0 
		&& !ERROR_SERVICE_ALREADY_RUNNING == GetLastError())
	{
		PLAST_ERR_RET(StartService,-2);
	}
	return 0;
}
static int CStopSys(SC_HANDLE sys)
{
	SERVICE_STATUS ssStatus; 

	if (! ControlService( sys,SERVICE_CONTROL_STOP,	&ssStatus) )
	{
		PLAST_ERR_RET(ControlService,-2);
	}
	return 0;
}
static int CDelSys(SC_HANDLE sys)
{
	if (! DeleteService(sys) ) 
	{
		PLAST_ERR_RET(DeleteService,-1);
	} 
	return 0;
}
//==================================================================================
static int CDoSysIO(const char *pSysName,DWORD dwCtrCode,
			   LPVOID pIn,DWORD dwInSize,
			   LPVOID pOut,DWORD &dwOutSize)
{
	HANDLE hFile = COpenSysFileHandle(pSysName);
	int nRet = CDoSysIOWithFileHandle(hFile,dwCtrCode,pIn,dwInSize,pOut,dwOutSize);
	CloseHandle(hFile);
	return nRet;
}

//nStartMode SERVICE_AUTO_START:HKEY_LOCAL_MACHINE\System\CurrentControlSet\Control
static SC_HANDLE CCreateSys(const char *pPath,
				 const char *pName,
				 DWORD dwDesiredAccess = SERVICE_ALL_ACCESS,
				 DWORD dwServiceType = SERVICE_KERNEL_DRIVER,
				 int nStartMode = SERVICE_DEMAND_START)
{

	SC_HANDLE ret = CreateServiceA(CGetSCMgr(),pName,pName,dwDesiredAccess,
		dwServiceType,nStartMode,SERVICE_ERROR_NORMAL,
		pPath,NULL,NULL,NULL,NULL,NULL);
	if (NULL == ret && ERROR_SERVICE_EXISTS != GetLastError())
	{
		PLAST_ERR_RET(CreateServiceA,NULL);
	}
	return ret;
}

static int CCreateAndStart(const char *pPath,const char *pName)
{
	CGetDebugPrivilege();
	SC_HANDLE sys = COpenSys(pName);
	if (NULL == sys)
	{
		sys = CCreateSys(pPath,pName);

	}
	CHECK_RETV(sys,-1);

	
	int ret = CStartSys(sys);
	CloseServiceHandle(sys);
	return ret;
}

static int CStopAndDelSys(const char *pName)
{
	CGetDebugPrivilege();
	SC_HANDLE sys = COpenSys(pName);
	CStopSys(sys);
	int ret = CDelSys(sys);
	CloseServiceHandle(sys);
	return ret;
}
