#pragma once
#include "XMicro.h"
#include "XType.h"
#include <cstdio>
#include <direct.h>
#include <io.h>
#include <string>
#include <vector>
#include <set>
using namespace std;
static void xpath_add_separator(string &s)
{
	if (0 == s.size())
	{
		return;
	}
	if (s[s.size() - 1] != FILE_SEPARATOR)
		s += FILE_SEPARATOR;
}

static void xpath_convert(str_t path)
{

#ifdef WIN32
	char src = LINUX_FILE_SEPARATOR;
	char dst = WIN_FILE_SEPARATOR;
#else
	char src = WIN_FILE_SEPARATOR;
	char dst = LINUX_FILE_SEPARATOR;
#endif
	for (size_t i = 0; i < strlen(path); i++)
	{
		if (src == path[i])
		{
			path[i] = dst;
		}
	}
}

static string xpath_get_filename(cstr_t path)
{
	cstr_t p = strrchr(path, FILE_SEPARATOR);
	if (p)
	{
		if (*(p + 1) == 0)
		{
			return "";
		}
		else
		{
			return p + 1;
		}
	}
	else
	{
		return path;
	}
}
static string xpath_get_path(cstr_t path)
{
	cstr_t p = strrchr(path, FILE_SEPARATOR);
	if (p)
	{
		if (*(p + 1) == 0)
		{
			return path;
		}
		else
		{
			return string(path,p+1);
		}
	}
	else
	{
		return path;
	}
}
//////////////////////////////////////////////////////////////////////////////////
static void xfile_del(cstr_t path)
{
	remove(path);
}

static bool xfile_exsit(cstr_t path)
{
	return 0 == _access(path, 0);
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

static size_t xfile_read(cstr_t pPath, str_t &buf)
{
	int64 len = xfile_get_size(pPath);
	CHECK_RETV(len >= 0, 0);

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
//////////////////////////////////////////////////////////////////////////////////
static bool xfoder_exsit(cstr_t path)
{
	return 0 == _access(path, 0);
}

static bool xfoder_createx(cstr_t path)
{
	char path_temp[MAX_PATH_LEN] = { 0 };
	strncpy(path_temp, path, MAX_PATH_LEN - 1);
	str_t sep = strchr(path_temp, FILE_SEPARATOR);
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
		sep = strchr(sep + 1, FILE_SEPARATOR);
	}
	mkdir(path_temp);
	return xfoder_exsit(path);
}

static void xfoder_get_files(cstr_t path, set<string> &ret, bool bGetChild = true)
{
	struct _finddata_t FileInfo;
	long Handle;
	string temp;

	string sBase = string(path);
	xpath_add_separator(sBase);
	string strSearchPath = sBase + "*.*";
	Handle = _findfirst(strSearchPath.c_str(), &FileInfo);
	if (-1 == Handle)
	{
		return;
	}

	do
	{
		temp = sBase + FileInfo.name;

		if (FileInfo.attrib & _A_SUBDIR)
		{
			if (strcmp(FileInfo.name, ".") && strcmp(FileInfo.name, ".."))
			{
				if (bGetChild)
				{
					xfoder_get_files(temp.c_str(), ret, bGetChild);
				}
			}
		}
		else
		{
			ret.insert(temp);
		}
	} while (!_findnext(Handle, &FileInfo));
	_findclose(Handle);
}
//////////////////////////////////////////////////////////////////////////////////

static void xfile_test()
{
	xfoder_createx(".\\1\\2");
	xfoder_createx(".\\2\\4\\");
	xfoder_createx(".\\3\\6\\7");

	xfoder_createx("\\4\\4");//这种会创建到根目录
	xfoder_createx("5\\5");
	/*xfoder_createx("E:\\123\\456");
	cout << xfile_exsit(".\\Debug\\client.tlog") << endl;
	cout << xfoder_exsit(".\\Debug\\") << endl;
	cout << xfoder_exsit(".\\Debug1\\") << endl;*/


	str_t buf = NULL;
	size_t ret = xfile_read(".\\stdafx.cpp", buf);

	xfile_write_append(".\\123.txt","ccc", 3);
	xfile_write_append(".\\123.txt","ddd", 3);

	xfile_write_new(".\\222.txt", "ccc", 3);
	xfile_write_new(".\\222.txt", "ddd", 3);

	const char *path = ".\\";
	cout << "full path=" << path << ",path="<< xpath_get_path(path)<< ",file="<<xpath_get_filename(path) << endl;

	path = ".\\abc";
	cout << "full path=" << path << ",path=" << xpath_get_path(path) << ",file=" << xpath_get_filename(path) << endl;


	path = ".\\abc\\";
	cout << "full path=" << path << ",path=" << xpath_get_path(path) << ",file=" << xpath_get_filename(path) << endl;


	path = ".\\abc\\123.txt";
	cout << "full path=" << path << ",path=" << xpath_get_path(path) << ",file=" << xpath_get_filename(path) << endl;


	path = "";
	cout << "full path=" << path << ",path=" << xpath_get_path(path) << ",file=" << xpath_get_filename(path) << endl;


	path = "\\";
	cout << "full path=" << path << ",path=" << xpath_get_path(path) << ",file=" << xpath_get_filename(path) << endl;

}

