#pragma once
#include <event2/event.h>
#include <event2/listener.h>
#include <event2/bufferevent.h>
#include <event2/event_wrap.hpp>
#include <iostream>
#include "../hhdr.h"
#include "../../../common/XFile.hpp"

#include "../../../common/XProcess.hpp"

using namespace std;





class ev_filecon
{
public:
	void on_close()
	{

	}
	void on_msg(f_msg &msg)
	{
		
	}
	void send_msg(const char *path, const char *name, int len, char *p)
	{
		f_msg hdr;
		hdr.sum_len = sizeof(f_msg) + sizeof(up_log) + len;
		hdr.nType = 1;

		up_log log;
		strcpy(log.szpath, path);
		strcpy(log.szname, name);
		log.len = len;

		bev.write(&hdr, sizeof(hdr));
		bev.write(&log, sizeof(log));
		bev.write(p, len);
	}
	int connect(const char *ip, short port)
	{
		bev.connect(ip, port);
		bev.set_cb(ev_filecon::read_cb, ev_filecon::write_cb, ev_filecon::event_cb, this);
		bev.enable_event(EV_READ);
		bev.enable_event(EV_WRITE);
		
		return 0;
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

extern event_base_wrap g_event;
static ev_filecon* create_ev_filecon()
{
	ev_filecon *pf = new ev_filecon;
	pf->bev.create(g_event.base);
	pf->connect("127.0.0.1", 5566);
	return pf;
}