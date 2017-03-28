
// WorkHelpDlg.cpp : 实现文件
//

#include "stdafx.h"
#include "WorkHelp.h"
#include "WorkHelpDlg.h"
#include "afxdialogex.h"
#include <string>
using namespace std;

#ifdef _DEBUG
#define new DEBUG_NEW
#endif


// CWorkHelpDlg 对话框



CWorkHelpDlg::CWorkHelpDlg(CWnd* pParent /*=NULL*/)
	: CDialogEx(CWorkHelpDlg::IDD, pParent)
{
	//m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);
}

void CWorkHelpDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
}

BEGIN_MESSAGE_MAP(CWorkHelpDlg, CDialogEx)
	ON_WM_PAINT()
	ON_WM_QUERYDRAGICON()
	ON_BN_CLICKED(IDC_BUTTON1, &CWorkHelpDlg::OnBnClickedButton1)
	ON_BN_CLICKED(IDC_BUTTON2, &CWorkHelpDlg::OnBnClickedButton2)
	ON_BN_CLICKED(IDC_BUTTON3, &CWorkHelpDlg::OnBnClickedButton3)
	ON_BN_CLICKED(IDC_BUTTON4, &CWorkHelpDlg::OnBnClickedButton4)
	ON_BN_CLICKED(IDC_BUTTON5, &CWorkHelpDlg::OnBnClickedButton5)
	ON_BN_CLICKED(IDC_BUTTON6, &CWorkHelpDlg::OnBnClickedButton6)
END_MESSAGE_MAP()


// CWorkHelpDlg 消息处理程序

BOOL CWorkHelpDlg::OnInitDialog()
{
	CDialogEx::OnInitDialog();



	return TRUE;  // 除非将焦点设置到控件，否则返回 TRUE
}

// 如果向对话框添加最小化按钮，则需要下面的代码
//  来绘制该图标。  对于使用文档/视图模型的 MFC 应用程序，
//  这将由框架自动完成。

void CWorkHelpDlg::OnPaint()
{
	if (IsIconic())
	{
		CPaintDC dc(this); // 用于绘制的设备上下文

		SendMessage(WM_ICONERASEBKGND, reinterpret_cast<WPARAM>(dc.GetSafeHdc()), 0);

		// 使图标在工作区矩形中居中
		int cxIcon = GetSystemMetrics(SM_CXICON);
		int cyIcon = GetSystemMetrics(SM_CYICON);
		CRect rect;
		GetClientRect(&rect);
		int x = (rect.Width() - cxIcon + 1) / 2;
		int y = (rect.Height() - cyIcon + 1) / 2;


	}
	else
	{
		CDialogEx::OnPaint();
	}
}

//当用户拖动最小化窗口时系统调用此函数取得光标
//显示。
HCURSOR CWorkHelpDlg::OnQueryDragIcon()
{
	return NULL;
}

#include "../../../com/Com/CMFC.hpp"
#include "../../../com/Com/CFile.hpp"
#include "../../../com/Com/CAnalysisCode.hpp"
void CWorkHelpDlg::OnBnClickedButton1()
{
	string val = GetEditVal(m_hWnd, IDC_EDIT1);
	//SetEditVal(m_hWnd, IDC_EDIT2, "123");
	Buf bf = XReadFile("leigongyong.txt");
	char szOut[2000] = { 0 };
	sprintf(szOut, bf.m_addr, val.c_str(), val.c_str(), val.c_str());
	SetEditVal(m_hWnd, IDC_EDIT2, szOut);
}


void CWorkHelpDlg::OnBnClickedButton2()
{
	string val = GetEditVal(m_hWnd, IDC_EDIT1);
	CProtoPara proto;
	proto.para_with_str(val);

	string ret = proto.general_code();
	SetEditVal(m_hWnd, IDC_EDIT2, ret);
}


void CWorkHelpDlg::OnBnClickedButton3()
{
	string val = GetEditVal(m_hWnd, IDC_EDIT1);
	CProtoPara proto;
	proto.para_with_str(val);

	string ret = proto.general_create_sql();
	SetEditVal(m_hWnd, IDC_EDIT2, ret);
}


void CWorkHelpDlg::OnBnClickedButton4()
{
	string val = GetEditVal(m_hWnd, IDC_EDIT1);
	CProtoPara proto;
	proto.para_with_str(val);

	string ret = proto.general_real_code();
	SetEditVal(m_hWnd, IDC_EDIT2, ret);
}


void CWorkHelpDlg::OnBnClickedButton5()
{ 
	// TODO: Add your control notification handler code here
	/*
	string acc;//账号
	const string& get_acc() { return acc; }
	void set_acc(string val) { acc = val; }
	*/
	string val = GetEditVal(m_hWnd, IDC_EDIT1);
	string ret="";
	int flag=0;
	string l="";
	string r="";
	for (int i = 0; i < val.size(); i++)
	{
		if (val[i] == '\n') 
		{
			flag = 0;
			l = "";
			r = "";
			continue;
		}
		if (val[i] == ' ')
		{
			flag = 1;
			continue;
		}
		if (val[i] == ';') 
		{
			flag = 0;
			ret = ret + "const " + l + "&" + " get_" + r + "()" + " {" + " return " + r+";}\r\n";
			ret = ret + "void set_" + r + "(const " + l + " &val) " + "{ " + r + " = " + "val" + "; }\r\n\r\n";
			l = "";
			r = "";
			continue;
		}
		if (flag == 0) 
		{
			l += val[i];
		}
		else 
		{
			r += val[i];
		}
	}
	
	SetEditVal(m_hWnd, IDC_EDIT2, ret);
}


void CWorkHelpDlg::OnBnClickedButton6()
{
	// TODO: Add your control notification handler code here
	//协议处理
	/*
	int process(ConnectMsg &msg);
	int Client::process(ConnectMsg &msg)
	{
		return RET_SUC;
	}
	SET_PKG_HANDLER(Goddess, ConnectMsg, E_CONNECTED);
	*/
	string val = GetEditVal(m_hWnd, IDC_EDIT1);
	string ret = "";
	string key_word="";
	for (int i = 0; i <= val.size(); i++) 
	{
		if (i == val.size()) 
		{
			ret = ret + "int process(" + key_word + " &msg);\r\n";
			ret = ret + "int Client::process(" + key_word + " &msg)\r\n";
			ret = ret + "{\r\n";
			ret = ret + "\treturn RET_SUC;\r\n";
			ret = ret + "}\r\n";
			ret = ret + "SET_PROTOBUF_HANDLER(Client," + key_word + "," + key_word.substr(0, key_word.size()-4) +");\r\n\r\n";
			key_word = "";
			break;
		}
		if (val[i] == '\n')
		{
			ret = ret + "int process(" + key_word + " &msg);\r\n";
			ret = ret + "int Client::process(" + key_word + " &msg)\r\n";
			ret = ret + "{\r\n";
			ret = ret + "\treturn RET_SUC;\r\n";
			ret = ret + "}\r\n";
			ret = ret + "SET_PROTOBUF_HANDLER(Client," + key_word + "," + key_word.substr(0, key_word.size()-5) +");\r\n\r\n";
			key_word = "";
			continue;
		}
		if (val[i] == ' ') 
		{
			continue;
		}
		key_word += val[i];
	}

	SetEditVal(m_hWnd, IDC_EDIT2, ret);
}
