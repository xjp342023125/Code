#pragma once
#include "../ComAll/XType.h"
#include "DMicro.h"
#include "DKeStruct.hpp"
#include "DKeExtern.hpp"


//////////////////////////////////////////////////////////////////////////////////////////////
//to do release?
static void DStrCharToUnicode(UNICODE_STRING *pStr,const char *p){
	ANSI_STRING sAnsi;
	RtlInitAnsiString(&sAnsi,p);
	RtlAnsiStringToUnicodeString( pStr, &sAnsi, true );
}

static void DStrUnicodeToChar(UNICODE_STRING *pStr,char *p){
	ANSI_STRING sAnsi;
	RtlUnicodeStringToAnsiString(&sAnsi,pStr,true);
	strncpy(p,sAnsi.Buffer,sAnsi.Length);
	p[sAnsi.Length]=0;
}
static void DStrToWStr(cstr_t pName, WCHAR* pWName){
	UNICODE_STRING str;
	DStrCharToUnicode(&str, pName);
	wcsncpy(pWName, str.Buffer, str.Length);
	pWName[str.Length] = 0;
}
//////////////////////////////////////////////////////////////////////////////////////////////
static PLDR_DATA_TABLE_ENTRY DGetSysList(PDRIVER_OBJECT DriverObject){
	PLDR_DATA_TABLE_ENTRY ldr = (PLDR_DATA_TABLE_ENTRY)DriverObject->DriverSection;
	return ldr;
}
//////////////////////////////////////////////////////////////////////////////////////////////
void DUnLoadDriver(IN PDRIVER_OBJECT DriverObject,UNICODE_STRING BaseDllName){
	UNICODE_STRING Win32Device;

	WCHAR strDosDevices[256];
	RtlInitEmptyUnicodeString(&Win32Device,strDosDevices,sizeof(strDosDevices));
	RtlAppendUnicodeToString(&Win32Device,L"\\DosDevices\\");
	RtlAppendUnicodeStringToString(&Win32Device,&BaseDllName);

	IoDeleteSymbolicLink(&Win32Device);
	IoDeleteDevice(DriverObject->DeviceObject);
}

static NTSTATUS DCreateDriverName(IN PDRIVER_OBJECT DriverObject,UNICODE_STRING BaseDllName){
	UNICODE_STRING DeviceName,Win32Device;
	PDEVICE_OBJECT DeviceObject = NULL;
	NTSTATUS status;

	P_USTR(&BaseDllName);

	WCHAR strDevice[256];
	RtlInitEmptyUnicodeString(&DeviceName,strDevice,sizeof(strDevice));
	RtlAppendUnicodeToString(&DeviceName,L"\\Device\\");
	RtlAppendUnicodeStringToString(&DeviceName,&BaseDllName);
	P_USTR(&DeviceName);

	WCHAR strDosDevices[256];
	RtlInitEmptyUnicodeString(&Win32Device,strDosDevices,sizeof(strDosDevices));
	RtlAppendUnicodeToString(&Win32Device,L"\\DosDevices\\");
	RtlAppendUnicodeStringToString(&Win32Device,&BaseDllName);
	P_USTR(&Win32Device);

	status = IoCreateDevice(DriverObject,
		0,
		&DeviceName,
		FILE_DEVICE_UNKNOWN,
		0,
		FALSE,
		&DeviceObject);
	if (!NT_SUCCESS(status))
		return status;
	if (!DeviceObject)
		return STATUS_UNEXPECTED_IO_ERROR;

	DeviceObject->Flags |= DO_DIRECT_IO;
	DeviceObject->AlignmentRequirement = FILE_WORD_ALIGNMENT;
	status = IoCreateSymbolicLink(&Win32Device, &DeviceName);

	DeviceObject->Flags &= ~DO_DEVICE_INITIALIZING;

	return STATUS_SUCCESS;
}


