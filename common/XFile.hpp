#pragma once
#include "XMicro.h"
#include "XType.h"
#include <cstdio>
#include <direct.h>
#include <io.h>

static void xfile_del(cstr_t path)
{
	remove(path);
}

static bool xfile_exsit(cstr_t path)
{
	return 0 == _access(path, 0);
}

static bool xfoder_exsit(cstr_t path)
{
	return 0 == _access(path, 0);
}


static bool xfoder_createx(cstr_t path)
{
	char path_temp[MAX_PATH_LEN] = { 0 };
	strncpy(path_temp, path, MAX_PATH_LEN - 1);
	str_t sep = strchr(path_temp, FILE_SEPARATOT);
	while (true)
	{
		if (NULL == sep)
		{
			break;
		}
		char c_temp = *sep;
		*sep = 0;
		_mkdir(path_temp);
		*sep = c_temp;
		sep = strchr(sep + 1, FILE_SEPARATOT);
	}
	mkdir(path_temp);
	return true;
}

static int64 xfile_get_size(cstr_t pPath)
{
	FILE *p = fopen(pPath, "r");
	CHECK_RETV(NULL != p, -1);
	CHECK_RETV(0 == fseek(p, 0, SEEK_END), -2);
	
	fpos_t pos = 0;
	CHECK_RETV(0 == fgetpos(p, &pos), -3);

	fclose(p);
	return pos;

}

static size_t xfile_read(cstr_t pPath,str_t &buf)
{
	int64 len = xfile_get_size(pPath);
	CHECK_RETV(len >= 0,0);

	buf = (str_t)xmalloc(len);
	CHECK_RETV(NULL != buf, 0);

	FILE *p = fopen(pPath, "rb");
	CHECK_RETV(NULL != p, 0);

	size_t ret = fread(buf, 1, len, p);
	fclose(p);
	return ret;
}

static size_t xfile_free(str_t buf)
{
	xfree(buf);
}

static int xfile_write_append(cstr_t pFile, cstr_t p, size_t nLen, int32 nOff = 0)
{
	FILE *PF;
	PF = fopen(pFile, "ab+");
	CHECK_RETV(NULL != PF, 0);
	CHECK_RETV(0 == fseek(PF, nOff, SEEK_SET), -1);
	size_t nRet = fwrite(p, 1, nLen, PF);
	CHECK_RETV(nRet == nLen, -2);
	fflush(PF);
	fclose(PF);
	return 0;
}

static int xfile_write_new(cstr_t pFile, cstr_t p, size_t nLen, int32 nOff = 0)
{
	xfile_del(pFile);
	return xfile_write_append(pFile, p, nLen, nOff);
}


static void xfile_test()
{
	/*xfoder_createx(".\\1\\2");
	xfoder_createx(".\\2\\4\\");
	xfoder_createx(".\\3\\6\\7");

	xfoder_createx("\\4\\4");
	xfoder_createx("5\\5");
	xfoder_createx("E:\\123\\456");
	cout << xfile_exsit(".\\Debug\\client.tlog") << endl;
	cout << xfoder_exsit(".\\Debug\\") << endl;
	cout << xfoder_exsit(".\\Debug1\\") << endl;*/


	str_t buf = NULL;
	size_t ret = xfile_read(".\\stdafx.cpp", buf);

	xfile_write_append(".\\123.txt","ccc", 3);
	xfile_write_append(".\\123.txt","ddd", 3);

	xfile_write_new(".\\222.txt", "ccc", 3);
	xfile_write_new(".\\222.txt", "ddd", 3);

	

}