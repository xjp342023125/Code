#pragma once
#include "../ComAll/XType.h"
#include "../ComAll/XSTL.hpp"
#include <vector>
#include <string>
#include <fstream>
#include  <io.h>
#include  <stdio.h>
#include  <stdlib.h>
using namespace std;
typedef vector<string> linevec_t;
static bool_t XReadFileLine(cstr_t pPath, linevec_t &lines)
{
	lines.clear();
	bool_t bRet = false;
	ifstream in_file;
	string strTemp;

	in_file.open(pPath, ios::in);
	if (!in_file.is_open())
	{
		goto Exit0;
	}
	while (getline(in_file, strTemp))
		lines.push_back(strTemp);
	bRet = true;
Exit0:
	in_file.close();
	return bRet;
}
static bool_t XWriteFileLine(cstr_t pPath, const linevec_t &lines)
{
	bool_t bRet = false;
	ofstream out_file;

	out_file.open(pPath, ios::out);
	if (!out_file.is_open())
	{
		goto Exit0;
	}
	for (unsigned int i = 0; i < lines.size(); ++i)
	{
		out_file << lines[i] << endl;
	}
	bRet = true;
Exit0:
	out_file.close();
	return bRet;
}
static int64 XGetFileSize(cstr_t pPath)
{
	FILE *p = fopen(pPath, "r");
	if (!p)
	{
		return -1;
	}
	fseek(p, 0, SEEK_END);
	fpos_t pos = 0;
	fgetpos(p, &pos);

	fclose(p);
	return pos;

}

static Buf XReadFile(cstr_t pPath)
{
	Buf buf;
	int64 nLen = XGetFileSize(pPath);
	if (nLen < 0)
	{
		return buf;
	}

	FILE *p = fopen(pPath, "rb");
	if (!p)
	{
		return buf;
	}
	buf.ReCapacity(nLen+10);

	nLen = fread(buf.m_addr, 1, buf.m_nCapacity, p);
	buf.m_nLen = nLen;
	fclose(p);
	return buf;

}

static int XWriteFile(cstr_t pFile, const int8* p, int64 nLen, int32 nOff = 0)
{
	FILE *PF;
	PF = fopen(pFile, "ab+");
	if (!PF)
	{
		return -1;
	}
	if (0 != fseek(PF, nOff, SEEK_SET))
	{
		fclose(PF);
		return -2;
	}
	int32 nRet = fwrite(p, 1, nLen, PF);
	if (nRet != nLen)
	{
		fclose(PF);
		return -3;
	}
	fflush(PF);
	fclose(PF);
	return 0;

}

static void XDelFile(cstr_t path)
{
	remove(path);
}

static bool exist_foder(cstr_t path)
{
	return 0 == _access(path, 0);
}
static bool create_foder(cstr_t path)
{

}