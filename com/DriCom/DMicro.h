#pragma once
#pragma warning(disable:4505)
#include "../ComAll/XType.h"

#define ARRAY_COUNT(t) (sizeof(t)/sizeof(t[0]))


//符号 格式说明符 类型 
//%c, %lc ANSI字符 char 
//%C, %wc 宽字符 wchar_t 
//%d, %i 十进制有符号整数 int 
//%D 十进制__int64 __int64 
//%L 十六进制的LARGE_INTEGER LARGE_INTEGER 
//%s, %ls NULL终止的ANSI字符串 char* 
//%S, %ws NULL终止的宽字符串 wchar_t* 
//%Z ANSI_STRING字符串 ANSI_STRING 
//%wZ UNICODE_STRING字符串 UNICODE_STRING 
//%u 十进制的ULONG ULONG 
//%x 小写字符十六进制的ULONG ULONG 
//%X 大写字符十六进制的ULONG ULONG 
//%p 指针Pointer 32/64位  
#define P_USTR(U_STR)	\
	if (NULL != (U_STR))	\
		KdPrint(("%s = %wZ\n",#U_STR,(U_STR)));\
	else\
		KdPrint(("%s = NULL\n",#U_STR));

#define P_CSTR(CSTR)  KdPrint(("%s = %s\n",#CSTR,(CSTR)));
#define P_INT(val)  KdPrint(("%s = %d\n",#val,(val)));
#define P_UINT(val)  KdPrint(("%s = %u\n",#val,(val)));
#define P_ADDR(val)  KdPrint(("%s = %x\n",#val,(val)));

// 根据 ZwServiceFunction 获取 ZwServiceFunction 在 SSDT 中所对应的服务的索引号
#define SYSCALL_INDEX(ServiceFunction) (*(PULONG)((PUCHAR)ServiceFunction + 1))
// 根据 ZwServiceFunction 来获得服务在 SSDT 中的索引号，然后再通过该索引号来获取 ntServiceFunction的地址
#define SYSCALL_FUNCTION(ServiceFunction) KeServiceDescriptorTable->ntoskrnl.ServiceTableBase[SYSCALL_INDEX(ServiceFunction)]

#define CHECK_RETV(value,ret)	if (0 == (value))\
{ return (ret); };
#define CHECK_RET(value)	CHECK_RETV(value,0)

#define WORD uint16	


