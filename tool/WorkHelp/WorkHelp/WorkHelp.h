
// WorkHelp.h : PROJECT_NAME Ӧ�ó������ͷ�ļ�
//

#pragma once

#ifndef __AFXWIN_H__
	#error "�ڰ������ļ�֮ǰ������stdafx.h�������� PCH �ļ�"
#endif

#include "resource.h"		// ������


// CWorkHelpApp: 
// �йش����ʵ�֣������ WorkHelp.cpp
//

class CWorkHelpApp : public CWinApp
{
public:
	CWorkHelpApp();

// ��д
public:
	virtual BOOL InitInstance();

// ʵ��

	DECLARE_MESSAGE_MAP()
};

extern CWorkHelpApp theApp;