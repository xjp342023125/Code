#pragma once

#define ARRAY_CNT(v)	(sizeof(v)/sizeof((v)[0]))
#define L_VALUE(v)		"["<<#v<<"]="<<v<<"."

#define WIN_FILE_SEPARATOT		'\\'
#define LINUX_FILE_SEPARATOT	'//'

#ifdef WIN32
	#define FILE_SEPARATOT	WIN_FILE_SEPARATOT
#else
	#define FILE_SEPARATOT	LINUX_FILE_SEPARATOT
#endif 

#define MAX_PATH_LEN	512

#define CHECK_RETV(value,ret)	if (0 == (value)) {  return (ret); };

#define xmalloc		malloc
#define xfree		free