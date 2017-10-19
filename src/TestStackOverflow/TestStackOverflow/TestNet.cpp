// TestNet.cpp : Defines the entry point for the console application.
//

#include "stdafx.h"

#pragma pack(1)
template<int nBufSize, int nZhanSize>
struct sStackOverFlow
{
	sStackOverFlow()
	{
		memset(szZhanWei, 0x8, nZhanSize );
		
		memset(sz, 0x7, nBufSize);
	}
	char sz[nBufSize];
	char szZhanWei[nZhanSize];
	void* addrRet;
	

};
#pragma pack()

void outstr(int a, const char *p)
{
	char sz[10];
	strcpy(sz, p);
	//==================
}
void fff()
{
	printf("hack func");
}
int _tmain(int argc, _TCHAR* argv[])
{
	sStackOverFlow<10, 0x2 + 4> ssss;
	ssss.addrRet = &fff;
	outstr(111, (char*)&ssss);
	getchar();
	return 0;
}

