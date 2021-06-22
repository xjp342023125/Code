
#include <pthread.h>
#include <stdio.h>
#include <thread>
#include <vector>
using namespace std;

int Global;

void *Thread1(void *x)
{
	Global++;
	return NULL;
}

__attribute__((no_sanitize_thread)) void *Thread2(void *x)
{
	Global--;
	return NULL;
}

void test1(){
	pthread_t t[2];
	pthread_create(&t[0], NULL, Thread1, NULL);
	pthread_create(&t[1], NULL, Thread2, NULL);
	pthread_join(t[0], NULL);
	pthread_join(t[1], NULL);
}
/////////////////////////////////////////////////////////
#define NO_TSAN __attribute__((no_sanitize_thread))

class XX
{
public:
	NO_TSAN void add();
	NO_TSAN void sub();

	int a;
};


void XX::add()
{
	++a;
}

void XX::sub()
{
	--a;
}

void test2(){
	XX x;

	thread t1 = thread(&XX::add,&x);
	thread t2 = thread(&XX::sub,&x);
	t1.join();
	t2.join();
}
/////////////////////////////////////////////////////////
int g_x3 = 0;

inline void do_3(int &val,int diff)
{
	val += diff;
}


inline void do_4(int &val,int diff)
{
	val += diff;
}
//__attribute__((no_sanitize_thread)) 
void add3(){
	do_3(g_x3,10);
}
//__attribute__((no_sanitize_thread)) 
void sub3(){
	do_4(g_x3,-10);
}
void test3(){
	thread t1 = thread(&add3);
	thread t2 = thread(&sub3);
	t1.join();
	t2.join();
}
///////////////////////////
shared_ptr<vector<int>> g_xxx4;
void xxx4_1(){
	if(g_xxx4){
		printf("%d\n",g_xxx4->size());
	}

	std::this_thread::sleep_for(std::chrono::seconds(3));

}

void xxx4_2(){
	std::this_thread::sleep_for(std::chrono::seconds(1));
	if(g_xxx4)
		g_xxx4 = nullptr;
	g_xxx4 = make_shared<vector<int>>();

	std::this_thread::sleep_for(std::chrono::seconds(3));
}
void test4(){
	g_xxx4 = nullptr;
	g_xxx4 = make_shared<vector<int>>();
	thread t1 = thread(&xxx4_1);
	thread t2 = thread(&xxx4_2);
	t1.join();
	t2.join();
}
int main()
{
	
	test4();
}
