// GetPoint.cpp : Defines the entry point for the console application.
//
#pragma once

#include "windows.h"
#include <iostream>

using namespace std;

static bool IsRed(COLORREF c){
	return 255 == GetRValue(c)
		&& 0 ==GetGValue(c) 
		&& 0 ==GetBValue(c) ;
}

static COLORREF GetWinColorByDC(HDC hDC,int x,int y){
	return GetPixel(hDC,x,y);
}

static HDC GetWinDC(cstr_t pWinName){
	return GetDC(FindWindowA(NULL,pWinName));
}

static POINT GetWinPos(cstr_t pWinName){
	POINT pp={0,0};
	HWND h = FindWindowA(NULL,pWinName);
	if (NULL == h){
		return pp;
	}
	ScreenToClient(h,&pp);
	pp.x = -pp.x;
	pp.y = -pp.y;
	return pp;
}

static void Click(DWORD interval){
	mouse_event(MOUSEEVENTF_LEFTDOWN,0,0,0,0);
	Sleep(interval);
	mouse_event(MOUSEEVENTF_LEFTUP,0,0,0,0);
}

static void MovMouse(int x,int y){
	static int XX=0;
	static int YY=0;
	if (0 == XX && 0 == YY)	{
		XX = GetSystemMetrics(SM_CXSCREEN);
		YY = GetSystemMetrics(SM_CYSCREEN);
	}
	
	mouse_event(MOUSEEVENTF_ABSOLUTE|MOUSEEVENTF_MOVE,x*65535/XX,y*65535/YY,0,0);
}

