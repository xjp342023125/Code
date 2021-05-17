#pragma once

#include "cstdio"
#include <cstdlib>
#include <iostream>
#include <vector>
#include <map>
#include <thread>
#include <chrono>
#include <set>
#include "cstring"
using namespace std;

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
#include <fcntl.h>
//////////////////////////////
typedef unsigned int ip_t;
typedef unsigned short port_t;
//////////////////////////////
#define ARRAY_CNT(v)	(sizeof(v)/sizeof((v)[0]))
#define X_CHECK(v,ret) \
    if(0 == (v)){printf("err!file:%s,line:%u,last err=%u \n",__FILE__,__LINE__,errno);return ret;}

#define X_CHECK_NO_RET(v) \
    if(0 == (v)){printf("err!file:%s,line:%u,last err=%u \n",__FILE__,__LINE__,errno);return ;}
#define X_CHECK_RET_BOOL(v) X_CHECK(v,false)
//////////////////////////////
#define invalid_sock -1
//////////////////////////////
static void sleep_ns(uint64_t ns){
    std::this_thread::sleep_for(std::chrono::nanoseconds(ns));
}

static sockaddr_in fill_addr(const char *ip, port_t port)
{
	sockaddr_in addr;
	memset(&addr, 0, sizeof(sockaddr_in));
	addr.sin_family = AF_INET;
	addr.sin_port = htons(port);
	addr.sin_addr.s_addr = inet_addr(ip);
	return addr;
}
//////////////////////////////
class xsock
{
public:
    void close(){
        if(invalid_sock == sock_){
            return ;
        }
        ::close(sock_);
        sock_ = invalid_sock;
    }
    bool tcp_create(){
        close();
        sock_ = ::socket(AF_INET, SOCK_STREAM, 0);
		X_CHECK_RET_BOOL(invalid_sock != sock_);
        return true;
    }

    bool xbind(const char *ip, port_t port)
	{
		sockaddr_in addr = fill_addr(ip,port);

		int ret = bind(sock_, (struct sockaddr*)&addr, sizeof(addr));
		return 0 == ret;
	}

	bool xlisten(int iBacklog=20)
	{
		return 0 == ::listen(sock_, iBacklog);
	}


    int xaccept()
	{
		int ret;

		struct sockaddr_in addr;
		socklen_t	addrlen = sizeof(addr);

		ret = ::accept(sock_, (struct sockaddr*)&addr, &addrlen);
		return ret;
	}

    bool set_nonblock(int value)
	{
		int oldflags = ::fcntl(sock_, F_GETFL, 0);
        X_CHECK_RET_BOOL(-1 != oldflags);

		if (value != 0)
			oldflags |= O_NONBLOCK;
		else
			oldflags &= ~O_NONBLOCK;
		/* Store modified flag word in the descriptor. */
		auto ret = ::fcntl(sock_, F_SETFL, oldflags);
        X_CHECK_RET_BOOL(-1 != sock_);
        return true;
	}

    bool connect(const char *ip, port_t port){
        sockaddr_in addr = fill_addr(ip,port);
        auto ret = ::connect(sock_,(struct sockaddr*)&addr,sizeof(addr));
        X_CHECK_RET_BOOL(-1 != ret);

        return true;
    }

    int recv_from(void *buf, 
                size_t n, 
                int flags = 0,
                struct sockaddr *src_addr = nullptr, 
                socklen_t *addrlen = nullptr)
    {
        auto ret = ::recvfrom(sock_,buf,n,flags,src_addr,addrlen);
        if(0 == ret){
            // When a stream socket peer has performed an orderly shutdown, the return value will be 0
            close();
            return 0;
        }else if(-1 == ret){
            // -1 if an error occurred.  In the event of an error,  errno is set to indicate the error.
            if(EINTR != errno && 
                EAGAIN != errno && 
                EWOULDBLOCK != errno){
                close();
                X_CHECK(false,-1);
            }
            return -1;
        }else{
            // return the number of bytes received
            return ret;
        }
    }

    int sendto(const void *buf, size_t len, int flags = 0,
                const struct sockaddr *dest_addr = nullptr, socklen_t addrlen = 0)
    {
        auto ret = ::sendto(sock_,buf,len,flags,dest_addr,addrlen);
        if(-1 == ret){
            // -1 if an error occurred.  In the event of an error,  errno is set to indicate the error.
            if(EINTR != errno && 
                EAGAIN != errno && 
                EWOULDBLOCK != errno){
                close();
            }
            X_CHECK(false,-1);
        }else{
            // return the number of bytes received
            return ret;
        }
    }
    uint32_t get_recv_buf_size(){
        uint32_t real = 0;
        socklen_t len = sizeof(uint32_t);

        auto ret = getsockopt(sock_,SOL_SOCKET,SO_RCVBUF,(char*)&real,&len);
        X_CHECK(-1 != ret,-1);
        return real;
    }
    bool set_recv_buf_size(uint32_t buf_size){
        auto ret = setsockopt(sock_,SOL_SOCKET,SO_RCVBUF,(char*)&buf_size,sizeof(buf_size));
        X_CHECK_RET_BOOL(-1 != ret);

        uint32_t real = get_recv_buf_size();
        if(real != buf_size){
            printf("set_recv_buf_size err,set=%u,real=%u \n",buf_size,real);
        }
        return true;
    }
public:
    int32_t sock_{-1};
};

#define MAX_EP_EVT_CNT 4096
class xevent{
public:
    virtual ~xevent(){}
    virtual void handle_evt(uint32_t evt_type){}
    virtual int32_t get_fd(){return invalid_sock;}
};

class xepoll{
public:
    bool init(){
        epfd_ = epoll_create(MAX_EP_EVT_CNT);
        X_CHECK_RET_BOOL(invalid_sock != epfd_);
        memset(&events_,0,sizeof(events_));
        return true;
    }

    bool remove(xevent *evt){
        auto ret = epoll_ctl(epfd_,EPOLL_CTL_DEL,evt->get_fd(),nullptr);
        X_CHECK_RET_BOOL(0 == ret);
        return true;
    }

    bool insert(uint32_t evt_type,xevent *evt){
        epoll_event ev = {evt_type,evt};
        auto fd = evt->get_fd();
        auto ret = epoll_ctl(epfd_,EPOLL_CTL_ADD,fd,&ev);
        return true;
    }

    void wait(int timeout = -1){
        int cnt = epoll_wait(epfd_,events_,MAX_EP_EVT_CNT,timeout);
        for(int i = 0;i < cnt; ++i){
            auto event = (xevent*)(events_[i].data.ptr);
            event->handle_evt(events_[i].events);
        }
    }
public:
    epoll_event events_[MAX_EP_EVT_CNT];
    int32_t epfd_{-1};
};

