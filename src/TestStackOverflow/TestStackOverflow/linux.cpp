#include <stdio.h>
#include <string.h>
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

// 有漏洞的函数
void good_func(const char *p){
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