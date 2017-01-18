#pragma once 


#include <vector>
#include <string>
#include "CLog.hpp"
#include "CurlWrap.hpp"
using namespace std;



#ifdef USE_LOG
extern CLog gHttpLog;
static void init_http_log()
{
	gHttpLog.InitDevice("http");
}

#define HINFO	gHttpLog.l()
#define HERROR	gHttpLog.l()<<"Error,Line"<<__LINE__<<","
#else
#define HINFO	GetNoLog()
#define HERROR	GetNoLog()
#endif


//===========================================================================================================




struct DownInfo{

	CURLcode downRet;
	CurlDownInfo<double> DSize;//down size  CURLINFO_CONTENT_LENGTH_DOWNLOAD
	CurlDownInfo<int>	RCode;// response code CURLINFO_RESPONSE_CODE

	bool DownOK()
	{
		return (RCode.cRet == CURLE_OK && 200 == RCode.Val	&& CURLE_OK == downRet);
	}

	DownInfo(){
		Init();
	}

	string GetStr(){
		char sz[1000] = { 0 };
		sprintf(sz, "[D Ret %d][DSize Ret=%d,val=%d][RCode Ret=%d,val=%d]",
			downRet,
			DSize.cRet, (unsigned)DSize.Val,
			RCode.cRet, RCode.Val
			);
		return string(sz);
	}

	void Init(){
		downRet = CURLE_OK;

		DSize.cRet = CURLE_OK;
		DSize.Val = 0;

		RCode.cRet = CURLE_OK;
		RCode.Val = 0;
	}
	bool HttpCodeIs(int code)
	{
		return RCode.cRet == CURLE_OK && RCode.Val == code;
	}
};


class CurlDMem
{
public:
	// 1 call Init
	// 2 curl set_opt:url....
	// 3 one of flow
	int64 GetFileLen()
	{
		curl_handle.set_get_header(1);
		curl_handle.set_nobody(1);
		DownInfo ret = CurlPerform();
		if (CURLE_OK == ret.downRet){
			int64 i64Ret = -1;

#define HTTP_LEN_FLAG	("Content-Length:")
			const char *p = strstr(bf.m_addr, HTTP_LEN_FLAG);
			if (p)
			{
				sscanf(p, "%*[^0-9]%lld", &i64Ret);
			}
			curl_handle.uninit();

			return i64Ret;
		}
		return -2;
	}


	DownInfo CurlPerform()
	{
		DownInfo ret;
		ret.downRet = curl_handle.perform();
		if (CURLE_OK == ret.downRet){
			ret.DSize = curl_handle.get_down_size();
			ret.RCode = curl_handle.get_response_code();
		}
		return ret;
	}
public:
	~CurlDMem()
	{
		bf.Clean();
	}

	bool InitSimple()
	{
		curl_handle.uninit();
		curl_wrap::global_init();
		
		CHECK_RETV(curl_handle.create(), false);
		curl_handle.set_write_func(CurlDMem::CurlWriteFileCallback);
		curl_handle.set_write_data(this);
		return true;
	}
	bool Init()
	{
		InitSimple();
		curl_handle.set_useragent("Mozilla/5.0 (Windows NT 6.1) AppleWebKit/537.36 (KHTML, like Gecko) Chrome/31.0.1650.63 Safari/537.36");
		curl_handle.set_follow_location(1);
		curl_handle.verify_ssl(0);
		return true;
	}

private:
	static size_t CurlWriteFileCallback(char *contents, size_t size, size_t nmemb, void *userp)
	{
		size_t realsize = size * nmemb;
		CurlDMem *pSD = (CurlDMem*)userp;
		pSD->bf.AddBuf(contents, realsize);
		return realsize;
	}
public:
	Buf bf;
	curl_wrap curl_handle;
};







