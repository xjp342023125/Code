#ifndef _CLOG_
#define _CLOG_

#include <vector>
#include <iostream>
#include <string>
#include <sstream>
#include <fstream>
#include "time.h"
#include "XLock.hpp"
#include "string.h"
using namespace std;

//////////////////////////////////////////////////////////////

class OutDevice
{
public:
	virtual void Init(const char *pContent) {};
	virtual void PutStr(const char *pContent) {};
	virtual void Close() {};
};
class OutPutDevice :public OutDevice
{
	virtual void PutStr(const char *pContent) 
	{
	#ifdef WIN32
		OutputDebugStringA(pContent);
	#endif
	}
};

class FileDevice :public OutDevice
{
public:
	virtual void Init(const char *pContent) 
	{
		m_sFileName = pContent;
		m_nIndex = 0;
		m_File = NULL;
		OpenFile();
	};
	virtual void PutStr(const char *pContent) 
	{
#define MAX_LINE_COUNT	(100000)
		if (++nLineCount > MAX_LINE_COUNT)
		{
			OpenFile();
		}
		fwrite(pContent,1,strlen(pContent),(FILE*)m_File);
		fflush(m_File);
		
	};
	virtual void Close() 
	{
		if (NULL != m_File)
		{
			fclose(m_File);
		}
		
	}
private:
	void OpenFile()
	{
		Close();
		time_t tNow = time(NULL);
		tm *ptmNow = localtime(&tNow),tmNow;
		memset(&tmNow,0,sizeof(tm));
		if (ptmNow)
		{
			tmNow = *ptmNow;
		}
		char szFileName[500] = {0};
		sprintf(szFileName,"%s%d_%d_%d_%d_%d_%d.log_%d",
			m_sFileName.c_str(),
			tmNow.tm_year+1900,
			tmNow.tm_mon+1,
			tmNow.tm_mday,
			tmNow.tm_hour,
			tmNow.tm_min,
			tmNow.tm_sec,
			++m_nIndex);
		nLineCount = 0;
		m_File = fopen(szFileName,"w");
	}
private:
	FILE*				m_File;
	int					m_nIndex;
	string				m_sFileName;
	int nLineCount;
};
class ConsoleDevice : public OutDevice
{
public:
	virtual void PutStr(const char *pContent) {printf("%s\n",pContent);};
};
//////////////////////////////////////////////////////////////
class CLog
{
public:
	CLog()
	{
		AddDevice(new ConsoleDevice);
		AddDevice(new OutPutDevice);
	}
	struct OneLine
	{
		OneLine(CLog *log)
		{
			m_pLog = log;
			time_t tNow = time(NULL);
			tm *ptmNow = localtime(&tNow);
			if(ptmNow)
			{
				m_Stream<<(ptmNow->tm_year+1900)
					<<"-"<<(ptmNow->tm_mon+1)
					<<"-"<<ptmNow->tm_mday
					<<" "<<ptmNow->tm_hour
					<<":"<<ptmNow->tm_min
					<<":"<<ptmNow->tm_sec<<" ";
			}
		}
		~OneLine(){m_Stream<<"\r\n";m_pLog->PutToDevice(m_Stream.str().c_str());}
		
		OneLine(const OneLine& r)
		{
			m_pLog = r.m_pLog;
		}


		template <typename T>
		OneLine& operator<<(const T &t)
		{
			m_Stream<<t;
			return *this;
		}
		CLog *m_pLog;
		stringstream m_Stream;
	};
public:
	static CLog& Get()
	{
		static CLog l;
		return l;
	}
	~CLog(){CloseDevice();}
public:
	void InitDevice(const char *pContent)
	{
		AddDevice(new FileDevice);
		
		for (DeviceVec_t::iterator it = m_DeviceVec.begin(); it != m_DeviceVec.end();++it)
		{	
			(*it)->Init(pContent);
		}
	}
	void PutToDevice(const char *pContent)
	{
		lock.lock();
		for (DeviceVec_t::iterator it = m_DeviceVec.begin(); it != m_DeviceVec.end();++it)
		{	
			(*it)->PutStr(pContent);
		}
		lock.unlock();
	}
	void CloseDevice()
	{
		for (DeviceVec_t::iterator it = m_DeviceVec.begin(); it != m_DeviceVec.end();++it)
		{	
			(*it)->Close();
			delete (*it);
		}
	}
public:
	
	OneLine l(){return OneLine(this);}
	void AddDevice(OutDevice *p){m_DeviceVec.push_back(p);}
private:
	typedef vector<OutDevice *>		DeviceVec_t;
	DeviceVec_t		m_DeviceVec;
	CAutoLock lock;
};



struct OneLineNoWrite
{
	OneLineNoWrite()
	{

	}

	template <typename T>
	OneLineNoWrite& operator<<(const T &t)
	{
		return *this;
	}
};
static OneLineNoWrite GetNoLog()
{
	OneLineNoWrite r;
	return r;
}


//============.h===============
//#define USE_LOG
//#ifdef USE_LOG
//extern CLog gSqlLog;
//static void init_mysql_log()
//{
//	gSqlLog.InitDevice("sql");
//}
//
//#define HINFO	gSqlLog.l()
//#define HERROR	gSqlLog.l()<<"Error,Line"<<__LINE__<<","
//#else
//#define HINFO	gSqlLog()
//#define HERROR	gSqlLog()
//#endif

//============.cpp=============
//CLog gSqlLog;
//init_mysql_log();
#endif