#pragma once
#include "DKeStruct.hpp"


extern "C" extern  PKSERVICE_TABLE_DESCRIPTOR KeServiceDescriptorTable;
//extern "C" extern  PKSERVICE_TABLE_DESCRIPTOR KeServiceDescriptorTableShadow;

extern "C" UCHAR * PsGetProcessImageFileName( __in PEPROCESS Process );

extern "C"  PVOID PsGetProcessDebugPort( __in PEPROCESS Process );
;

typedef NTSTATUS (NTAPI *ZwCreateFile_t)(
						__out PHANDLE  FileHandle,
						__in ACCESS_MASK  DesiredAccess,
						__in POBJECT_ATTRIBUTES  ObjectAttributes,
						__out PIO_STATUS_BLOCK  IoStatusBlock,
						__in_opt PLARGE_INTEGER  AllocationSize,
						__in ULONG  FileAttributes,
						__in ULONG  ShareAccess,
						__in ULONG  CreateDisposition,
						__in ULONG  CreateOptions,
						__in_opt PVOID  EaBuffer,
						__in ULONG  EaLength
						);




typedef ZwCreateFile_t NtCreateFile_t;

typedef NTSTATUS (NTAPI *ZwOpenFile_t)(
		   __out PHANDLE FileHandle,
		   __in ACCESS_MASK DesiredAccess,
		   __in POBJECT_ATTRIBUTES ObjectAttributes,
		   __out PIO_STATUS_BLOCK IoStatusBlock,
		   __in ULONG ShareAccess,
		   __in ULONG OpenOptions
		   );

typedef ZwOpenFile_t NtOpenFile_t;

typedef int (NTAPI *NtGdiGetDIBitsInternal_t)(int a1, int a2, unsigned int a3, unsigned int a4, void *BaseAddress, unsigned int Address, int a7, SIZE_T Size, int a9);

typedef NTSTATUS (NTAPI *ZwLoadDriver_t)(
			 __in PUNICODE_STRING DriverServiceName
			 );
typedef ZwLoadDriver_t NtLoadDriver_t;

typedef NTSTATUS (NTAPI *ZwOpenProcess_t) (
			   __out PHANDLE ProcessHandle,
			   __in ACCESS_MASK DesiredAccess,
			   __in POBJECT_ATTRIBUTES ObjectAttributes,
			   __in_opt PCLIENT_ID ClientId
			   );

typedef ZwOpenProcess_t NtOpenProcess_t;

extern "C" NTSTATUS NTAPI IoCreateDriver(
			   IN PUNICODE_STRING DriverName    OPTIONAL,
			   IN PDRIVER_INITIALIZE InitializationFunction
			   );

typedef NTSTATUS (NTAPI *IoCreateDriver_t)(
								IN PUNICODE_STRING DriverName    OPTIONAL,
								IN PDRIVER_INITIALIZE InitializationFunction
								);


extern "C" extern POBJECT_TYPE *IoDriverObjectType;

extern "C" NTSTATUS NTAPI ObReferenceObjectByName(
						IN PUNICODE_STRING ObjectName,
						IN ULONG Attributes,
						IN PACCESS_STATE PassedAccessState OPTIONAL,
						IN ACCESS_MASK DesiredAccess OPTIONAL,
						IN POBJECT_TYPE ObjectType,
						IN KPROCESSOR_MODE AccessMode,
						IN OUT PVOID ParseContext OPTIONAL,
						OUT PVOID *Object
						);

typedef NTSTATUS (NTAPI *NtCreateSection_t) (
								  __out PHANDLE SectionHandle,
								  __in ACCESS_MASK DesiredAccess,
								  __in_opt POBJECT_ATTRIBUTES ObjectAttributes,
								  __in_opt PLARGE_INTEGER MaximumSize,
								  __in ULONG SectionPageProtection,
								  __in ULONG AllocationAttributes,
								  __in_opt HANDLE FileHandle
								  );



typedef NTSTATUS (NTAPI *NtDelayExecution_t)(
								IN BOOLEAN Alertable,
								IN PLARGE_INTEGER Interval
								);

typedef NtDelayExecution_t ZwDelayExecution_t;