static PDEVICE_OBJECT DGetDeviceByName(const WCHAR *pName){
	UNICODE_STRING ustrName;
	RtlInitUnicodeString(&ustrName,pName);
	P_USTR(&ustrName);
	PFILE_OBJECT fobj = NULL;
	PDEVICE_OBJECT dobj = NULL;

	NTSTATUS nState = IoGetDeviceObjectPointer(&ustrName,FILE_ALL_ACCESS,&fobj,&dobj);

	if (NT_SUCCESS(nState))
	{
		ObDereferenceObject(fobj);
	}
	KdPrint(("GetDeviceByName  %u,ret=%x",dobj,nState));
	return dobj;
}
static PDRIVER_OBJECT DGetDriverByName(cstr_t pName){
	NTSTATUS status;

	PDRIVER_OBJECT pDriver=NULL;
	UNICODE_STRING uName;
	DStrCharToUnicode(&uName,pName);

	status = ObReferenceObjectByName(&uName, 
		OBJ_CASE_INSENSITIVE|OBJ_KERNEL_HANDLE,
		NULL,
		0,
		*IoDriverObjectType, //这个参数指明DriverObject
		KernelMode,				//内核模式
		NULL,
		(PVOID*)&pDriver);
	KdPrint(("get dri by name %s %x", pName, pDriver));
	return pDriver;
}
static int DHideDriverName(PDRIVER_OBJECT pDri){
	if (pDri)
	{
		((PLDR_DATA_TABLE_ENTRY)pDri->DriverSection)->FullDllName.Buffer = NULL;
		return 0;
	}
	return -1;
}
static void DBreakList(LIST_ENTRY l){
	LIST_ENTRY *F = l.Flink;
	LIST_ENTRY *B = l.Blink;
	if (F)
	{
		F->Blink = B ? B->Flink : NULL;
	}
	if (B)
	{
		B->Blink = F ? F->Blink : NULL;
	}
	
	
}
static int DHideDriver(PDRIVER_OBJECT pDri){
	if (pDri)
	{
		PLDR_DATA_TABLE_ENTRY p = (PLDR_DATA_TABLE_ENTRY)pDri->DriverSection;
		DBreakList(p->InLoadOrderLinks);
		//DBreakList(p->InMemoryOrderLinks);
		//DBreakList(p->InInitializationOrderLinks);

		return 0;
	}
	return -1;
}
//////////////////////////////////////////////////////////////////////////////////////////////

//设置为不可写
static void DisableWrite(addr_t addr = nullptr){
	UNREFERENCED_PARAMETER(addr);

	_asm{
		mov eax,cr0
		or eax,10000h
		mov cr0,eax
		sti
	}
}
// 设置为可写
static void EnableWrite(addr_t addr = nullptr){
	UNREFERENCED_PARAMETER(addr);
	_asm{
		cli
		mov eax,cr0
		and eax,not 10000h
		mov cr0,eax
	}
}

//////////////////////////////////////////////////////////////////////////////////////////////
static int DriverSleep(ULONG MillionSecond){
	NTSTATUS st;
	LARGE_INTEGER DelayTime;
	DelayTime = RtlConvertLongToLargeInteger(-10000*MillionSecond);
	st=KeDelayExecutionThread( KernelMode, FALSE, &DelayTime );
	return (NT_SUCCESS(st));
}

//////////////////////////////////////////////////////////////////////////////////////////////
static PEPROCESS DProPid2Eprocess(HANDLE pid){
	PEPROCESS ret = NULL;
	if (NT_SUCCESS(PsLookupProcessByProcessId(pid,&ret))){
		return ret;
	}
	return NULL;
}

static int DProAttachPro(HANDLE pid){
	PEPROCESS pro = DProPid2Eprocess(pid);
	if (NULL == pro){	return -1;}
	KeAttachProcess(pro);
	P_CSTR("attach done");
	return 0;
}

