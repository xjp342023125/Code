#include <stdio.h>
#include <tchar.h>
#pragma pack(1)
//���Ĺ���Ĳ���
template<int distance>
struct sStackOverFlow{
	char local[distance];
	void* addrRet;// ��������ַ
};
#pragma pack()

// ��©���ĺ���
void good_func(const char *p){
	int a = 111;
	char sz[16];
	int b = 222;
	printf("my func dis=%u \n", (char*)&p - (char*)sz);
	strcpy(sz, p);
}

// �������
void hack_func(){
	printf("hack func ");
}

int _tmain(int argc, _TCHAR* argv[]){
	sStackOverFlow<32-4> para;//ħ��һ�����
	memset(&para, 0xf, sizeof(para));
	para.addrRet = &hack_func;
	good_func( (char*)&para);// ������������
	return 0;
}

