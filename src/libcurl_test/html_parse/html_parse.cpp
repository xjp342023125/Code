// html_parse.cpp : 定义控制台应用程序的入口点。
//

#include "stdafx.h"

#include "../../../com/Com/CAllHead.h"
#include <event2/event.h>
#pragma comment ( lib,"../../../third/out/lib/libcurl.lib" )
#pragma comment ( lib,"../../../third/out/lib/event.lib" )
#include <sstream>
using namespace std;
void test_dowm();
int test_multi();
void test_multi_with_libevent();
int _tmain(int argc, _TCHAR* argv[])
{
	//test_dowm();
	test_multi();
	return 0;

}
void test_dowm()
{
	CurlDMem dm;
	dm.Init();
	dm.curl_handle.set_url("https://github.com/curl/curl");
	dm.curl_handle.set_debug_out(1);
	dm.CurlPerform();
	XWriteFile("out.html", dm.bf.m_addr, dm.bf.m_nLen);
}

void add_url_to_multi(CURLM *m, const char *url)
{
	CURL *http_handle;
	http_handle = curl_easy_init();
	/* set the options (I left out a few, you'll get the point anyway) */
	curl_easy_setopt(http_handle, CURLOPT_URL, url);

	/* add the individual transfers */
	curl_multi_add_handle(m, http_handle);
}
int test_multi()
{
	CURLM *multi_handle;

	int still_running; /* keep number of running handles */
	int repeats = 0;

	curl_global_init(CURL_GLOBAL_DEFAULT);

	/* init a multi stack */
	multi_handle = curl_multi_init();

	add_url_to_multi(multi_handle, "http://www.example.com/");
	add_url_to_multi(multi_handle, "http://www.example.com/");
	//add_url_to_multi(multi_handle, "www.csdn.net.cn");

	

	/* we start some action by calling perform right away */
	curl_multi_perform(multi_handle, &still_running);

	do {
		CURLMcode mc; /* curl_multi_wait() return code */
		int numfds;

		/* wait for activity, timeout or "nothing" */
		mc = curl_multi_wait(multi_handle, NULL, 0, 1000, &numfds);

		if (mc != CURLM_OK) {
			fprintf(stderr, "curl_multi_wait() failed, code %d.\n", mc);
			break;
		}

		/* 'numfds' being zero means either a timeout or no file descriptors to
		wait for. Try timeout on first occurrence, then assume no file
		descriptors and no file descriptors to wait for means wait for 100
		milliseconds. */

		if (!numfds) {
			repeats++; /* count number of repeated zero numfds */
			if (repeats > 1) {
				Sleep(100); /* sleep 100 milliseconds */
			}
		}
		else
			repeats = 0;

		curl_multi_perform(multi_handle, &still_running);
	} while (still_running);



	curl_multi_cleanup(multi_handle);

	curl_global_cleanup();

	return 0;
}
//=========================================================
#define MSG_OUT stdout /* Send info to stdout, change to stderr if you want */

/* Global information, common to all connections */
typedef struct _GlobalInfo
{
	struct event_base *evbase;
	struct event *fifo_event;
	struct event *timer_event;
	CURLM *multi;
	int still_running;
} GlobalInfo;

/* Information associated with a specific easy handle */
typedef struct _ConnInfo
{
	CURL *easy;
	char *url;
	GlobalInfo *global;
	char error[CURL_ERROR_SIZE];
} ConnInfo;

/* Information associated with a specific socket */
typedef struct _SockInfo
{
	curl_socket_t sockfd;
	CURL *easy;
	int action;
	long timeout;
	struct event *ev;
	int evset;
	GlobalInfo *global;
} SockInfo;
/* Die if we get a bad CURLMcode somewhere */
static void mcode_or_die(const char *where, CURLMcode code)
{
	if (CURLM_OK != code) {
		const char *s;
		switch (code) {
		case     CURLM_BAD_HANDLE:         s = "CURLM_BAD_HANDLE";         break;
		case     CURLM_BAD_EASY_HANDLE:    s = "CURLM_BAD_EASY_HANDLE";    break;
		case     CURLM_OUT_OF_MEMORY:      s = "CURLM_OUT_OF_MEMORY";      break;
		case     CURLM_INTERNAL_ERROR:     s = "CURLM_INTERNAL_ERROR";     break;
		case     CURLM_UNKNOWN_OPTION:     s = "CURLM_UNKNOWN_OPTION";     break;
		case     CURLM_LAST:               s = "CURLM_LAST";               break;
		default: s = "CURLM_unknown";
			break;
		case     CURLM_BAD_SOCKET:         s = "CURLM_BAD_SOCKET";
			fprintf(MSG_OUT, "ERROR: %s returns %s\n", where, s);
			/* ignore this error */
			return;
		}
		fprintf(MSG_OUT, "ERROR: %s returns %s\n", where, s);
		exit(code);
	}
}

/* Check for completed transfers, and remove their easy handles */
static void check_multi_info(GlobalInfo *g)
{
	char *eff_url;
	CURLMsg *msg;
	int msgs_left;
	ConnInfo *conn;
	CURL *easy;
	CURLcode res;

	fprintf(MSG_OUT, "REMAINING: %d\n", g->still_running);
	while ((msg = curl_multi_info_read(g->multi, &msgs_left))) {
		if (msg->msg == CURLMSG_DONE) {
			easy = msg->easy_handle;
			res = msg->data.result;
			curl_easy_getinfo(easy, CURLINFO_PRIVATE, &conn);
			curl_easy_getinfo(easy, CURLINFO_EFFECTIVE_URL, &eff_url);
			fprintf(MSG_OUT, "DONE: %s => (%d) %s\n", eff_url, res, conn->error);
			curl_multi_remove_handle(g->multi, easy);
			free(conn->url);
			curl_easy_cleanup(easy);
			free(conn);
		}
	}
}


/* Called by libevent when our timeout expires */
static void timer_cb(int fd, short kind, void *userp)
{
	GlobalInfo *g = (GlobalInfo *)userp;
	CURLMcode rc;
	(void)fd;
	(void)kind;

	rc = curl_multi_socket_action(g->multi,
		CURL_SOCKET_TIMEOUT, 0, &g->still_running);
	mcode_or_die("timer_cb: curl_multi_socket_action", rc);
	check_multi_info(g);
}
void test_multi_with_libevent()
{
	GlobalInfo g;

	memset(&g, 0, sizeof(GlobalInfo));
	g.evbase = event_base_new();

	g.multi = curl_multi_init();
	g.timer_event = evtimer_new(g.evbase, timer_cb, &g);
}