#pragma once
#include <map>
#include <string>
#include <vector>
#include <set>
#include <iostream>
#include "CStr.hpp"
#include <memory>
using namespace std;
#define CHECK(val)	if(!(val)) return ;

static cstr_t SkipNULL(cstr_t p){
	while (*p == ' ' || *p == '\r' || *p == '\n' || *p == '	'){	++p;}	return p;
}
static cstr_t FindNull(cstr_t p){
	while (*p)
	{
		if (*p == ' ' || *p == '\r' || *p == '\n' || *p == '	')
			return p;
		else
			++p;
	}
	return NULL;
}

typedef string url_t;
typedef vector<url_t> urlvec_t;
//typedef multiset<url_t> urlset_t;
typedef set<url_t> urlset_t;

typedef string mail_t;
typedef set<mail_t> mailset_t;
typedef vector<int> idxvec_t;

//#define TEST_HTTP_MEM
#ifdef TEST_HTTP_MEM
extern int32 new_cnt;
extern int32 free_cnt;
#endif


class CHttpNode :public enable_shared_from_this<CHttpNode>
{
public:
	CHttpNode()
	{
#ifdef TEST_HTTP_MEM
		++new_cnt;
#endif
		m_bClose = false; 
		pParent = NULL;
	}
	~CHttpNode()
	{
#ifdef TEST_HTTP_MEM
		++free_cnt;
#endif
	}

	typedef shared_ptr<CHttpNode>	node_ptr_t;
	node_ptr_t GetC(int i)
	{
		if (i >= GetCSize())
		{
			return NULL;
		}
		return m_Tree[i];
	}
	void AddC(node_ptr_t Child)
	{
		if (Child)
		{
			m_Tree.push_back(Child);
			Child->pParent = this;
		}
	}


	vector<node_ptr_t> GetWebUrl()
	{
		vector<node_ptr_t> urls;
		for (auto i : m_Tree)
		{
			auto suburls = i->GetWebUrl();
			for (auto i2:suburls)
			{
				urls.push_back(i2);
			}
		}
		auto urlit =mapAttr.find("href");
		if (urlit != mapAttr.end())
		{
			urls.push_back(this->shared_from_this());
		}
		return urls;
	}
	int GetCSize()
	{
		return m_Tree.size();
	}
	idxvec_t GetNodeIdx(cstr_t sValToFind)
	{
		idxvec_t ret;
		TravelVal(*this, sValToFind, ret);
		return ret;
	}
	node_ptr_t GetNodeByIdx(idxvec_t idxs)
	{
		node_ptr_t p = this->shared_from_this();
		while (idxs.size()>0)
		{
			int i = idxs.back();
			idxs.pop_back();
			if (-1 == i)
				return p;

			if (i >= p->GetCSize())
				return NULL;

			p = p->GetC(i);
		}
		return p;
	}
	string ToStr()
	{
		string ret;
		ret = string("<") + sName + ">";
		if (m_Tree.size() != 0)
		{
			ret += "\n";
			for (size_t i = 0; i < m_Tree.size(); i++)
			{
				ret = ret + m_Tree[i]->ToStr() + "\n";
			}
		}
		ret += sVal;
		if (m_bClose)
		{
			ret += string("</") + sName + ">";
		}
		return ret;
	}



	vector<node_ptr_t> GetNodesByIdx(vector<int> &idxs1, vector<int> &idxs2)
	{
		vector<CHttpNode::node_ptr_t> ret;
		if (idxs1.size() != idxs2.size())
		{
			return ret;
		}
		CHttpNode *p = this;
		int nIdx = 0;
		for (size_t i = 0; i < idxs1.size(); i++)
		{
			if (idxs1[i] != idxs2[i])
			{
				vector<int> temp(idxs1.begin() + i + 1, idxs1.end());
				CHttpNode::node_ptr_t pDst = GetNodeByIdx(temp);
				if (!pDst)
				{
					return ret;
				}
				cout << pDst;

				temp = idxs1;
				for (size_t i2 = 0; i2 < pDst->GetCSize(); i2++)
				{
					temp[i] = i2;
					CHttpNode::node_ptr_t pDst = GetNodeByIdx(temp);
					if (pDst)
					{
						ret.push_back(pDst);
					}
				}
			}
		}

		return ret;
	}
private:
	static bool TravelVal(CHttpNode &node, const char *sToFind, vector<int> &idxs)
	{
		if (strstr(node.sVal.c_str(), sToFind))
		{
			idxs.push_back(-1);
			return true;
		}
		for (auto it = node.mapAttr.begin(); it != node.mapAttr.end(); ++it)
		{
			if (strstr(it->second.c_str(), sToFind))
			{
				return true;
			}
		}
		for (int i = 0; i < node.GetCSize(); ++i)
		{
			if (TravelVal(*node.GetC(i), sToFind, idxs))
			{
				idxs.push_back(i);
				return true;
			}
		}
		return false;
	}


public:
	vector<node_ptr_t>				m_Tree;
	string sName;
	string sVal;
	multimap<string,string> mapAttr;
	bool						m_bClose;
	CHttpNode *pParent;
};


