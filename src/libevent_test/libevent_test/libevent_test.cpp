// libevent_test.cpp : Defines the entry point for the console application.
//

#include "stdafx.h"
#include "event2\event_wrap.hpp"
#pragma comment(lib,"..\\..\\..\\third\\libevent\\build\\lib\\Debug\\event.lib")
int main()
{
	event_base_wrap event;
	event.init();
	event.async_loop();
    return 0;
}

