#include <stdio.h>
#include <string.h>
#include <string>
using namespace std;
#pragma pack(1)
//精心构造的参数
template<int distance>
struct sStackOverFlow{
        char local[distance];
        void* addrRet;// 恶意代码地址
};
#pragma pack()

// 恶意代码
void  hack_func(){
        printf("hack func ");
		getchar();
}

string g_func_name;
class check_stack {
public:
	check_stack(const char *f) {
		g_func_name = f;
		val_ = 0x11223344;
	}

	~check_stack() {
		if (0x11223344 != val_)	{
			printf("stack err,func=%s", g_func_name.c_str());
		}
	}
	int val_;
};
#define CHECK_STACK		check_stack a(__FUNCTION__);

// 有漏洞的函数
void good_func(const char *p){
		CHECK_STACK
        int a = 111;
        char sz[16];
        int b = 222;
        printf("my func  \n");
        strcpy(sz, p);
}

int main(){
        sStackOverFlow<0x28> para;//魔术一会解释
        memset(&para, 0xf, sizeof(para));
        para.addrRet =(void*) &hack_func;
        printf("%x\n",para.addrRet);
        good_func((char*)&para);// 调用正常函数
        return 0;
}
//g++ -g3 -O0 -fno-stack-protector 1.cpp