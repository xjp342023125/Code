// TestNet.cpp : Defines the entry point for the console application.
//

#include "stdafx.h"
#include "../../../common/XSock.hpp"


CInitNet g_win_init_net;
int _tmain(int argc, _TCHAR* argv[])
{
	XSock sock;
	sock.TCPCreate();

	CAddr addr;
	addr.InitAddr("127.0.0.1", 1313);
	sock.Connect(addr);
	cout << SOCK_ERR << endl;
	sock.SetNoDelay();
	sock.SetRecvTimeOut(3000);
	char sz[20];
	sock.TCPRecv(sz, 20);
	cout << SOCK_ERR << endl;

	cout << "over";
	return 0;

}

