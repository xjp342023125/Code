// libevent_test.cpp : Defines the entry point for the console application.
//

#include "stdafx.h"
#include "ev_server.hpp"
#pragma comment(lib,"..\\..\\..\\third\\libevent\\build\\lib\\Debug\\event.lib")
#pragma comment(lib, "ws2_32.lib")

int main()
{
#ifdef _WIN32
	WSADATA wsa_data;
	WSAStartup(0x0201, &wsa_data);
#endif

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
