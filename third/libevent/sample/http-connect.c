#include <stdlib.h>
#include <string.h>
#include "event2/evhttp_tool.hpp"

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
	string ret = "/account/verify-session/" + appid;
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



class evhttp_wrap_xg :public evhttp_wrap
{
public:
	virtual void on_http_ok()
	{
		struct evbuffer * data = 0;
		data = this->buffer;
		printf("got %d bytes\n", data ? evbuffer_get_length(data) : -1);

		if (data)
		{
			const unsigned char * joined = evbuffer_pullup(data, -1);
			printf("data itself:\n====================\n");
			fwrite(joined, evbuffer_get_length(data), 1, stderr);
			printf("\n====================\n");
		}
		evbuffer_drain(data, evbuffer_get_length(data));

		init_con("a2.xgsdk.com");
		make_get_query(get_xgsdk_str("17952", AUTH_STR));
	}
};
int main(int argc, char **argv)
{
	struct event_base * base = NULL;

	evhttp_wrap_xg http;
#ifdef WIN32
	WORD wVersionRequested;
	WSADATA wsaData;

	wVersionRequested = MAKEWORD(2, 2);

	(void)WSAStartup(wVersionRequested, &wsaData);
#endif
	base = event_base_new();
	http.init(base);
	http.init_con("a2.xgsdk.com");
	http.make_get_query(get_xgsdk_str("17952", AUTH_STR));
	
	while (1)
	{
		int ret = event_base_loop(base, EVLOOP_NONBLOCK);
		//printf("event_base_loop=%d\n", ret);
		//Sleep(100);
	}


	return 0;
}