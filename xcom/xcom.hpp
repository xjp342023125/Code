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

#define X_CHECK_0(v,ret) \
    if(0 != (v)){printf("err!file:%s,line:%u,last err=%u \n",__FILE__,__LINE__,errno);return ret;}


#define X_CHECK_NO_RET(v) \
    if(0 == (v)){printf("err!file:%s,line:%u,last err=%u \n",__FILE__,__LINE__,errno);return ;}
#define X_CHECK_RET_BOOL(v) X_CHECK(v,false)
//////////////////////////////
#define invalid_sock -1
//////////////////////////////
static void sleep_ns(uint64_t ns){
    std::this_thread::sleep_for(std::chrono::nanoseconds(ns));
}





