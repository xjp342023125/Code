#pragma once
// Com All!
#include "XType.h"
#include "XSTL.hpp"


static void xstrcpy(cstr_t src,str_t dst,int8 cend){
	while (*src != 0 && *src != cend){
		*dst = *src;
		++dst;
		++src;
	}
}

static cstr_t xskipchar(cstr_t pBegin,cstr_t pEnd,int8 c){
	while (*pBegin == c && pBegin < pEnd)	{
		++pBegin;
	}
	return pBegin;
}

struct XStr{
	int8	sz[30];
};
typedef XArray<XStr,20>	Paras_t;

static Paras_t xstrtokex(cstr_t pBegin,cstr_t pEnd,int8 cSplit=' '){
	Paras_t paras;
	paras.Init();
	while (pBegin && pBegin<pEnd)	{
		XStr xTemp;
		memset(&xTemp,0,sizeof(xTemp));
		pBegin = xskipchar(pBegin,pEnd,cSplit);
		if (pBegin<pEnd){
			xstrcpy(pBegin,xTemp.sz,cSplit);
			pBegin = strchr(pBegin,cSplit);
			paras.AddT(xTemp);
		}
	}
	return paras;
}


#define SAMECHAR(a,b) (((a) | 0x20) == ((b) | 0x20))
template<class char_t>
static const char_t* xstrstr(const char_t *pSrc,const char_t * pDst,bool bCaseSensitive){
	const char_t * pToFind = NULL;
	const char_t * pBase = NULL;
	while (0 != *pSrc){
		pBase = pSrc;pToFind = pDst;
		if (bCaseSensitive)	{
			while(0 != *pBase && 0 != *pToFind && *pBase == *pToFind){
				++pBase;++pToFind;
			}
		}else{
			while(0 != *pBase && 0 != *pToFind && SAMECHAR(*pBase, *pToFind)){
				++pBase;++pToFind;
			}
		}
		if (0 == *pToFind){
			return pSrc;
		}else{
			++pSrc;
		}
	}
	return NULL;
}
static cstr_t xstrstri(cstr_t pSrc, cstr_t pDst)
{
	return xstrstr<char>(pSrc, pDst, false);
}
static int32 xatoi(const char *str)
{
	int32 value = 0;
	int32 nflag=1;
	if (*str == '-'){
		nflag=-1;
		++str;
	}

	while(*str>='0' && *str<='9'){
		value *= 10;
		value += *str - '0';
		str++;
	}
	value*=nflag;
	
	return value;
}


static void EncodeBuf(Buf &bf, uint8 cKey)
{
	for (int i = 0; i < bf.m_nCapacity; ++i)
	{
		((uint8*)bf.m_addr)[i] ^= cKey;
	}
}

const char szCode[] = "qwertyuiopasdfghjklzxcvbnm0123456789QWERTYUIOPASDFGHJKLZXCVBNM";
const int nszCodeLen = sizeof(szCode)-1;


static void ChangeToDst(char cSrc, char &cDst1, char &cDst2)
{
	cDst1 = szCode[((unsigned char)cSrc) / nszCodeLen];
	cDst2 = szCode[((unsigned char)cSrc) % nszCodeLen];
}

static void ChangeToSrc(char &cSrc, char cDst1, char cDst2)
{
	int id1 = -1, id2 = -1;
	for (int i = 0; i < nszCodeLen; ++i)
	{
		if (cDst1 == szCode[i])
		{
			id1 = i;
		}
		if (cDst2 == szCode[i])
		{
			id2 = i;
		}
	}

	cSrc = char(id1 * nszCodeLen + id2);
}
// 加密函数
// nSrcLen:包括 \0 
// nDstLen:至少为nSrcLen二倍
// 返回值:0为正常
// src->dst
// -->
static int Encode(const char *pSrc, int nSrcLen, char *pDst, int nDstLen)
{
	if (nDstLen < 2 * nSrcLen)
	{
		return -1;
	}
	for (int i = 0; i < nSrcLen; ++i)
	{
		ChangeToDst(pSrc[i], pDst[2 * i], pDst[2 * i + 1]);
	}
	return 0;
}
// 解密函数
// nSrcLen:包括 \0 
// nDstLen:至少为nSrcLen二倍
// 返回值:0为正常
// dst->src
//<--
static int Decode(char *pSrc, int nSrcLen, const char *pDst, int nDstLen)
{
	if (nSrcLen < nDstLen /2 )
	{
		return -1;
	}
	for (int i = 0; i < nDstLen/2; ++i)
	{
		ChangeToSrc(pSrc[i], pDst[2 * i], pDst[2 * i + 1]);
	}
	return 0;
}

// test func
//=====================================================================

