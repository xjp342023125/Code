#pragma once 
#include "XType.h"
#include "malloc.h"

static addr_t XGetMem(int64 nSize){
	addr_t p = malloc((size_t)nSize);
	memset(p,0,(size_t)nSize);
	return p;
}

static void XFreeMem(addr_t addr){
	if (addr){
		free(addr);
	}
}