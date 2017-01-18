#pragma once
#include <iostream>
#include "VMProtectSDK.h"
#include "CFunc.hpp"
using namespace std;


//rw
//00：执行断点
//01：数据写入断点
//10：I/0读写断点
//11：读写断点，读取指令不算


//len
//00：1字节（执行断点只能是1字节长）
//01：2字节
//10：未定义或者是8字节（和cpu的系列有关系）
//11：4字节
enum E_DR7_RWType
{
	E_DR7_Exc = 0,
	E_DR7_W = 1,
	E_DR7_RWE = 2,
	E_DR7_RW = 3,
};
enum E_DR7_Len
{
	E_DR7_Len1 = 0,
	E_DR7_Len2 = 1,
	E_DR7_Len4 = 3,
};
struct SDR7
{


	uint32 L0 : 1;
	uint32 G0 : 1;
	uint32 L1 : 1;
	uint32 G1 : 1;
	uint32 L2 : 1;
	uint32 G2 : 1;
	uint32 L3 : 1;
	uint32 G3 : 1;

	uint32 nouse1 : 3;

	uint32 GD : 1;

	uint32 RW0 : 2;
	uint32 Len0 : 2;
	uint32 RW1 : 2;
	uint32 Len1 : 2;
	uint32 RW2 : 2;
	uint32 Len2 : 2;
	uint32 RW3 : 2;
	uint32 Len3 : 2;




	void Set0(E_DR7_RWType eType, E_DR7_Len eLen)
	{
		L0 = 1;		G0 = 1;		Len0 = eLen;		RW0 = eType;
		if (E_DR7_Exc == eType)
			Len0 = E_DR7_Len1;
	}

	void Set1(E_DR7_RWType eType, E_DR7_Len eLen)
	{
		L1 = 1;		G1 = 1;		Len1 = eLen;		RW1 = eType;
		if (E_DR7_Exc == eType)
			Len1 = E_DR7_Len1;
	}

	void Set2(E_DR7_RWType eType, E_DR7_Len eLen)
	{
		L2 = 1;		G2 = 1;		Len2 = eLen;		RW2 = eType;
		if (E_DR7_Exc == eType)
			Len2 = E_DR7_Len1;
	}

	void Set3(E_DR7_RWType eType, E_DR7_Len eLen)
	{
		L3 = 1;		G3 = 1;		Len3 = eLen;		RW3 = eType;
		if (E_DR7_Exc == eType)
			Len3 = E_DR7_Len1;
	}

};


static void func(addr_t addr)
{
	addr_t addrJmp = addr;
	__asm
	{
		push 0
		jmp addrJmp
	}
}
template <class T>
T& GetValue(addr_t addr)
{
	return *(T*)addr;
};

