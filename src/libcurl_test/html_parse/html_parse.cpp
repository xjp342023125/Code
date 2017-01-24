// html_parse.cpp : 定义控制台应用程序的入口点。
//

#include "stdafx.h"

#include "../../../com/Com/CAllHead.h"
#include <event2/event.h>
#pragma comment ( lib,"../../../third/out/lib/libcurl_debug.lib" )
#pragma comment ( lib,"../../../third/out/lib/event.lib" )
#include <sstream>
using namespace std;
void test_dowm();
int test_multi();
void test_multi_with_libevent();
int _tmain(int argc, _TCHAR* argv[])
{
#ifdef _WIN32
	WSADATA wsa_data;
	WSAStartup(0x0201, &wsa_data);
#endif
	//test_dowm();
	//test_multi();
	test_multi_with_libevent();
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

/* Update the event timer after curl_multi library calls */
static int multi_timer_cb(CURLM *multi, long timeout_ms, GlobalInfo *g)
{
	struct timeval timeout;
	(void)multi; /* unused */

	timeout.tv_sec = timeout_ms / 1000;
	timeout.tv_usec = (timeout_ms % 1000) * 1000;
	fprintf(MSG_OUT, "multi_timer_cb: Setting timeout to %ld ms\n", timeout_ms);
	evtimer_add(g->timer_event, &timeout);
	return 0;
}
/* Clean up the SockInfo structure */
static void remsock(SockInfo *f)
{
	if (f) {
		if (f->evset)
			event_free(f->ev);
		free(f);
	}
}

static void event_cb(int fd, short kind, void *userp)
{
	GlobalInfo *g = (GlobalInfo*)userp;
	CURLMcode rc;

	int action =
		(kind & EV_READ ? CURL_CSELECT_IN : 0) |
		(kind & EV_WRITE ? CURL_CSELECT_OUT : 0);

	rc = curl_multi_socket_action(g->multi, fd, action, &g->still_running);
	mcode_or_die("event_cb: curl_multi_socket_action", rc);

	check_multi_info(g);
	if (g->still_running <= 0) {
		fprintf(MSG_OUT, "last transfer done, kill timeout\n");
		if (evtimer_pending(g->timer_event, NULL)) {
			evtimer_del(g->timer_event);
		}
	}
}
/* Assign information to a SockInfo structure */
static void setsock(SockInfo *f, curl_socket_t s, CURL *e, int act,
	GlobalInfo *g)
{
	int kind =
		(act&CURL_POLL_IN ? EV_READ : 0) | (act&CURL_POLL_OUT ? EV_WRITE : 0) | EV_PERSIST;

	f->sockfd = s;
	f->action = act;
	f->easy = e;
	if (f->evset)
		event_free(f->ev);
	f->ev = event_new(g->evbase, f->sockfd, kind, event_cb, g);
	f->evset = 1;
	event_add(f->ev, NULL);
}
/* Initialize a new SockInfo structure */
static void addsock(curl_socket_t s, CURL *easy, int action, GlobalInfo *g)
{
	SockInfo *fdp = (SockInfo*)calloc(sizeof(SockInfo), 1);

	fdp->global = g;
	setsock(fdp, s, easy, action, g);
	curl_multi_assign(g->multi, s, fdp);
}
/* Called by libevent when we get action on a multi socket */



/* CURLMOPT_SOCKETFUNCTION */
static int sock_cb(CURL *e, curl_socket_t s, int what, void *cbp, void *sockp)
{
	GlobalInfo *g = (GlobalInfo*)cbp;
	SockInfo *fdp = (SockInfo*)sockp;
	const char *whatstr[] = { "none", "IN", "OUT", "INOUT", "REMOVE" };

	fprintf(MSG_OUT,
		"socket callback: s=%d e=%p what=%s ", s, e, whatstr[what]);
	if (what == CURL_POLL_REMOVE) {
		fprintf(MSG_OUT, "\n");
		remsock(fdp);
	}
	else {
		if (!fdp) {
			fprintf(MSG_OUT, "Adding data: %s\n", whatstr[what]);
			addsock(s, e, what, g);
		}
		else {
			fprintf(MSG_OUT,
				"Changing action from %s to %s\n",
				whatstr[fdp->action], whatstr[what]);
			setsock(fdp, s, e, what, g);
		}
	}
	return 0;
}

/* CURLOPT_WRITEFUNCTION */
static size_t write_cb(void *ptr, size_t size, size_t nmemb, void *data)
{
	size_t realsize = size * nmemb;
	ConnInfo *conn = (ConnInfo*)data;
	(void)ptr;
	(void)conn;
	return realsize;
}

/* CURLOPT_PROGRESSFUNCTION */
static int prog_cb(void *p, double dltotal, double dlnow, double ult,
	double uln)
{
	ConnInfo *conn = (ConnInfo *)p;
	(void)ult;
	(void)uln;

	fprintf(MSG_OUT, "Progress: %s (%g/%g)\n", conn->url, dlnow, dltotal);
	return 0;
}
void add_url_to_multi_libevent(GlobalInfo *g, const char *url)
{
	ConnInfo *conn;
	CURLMcode rc;

	conn = (ConnInfo*)calloc(1, sizeof(ConnInfo));
	memset(conn, 0, sizeof(ConnInfo));
	conn->error[0] = '\0';

	conn->easy = curl_easy_init();
	if (!conn->easy) {
		fprintf(MSG_OUT, "curl_easy_init() failed, exiting!\n");
		exit(2);
	}
	conn->global = g;
	conn->url = strdup(url);
	curl_easy_setopt(conn->easy, CURLOPT_URL, conn->url);
	curl_easy_setopt(conn->easy, CURLOPT_WRITEFUNCTION, write_cb);
	curl_easy_setopt(conn->easy, CURLOPT_WRITEDATA, conn);
	curl_easy_setopt(conn->easy, CURLOPT_VERBOSE, 1L);
	curl_easy_setopt(conn->easy, CURLOPT_ERRORBUFFER, conn->error);
	curl_easy_setopt(conn->easy, CURLOPT_PRIVATE, conn);
	curl_easy_setopt(conn->easy, CURLOPT_NOPROGRESS, 0L);
	curl_easy_setopt(conn->easy, CURLOPT_PROGRESSFUNCTION, prog_cb);
	curl_easy_setopt(conn->easy, CURLOPT_PROGRESSDATA, conn);
	curl_easy_setopt(conn->easy, CURLOPT_PROGRESSDATA, conn);
	curl_easy_setopt(conn->easy,CURLOPT_USERAGENT,"Mozilla/5.0 (Windows NT 6.1) AppleWebKit/537.36 (KHTML, like Gecko) Chrome/31.0.1650.63 Safari/537.36");
	curl_easy_setopt(conn->easy, CURLOPT_SSL_VERIFYPEER, 0);
	curl_easy_setopt(conn->easy, CURLOPT_FOLLOWLOCATION, 1);
	curl_easy_setopt(conn->easy, CURLOPT_VERBOSE, 1);
	fprintf(MSG_OUT,"Adding easy %p to multi %p (%s)\n", conn->easy, g->multi, url);
	rc = curl_multi_add_handle(g->multi, conn->easy);
	mcode_or_die("new_conn: curl_multi_add_handle", rc);
}
void test_multi_with_libevent()
{
	//CreateThread(0, 0, 0, 0, 0, 0);

	GlobalInfo g;

	memset(&g, 0, sizeof(GlobalInfo));
	g.evbase = event_base_new();

	g.multi = curl_multi_init();
	g.timer_event = evtimer_new(g.evbase, timer_cb, &g);


	curl_multi_setopt(g.multi, CURLMOPT_SOCKETFUNCTION, sock_cb);
	curl_multi_setopt(g.multi, CURLMOPT_SOCKETDATA, &g);
	curl_multi_setopt(g.multi, CURLMOPT_TIMERFUNCTION, multi_timer_cb);
	curl_multi_setopt(g.multi, CURLMOPT_TIMERDATA, &g);

	//add_url_to_multi_libevent(&g, "https://github.com/");
	add_url_to_multi_libevent(&g, "http://ip.taobao.com/service/getIpInfo.php?ip=127.0.0.1");
	//add_url_to_multi_libevent(&g, "http://mat.client.dl.kingsoft.com/MATOnline_2.1.6.839.rar");
	add_url_to_multi_libevent(&g, "http://mat.update.dl.kingsoft.com/update838-839.exe");

	/* we don't call any curl_multi_socket*() function yet as we have no handles
	added! */
	
	event_base_dispatch(g.evbase);

	/* this, of course, won't get called since only way to stop this program is
	via ctrl-C, but it is here to show how cleanup /would/ be done. */

	event_free(g.timer_event);
	event_base_free(g.evbase);
	curl_multi_cleanup(g.multi);
}