static DownInfo CurlDown(cstr_t pUrl, CurlDMem *pSD)
{
	HINFO << "=======================begin down url=" << pUrl;
	DownInfo  Ret;
	do {
		Ret.Init();
		pSD->Init();
		pSD->curl_handle.set_url(pUrl);
		DWORD dwHaveLen = pSD->bf.m_nLen;
		if (0 != dwHaveLen)
		{
			pSD->curl_handle.set_resume_from(dwHaveLen);
		}

		Ret = pSD->CurlPerform();
		// log
		//=============================================
		HINFO << "CurlDown " << Ret.GetStr().c_str()
			<< ",old buf len=" << dwHaveLen
			<< ",new buf len=" << pSD->bf.m_nLen
			<< ",down = " << pSD->bf.m_nLen - dwHaveLen;
		if (CURLE_OK != Ret.downRet && curl_easy_strerror(Ret.downRet))
		{
			HERROR << "curl ret = " << Ret.downRet
				<< ",msg:" << curl_easy_strerror(Ret.downRet);
		}
		if (CURLE_OK == Ret.RCode.cRet && 200 != Ret.RCode.Val)
		{
			HERROR << "Error http code = " << Ret.RCode.Val;
		}
		//=============================================
		if (CURLE_RANGE_ERROR == Ret.downRet)
		{
			break;
		}
		//=============================================
		if (CURLE_COULDNT_RESOLVE_HOST == Ret.downRet
			|| CURLE_UNSUPPORTED_PROTOCOL == Ret.downRet)
		{
			pSD->bf.Clean();
			break;
		}
		//=============================================
		if (Ret.HttpCodeIs(416)
			|| Ret.HttpCodeIs(404))
		{
			break;
		}
		Sleep(100);
	} while (true);
	HINFO << "=======================end down url=" << pUrl;
	return Ret;
}

//=======================================================================
struct sSmtpBody
{
	int nIndex;
	vector<string> ss;
};

static size_t smtp_read_func(char *ptr, size_t size, size_t nmemb, void *userp)
{
	struct sSmtpBody *ss = (sSmtpBody *)userp;
	const char *data;

	if ((size == 0) || (nmemb == 0) || ((size*nmemb) < 1)) {
		return 0;
	}
	if (ss->nIndex <= ss->ss.size() - 1)
	{
		data = ss->ss[ss->nIndex].c_str();
		size_t len = strlen(data);
		memcpy(ptr, data, len);
		++ss->nIndex;
		return len;
	}

	return 0;

}

static int SendMain(
	cstr_t pSmtpServer,
	cstr_t pAcc,
	cstr_t pPass,
	cstr_t pFrom,
	cstr_t pTo,
	cstr_t pCC,
	cstr_t pTitle,
	cstr_t pContent
	)
{
	curl_wrap curl;
	CURLcode res = CURLE_OK;
	
	sSmtpBody sContext;
	sContext.nIndex = 0;
	{
		string s;

		s = string("To: ") + pTo+ "\r\n";
		sContext.ss.push_back(s);

		s = string("From: ") + pFrom + "\r\n";
		sContext.ss.push_back(s);

		s = string("Cc: ") + pCC + "\r\n";
		sContext.ss.push_back(s);

		s = string("Subject: ") + pTitle + "\r\n\r\n";
		sContext.ss.push_back(s);

		s = pContent;
		sContext.ss.push_back(s);

		s = "\r\n";
		sContext.ss.push_back(s);
	}
	curl_wrap::global_init();

	if (curl.create()) {
		/* This is the URL for your mailserver */
		curl.set_debug_out(1);
		curl.set_get_header(1);
		curl.set_url(pSmtpServer);
		curl.set_mail_from(pFrom);

		struct curl_slist *recipients = NULL;
		recipients = curl_slist_append(recipients, pTo);
		recipients = curl_slist_append(recipients, pCC); 
		curl.set_slist(recipients);

		curl.set_read_func(smtp_read_func);
		curl.set_upload(1);
		curl.set_user(pAcc);
		curl.set_pass(pPass);
		res = curl.perform();

		/* Check for errors */
		if (res != CURLE_OK)
			fprintf(stderr, "curl_easy_perform() failed: %s\n",
			curl_easy_strerror(res));

		/* Free the list of recipients */
		curl_slist_free_all(recipients);
	}

	return (int)res;
}