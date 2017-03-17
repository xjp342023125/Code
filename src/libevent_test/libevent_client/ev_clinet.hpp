#pragma once
#include <event2/event.h>
#include <event2/listener.h>
#include <event2/bufferevent.h>
#include <event2/event_wrap.hpp>
#include <iostream>
using namespace std;
#pragma	pack(1)
#pragma warning(disable: 4200)
struct f_msg
{
	int sum_len;
	int nType;
	char sz[];
};

struct up_log
{
	char szpath[256];
	char szname[100];
	int len;
	char sz[];
};

struct up_log_ret
{
	int ret;
};

#pragma warning(default: 4200)
#pragma pack()

class ev_filecon
{
public:
	void on_close()
	{

	}
	void on_msg(f_msg &msg)
	{
		
	}
public:
	static void read_cb(struct bufferevent *bev, void *ctx)
	{
		
		ev_filecon *p_this = (ev_filecon*)ctx;
		evbuffer *in = p_this->bev.get_input();

		buffer_wrap bw; 
		bw.buffer = in;
		while (true)
		{
			if (bw.get_len() < sizeof(f_msg))//pkg header not enough
			{
				break;
			}
			f_msg *msg = (f_msg*)bw.get_buffer();
			if (msg->sum_len < sizeof(f_msg))//pkg header len,err
			{
				p_this->on_close();
				p_this->bev.free();
				delete p_this;
				return;
			}
			if (bw.get_len() < msg->sum_len)//data not enough
			{
				break;
			}
			p_this->on_msg(*msg);
			bw.remove(msg->sum_len);
		}
	}
	static void write_cb(struct bufferevent *bev, void *ctx)
	{

	}
	static void event_cb(struct bufferevent *bev, short what, void *ctx)
	{
		ev_filecon *p_this = (ev_filecon*)ctx;

		if (buffer_event_wrap::is_event(what, BEV_EVENT_EOF))
		{
			p_this->on_close();
			return;
		}
	}
public:
	buffer_event_wrap bev;
};
