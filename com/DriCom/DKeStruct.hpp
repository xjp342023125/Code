#pragma once
#include "Ntifs.h"
typedef struct _LDR_DATA_TABLE_ENTRY {
	LIST_ENTRY    InLoadOrderLinks;
	LIST_ENTRY    InMemoryOrderLinks;
	LIST_ENTRY    InInitializationOrderLinks;
	PVOID      DllBase;
	PVOID      EntryPoint;
	ULONG      SizeOfImage;
	UNICODE_STRING  FullDllName;
	UNICODE_STRING  BaseDllName;
}LDR_DATA_TABLE_ENTRY,*PLDR_DATA_TABLE_ENTRY;

// 用来定义 SSDT 结构
typedef struct _KSYSTEM_SERVICE_TABLE
{
	PULONG  ServiceTableBase;                               // SSDT (System Service Dispatch Table)的基地址
	PULONG  ServiceCounterTableBase;                        // 用于 checked builds, 包含 SSDT 中每个服务被调用的次数
	ULONG   NumberOfService;                                // 服务函数的个数, NumberOfService * 4 就是整个地址表的大小
	ULONG   ParamTableBase;                                 // SSPT(System Service Parameter Table)的基地址
} KSYSTEM_SERVICE_TABLE, *PKSYSTEM_SERVICE_TABLE;

typedef struct _KSERVICE_TABLE_DESCRIPTOR
{
	KSYSTEM_SERVICE_TABLE   ntoskrnl;                       // ntoskrnl.exe 的服务函数
	KSYSTEM_SERVICE_TABLE   win32k;                         // win32k.sys 的服务函数(GDI32.dll/User32.dll 的内核支持)
	KSYSTEM_SERVICE_TABLE   notUsed1;
	KSYSTEM_SERVICE_TABLE   notUsed2;
}KSERVICE_TABLE_DESCRIPTOR, *PKSERVICE_TABLE_DESCRIPTOR;