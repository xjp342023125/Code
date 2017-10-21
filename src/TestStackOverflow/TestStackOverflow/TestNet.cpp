#include <stdio.h>
#include <tchar.h>
#pragma pack(1)
//精心构造的参数
template<int distance>
struct sStackOverFlow{
	char local[distance];
	void* addrRet;// 恶意代码地址
};
#pragma pack()

// 有漏洞的函数
void good_func(const char *p){
	int a = 111;
	char sz[16];
	int b = 222;
	printf("my func dis=%u \n", (char*)&p - (char*)sz);
	strcpy(sz, p);
}

// 恶意代码
void hack_func(){
	printf("hack func ");
}

int _tmain(int argc, _TCHAR* argv[]){
	sStackOverFlow<32-4> para;//魔术一会解释
	memset(&para, 0xf, sizeof(para));
	para.addrRet = &hack_func;
	good_func( (char*)&para);// 调用正常函数
	return 0;
}

