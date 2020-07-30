#include "xcom.hpp"


void test1(){
    xsock x;
    x.xtcp_create();
    //x.xset_nonblock(1);
    x.connect("185.199.109.153",80);
    cout<<"conn! "<<endl;
    cout<<"recv buf = "<<x.get_recv_buf_size()<<endl;
    x.set_recv_buf_size(32*1024*1024);
    x.sendto("abcddddddddddddddddddddddddddddddd",4000);
    cout<<"send!"<<endl;

    char sss[2000]={0};
    auto ret = x.recv_from(sss,2000);
    cout<<sss;

}

void test2()
{
    xepoll ep;
    ep.init();
}


void test3(){
    
}
int main(int argc, char* argv[])
{
    //test1();
    //test2();
    return 0;
}