static CHttpNode::node_ptr_t CreateHttpNode()
{
	CHttpNode::node_ptr_t p(new CHttpNode);

	return p;
}


static void ParaAttr(CHttpNode &para){
	const char *p = para.sName.c_str();
	const char *pEnd = p+para.sName.size();
	const char *pKong = FindNull(p);
	CHECK(pKong);
	
	string sTempName(p,pKong);
	p = pKong;
	while (p < pEnd){
		p=SkipNULL(p);
		const char *pD = strchr(p,'=');
		if (!pD){
			break;
		}
		
		const char *p1 = NULL;
		const char *p2 = NULL;

		if (p1 = strchr(pD,'"')){
			p1+=1;
			p2 = strchr(p1,'"');
		}else if (p1 = strchr(pD,'\'')){
			p1+=1;
			p2 = strchr(p1,'\'');
		}else{
			p1 = pD+1;
			p2 = FindNull(p1);
		}
		if (NULL == p2){
			p2 = pEnd;
		}
		
		para.mapAttr.insert(make_pair(string(p, pD), string(p1, p2)));
		p=p2+1;
	}
	para.sName = sTempName;
}


static bool AddTree(vector<CHttpNode::node_ptr_t> &tree, CHttpNode::node_ptr_t toAdd){
	const char *pAddName = toAdd->sName.c_str();
	int nLen = toAdd->sName.size();
	if (nLen >= 1 && pAddName[0] == '/')
	{
		int i = tree.size() - 1;
		for (; i >= 0; --i)	{
			if (!tree[i]->m_bClose && 0 == strcmp(tree[i]->sName.c_str(), pAddName+1)){
				tree[i]->m_bClose = true;
				for (int j = i + 1; j < tree.size(); ++j)	{
					tree[i]->AddC(tree[j]);
				}
				tree.erase(tree.begin() + i + 1, tree.end());
				return false;
			}
		}
	}
	
	tree.push_back(toAdd);
	return true;

}

static CHttpNode::node_ptr_t ParserHttp(const char *pB, const char *pE){
	
	
	vector<CHttpNode::node_ptr_t>	para1;
	bool bIsJS = false;

	while (pB < pE && *pB != '<')
	{
		++pB;
	}

	while (pB<pE){
		pB = SkipNULL(pB);
		
		const char *pL = strchr(pB,'<');
		const char *pR = strchr(pB,'>');
		if (NULL == pL){
			break;
		}
		if (NULL == pR)
		{
			return NULL;
		}
		//<abc>
		if (pB == pL){
			if ('!' != pL[1]){
				CHttpNode::node_ptr_t pparaTemp = CreateHttpNode();
				pparaTemp->sName = string(pL + 1, pR);
				ParaAttr(*pparaTemp);
				bool bAdd = AddTree(para1,pparaTemp);
				if ("script" == pparaTemp->sName){
					const char *pEnd = strstr(pL,"</script>");
					if (pEnd)
					{
						pparaTemp->sVal = string(pR + 1, pEnd);
						pparaTemp->m_bClose = true;
						pB = pEnd + strlen("</script>");
					}
					
					continue;
				}

			}
			pB = pR+1;
		}else{
			if (para1.size() == 0)
			{
				return NULL;
			}
			para1[para1.size()-1]->sVal = string(pB,pL);
			pB = pL;
		}
	}
	CHttpNode::node_ptr_t pN = CreateHttpNode();
	for (int i = 0;i < para1.size();++i){
		pN->AddC(para1[i]);
	}
	return pN;
}

#include "CHttpDown.hpp"


static CHttpNode::node_ptr_t GetHttpNodeEx(CurlDMem &sd, bool convert_utf8)
{
	if (NULL == sd.bf.m_addr)
		return NULL;
	char *p = NULL;
	if (convert_utf8)
		p = Utf8_Ansi(sd.bf.m_addr);
	else
		p = sd.bf.m_addr;

	CHttpNode::node_ptr_t pRoot = ParserHttp(p, p + strlen(p));
	if (convert_utf8)
		XFreeMem(p);

	return pRoot;
}
#include "CSock.hpp"
static CHttpNode::node_ptr_t GetHttpNodeByURL(cstr_t url, bool convert_utf8, CAddr *proxy = NULL)
{
	CurlDMem sd;
	sd.Init();
	sd.curl_handle.set_url(url);
	if (proxy)
	{
		sd.curl_handle.set_proxy(proxy->GetIPStr(), proxy->GetPort());
	}
	DownInfo Ret = sd.CurlPerform();
	CHttpNode::node_ptr_t p = GetHttpNodeEx(sd, true);
	sd.bf.Clean();
	return p;
}

typedef CHttpNode::node_ptr_t http_ptr_t;