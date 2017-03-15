// libevent_test.cpp : Defines the entry point for the console application.
//

#include "stdafx.h"
#include "event2\event_wrap.hpp"
#pragma comment(lib,"..\\..\\..\\third\\libevent\\build\\lib\\Debug\\event.lib")
int main()
{
#ifdef _WIN32
	WSADATA wsa_data;
	WSAStartup(0x0201, &wsa_data);
#endif

	event_base_wrap event;
	event.init();

	listen_event_wrap listener;
	listener.init(event.base, "0.0.0.0", 5566);

	while (true)
	{
		event.async_loop();
		Sleep(1000);
	}
	
    return 0;
}

