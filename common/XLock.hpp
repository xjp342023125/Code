#pragma once

#ifdef WIN32
#include "windows.h"
#define __PLATEFORM_WINDOWS__
#else
#include "pthread.h"
#define __PLATEFORM_LINUX__
#endif

class CAutoLock
{
public:
	inline CAutoLock()
	{
#if defined(__PLATEFORM_LINUX__)
		pthread_mutex_init(&m_mtx, NULL);
#elif defined(__PLATEFORM_WINDOWS__)
		::InitializeCriticalSection(&m_cs);
#endif
	}

	inline ~CAutoLock()
	{
#if defined(__PLATEFORM_LINUX__)
		pthread_mutex_destroy(&m_mtx);
#elif defined(__PLATEFORM_WINDOWS__)
		::DeleteCriticalSection(&m_cs);
#endif
	}

	inline void lock()
	{
#if defined(__PLATEFORM_LINUX__)
		pthread_mutex_lock(&m_mtx);
#elif defined(__PLATEFORM_WINDOWS__)
		EnterCriticalSection(&m_cs);
#endif
	}

	inline bool try_lock()
	{
#if defined(__PLATEFORM_LINUX__)
		return pthread_mutex_trylock(&m_mtx) == 0 ? true : false;
#elif defined(__PLATEFORM_WINDOWS__)
		return TryEnterCriticalSection(&m_cs) ? true : false;
#endif
	}

	inline void unlock()
	{
#if defined(__PLATEFORM_LINUX__)
		pthread_mutex_unlock(&m_mtx);
#elif defined(__PLATEFORM_WINDOWS__)
		LeaveCriticalSection(&m_cs);
#endif
	}

private:
#if defined(__PLATEFORM_LINUX__)
	pthread_mutex_t m_mtx;
#elif defined(__PLATEFORM_WINDOWS__)
	CRITICAL_SECTION m_cs; 
#endif
};