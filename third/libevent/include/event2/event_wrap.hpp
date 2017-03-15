#pragma once
#include <event2/event.h>
#include <event2/listener.h>
#include <event2/bufferevent.h>
#pragma comment(lib, "ws2_32.lib")

static sockaddr_in convert_to_kenerl_addr(const char *ip, short port)
{
	sockaddr_in addr;
	memset(&addr, 0, sizeof(sockaddr_in));
	addr.sin_family = AF_INET;
	addr.sin_port = htons(port);
	addr.sin_addr.s_addr = inet_addr(ip);
	return addr;
}

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


class buffer_event_wrap;
typedef void (*do_buffer_event_wrap_func)(buffer_event_wrap *,void *);
class buffer_event_wrap
{
public:
	friend class listen_event_wrap;
public:
	buffer_event_wrap()
	{
		bev = NULL;
		para = NULL;
		events = 0;
	}


	evbuffer* get_output()	{return bufferevent_get_output(bev);}
	evbuffer* get_input()	{return bufferevent_get_input(bev);	}
	short get_events()		{return events;	}
	void free()				{bufferevent_free(bev);	}
private:
	static void	on_write(struct bufferevent *bev, void *user_data)
	{
		buffer_event_wrap *p = (buffer_event_wrap*)user_data;
		p->write_cb(p, p->para);
	}
	static void	on_read(struct bufferevent *bev, void *user_data)
	{
		buffer_event_wrap *p = (buffer_event_wrap*)user_data;
		p->read_cb(p, p->para);
	}
	static void	on_event(struct bufferevent *bev, short events, void *user_data)
	{
		buffer_event_wrap *p = (buffer_event_wrap*)user_data;
		p->events = events;
		p->event_cb(p, p->para);
	}

	int init_with_sock(event_base *base, evutil_socket_t fd, int flag = BEV_OPT_CLOSE_ON_FREE)
	{
		bev = bufferevent_socket_new(base, fd, flag);
		bufferevent_setcb(bev, on_read, on_write, on_event, this);

		bufferevent_enable(bev, EV_WRITE);
		bufferevent_enable(bev, EV_READ);
		return 0;
	}
public:
	bufferevent *bev;

	do_buffer_event_wrap_func read_cb;
	do_buffer_event_wrap_func write_cb;
	do_buffer_event_wrap_func event_cb;

	void *para;
	short events;
};

class listen_event_wrap
{
public:
	listen_event_wrap()
	{
		ev = NULL;
	}
public:
	int init(event_base *base,const char *ip,short port, int flag= LEV_OPT_CLOSE_ON_FREE| LEV_OPT_REUSEABLE, int backlog = -1)
	{
		sockaddr_in sa = convert_to_kenerl_addr(ip,port);
		ev = evconnlistener_new_bind(base, listen_cb, (void*)this, flag, backlog, (sockaddr*)&sa, sizeof(sockaddr_in));
		return 0;
	}
private:
	static void listen_cb(struct evconnlistener *, evutil_socket_t sock, struct sockaddr *, int socklen, void *user)
	{
		listen_event_wrap *p = (listen_event_wrap*)user;
		buffer_event_wrap *p_con = new buffer_event_wrap;
		p_con->init_with_sock(evconnlistener_get_base(p->ev), sock);

		p->on_con(p_con);
	}
	virtual void on_con(buffer_event_wrap *p)
	{

	}
	
public:
	struct evconnlistener *ev;
};


