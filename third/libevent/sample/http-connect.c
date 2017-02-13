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
		event_base_loopexit(ctx->base, 0);
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
		event_base_loopexit(ctx->base, 0);
		return;
	}

	evbuffer_add_buffer(ctx->buffer, req->input_buffer);
	ctx->ok = 1;
}

struct download_context * context_new(const char *url)
{
	struct download_context * ctx = 0;
	ctx = (struct download_context*)calloc(1, sizeof(struct download_context));
	ctx->uri = evhttp_uri_parse(url);
	if (!ctx->uri) return 0;

	ctx->base = event_base_new();
	ctx->buffer = evbuffer_new();
	ctx->dnsbase = evdns_base_new(ctx->base, 1);

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
	int port = evhttp_uri_get_port(ctx->uri);
	if (port == -1) port = 80;
	if (ctx->conn) evhttp_connection_free(ctx->conn);

	printf("host:%s, port:%d, path:%s\n", evhttp_uri_get_host(ctx->uri), port, evhttp_uri_get_path(ctx->uri));

	ctx->conn = evhttp_connection_base_new(ctx->base, ctx->dnsbase, evhttp_uri_get_host(ctx->uri), port);
	ctx->req = evhttp_request_new(download_callback, ctx);
	evhttp_make_request(ctx->conn, ctx->req, EVHTTP_REQ_GET, evhttp_uri_get_path(ctx->uri));
	evhttp_add_header(ctx->req->output_headers, "Host", evhttp_uri_get_host(ctx->uri));

	return 0;
}

struct evbuffer *download_url(const char *url)
{
	struct download_context * ctx = context_new(url);
	if (!ctx) return 0;

	event_base_dispatch(ctx->base);

	struct evbuffer * retval = 0;
	if (ctx->ok)
	{
		retval = ctx->buffer;
		ctx->buffer = 0;
	}

	context_free(ctx);
	return retval;
}

int main(int argc, char **argv)
{
	struct evbuffer * data = 0;


#ifdef WIN32
	WORD wVersionRequested;
	WSADATA wsaData;

	wVersionRequested = MAKEWORD(2, 2);

	(void)WSAStartup(wVersionRequested, &wsaData);
#endif

	data = download_url("http://example.com/");

	printf("got %d bytes\n", data ? evbuffer_get_length(data) : -1);

	if (data)
	{
		const unsigned char * joined = evbuffer_pullup(data, -1);
		printf("data itself:\n====================\n");
		fwrite(joined, evbuffer_get_length(data), 1, stderr);
		printf("\n====================\n");
		evbuffer_free(data);
	}

	return 0;
}