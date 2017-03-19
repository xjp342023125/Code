// libevent_test.cpp : Defines the entry point for the console application.
//

#include "stdafx.h"
#include "ev_server.hpp"
#pragma comment(lib,"..\\..\\..\\third\\libevent\\build\\lib\\Debug\\event.lib")
#pragma comment(lib, "ws2_32.lib")

CLog gSqlLog;

int main()
{
#ifdef _WIN32
	WSADATA wsa_data;
	WSAStartup(0x0201, &wsa_data);
#endif
	init_mysql_log();
	event_base_wrap event;
	event.init();

	ev_fileserver fs;
	fs.init("", "0.0.0.0", 5566, event.base);
	while (true)
	{
		event.async_loop();
		Sleep(1000);
	}
	
    return 0;
}

