// libevent_test.cpp : Defines the entry point for the console application.
//

#include "stdafx.h"
#include <event2/event.h>
#include <event2/listener.h>
#include <event2/bufferevent.h>
#include <event2/event_wrap.hpp>
#include "../../../common/XProcess.hpp"
#include <signal.h>

#include <iostream>
#pragma comment(lib,"..\\..\\..\\third\\libevent\\build\\lib\\Debug\\event.lib")
#pragma comment(lib, "ws2_32.lib")


int main(int argc, char* argv[])
{
	xprocess_change_workpath(argv[0]);
#ifdef _WIN32
	WSADATA wsa_data;
	WSAStartup(0x0201, &wsa_data);
#endif
	
	extern void test_timer();
	test_timer();
    return 0;
}

