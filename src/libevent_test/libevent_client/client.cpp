// libevent_test.cpp : Defines the entry point for the console application.
//

#include "stdafx.h"
#include "ev_clinet.hpp"
#include "guard_path.hpp"

#pragma comment(lib,"..\\..\\..\\third\\libevent\\build\\lib\\Debug\\event.lib")
#pragma comment(lib, "ws2_32.lib")

event_base_wrap g_event;
int main(int argc, char* argv[])
{

	xprocess_change_workpath(argv[0]);
	//xfile_test();

#ifdef _WIN32
	WSADATA wsa_data;
	WSAStartup(0x0201, &wsa_data);
#endif
	g_event.init();

	guard_path guard;
	guard.init(".\\realm");

	while (true)
	{
		g_event.async_loop();
		guard.run();
		Sleep(1000);
	}
	
    return 0;
}

