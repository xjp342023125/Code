#pragma once
#include <event2/event.h>
#include <event2/listener.h>
#include <event2/bufferevent.h>
#include <event2/event_wrap.hpp>
#include <iostream>
#include "../hhdr.h"
#include "../../../common/XFile.hpp"
#include "../../../common/XLog.hpp"
#include "../../../common/XProcess.hpp"

using namespace std;

#define USE_LOG
#ifdef USE_LOG
extern CLog gSqlLog;
static void init_mysql_log()
{
	gSqlLog.InitDevice("fs");
}

#define HINFO	gSqlLog.l()
#define HERROR	gSqlLog.l()<<"Error,Line"<<__LINE__<<","
#else
#define HINFO	gSqlLog()
#define HERROR	gSqlLog()
#endif

class ev_filecon
{
public:
	void on_close()
	{
		HINFO << "disconnect" ;
	}
	void on_msg(f_msg &msg)
	{
		up_log *log_info = (up_log*)msg.sz;
		if (msg.sum_len != sizeof(f_msg) + sizeof(up_log) + log_info->len)
		{
			HINFO << "err" ;
			return;
		}
		HINFO << log_info->szpath << "  ,name=" << log_info->szname ;
		if (strstr(log_info->szpath,".."))
		{
			HERROR << "error path  " << log_info->szpath;
			return;
		}
		xpath_convert(log_info->szpath);
		string path = log_info->szpath;
		if (path[path.size() -1 ] != FILE_SEPARATOR)
		{
			path += FILE_SEPARATOR;
		}
		HINFO<<xfoder_createx(path.c_str());
		path += log_info->szname;
		HINFO<<xfile_write_new(path.c_str() , log_info->sz, log_info->len);
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
		HERROR << "event =" << what;
		
		if (buffer_event_wrap::is_event(what, BEV_EVENT_EOF) 
			|| buffer_event_wrap::is_event(what, BEV_EVENT_ERROR))
		{
			p_this->on_close();
			return;
		}
	}
public:
	buffer_event_wrap bev;
};

class ev_fileserver
{
public:
	bool init(const char *path, const char *ip, short port, event_base *base)
	{
		return listener.init(base, ev_fileserver::cb, this, ip, port);
	}
public:
	static void cb(struct evconnlistener *, evutil_socket_t fd, struct sockaddr *, int socklen, void *p)
	{
		ev_filecon *fc = new ev_filecon;
		if (NULL == fc)
		{
			return;
		}
		ev_fileserver *fs = (ev_fileserver*)p;
		fc->bev.init_with_exist_sock(fs->listener.get_base(), fd);
		fc->bev.set_cb(ev_filecon::read_cb, ev_filecon::write_cb, ev_filecon::event_cb, fc);
		fc->bev.enable_event(EV_READ);
		fc->bev.enable_event(EV_WRITE);
		HINFO << "new" ;
	}
	listen_event_wrap listener;
};