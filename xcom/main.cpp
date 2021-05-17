#include "xcom.hpp"

class x_sock_event:public xevent{
public:
    virtual ~x_sock_event(){}
    virtual void handle_evt(uint32_t evt_type){
        char sz[100];
        int ret = recv(fd_.sock_,sz,100,0);
        printf("recv ret = %d\n",ret);
        fd_.close();
        delete this;
    }
    virtual int32_t get_fd(){return fd_.sock_;}

    xsock fd_;
};

class x_listener:public xevent{
public:
    virtual ~x_listener(){}
    virtual void handle_evt(uint32_t evt_type){
        auto ret = fd_.xaccept();
        if(invalid_sock == ret){
            printf("xaccept err\n");
        }
        x_sock_event *xse = new x_sock_event;
        xse->fd_.sock_ = ret;

        ret = ep->insert(EPOLLIN|EPOLLOUT,xse);
        X_CHECK_NO_RET(ret);
        printf("xaccept ok\n");

    }
    virtual int32_t get_fd(){return fd_.sock_;}

    xsock fd_ ;
    xepoll *ep = nullptr;
};

int main(int argc, char* argv[])
{
    //test1();
    //test2();
    bool ret = false;

    xepoll ep;
    ep.init();
    x_listener l;

    l.ep = &ep;

    ret = l.fd_.tcp_create();
    X_CHECK(ret,-1);

    ret = l.fd_.xbind("0.0.0.0",4444);
    X_CHECK(ret,-1);

    ret = l.fd_.xlisten();
    X_CHECK(ret,-1);

    
    ret = ep.insert(EPOLLIN|EPOLLOUT,&l);
    X_CHECK(ret,-1);


    while (true)
    {
        ep.wait(1000);
        /* code */
    }
    
    return 0;
}