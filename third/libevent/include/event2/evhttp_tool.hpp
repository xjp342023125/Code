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
		dnsbase = NULL;
		conn = NULL;
	}
	~evhttp_wrap()
	{
		clean();
	}
	//eg:a2.xgsdk.com
	bool init_con(struct event_base * base,string host, int port=80)
	{
		this->dnsbase = evdns_base_new(base, 1);
		conn = evhttp_connection_base_new(base, this->dnsbase, host.c_str(), port);
		this->host = host;
		return NULL != conn;
	}
	//eg:/account/verify-session/17952?authInfo=abc123&ts=20170215161926&type=verify-session
	int make_get_query(string query)
	{
		struct evhttp_request *req = evhttp_request_new(evhttp_wrap::download_callback, this);
		if (NULL == req)
		{
			return -1;
		}
		if (0 != evhttp_make_request(this->conn, req, EVHTTP_REQ_GET, query.c_str()))
		{
			return -2;
		}
		if (0 != evhttp_add_header(req->output_headers, "Host", this->host.c_str()))
		{
			return -3;
		}
		evhttp_request_set_error_cb(req, evhttp_wrap::error_cb);
		return 0;
	}
	void clean()
	{
		if (this->dnsbase)
		{
			evdns_base_free(this->dnsbase,0);
			this->dnsbase = NULL;
		}
		if (this->conn)
		{
			evhttp_connection_free(this->conn);
			this->conn = NULL;
		}
	}
	virtual void on_http_ok(struct evhttp_request *req) = 0;
	virtual void on_error(evhttp_request_error err)
	{

	};
private:
	static void download_callback(struct evhttp_request *req, void *arg)
	{
		if (!arg) {
			printf("arg NULL\n");
			return;
		}

		evhttp_wrap * ctx = (evhttp_wrap*)arg;
		ctx->on_http_ok(req);
	}

	static void error_cb(enum evhttp_request_error err, void *p)
	{
		if (!p) {
			printf("arg NULL\n");
			return;
		}

		evhttp_wrap * ctx = (evhttp_wrap*)p;
		ctx->on_error(err);
	}
public:
	struct evdns_base * dnsbase;
	struct evhttp_connection * conn;
	string host;
};