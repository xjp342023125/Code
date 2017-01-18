#pragma once
#include "windows.h"
#include <wininet.h> 
#pragma comment(lib, "wininet.lib" )
#include <string>
#include <vector>
#include <iostream>
using namespace std;

#define TRACE_ERROR(v) TRACE_ERROR_KE(v,GetLastError())

#define TRACE_ERROR_KE(v,n)\
	char szError[500];\
	DWORD dwErr= (n);\
	sprintf(szError,"%s errcode=%d",#v,dwErr);\
	cout<<szError<<endl;
	//::MessageBoxA(0,szError,"error",0);



class CFTP
{
public:
	~CFTP()
	{
		Close();
	}
	string sIP,sAcc,sPass;
	int m_nPort;

	int InitFtp(const char *pIP,int nPort,const char *pAcc,const char *pPass)
	{
		sIP = pIP;
		sAcc = pAcc;
		sPass = pPass;
		m_nPort = nPort;
		return InnerInit();
	}

	int InnerInit()
	{
		// 打开网络
		hInternet = InternetOpen(NULL, LOCAL_INTERNET_ACCESS, NULL, 0, 0 ); 
		if ( hInternet == NULL ) 
		{ 
			TRACE_ERROR(InternetOpen);
			return -1; 
		}

		// 连接服务器 
		hHost = InternetConnectA(hInternet, sIP.c_str(), m_nPort, sAcc.c_str(), sPass.c_str(), 
			INTERNET_SERVICE_FTP, INTERNET_FLAG_PASSIVE, 0); 
		if ( hHost == NULL ) 
		{ 
			TRACE_ERROR(InternetConnectA);
			return -2; 
		}
		return 0;
	}

	
	int UpFile(const char *pLocal,const char *pDst)
	{
		if ( !FtpPutFileA(hHost, pLocal, pDst, FTP_TRANSFER_TYPE_BINARY, 0))
		{
			TRACE_ERROR(FtpPutFileA);
			return   -1;   
		}
		return 0;
	}
	int DelFile(const char *pPath)
	{
		if (!FtpDeleteFileA(hHost,pPath) && GetLastError() != 12003)
		{
			TRACE_ERROR(FtpDeleteFileA);
			return   -1;
		}
		
		
		return 0;
	}
	
	
	int DownFile(const char *pLocal,const char *pDst)
	{
		DeleteFileA(pLocal);
		BOOL nRet = FtpGetFileA(hHost,pDst,pLocal,TRUE,
			FILE_ATTRIBUTE_NORMAL,
			FTP_TRANSFER_TYPE_BINARY,
			0);
		if (TRUE != nRet)
		{
			TRACE_ERROR(FtpGetFileA);
			return -1;
		}
		return 0;
	}
	//FtpFindFirstFile is similar to the FindFirstFile function. Note, however, that only one FtpFindFirstFile can occur at a time within a given FTP session. The enumerations, therefore, are correlated with the FTP session handle. This is because the FTP protocol allows only a single directory enumeration per session.
	// 所以一次只能找一个 要不然就遍历
	WIN32_FIND_DATAA* GetFileInfo(const char *pPath)
	{
		static WIN32_FIND_DATAA fileinfo;
		if (NULL != FtpFindFirstFileA(hHost,pPath,&fileinfo,INTERNET_FLAG_NO_CACHE_WRITE,0))
		{
			return &fileinfo;
		}
		
		return NULL;
	}

	int SetDir(const char *p)
	{
		if   (!FtpSetCurrentDirectoryA(hHost,p))   
		{      
			return -1;
		}
		return 0;
	}

	bool IsWeWant(WIN32_FIND_DATAA f)
	{
		if (0 == strcmp(f.cFileName,"."))
		{
			return false;
		}
		if (0 == strcmp(f.cFileName,".."))
		{
			return false;
		}
		return true;
	}
	bool CreateDir(const char *p)
	{
		return FtpCreateDirectoryA(hHost,p);
	}
	int GetFiles(const char *pPath,vector<WIN32_FIND_DATAA> &files)
	{
		files.clear();
		if(0 != SetDir(pPath))
		{
			return -1;
		}
		
		WIN32_FIND_DATAA fileinfo;
		HANDLE hF = FtpFindFirstFileA(hHost,pPath,&fileinfo,INTERNET_FLAG_NO_CACHE_WRITE,0);
		if (NULL != hF);
		{
			if (IsWeWant(fileinfo))
			{
				files.push_back(fileinfo);
			}
			
		}

		while (InternetFindNextFileA(hF,&fileinfo))
		{
			if (IsWeWant(fileinfo))
			{
				files.push_back(fileinfo);
			}
		}

		return 0;
	}
private:
	void Close()
	{
		//   关闭句柄 
		if (hHost != NULL)
		{
			InternetCloseHandle(hHost);   
		}
		if (hInternet != NULL)
		{
			InternetCloseHandle(hInternet); 
		}
	}
public:
	HINTERNET hInternet;
	HINTERNET hHost;
};

