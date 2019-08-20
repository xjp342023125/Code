#pragma once
#include "XMicro.h"
#include "XType.h"
#include "cstdio"
#include <cstdlib>
#include <iostream>
#include "cstring"
using namespace std;


#ifdef WIN32
#include "winsock2.h"
#include "WS2tcpip.h"

#pragma comment(lib,"Ws2_32.lib")
#else
#include <errno.h>
#include <sys/types.h>
#include <netinet/in.h>
#include <sys/socket.h>
#include <sys/wait.h>
#include <unistd.h>
#include <arpa/inet.h>
#include <pthread.h>
#include <fcntl.h>
#include <sys/epoll.h>
#include <sys/select.h>
#include <sys/time.h>
#include <pthread.h>
#include <unistd.h>
#endif


#ifdef WIN32

#else
typedef int SOCKET;
#define INVALID_SOCKET (SOCKET)(~0)
#define SOCKET_ERROR (-1)
#define __stdcall
#endif

#ifdef WIN32
#define SOCK_ERR	GetLastError()
#define xclosesocket   ::closesocket
#else
#define SOCK_ERR	errno
#define xclosesocket   ::close
#endif



class CAddr
{
public:
	CAddr()
	{
		m_ip = 0;
		m_Port = 0;
	}
	void InitAddr(ip_t ip, port_t port)
	{
		m_ip = ip;
		m_Port = port;
	}
	void InitAddr(const char *ip, port_t port)
	{
		m_ip = inet_addr(ip);
		m_Port = port;

	}
	void InitAddr(const sockaddr_in& addr)
	{
		m_ip = addr.sin_addr.s_addr;
		m_Port = htons(addr.sin_port);
	}
	void InitAddr(const sockaddr& addr)
	{
		InitAddr(*(sockaddr_in*)&addr);
	}
	
	bool InitWithUrl(const char *pURL, port_t port = 80)
	{

		hostent *pH = gethostbyname(pURL);
		if (NULL == pH)
		{
			return false;
		}
		ip_t ip = *(ip_t*)(*pH->h_addr_list);
		InitAddr(ip, port);
		return true;
	}
	const char *GetIPStr()const
	{
		in_addr addr;
		addr.s_addr = m_ip;
		return inet_ntoa(addr);
	}
	void  FillAddr(sockaddr &addr)const
	{
		memset(&addr, 0, sizeof(sockaddr));
		((sockaddr_in*)(&addr))->sin_family = AF_INET;
		((sockaddr_in*)(&addr))->sin_port = htons(m_Port);
		((sockaddr_in*)(&addr))->sin_addr.s_addr = m_ip;
	}
	inline port_t GetPort()const
	{
		return m_Port;
	}
	const char *GetAddrStr()const
	{
		static char szTemp[100];
		sprintf(szTemp, " ip:%s,port:%d", GetIPStr(), m_Port);
		return szTemp;
	}

public:
	ip_t	m_ip;
	port_t	m_Port;
};

static bool operator < (const CAddr& l, const CAddr& r)
{
	if (l.m_ip < r.m_ip)
		return true;
	else
		return false;

	if (l.m_Port < r.m_Port)
		return true;
	else
		return false;
	return false;
}

struct accept_ret_t
{
	accept_ret_t()
	{
		s = 0;
		addr.InitAddr("", 0);
	}
	SOCKET s;
	CAddr addr;
};
class XSock
{
public:
	XSock()
	{
		sock = INVALID_SOCKET;
	}
	~XSock()
	{
		Close();
	}
	void Close()
	{
		if (INVALID_SOCKET != sock)
		{
			xclosesocket(sock);
			sock = INVALID_SOCKET;
		}
	}
	bool SetSendTimeOut(uint32 millisecond)
	{
#ifdef WIN32
		DWORD time = millisecond;
		if (setsockopt(sock, SOL_SOCKET, SO_SNDTIMEO, (const char*)&time, sizeof(DWORD)) == -1) {
			return false;
		}
#else
		struct timeval tv;
		tv.tv_sec = millisecond / 1000;
		tv.tv_usec = (millisecond % 1000) * 1000;
		if (setsockopt(sock, SOL_SOCKET, SO_SNDTIMEO, (const char*)&tv, sizeof(tv)) == -1) {
			return false;
		}
#endif
		return true;
	}
	bool SetRecvTimeOut(uint32 millisecond)
	{
#ifdef WIN32
		DWORD time = millisecond;
		if (setsockopt(sock, SOL_SOCKET, SO_RCVTIMEO, (const char*)&time, sizeof(DWORD)) == -1) {
			return false;
		}
#else
		struct timeval tv;
		tv.tv_sec = millisecond / 1000;
		tv.tv_usec = (millisecond % 1000) * 1000;
		if (setsockopt(sock, SOL_SOCKET, SO_RCVTIMEO, (const char*)&tv, sizeof(tv)) == -1) {
			return false;
		}
#endif
		return true;
	}

