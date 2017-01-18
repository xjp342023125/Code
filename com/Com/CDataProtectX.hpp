#pragma once
#include"malloc.h"
#pragma warning(push)
#pragma warning(disable : 4311)
#pragma warning(disable : 4312)



template<class T>
class CDateProtect
{
public:
#define PRO_LIMIT1 20
#define PRO_LIMIT2 3
#define PRO_MAX (PRO_LIMIT1 + PRO_LIMIT2 + 10)
	explicit CDateProtect()
	{
		m_Inited = 0;
	}
	explicit CDateProtect(const T &t)
	{
		m_Inited = 0;
		this->GetT() = t;
	}
	void Init()
	{
		T *pT = (T*)malloc(sizeof(T)) ;

		unsigned int *pAddr[PRO_MAX];//一堆地址
		for (int i = 0; i < PRO_MAX; ++i)
		{
			pAddr[i] = (unsigned int*)malloc(sizeof(unsigned int));
		}	
		*pAddr[0] = (unsigned int)pT;
		for (int i = 1; i < PRO_MAX; ++i)
		{
			*pAddr[i]  = (unsigned int)pAddr[i-1];
		}

		SetCnt(GetTickCount() % PRO_LIMIT1 + PRO_LIMIT2);

		m_Base = (unsigned int)pAddr[GetCnt() - 1];
		m_Inited = 1;
	}
	T& GetT()
	{
		if (!m_Inited)
		{
			Init();
		}
		unsigned int *pAddr[PRO_MAX] = {0};//一堆地址
		pAddr[GetCnt()  - 1] = (unsigned int*)m_Base;
		for (int i =GetCnt() - 1 -1;i >=0;--i )
		{
			pAddr[i] = (unsigned int*)(void*)(*pAddr[i+1]);
		}
		return *(T*)pAddr[0];
	}
private:
	void SetCnt(unsigned int dwCnt)
	{
		m_Cnt = dwCnt*3 + 100;
	}
	unsigned int GetCnt()
	{
		return ( (m_Cnt-100) / 3);
	}
public:
	unsigned int m_Base;
	int m_Inited;
	unsigned int m_Cnt;
};

#pragma warning (pop)
