#pragma once

#define ARRAY_CNT(v)	(sizeof(v)/sizeof((v)[0]))
#define L_VALUE(v)		"["<<#v<<"]="<<v<<"."

#define WIN_FILE_SEPARATOR		'\\'
#define LINUX_FILE_SEPARATOR	'//'

#ifdef WIN32
	#define FILE_SEPARATOR	WIN_FILE_SEPARATOR
#else
	#define FILE_SEPARATOR	LINUX_FILE_SEPARATOR
#endif 

#define MAX_PATH_LEN	512

#define CHECK_RETV(value,ret)	if (0 == (value)) {  return (ret); };

#define xmalloc		malloc
#define xfree		free