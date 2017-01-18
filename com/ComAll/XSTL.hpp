#pragma once
#include "XType.h"
#include "XComFunc.hpp"
template<class T,int MAX_CNT>
class  XArray
{
public:
	void Init()	{
		Cnt = 0;
		memset(Array ,0, sizeof(T) * MAX_CNT);
	}

	bool AddT(const T &t){
		if(Cnt >= MAX_CNT)
			return false;
		Array[Cnt++] = t;
		return true;
	}
	bool FindT(const T &t){
		for(uint32 i = 0;i < Cnt;++i){
			if(t == Array[i])
				return true;
		}
		return false;
	}
public:
	uint32	Cnt;
	T		Array[MAX_CNT];
};

struct Buf
{
	Buf(){m_nCapacity = 0;m_addr = NULL;m_nLen = 0;}

	void AddBuf(addr_t addr, int64 nSize){
		if (m_nLen + nSize > m_nCapacity){
			ReCapacity((m_nLen + nSize)*2+1000);
		}
		memcpy(m_addr+m_nLen,addr,(size_t)nSize);
		m_nLen+=nSize;
	}

	void ReCapacity(int64 nNewCapacity)
	{
		addr_t pNew = XGetMem(nNewCapacity);
		memcpy(pNew, m_addr, (size_t)m_nLen);
		XFreeMem(m_addr);
		m_addr = (int8*)pNew;
		m_nCapacity= nNewCapacity;
	}
	void Clean()
	{
		if (m_addr)
		{
			XFreeMem(m_addr);
		}
		m_nCapacity = 0; m_addr = NULL; m_nLen = 0;
	}

	int64 m_nCapacity;
	int64 m_nLen;
	int8* m_addr;
};
