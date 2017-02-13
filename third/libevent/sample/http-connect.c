#include <stdlib.h>
#include <string.h>
#include "event2/event.h"
#include "event2/http.h"
#include "event2/buffer.h"
#include "event2/http_struct.h"
#include "event2/dns.h"


struct download_context {
	struct evhttp_uri * uri;
	struct event_base * base;
	struct evdns_base * dnsbase;
	struct evhttp_connection * conn;
	struct evhttp_request *req;
	struct evbuffer *buffer;
	int ok;
};

static void download_callback(struct evhttp_request *req, void *arg);
static int download_renew_request(struct download_context *ctx);

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
		evbuffer_free(data);
	}
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
	case HTTP_MOVEPERM:
	case HTTP_MOVETEMP:
		new_location = evhttp_find_header(req->input_headers, "Location");
		if (!new_location) return;
		new_uri = evhttp_uri_parse(new_location);
		if (!new_uri)return;
		evhttp_uri_free(ctx->uri);
		ctx->uri = new_uri;
		download_renew_request(ctx);
		return;
	default:/* failed */
		//event_base_loopexit(ctx->base, 0);
		return;
	}

	
	ctx->ok = 1;
}

struct download_context * context_new(const char *url, struct event_base * base)
{
	struct download_context * ctx = 0;
	ctx = (struct download_context*)calloc(1, sizeof(struct download_context));
	ctx->uri = evhttp_uri_parse(url);
	if (!ctx->uri) return 0;

	ctx->base = base;
	ctx->buffer = evbuffer_new();
	ctx->dnsbase = evdns_base_new(ctx->base, 1);
	ctx->ok = 0;

	download_renew_request(ctx);
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

static int download_renew_request(struct download_context *ctx)
{
	char path_query[1000] = {0};
	int port = evhttp_uri_get_port(ctx->uri);
	if (port == -1) port = 80;
	if (ctx->conn) evhttp_connection_free(ctx->conn);

	printf("host:%s, port:%d, path:%s,query:%s\n", 
		evhttp_uri_get_host(ctx->uri), 
		port, 
		evhttp_uri_get_path(ctx->uri),
		evhttp_uri_get_query(ctx->uri));


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
	
	struct event_base * base = NULL;
	struct download_context *ctx1 = NULL;
	struct download_context *ctx2 = NULL;

#ifdef WIN32
	WORD wVersionRequested;
	WSADATA wsaData;

	wVersionRequested = MAKEWORD(2, 2);

	(void)WSAStartup(wVersionRequested, &wsaData);
#endif
	base = event_base_new();
	ctx1 = context_new("http://example.com/", base);
	ctx2 = context_new("http://ip.taobao.com/service/getIpInfo.php?ip=218.24.136.211", base);

	event_base_dispatch(base);


	return 0;
}