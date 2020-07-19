#include "xcom.hpp"


void test1(){
    xsock x;
    x.xtcp_create();
    //x.xset_nonblock(1);
    x.xconnect("185.199.109.153",80);
    cout<<"conn! "<<endl;
    cout<<"recv buf = "<<x.get_recv_buf_size()<<endl;
    x.set_recv_buf_size(32*1024*1024);
    x.xsendto("abcddddddddddddddddddddddddddddddd",4000);
    cout<<"send!"<<endl;

    char sss[2000]={0};
    auto ret = x.xrecv_from(sss,2000);
    cout<<sss;

}
int main(int argc, char* argv[])
{
    test1();
    return 0;
}