static void DProDetachPro(){
	KeDetachProcess();
	P_CSTR("detach done")
}

static void DProKill(HANDLE pid){
	PEPROCESS pe = DProPid2Eprocess(pid);
	if (NULL == pe)	{
		return ;
	}
	KeAttachProcess(pe); //这里改为指定进程的 EPROCESS 

	for(int i=0x10000;i<0x20000000;i+=PAGE_SIZE) { 
		__try 	{ 
			memset((addr_t)i,0,PAGE_SIZE); //把进程内存全部置零 
		} 
		_except(1) 	{ 
			; 
		} 
	} 

	KeDetachProcess(); 
}
//////////////////////////////////////////////////////////////////////////////////////////////
static addr_t DGetFuncAddr(const char *p){
	addr_t retval = 0;
	if (0 == strcmp(p,"KiFastCallEntry"))	{
		__asm{
			pushfd
			pushad
			mov ecx, 0x176;
			rdmsr;
			mov retval, eax; // KiFastCallEntry
			popad;
			popfd;

		}
		return retval;
	}

	
	UNICODE_STRING str;
	DStrCharToUnicode(&str,p);
	//If the function name can be resolved, the routine returns a pointer to the function. Otherwise, the routine returns NULL.

	retval = (addr_t)MmGetSystemRoutineAddress(&str);
	
	return retval;
}

static addr_t DGetShawdomSSDT(){
	addr_t addr = DGetFuncAddr("KeAddSystemServiceTable");
	if (NULL == addr){
		P_CSTR("DGetShawdomSSDT err1");
		return NULL;
	}
	for (int i = 0;i < 200;++i){
		__try{
			PKSERVICE_TABLE_DESCRIPTOR* p =(PKSERVICE_TABLE_DESCRIPTOR*)((int8*)addr+i);
			if (MmIsAddressValid(*p)	&& *p != KeServiceDescriptorTable )
			{
				P_ADDR(*p);
				if (0 == memcmp(&(*p)->ntoskrnl,&KeServiceDescriptorTable->ntoskrnl,sizeof(KSYSTEM_SERVICE_TABLE)))
				{
					return *p;
				}			
			}
		}__except(1){
			P_CSTR("DGetShawdomSSDT except1");
			return NULL;
		}
	}
	return NULL;
}

static addr_t DGetSSDTTFuncAddr(PKSERVICE_TABLE_DESCRIPTOR pSSDT,int nIdx)
{
	P_ADDR(pSSDT);
	P_ADDR(pSSDT->ntoskrnl.ServiceTableBase);
	return *(addr_t*)(pSSDT->ntoskrnl.ServiceTableBase+nIdx) ;
}
static addr_t DGetShadowSSDTTFuncAddr(PKSERVICE_TABLE_DESCRIPTOR pSSDT,int nIdx)
{
	return *(addr_t*)(pSSDT->win32k.ServiceTableBase+nIdx) ;
}


//////////////////////////////////////////////////////////////////////////////////////////////
enum E_OSVersion
{
	E_XP,
	E_Vista,
	E_Win7,
	E_Win8OrGreater,
	E_End
};

static E_OSVersion  DGetOSVersion(){
	E_OSVersion eRet = E_End;
	ULONG uMajor = 0;
	ULONG uMin = 0;
	PsGetVersion(&uMajor,&uMin,NULL,NULL);

	if( uMajor == 6 && uMin == 2) //win8系统
	{
		eRet = E_Win8OrGreater;
	}
	if(uMajor == 6 && uMin == 1) //win7系统
	{
		eRet = E_Win7;
	}
	if(uMajor == 5 && uMin == 1) //xp系统
	{
		eRet = E_XP;
	}
	if(uMajor == 6 && uMin == 0) //vista系统
	{
		eRet = E_Vista;
	}
	KdPrint(("os %d,%d,%d",uMajor,uMin,eRet));
	return eRet;
}