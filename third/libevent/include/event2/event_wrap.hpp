#pragma once
#include <event2/event.h>

class event_base_wrap
{
public:
	event_base_wrap()
	{
		base = NULL;
	}

	bool init()
	{
		base = event_base_new();
		return NULL != base;
	}

	void async_loop()
	{
		event_base_loop(base, EVLOOP_NONBLOCK);
	}
public:
	struct event_base *base;
};