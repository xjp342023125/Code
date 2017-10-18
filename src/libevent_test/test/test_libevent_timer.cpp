#include "stdafx.h"
#include <event2/event.h>
#include <event2/listener.h>
#include <event2/bufferevent.h>
#include <event2/event_wrap.hpp>
#include "../../../common/XProcess.hpp"
#include <signal.h>


// 定时事件回调函数   
void onTime(int sock, short event, void *arg)
{
	printf("Hello,World!\n");

	struct timeval tv;
	tv.tv_sec = 1;
	tv.tv_usec = 0;
	struct event* ev = (struct event*)arg;
	// 重新添加定时事件（定时事件触发后默认自动删除）   
	//event_add((struct event*)arg, &tv);
}

void test_timer()
{
	event_base_wrap event_base;
	event_base.init();


	//event *timer = evtimer_new(event_base.base,onTime, event_self_cbarg());
	event *timer = event_new(event_base.base, -1, EV_PERSIST, onTime, event_self_cbarg());
	struct timeval one_sec = { 1, 0 };
	event_add(timer, &one_sec);
	while (true)
	{
		event_base.async_loop();
		Sleep(1000);
	}
}


void onEvent(int sock, short event, void *arg)
{
	printf("onEvent %d\n", event);
}
void test_event()
{
	event_base_wrap event_base;
	event_base.init();

	event *ev = event_new(event_base.base, -1, 0, onEvent, event_self_cbarg());
	auto ret = event_add(ev, nullptr);

	while (true)
	{
		event_base.async_loop();
		event_active(ev, 1, 100);
		Sleep(1000);
	}

}