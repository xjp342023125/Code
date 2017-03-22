#pragma once
#include <event2/event.h>
#include <event2/listener.h>
#include <event2/buffer.h>
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
	static void set_log_cb(event_log_cb func)
	{
		event_set_log_callback(func);
	}

	static void enable_debug_log()
	{
		event_enable_debug_logging(1);
	}

public:
	struct event_base *base;
};



class buffer_wrap
{
public:
	buffer_wrap()
	{
		buffer = NULL;
	}
	void free()
	{
		evbuffer_free(buffer);
	}
	size_t get_len()
	{
		return evbuffer_get_length(buffer);
	}
	unsigned char* get_buffer(ev_ssize_t size = -1)
	{
		return evbuffer_pullup(buffer, size);
	}
	int remove(size_t len)
	{
		return evbuffer_drain(buffer, len);
	}

	evbuffer *buffer;
};


class buffer_event_wrap
{
public:
	friend class listen_event_wrap;
public:
	buffer_event_wrap()
	{
		bev = NULL;
	}
	~buffer_event_wrap()
	{
		free();
	}
	void free() {
		if (bev)
		{
			bufferevent_free(bev);
			bev = NULL;
		}
	}

	evbuffer* get_output() { return bufferevent_get_output(bev); }
	evbuffer* get_input() { return bufferevent_get_input(bev); }
public:
	void enable_event(int flag = EV_READ)
	{
		bufferevent_enable(bev, flag);
	}
	void disable_event(int flag = EV_READ)
	{
		bufferevent_disable(bev, flag);
	}
	//static void read_cb(struct bufferevent *bev, void *ctx)
	//static void write_cb(struct bufferevent *bev, void *ctx)
	//static void event_cb(struct bufferevent *bev, short what, void *ctx)
	void set_cb(bufferevent_data_cb readcb, bufferevent_data_cb writecb,bufferevent_event_cb eventcb, void *cbarg)
	{
		bufferevent_setcb(bev, readcb, writecb, eventcb, cbarg);
	}
	bool init_with_exist_sock(event_base *base, evutil_socket_t fd, int flag = BEV_OPT_CLOSE_ON_FREE)
	{
		bev = bufferevent_socket_new(base, fd, flag);
		return bev != NULL;
	}
	
	//struct bufferevent *bufferevent_socket_new(struct event_base *base, evutil_socket_t fd, int options);
	bool create(event_base *base, int options=BEV_OPT_CLOSE_ON_FREE)
	{
		bev = bufferevent_socket_new(base, -1, options);
		return NULL != bev;
	}
	int connect(const char *ip, short port)
	{
		sockaddr_in sa = convert_to_kenerl_addr(ip, port);
		return bufferevent_socket_connect(bev, (sockaddr*)&sa, sizeof(sockaddr_in));
	}

	int write(const void *data, size_t size)
	{
		return bufferevent_write(bev,data,size);
	}
public:
	static bool is_event(int event, int check = BEV_EVENT_EOF)
	{
		return event & check;
	}
public:
	bufferevent *bev;
};

class listen_event_wrap
{
public:
	listen_event_wrap()
	{
		ev = NULL;
	}
public:
	//static void listen_cb(struct evconnlistener *, evutil_socket_t fd, struct sockaddr *, int socklen, void *p)
	bool init(event_base *base, evconnlistener_cb cb, void *ptr,const char *ip,short port, 
		int flag= LEV_OPT_CLOSE_ON_FREE| LEV_OPT_REUSEABLE, int backlog = -1)
	{
		sockaddr_in sa = convert_to_kenerl_addr(ip,port);
		ev = evconnlistener_new_bind(base, cb, ptr, flag, backlog, (sockaddr*)&sa, sizeof(sockaddr_in));
		return NULL != ev;
	}
	event_base* get_base()
	{
		return evconnlistener_get_base(ev);
	}
	
public:
	struct evconnlistener *ev;
};


