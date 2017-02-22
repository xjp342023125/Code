#pragma once
#include <string>

#include <event2/event.h>
#include <event2/buffer.h>
#include <event2/http.h>

#define CHECK_RETURN(exp)\
	if(!(exp))\
	{\
		return;\
	}

#define CHECK_RETURNV(exp,ret)\
	if(!(exp))\
	{\
		return ret;\
	}\

using namespace std;
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
		CHECK_RETURNV(base, -1);
		http_server = evhttp_new(base);
		CHECK_RETURNV(http_server, -1);

		int ret = evhttp_bind_socket(http_server, ip.c_str(), port);
		CHECK_RETURNV(0 == ret, -3);

		evhttp_set_gencb(http_server, generic_handler, this);


		return 0;
	}
	
	void web_exit()
	{
		evhttp_free(http_server);
	}


	virtual void do_req(struct evhttp_request &req, struct evbuffer &buf) = 0;
private:
	static void generic_handler(struct evhttp_request *req, void *arg)
	{
		evhttp_server_wrap *pthis = (evhttp_server_wrap*)arg;
		struct evbuffer *buf = evbuffer_new();
		CHECK_RETURN(req);
		CHECK_RETURN(arg);
		CHECK_RETURN(pthis);
		CHECK_RETURN(buf);

		pthis->do_req(*req, *buf);

		evhttp_send_reply(req, HTTP_OK, "OK", buf);
		evbuffer_free(buf);
	}
private:
	struct evhttp *http_server;
};