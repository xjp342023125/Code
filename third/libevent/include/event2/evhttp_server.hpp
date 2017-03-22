#pragma once
#include <string>

#include <event2/event.h>
#include <event2/buffer.h>
#include <event2/http.h>

#define EV_CHECK_RETURN(exp)\
	if(!(exp))\
	{\
		return;\
	}

#define EV_CHECK_RETURNV(exp,ret)\
	if(!(exp))\
	{\
		return ret;\
	}\

using namespace std;


struct http_ret
{
	http_ret()
	{
		code = HTTP_OK;
	}
	int code;
	string ret;
};
class evhttp_server_wrap 
{
public:
	evhttp_server_wrap()
	{
		http_server = NULL;
	}

	virtual ~evhttp_server_wrap()
	{
	}
	
	int init_web(string ip, int port, event_base* base)
	{
		EV_CHECK_RETURNV(base, -1);
		http_server = evhttp_new(base);
		EV_CHECK_RETURNV(http_server, -1);

		int ret = evhttp_bind_socket(http_server, ip.c_str(), port);
		EV_CHECK_RETURNV(0 == ret, -3);

		evhttp_set_gencb(http_server, generic_handler, this);


		return 0;
	}
	
	void web_exit()
	{
		evhttp_free(http_server);
	}


	virtual http_ret do_req(struct evhttp_request &req, struct evbuffer &buf) = 0;
private:
	static void generic_handler(struct evhttp_request *req, void *arg)
	{
		evhttp_server_wrap *pthis = (evhttp_server_wrap*)arg;
		struct evbuffer *buf = evbuffer_new();
		EV_CHECK_RETURN(req);
		EV_CHECK_RETURN(arg);
		EV_CHECK_RETURN(pthis);
		EV_CHECK_RETURN(buf);

		http_ret ret = pthis->do_req(*req, *buf);

		evbuffer_add(buf, ret.ret.c_str(), ret.ret.size());
		evhttp_send_reply(req, ret.code,"OK", buf);
		evbuffer_free(buf);
	}
private:
	struct evhttp *http_server;
};