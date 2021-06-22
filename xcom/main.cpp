#include "xall.hpp"




int test_listen();
void test_open_file();//打开文件到上限 ，ulimit 设置
int test_self_con();
int test_reuse(int argc, char* argv[]);
int main(int argc, char* argv[])
{
    //test_open_file();
    //test1();
    //test2();
    //test_listen();
    //test_self_con();
    test_reuse(argc,argv);
    return 0;
}
int test_reuse(int argc, char* argv[]){
    xsock x;
    bool ret = x.tcp_create();
    X_CHECK(ret,-1);

    if(2 == argc){
        ret = x.reuse_addr(1);
        X_CHECK(ret,-1);

        ret = x.reuse_port(1);
        X_CHECK(ret,-1);

        printf("re use\n");
    }

    ret = x.xbind("0.0.0.0",9999);
    X_CHECK(ret,-1);

    ret = x.xlisten();
    X_CHECK(ret,-1);
    printf("ok");
    while (1)
    {
        /* code */
        sleep_ns(1000000);
    }
    
    return 0;
}
class x_sock_event:public xevent{
public:
    virtual ~x_sock_event(){}
    virtual void handle_evt(uint32_t evt_type){
        if(EPOLLIN == evt_type)
        {
            char sz[100];
            int ret = recv(fd_.sock_, sz, 100, 0);
            if (0 == ret)
            {
                fd_.close();
                delete this;
                printf("closed \n");
            }
            else
            {
                printf("recv ret = %d\n", ret);
            }
        }
        if(EPOLLOUT == evt_type)
        {
            printf("connected!\n");
            auto local_addr = fd_.get_peer_addr();
            auto peer_addr = fd_.get_peer_addr();
            printf("local=%s\n",get_addr_str(&local_addr).c_str());
            printf("peer=%s\n",get_addr_str(&peer_addr).c_str());
        }
        
        

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

        ret = ee_->insert(EPOLLIN|EPOLLOUT,xse);
        X_CHECK_NO_RET(ret);
        printf("xaccept ok\n");

    }
    virtual int32_t get_fd(){return fd_.sock_;}

    xsock fd_ ;
};

int test_listen(){
    bool ret = false;

    xepoll ep;
    ep.init();
    x_listener l;

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

int test_self_con()
{
    xepoll ep;
    ep.init();

    x_sock_event l;

    auto ret = l.fd_.tcp_create();
    X_CHECK(ret,-1);

    ret = l.fd_.xbind("127.0.0.1",4444);
    X_CHECK(ret,-1);

    ret = ep.insert(EPOLLIN|EPOLLOUT|EPOLLET,&l);
    X_CHECK(ret,-1);

    ret = l.fd_.connect("127.0.0.1",4444);
    X_CHECK(ret,-1);

    while (true)
    {
        ep.wait(1000);
        /* code */
    }

    return 0;
}


void test_open_file(){
    for (size_t i = 0; i < 100; i++)
    {
        xsock *s = new xsock;
        auto ret = s->tcp_create();
        printf("tcp c ret =%u\n",ret);
    }
    exit(0);
    
}