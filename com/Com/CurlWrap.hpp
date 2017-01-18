#pragma once
#include "../ComAll/XSTL.hpp"
#include "CMicro.hpp"
#include "../third/curl/include/curl/curl.h"

template <class T>
struct CurlDownInfo
{
	CURLcode cRet;
	T Val;
};


class curl_wrap
{
public:
	curl_wrap()
	{
		curl_handle = NULL;
	}
	~curl_wrap()
	{
		uninit();
	}
	static CURLcode global_init()
	{
		return curl_global_init(CURL_GLOBAL_ALL);
	}
	bool create()
	{
		curl_handle = curl_easy_init();
		return NULL != curl_handle;
	}
	void uninit()
	{
		if (curl_handle)
		{
			curl_easy_cleanup(curl_handle);
		}
		curl_handle = NULL;
	}
public:
	CURLcode set_user_pass(cstr_t user_pass)
	{
		//"user:pass"
		return curl_easy_setopt(curl_handle, CURLOPT_USERPWD, user_pass);
	}
	CURLcode set_user(cstr_t user)
	{
		return curl_easy_setopt(curl_handle, CURLOPT_USERNAME, user);
	}
	CURLcode set_pass(cstr_t set_pass)
	{
		return curl_easy_setopt(curl_handle, CURLOPT_PASSWORD, set_pass);
	}
	CURLcode set_url(cstr_t url)
	{
		return curl_easy_setopt(curl_handle, CURLOPT_URL, url);
	}
	CURLcode set_proxy(cstr_t ip, int32 port)
	{
		char sztemp[50] = { 0 };
		sprintf(sztemp, "%s:%d", ip, port);
		return curl_easy_setopt(curl_handle, CURLOPT_PROXY, sztemp);
	}
	CURLcode set_timeout(int32 ms_cnt)//ms
	{
		return curl_easy_setopt(curl_handle, CURLOPT_TIMEOUT_MS, ms_cnt);
	}
	CURLcode set_post_simple(cstr_t data)
	{
		return curl_easy_setopt(curl_handle, CURLOPT_POSTFIELDS, data);
	}
	CURLcode set_ref(cstr_t url)
	{
		return curl_easy_setopt(curl_handle, CURLOPT_REFERER, url);
	}
	CURLcode set_useragent(cstr_t agent)
	{
		return curl_easy_setopt(curl_handle, CURLOPT_USERAGENT, agent);
	}
	CURLcode set_debug_out(int open)
	{
		return curl_easy_setopt(curl_handle, CURLOPT_VERBOSE, open);
	}
	CURLcode verify_ssl(int open)
	{
		return curl_easy_setopt(curl_handle, CURLOPT_SSL_VERIFYPEER, open);
	}
	CURLcode set_follow_location(int open)
	{
		return curl_easy_setopt(curl_handle, CURLOPT_FOLLOWLOCATION, open);
	}
	CURLcode set_get_header(int open)
	{
		return curl_easy_setopt(curl_handle, CURLOPT_HEADER, open);
	}
	CURLcode set_nobody(int open)
	{
		return curl_easy_setopt(curl_handle, CURLOPT_NOBODY, open);
	}

	CURLcode set_write_data(void *p)
	{
		return curl_easy_setopt(curl_handle, CURLOPT_WRITEDATA, p);
	}
	CURLcode set_write_func(curl_write_callback func)
	{
		return curl_easy_setopt(curl_handle, CURLOPT_WRITEFUNCTION, func);
	}
	CURLcode set_resume_from(curl_off_t nlen)
	{
		return curl_easy_setopt(curl_handle, CURLOPT_RESUME_FROM_LARGE, nlen);
	}
	CURLcode set_slist(curl_slist *recipients)
	{
		return curl_easy_setopt(curl_handle, CURLOPT_MAIL_RCPT, recipients);
	}
	CURLcode set_read_func(curl_read_callback rf)
	{
		return curl_easy_setopt(curl_handle, CURLOPT_READFUNCTION, rf);
	}
	CURLcode set_read_data(void *p)
	{
		return curl_easy_setopt(curl_handle, CURLOPT_READDATA, p);
	}
	CURLcode set_upload(int open)
	{
		return curl_easy_setopt(curl_handle, CURLOPT_UPLOAD, open);
	}
	//=mail==
	CURLcode set_mail_from(cstr_t mail_from)
	{
		return curl_easy_setopt(curl_handle, CURLOPT_MAIL_FROM, mail_from);
	}

	CURLcode set_head_read_func(curl_write_callback func)
	{
		return curl_easy_setopt(curl_handle, CURLOPT_HEADERFUNCTION, func);
	}
	CURLcode set_head_read_data(void *p)
	{
		return curl_easy_setopt(curl_handle, CURLOPT_HEADERDATA, p);
	}
public:
	CurlDownInfo<double> get_down_size()
	{
		CurlDownInfo<double> ret;
		ret.cRet = curl_easy_getinfo(curl_handle, CURLINFO_CONTENT_LENGTH_DOWNLOAD, &ret.Val, sizeof(ret.Val));
		return ret;
	}
	CurlDownInfo<int> get_response_code()
	{
		CurlDownInfo<int> ret;
		ret.cRet = curl_easy_getinfo(curl_handle, CURLINFO_RESPONSE_CODE, &ret.Val, sizeof(ret.Val));
		return ret;
	}
public:
	CURLcode perform()
	{
		return curl_easy_perform(curl_handle);
	}
public:
	CURL *curl_handle;
};