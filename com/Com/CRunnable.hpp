#pragma once
#include "windows.h"
#include "process.h"

enum E_Thread_State
{
	E_Thread_Init,
	E_Thread_Running,
	E_Thread_Over
};
class CRunnable
{
public:
	CRunnable()
	{
		state = E_Thread_Init;
	}
	int run()
	{
		m_hThread = ::_beginthreadex(0,
			0,
			thread_main,
			this,
			0/*CREATE_SUSPENDED*/, 0);

		if (NULL == m_hThread)
		{
			return -1;
		}
		return 0;
	}
	bool running(){ return E_Thread_Running == state; }
	bool runover(){ return E_Thread_Over == state; }
protected:
	virtual void exec() = 0;
private:
	static unsigned int __stdcall thread_main(void* pVoid)
	{
		CRunnable *pRunner = (CRunnable*)pVoid;
		pRunner->state = E_Thread_Running;
		pRunner->exec();
		pRunner->state = E_Thread_Over;
		return NULL;
	}
public:
	uintptr_t m_hThread;
	E_Thread_State state;

};