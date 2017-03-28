#pragma once
#include "afxwin.h"
#include <string>
#include "time.h"
#include "CStr.hpp"
using namespace std;
static void CEditAddText(CEdit &m_edit,const char *p,bool bAddTime = false){
	string s;
	if (bAddTime){
		time_t tNow = time(NULL);
		s = string(ctime(&tNow));
		s+='	';
	}
	s+=p;
	s+='\n';
	int nLength=m_edit.SendMessage(WM_GETTEXTLENGTH);  
	m_edit.SetSel(nLength,  nLength);  
	CStringW cw(s.c_str());
	m_edit.ReplaceSel(cw);
}


static string GetEditVal(HWND m_hWnd, int ID)
{
	wchar_t sz[30000];
	GetDlgItemTextW(m_hWnd, ID, sz, 30000);
	char * p = Unicode_Ansi(sz);
	string ret = p;
	XFreeMem(p);
	return ret;
}

static void SetEditVal(HWND m_hWnd, int ID, string val)
{
	wchar_t *p = Ansi_Unicode(val.c_str());
	SetDlgItemTextW(m_hWnd, ID, p);
	XFreeMem(p);
}