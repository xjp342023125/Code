#pragma once
#include "XMicro.h"
#include "XType.h"
#include <direct.h>

static bool xprocess_change_workpath(cstr_t path)
{
	char path_temp[MAX_PATH_LEN] = { 0 };
	strncpy(path_temp, path, MAX_PATH_LEN - 1);
	str_t sep = strrchr(path_temp, FILE_SEPARATOR);
	if (sep)
	{
		*sep = 0;
	}

	int ret = chdir(path_temp);
	return 0 == ret;
}