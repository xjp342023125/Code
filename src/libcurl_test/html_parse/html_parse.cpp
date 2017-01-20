// html_parse.cpp : 定义控制台应用程序的入口点。
//

#include "stdafx.h"

#include "../../../com/Com/CAllHead.h"
#include <sstream>
using namespace std;
void test_dowm();
int test_multi();
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
