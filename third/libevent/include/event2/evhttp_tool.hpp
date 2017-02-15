#pragma once
#include <event2/event.h>
#include <event2/http.h>
#include <event2/buffer.h>
#include <event2/http_struct.h>
#include <event2/dns.h>
#include <string>
using namespace std;
class evhttp_wrap
{
public:
	evhttp_wrap()
	{
		base = NULL;
		dnsbase = NULL;
		conn = NULL;
		req = NULL;
		buffer = NULL;
	}
	void init(struct event_base * base)
	{
		this->base = base;
		this->buffer = evbuffer_new();
		this->dnsbase = evdns_base_new(this->base, 1);
	}
	//eg:a2.xgsdk.com
	int init_con(string host, int port=80)
	{
		conn = evhttp_connection_base_new(this->base, this->dnsbase, host.c_str(), port);
		this->host = host;
		return 0;
	}
	//eg:/account/verify-session/17952?authInfo=abc123&ts=20170215161926&type=verify-session
	int make_get_query(string query)
	{
		this->req = evhttp_request_new(evhttp_wrap::download_callback, this);
		evhttp_make_request(this->conn, this->req, EVHTTP_REQ_GET, query.c_str());
		evhttp_add_header(this->req->output_headers, "Host", this->host.c_str());
		return 0;
	}
	virtual void on_http_ok() = 0;
private:
	static void download_callback(struct evhttp_request *req, void *arg)
	{
		evhttp_wrap * ctx = (evhttp_wrap*)arg;
		const char * new_location = 0;
		if (!req) {
			printf("timeout\n");
			return;
		}

		switch (req->response_code)
		{
		case HTTP_OK:
			evbuffer_add_buffer(ctx->buffer, req->input_buffer);
			ctx->on_http_ok();
			break;
		default:/* failed */
			return;
		}
	}
public:
	struct event_base * base;
	struct evdns_base * dnsbase;
	struct evhttp_connection * conn;
	struct evhttp_request *req;
	struct evbuffer *buffer;
	string host;
};