	bool SetNoDelay()
	{
		int yes = 1;

		if (setsockopt(sock, IPPROTO_TCP, TCP_NODELAY, (const char*)&yes, sizeof(yes)) == -1) {
			return false;
		}
		return true;
	}

	bool SetReUseAddr(int v)
	{
		int ret = setsockopt(sock, SOL_SOCKET, SO_REUSEADDR, (const char*)&v, sizeof(v));
		return 0 == ret;
	}
	//1 :non block
	int SetNonBlock(int value)
	{
#ifndef WIN32
		int oldflags = ::fcntl(sock, F_GETFL, 0);
		/* If reading the flags failed, return error indication now. */
		if (oldflags == -1)
			return -1;

		/* Set just the flag we want to set. */
		if (value != 0)
			oldflags |= O_NONBLOCK;
		else
			oldflags &= ~O_NONBLOCK;
		/* Store modified flag word in the descriptor. */
		return ::fcntl(m_iSock, F_SETFL, oldflags);
#else
		if (::ioctlsocket(sock, FIONBIO, (u_long FAR*)&value) == SOCKET_ERROR)
		{
			return -1;
		}

		return 0;
#endif
	}

	bool Bind(CAddr caddr)
	{
		sockaddr addr;
		caddr.FillAddr(addr);

		int ret = bind(sock, &addr, sizeof(addr));
		return 0 == ret;
	}
	bool Listen(int iBacklog=20)
	{
		return ::listen(sock, iBacklog);
	}
	
	accept_ret_t Accept()
	{
		accept_ret_t ret;

		struct sockaddr_in addr;
		int	addrlen = sizeof(addr);

		ret.s = ::accept(sock, (struct sockaddr*)&addr, &addrlen);
		ret.addr.InitAddr(addr);
		return ret;
	}
	bool Connect(CAddr addr)
	{
		sockaddr saddr;
		addr.FillAddr(saddr);
		int result = ::connect(sock, &saddr, sizeof(saddr));
		return 0 == result;
	}

	int TCPRecv(char *buf,int len,int flag=0)
	{
		return recv(sock, buf, len, flag);
	}

	bool TCPCreate()
	{
		Close();
		sock = ::socket(AF_INET, SOCK_STREAM, 0);
		return INVALID_SOCKET != sock;
	}
//=================================================
	bool UdpCreate()
	{
		Close();
		sock = socket(AF_INET, SOCK_DGRAM, IPPROTO_UDP);
		return INVALID_SOCKET != sock;
	}
	int UdpSend(CAddr caddr, const char *buf, int nlen)
	{
		sockaddr addr;
		caddr.FillAddr(addr);
		return sendto(sock, buf, nlen, 0, &addr, sizeof(sockaddr));;
	}

	int UdpRecv(char *buf, int nLen, CAddr &caddr)
	{
		sockaddr addr;
		socklen_t nAddrLen = sizeof(sockaddr);
		int nRet = recvfrom(sock,
			buf,
			nLen,
			0,
			&addr,
			&nAddrLen);
		caddr.InitAddr(addr);
		return nRet;
	}
public:
	SOCKET sock;
};

#ifdef WIN32
class CInitNet
{
public:
	CInitNet()
	{
		WSADATA wsaData;
		WSAStartup(MAKEWORD(2, 2), &wsaData);
	}
};
extern CInitNet g_win_init_net;
#endif