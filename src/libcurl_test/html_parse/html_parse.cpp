// html_parse.cpp : 定义控制台应用程序的入口点。
//

#include "stdafx.h"

#include "../../../com/Com/CHttpDown.hpp"
//#include "../../../com/Com/CFunc.hpp"
#include "../../../com/Com/CFile.hpp"
#pragma comment ( lib,"../../../third/out/lib/libcurl.lib" )
#include <sstream>
using namespace std;

void test_dowm()
{
	CurlDMem dm;
	dm.Init();
	dm.curl_handle.set_url("https://github.com/curl/curl");
	dm.curl_handle.set_debug_out(1);
	dm.CurlPerform();
	XWriteFile("out.html", dm.bf.m_addr, dm.bf.m_nLen);
}
int _tmain(int argc, _TCHAR* argv[])
{
	test_dowm();
	return 0;

}

