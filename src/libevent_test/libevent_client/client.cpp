// libevent_test.cpp : Defines the entry point for the console application.
//

#include "stdafx.h"
#include "ev_clinet.hpp"
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

	while (true)
	{
		event.async_loop();
		Sleep(1000);
	}
	
    return 0;
}

