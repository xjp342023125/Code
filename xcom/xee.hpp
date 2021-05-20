#pragma once
#include "xcom.hpp"

class xepoll;
class xevent{
public:
    virtual ~xevent(){}
    virtual void handle_evt(uint32_t evt_type){}
    virtual int32_t get_fd(){return invalid_sock;}

public:
    xepoll *ee_ = nullptr;
};

class xepoll{
public:
#define MAX_EP_EVT_CNT 4096
    bool init(){
        epfd_ = epoll_create(MAX_EP_EVT_CNT);
        X_CHECK_RET_BOOL(invalid_sock != epfd_);
        memset(&events_,0,sizeof(events_));
        return true;
    }

    bool remove(xevent *evt){
        auto ret = epoll_ctl(epfd_,EPOLL_CTL_DEL,evt->get_fd(),nullptr);
        X_CHECK_0(ret,false);
        return true;
    }

    bool insert(uint32_t evt_type,xevent *evt){
        epoll_event ev = {evt_type,evt};
        auto fd = evt->get_fd();
        
        auto ret = epoll_ctl(epfd_,EPOLL_CTL_ADD,fd,&ev);
        X_CHECK_0(ret,false);
        evt->ee_ = this;
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