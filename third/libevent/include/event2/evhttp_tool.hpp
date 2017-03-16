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
	}
	~evhttp_wrap()
	{
		clean();
	}
	void init(struct event_base * base)
	{
		this->base = base;
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
private:
	static void download_callback(struct evhttp_request *req, void *arg)
	{
		if (!arg) {
			printf("arg NULL\n");
			return;
		}
		if (!req) {
			printf("req NULL\n");
			return;
		}
		evhttp_wrap * ctx = (evhttp_wrap*)arg;
		ctx->on_http_ok(req);
	}
public:
	struct event_base * base;
	struct evdns_base * dnsbase;
	struct evhttp_connection * conn;
	struct evhttp_request *req;
	string host;
};