#pragma once
#include "XMicro.h"
#include "XType.h"
#include <cstdio>
#include <direct.h>
#include <io.h>
static void xdel_file(cstr_t path)
{
	remove(path);
}

static bool xexsit_file(cstr_t path)
{
	return 0 == _access(path, 0);
}

static bool xexsit_foder(cstr_t path)
{
	return 0 == _access(path, 0);
}


static bool xcreat_foderex(cstr_t path)
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