static void DoHard(LPEXCEPTION_POINTERS ExceptionInfo,int n)
{
	VMProtectBegin(__FUNCTION__);
	if (EXCEPTION_SINGLE_STEP == ExceptionInfo->ExceptionRecord->ExceptionCode )
	{
		DWORD eip = ExceptionInfo->ContextRecord->Eip;
		addr_t addr = addr_t(eip - sizeof(DWORD)*(1 + n));
		DWORD key=GetValue<DWORD>(addr);
		//cout<<",n="<<n
		//	<<",eip="<<eip
		//	<<",addr="<<addr
		//	<<",val="<<GetValue<DWORD>(addr)
		//	<<endl;
		GetValue<DWORD>(addr) = key*key;
	}else
	{
		//cout<<"not in"<<endl;
	}
	VMProtectEnd();
}
template<int n>
static INT CheckHardBreak( LPEXCEPTION_POINTERS ExceptionInfo )
{
	DoHard(ExceptionInfo,n);
	return EXCEPTION_EXECUTE_HANDLER;
}
class CAntiHard
{
public:
	BOOL Init()
	{
		VMProtectBegin(__FUNCTION__);

		memset(dw,0x90,sizeof(dw));
		ZeroMem(c);
		c.ContextFlags = CONTEXT_DEBUG_REGISTERS;

		index[0]=GetTickCount()%100;
		index[1]=GetTickCount()%79;
		index[2]=GetTickCount()%59;
		index[3]=GetTickCount()%39;
		c.Dr0 = (DWORD)&dw[index[0]];
		c.Dr1 = (DWORD)&dw[index[1]];
		c.Dr2 = (DWORD)&dw[index[2]];
		c.Dr3 = (DWORD)&dw[index[3]];

		SDR7 dr7;
		ZeroMem(dr7);
		dr7.Set0(E_DR7_Exc,E_DR7_Len1);
		dr7.Set1(E_DR7_Exc,E_DR7_Len1);
		dr7.Set2(E_DR7_Exc,E_DR7_Len1);
		dr7.Set3(E_DR7_Exc,E_DR7_Len1);
		memcpy(&c.Dr7,&dr7,sizeof(c.Dr7));

		VMProtectEnd();
		return SetThreadContext(GetCurrentThread(),&c);
	}
	BOOL Check()
	{
		return Check1() 
			&& Check2();
	}


private:
	BOOL Check2()
	{
		VMProtectBegin(__FUNCTION__);
		static int nnn = 0;
		indexCheck = index[nnn++ % 4];
		addr_t addr = (addr_t)&dw[indexCheck];
		int nExc = -1;
		VMProtectEnd();
#define CCC(n)	if (-1 ==nExc && n == indexCheck){nExc = n; __try{	func(addr);	}__except( CheckHardBreak<n>( GetExceptionInformation()) ) 	{}	}
		CCC(0);	CCC(1);	CCC(2);	CCC(3);	CCC(4);	CCC(5);	CCC(6);	CCC(7);	CCC(8);	CCC(9);
		CCC(10);	CCC(11);	CCC(12);	CCC(13);	CCC(14);	CCC(15);	CCC(16);	CCC(17);	CCC(18);	CCC(19);
		CCC(20);	CCC(21);	CCC(22);	CCC(23);	CCC(24);	CCC(25);	CCC(26);	CCC(27);	CCC(28);	CCC(29);
		CCC(30);	CCC(31);	CCC(32);	CCC(33);	CCC(34);	CCC(35);	CCC(36);	CCC(37);	CCC(38);	CCC(39);
		CCC(40);	CCC(41);	CCC(42);	CCC(43);	CCC(44);	CCC(45);	CCC(46);	CCC(47);	CCC(48);	CCC(49);
		CCC(50);	CCC(51);	CCC(52);	CCC(53);	CCC(54);	CCC(55);	CCC(56);	CCC(57);	CCC(58);	CCC(59);
		CCC(60);	CCC(61);	CCC(62);	CCC(63);	CCC(64);	CCC(65);	CCC(66);	CCC(67);	CCC(68);	CCC(69);
		CCC(70);	CCC(71);	CCC(72);	CCC(73);	CCC(74);	CCC(75);	CCC(76);	CCC(77);	CCC(78);	CCC(79);
		CCC(80);	CCC(81);	CCC(82);	CCC(83);	CCC(84);	CCC(85);	CCC(86);	CCC(87);	CCC(88);	CCC(89);
		CCC(90);	CCC(91);	CCC(92);	CCC(93);	CCC(94);	CCC(95);	CCC(96);	CCC(97);	CCC(98);	CCC(99);

		//cout<<"main addr="<<addr
		//	<<",check addr="<<&indexCheck
		//	<<endl;
		
		return (indexCheck == nExc*nExc);
	}
	BOOL Check1()
	{
		VMProtectBegin(__FUNCTION__);

		CONTEXT cc = {CONTEXT_DEBUG_REGISTERS};
		GetThreadContext(GetCurrentThread(),&cc);

		BOOL check1 = (c.Dr0 == cc.Dr0
			&& c.Dr1 == cc.Dr1
			&& c.Dr2 == cc.Dr2
			&& c.Dr3 == cc.Dr3);
		VMProtectEnd();
		return check1;
	}
public:
	CONTEXT c;
	DWORD index[4];
	DWORD indexCheck;
	DWORD dw[100];
};