#pragma once
#include<windows.h>
#include "../ComAll/XSTL.hpp"

static Buf ScreenSnap(HBITMAP hBitmap, HDC dc)
{
	Buf bf;
	BITMAP bmInfo;
	DWORD bmDataSize;
	char *bmData;//位图数据
	GetObject(hBitmap,sizeof(BITMAP),&bmInfo);//根据位图句柄，获取位图信息
	bmDataSize=bmInfo.bmWidthBytes*bmInfo.bmHeight;//计算位图数据大小
	//bmData=new char[bmDataSize];//分配数据
	bmData = (char*)malloc(bmDataSize);
	BITMAPFILEHEADER bfh;//位图文件头
	bfh.bfType=0x4d42;
	bfh.bfSize=bmDataSize+54;
	bfh.bfReserved1=0;
	bfh.bfReserved2=0;
	bfh.bfOffBits=54;
	BITMAPINFOHEADER bih;//位图信息头
	bih.biSize=40;
	bih.biWidth=bmInfo.bmWidth;
	bih.biHeight=bmInfo.bmHeight;
	bih.biPlanes=1;
	bih.biBitCount=24;
	bih.biCompression=BI_RGB;
	bih.biSizeImage=bmDataSize;
	bih.biXPelsPerMeter=0;
	bih.biYPelsPerMeter=0;
	bih.biClrUsed=0;
	bih.biClrImportant=0;
	::GetDIBits(dc,hBitmap,0,bmInfo.bmHeight,bmData,(BITMAPINFO *)&bih,DIB_RGB_COLORS);//获取位图数据部分
	bf.AddBuf(&bfh, sizeof(BITMAPFILEHEADER));
	bf.AddBuf(&bih, sizeof(BITMAPINFOHEADER));
	bf.AddBuf(bmData,bmDataSize);
	free(bmData);
	return bf;
}
static Buf _Jietu()
{
	Buf bf;
	HWND DeskWnd=::GetDesktopWindow();//获取桌面窗口句柄
	RECT DeskRC;
	::GetClientRect(DeskWnd,&DeskRC);//获取窗口大小
	HDC DeskDC=GetDC(DeskWnd);//获取窗口DC
	HBITMAP DeskBmp=::CreateCompatibleBitmap(DeskDC,DeskRC.right,DeskRC.bottom);//兼容位图
	HDC memDC=::CreateCompatibleDC(DeskDC);//兼容DC
	SelectObject(memDC,DeskBmp);//把兼容位图选入兼容DC中
	BitBlt(memDC,0,0,DeskRC.right,DeskRC.bottom,DeskDC,0,0,SRCCOPY);//拷贝DC
	bf = ScreenSnap(DeskBmp, DeskDC);
	ReleaseDC(DeskWnd,DeskDC);
	DeleteObject(DeskBmp);
	ReleaseDC(DeskWnd,memDC);
	return bf;
}