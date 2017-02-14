#include <stdlib.h>
#include <string.h>
#include "event2/event.h"
#include "event2/http.h"
#include "event2/buffer.h"
#include "event2/http_struct.h"
#include "event2/dns.h"
#include <string>
#include "time.h"
using namespace std;
#define AUTH_STR "eyJhdXRoVG9rZW4iOiJhdXRoVG9rZW4iLCJjaGFubmVsSWQiOiJtaSIsImRldmljZUlkIjoiZGV2aWNlSWQiLCJuYW1lIjoibmFtZSIsInBsYW5JZCI6IjEiLCJ4Z0FwcElkIjoiMjAxOCIsInNpZ24iOiJmYTM0MzgxZGM1ODRmNjMxYTg3YTA0MzZlNDllZjRkM2E3MWVlNTVkIiwidHMiOiIyMDE1MDcyMzE1MDAyOCIsInVpZCI6InVpZCJ9"

struct download_context {
	struct evhttp_uri * uri;
	struct event_base * base;
	struct evdns_base * dnsbase;
	struct evhttp_connection * conn;
	struct evhttp_request *req;
	struct evbuffer *buffer;
	int ok;
};
static int download_renew_request(struct download_context *ctx, const char *url);

bool get_tm(time_t in, tm &out)
{
	memset(&out, 0, sizeof(out));
	tm *temp = localtime(&in);
	if (nullptr == temp)
	{
		return false;
	}
	out = *temp;
	return true;
}
string get_xgsdk_str(string appid, string auth)
{
	string ret = "http://a2.xgsdk.com/account/verify-session/" + appid;
	ret += "?authInfo=" + auth;

	tm t;
	get_tm(time(NULL), t);
	char pwszBuf[50];
	sprintf(pwszBuf, "%i%2.2i%2.2i%2.2i%2.2i%2.2i",
		t.tm_year + 1900, t.tm_mon + 1, t.tm_mday, t.tm_hour, t.tm_min, t.tm_sec);

	ret += string("&ts=") + pwszBuf;
	ret += "&type=verify-session";
	return ret;
}

static void on_http_ok(struct download_context * ctx)
{
	struct evbuffer * data = 0;
	data = ctx->buffer;
	printf("got %d bytes\n", data ? evbuffer_get_length(data) : -1);

	if (data)
	{
		const unsigned char * joined = evbuffer_pullup(data, -1);
		printf("data itself:\n====================\n");
		fwrite(joined, evbuffer_get_length(data), 1, stderr);
		printf("\n====================\n");
	}
	evbuffer_drain(data, evbuffer_get_length(data));

	download_renew_request(ctx, get_xgsdk_str("17952", AUTH_STR).c_str());
}
static void download_callback(struct evhttp_request *req, void *arg)
{
	struct download_context * ctx = (struct download_context*)arg;
	struct evhttp_uri * new_uri = 0;
	const char * new_location = 0;
	if (!req) {
		printf("timeout\n");
		return;
	}

	switch (req->response_code)
	{
	case HTTP_OK:
		evbuffer_add_buffer(ctx->buffer, req->input_buffer);
		on_http_ok(ctx);
		//event_base_loopexit(ctx->base, 0);
		break;
	default:/* failed */
		//event_base_loopexit(ctx->base, 0);
		return;
	}

	
	ctx->ok = 1;
}

struct download_context * context_new( struct event_base * base)
{
	struct download_context * ctx = 0;
	ctx = (struct download_context*)calloc(1, sizeof(struct download_context));

	ctx->base = base;
	ctx->buffer = evbuffer_new();
	ctx->dnsbase = evdns_base_new(ctx->base, 1);
	ctx->ok = 0;
	return ctx;
}

void context_free(struct download_context *ctx)
{
	if (ctx->conn)
		evhttp_connection_free(ctx->conn);

	if (ctx->buffer)
		evbuffer_free(ctx->buffer);

	if (ctx->uri)
		evhttp_uri_free(ctx->uri);

	free(ctx);
}

static int download_renew_request(struct download_context *ctx, const char *url)
{
	if (ctx->uri)
	{
		evhttp_uri_free(ctx->uri);
		ctx->uri = NULL;
	}
	ctx->uri = evhttp_uri_parse(url);
	if (!ctx->uri)
		return 0;
	char path_query[1000] = {0};
	int port = evhttp_uri_get_port(ctx->uri);
	if (port == -1) port = 80;
	if (ctx->conn) evhttp_connection_free(ctx->conn);

	//printf("host:%s, port:%d, path:%s,query:%s\n", 
	//	evhttp_uri_get_host(ctx->uri), 
	//	port, 
	//	evhttp_uri_get_path(ctx->uri),
	//	evhttp_uri_get_query(ctx->uri));


	ctx->conn = evhttp_connection_base_new(ctx->base, ctx->dnsbase, evhttp_uri_get_host(ctx->uri), port);
	ctx->req = evhttp_request_new(download_callback, ctx);
	
	if (evhttp_uri_get_query(ctx->uri))
	{
		sprintf(path_query, "%s?%s", evhttp_uri_get_path(ctx->uri), evhttp_uri_get_query(ctx->uri));
		evhttp_make_request(ctx->conn, ctx->req, EVHTTP_REQ_GET, path_query);
	}
	else
	{
		evhttp_make_request(ctx->conn, ctx->req, EVHTTP_REQ_GET, evhttp_uri_get_path(ctx->uri));
	}
	
	evhttp_add_header(ctx->req->output_headers, "Host", evhttp_uri_get_host(ctx->uri));

	return 0;
}

int main(int argc, char **argv)
{
	get_xgsdk_str("17952","abcd");
	struct event_base * base = NULL;
	download_context *ctx = NULL;

#ifdef WIN32
	WORD wVersionRequested;
	WSADATA wsaData;

	wVersionRequested = MAKEWORD(2, 2);

	(void)WSAStartup(wVersionRequested, &wsaData);
#endif
	base = event_base_new();


	ctx = context_new(base);
	download_renew_request(ctx, get_xgsdk_str("17952", AUTH_STR).c_str());
	
	while (1)
	{
		int ret = event_base_loop(base, EVLOOP_NONBLOCK);
		//printf("event_base_loop=%d\n", ret);
		//Sleep(100);
	}


	return 0;
}