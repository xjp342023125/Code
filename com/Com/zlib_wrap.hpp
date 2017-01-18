#pragma once
#include "../ComAll/XSTL.hpp"
#include "../third/zlib/zlib.h"


static Buf XCompressZlib(const Buf &src)
{
	Buf bf;
	
	while (true)
	{
		uLongf Cap = bf.m_nCapacity;
		int nRet = compress((Bytef*)bf.m_addr, &Cap, (Bytef*)src.m_addr, src.m_nLen);
		cout << "XCompressZlib"<<nRet << endl;
		if (Z_STREAM_ERROR == nRet || Z_BUF_ERROR == nRet)
		{
			bf.ReCapacity(bf.m_nCapacity * 2+100);
		}
		else
		{
			bf.m_nLen = Cap;
			break;
		}
	}
	
	return bf;
}

static Buf XUnCompressZlib(const Buf &src)
{
	Buf bf;

	while (true)
	{
		uLongf Cap = bf.m_nCapacity;
		int nRet = uncompress((Bytef*)bf.m_addr, &Cap, (Bytef*)src.m_addr, src.m_nLen);
		cout << "XUnCompressZlib"<<nRet << endl;
		if (Z_STREAM_ERROR == nRet || Z_BUF_ERROR == nRet)
		{
			bf.ReCapacity(bf.m_nCapacity * 2 + 100);
		}
		else
		{
			bf.m_nLen = Cap;
			break;
		}
	}
	return bf;
}
//dst->src
static void XCompressZlibFile(cstr_t dst, cstr_t src)
{
	DeleteFileA(dst);
	Buf bf = XReadFile(src);
	Buf dstbf = XCompressZlib(bf);
	XWriteFile(dst, dstbf.m_addr, dstbf.m_nLen);
}
//dst->src
static void XUnCompressZlibFile(cstr_t dst, cstr_t src)
{
	DeleteFileA(dst);
	Buf bf = XReadFile(src);
	Buf dstbf = XUnCompressZlib(bf);
	XWriteFile(dst, dstbf.m_addr, dstbf.m_nLen);
}