#pragma once
#include "XType.h"


enum E_JMP_TYPE
{
	E_JMP,
	E_CALL,
	E_PUSH,
	E_MODIFY_ESP_MEM,
	E_ADD_ESP,
	E_SUB_ESP,
	E_SHORT_JMP,
	E_JMP_TYPE_END
};
//68 00 10 0B 00       push        offset f1 (0B1000h)  
//C7 04 24 40 48 22 01 mov         dword ptr [esp],offset hook+8 (1224840h)  
//E9 B9 FF FF FF       jmp         f2 (0B1040h)  
//83 C4 04             add         esp,4
//83 ec 04             sub         esp,4
static void SetJmpTo(uint8 *addr,uint8 *val,E_JMP_TYPE etype){

	if (E_JMP == etype)	{
		*(uint8*)(addr) = 0xe9;
		*(addr_t*)(addr+1) = addr_t(val - (addr+5));
	}else if (E_CALL == etype)	{
		*(uint8*)(addr) = 0xe8;
		*(addr_t*)(addr+1) = addr_t(val - (val+5));
	}else if (E_PUSH == etype)	{
		*(uint8*)(addr) = 0x68;
		*(addr_t*)(addr+1) = val;
	}else if (E_MODIFY_ESP_MEM == etype){
		*(uint8*)(addr) = 0xc7;
		*(uint8*)(addr+1) = 0x04;
		*(uint8*)(addr+2) = 0x24;
		*(addr_t*)(addr+3) = val;
	}else if (E_ADD_ESP == etype){
		*(uint8*)(addr) = 0x83;
		*(uint8*)(addr+1) = 0xc4;
		*(addr_t*)(addr+2) = val;
	}else if (E_SUB_ESP == etype){
		*(uint8*)(addr) = 0x83;
		*(uint8*)(addr+1) = 0xec;
		*(addr_t*)(addr+2) = val;
	}else if(E_SHORT_JMP == etype)
	{
		*(uint8*)(addr) = 0xEB;
		*(uint8*)(addr+1) = (uint8)(val - (addr+2));
	}
}