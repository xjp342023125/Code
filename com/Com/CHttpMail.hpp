#pragma once

#include "CHttpTree.hpp"

class CHttpMail : public CHttpNode
{
public:
	mailset_t GetWebMail()
	{
		mailset_t mails;
		for (auto i : m_Tree)
		{
			auto submails = i->GetWebMail();
			mails.insert(submails.begin(), submails.end());
		}
		if (have_eamil(sVal))
		{
			mails.insert(sVal);
		}
		return mails;
	}
};



class CWeb
{
public:
	void GetMails()
	{
		map<mail_t, url_t> ret;
		for (auto it : pages)
		{
			if (it.second)
			{
				auto mails = it.second->GetWebMail();
				for (auto mit : mails)
				{
					ret.insert(make_pair(mit, it.first));
				}
			}
		}
		for (auto it : ret)
		{
			cout << "mail=" << it.first << " ,url=" << it.second << endl;
		}
	}
	bool InitWithURL(cstr_t pURL, bool convert_utf8)
	{
		base_url = GetURLBase(pURL);
		CHttpNode::node_ptr_t  page = GetHttpNodeByURL(pURL, true);
		auto urls = page->GetWebUrl();
		urlset_t works;
		AddToWork(works, urls);
		while (works.size() > 0)
		{
			url_t it = *works.begin();
			works.erase(works.begin());

			CHttpNode::node_ptr_t  p = GetHttpNodeByURL(it.c_str(), true);
			pages.insert(make_pair(it, p));
			if (NULL == p)
			{
				err_url[it]++;
				continue;
			}
			else
			{
				cout << it << endl;
			}

			auto urls_new = p->GetWebUrl();
			AddToWork(works, urls_new);
			cout << urls_new.size() << "-" << works.size() << "-" << pages.size() << endl;
		}
		return true;
	}
private:
	void AddToWork(urlset_t &works, urlset_t &toadd)
	{
		for (auto i : toadd)
		{
			if (NULL != strstr(i.c_str(), "javascript") || NULL != strstr(i.c_str(), "JavaScript") || "" == i)
			{
				//cout << i << "  ignore"<<endl;
				ignore_url[i]++;
				continue;
			}


			if (!is_url(i))
			{
				if ('/' != i[0])
				{
					i = base_url + "/" + i;
				}
				else
				{
					i = base_url + i;
				}
			}


			if (0 != pages.count(i))
				continue;


			if (NULL != strstr(i.c_str(), base_url.c_str()))
			{
				works.insert(i);
			}

		}
	}

	map<url_t, CHttpNode::node_ptr_t >	pages;
	string base_url;
	map<url_t, int> err_url;
	map<url_t, int> ignore_url;
};