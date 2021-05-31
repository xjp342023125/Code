#pragma once
#include "xcom.hpp"

static sockaddr_in fill_addr(const char *ip, port_t port)
{
	sockaddr_in addr;
	memset(&addr, 0, sizeof(sockaddr_in));
	addr.sin_family = AF_INET;
	addr.sin_port = htons(port);
	addr.sin_addr.s_addr = inet_addr(ip);
	return addr;
}
// in_addr_t uint32_t
static in_addr_t get_addr_ip(sockaddr_in *addr){
    return addr->sin_addr.s_addr;
}
// in_port_t uint16_t
static in_port_t get_addr_port(sockaddr_in *addr){
    return htons(addr->sin_port);
}

static string get_addr_str(sockaddr_in* addr){
    char sz[100]={0};
    inet_ntop(AF_INET,(void*)&addr->sin_addr,sz,100);
    string ret = string(sz)+to_string(get_addr_port(addr));
    return  ret;
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
        X_CHECK_0(ret,false);
		return true;
	}

	bool xlisten(int iBacklog=20)
	{
		int ret = ::listen(sock_, iBacklog);
        X_CHECK_0(ret,false);
		return true;
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
        X_CHECK_0(ret,false);
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

    sockaddr_in get_local_addr(){
        sockaddr_in addr;
        
        socklen_t len = sizeof(addr);
        auto ret = getsockname(sock_,(sockaddr*)&addr,&len);
        if(0 != ret){
            memset(&addr,0,sizeof(addr));
        }
        return addr;
    }
    
    sockaddr_in get_peer_addr(){
        sockaddr_in addr;
        
        socklen_t len = sizeof(addr);
        auto ret = getpeername(sock_,(sockaddr*)&addr,&len);
        if(0 != ret){
            memset(&addr,0,sizeof(addr));
        }
        return addr;
    }

    bool reuse_addr(){
        return true;
    }
public:
    int32_t sock_{-1};
};