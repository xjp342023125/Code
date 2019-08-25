#pragma once
#include <iostream>
#include <vector>
#include <algorithm>
#include <condition_variable>
#include <thread>
#include <chrono>
#include <queue>
using namespace std;

static void test_detach_thread_func()
{
	for (auto i = 0; i < 3;++i)
	{
		std::this_thread::sleep_for(std::chrono::seconds(1));
		cout << i<<"  "<< this_thread::get_id() << endl;
	}
}
static void test_detach()
{
	{
		cout << "main:  " << this_thread::get_id() << endl;
		thread t(test_detach_thread_func);
		t.detach();//要有这句
	}
	std::this_thread::sleep_for(std::chrono::seconds(10));
}
//==========================================
static void test_joinble_thread_func()
{
	std::this_thread::sleep_for(std::chrono::seconds(1));
	cout << "test_joinble_thread_func end  " << this_thread::get_id() << endl;
}
static void test_joinble()
{
	cout << "==================" << endl;
	thread t(test_joinble_thread_func);
	cout << t.joinable() << endl;
	std::this_thread::sleep_for(std::chrono::seconds(2));
	cout << t.joinable() << endl;
	t.join();
	//t.join();//peng!
	cout << t.joinable() << endl;

}

static void test_lock_multi_lock()
{
	mutex a, b;
	unique_lock<mutex > loacka(a,defer_lock);
	unique_lock<mutex > loackb(b,defer_lock);
	std::lock(a, b);
	cout << "lock ok" << endl;
}

static void test_dead_lock()
{
	mutex a1, a2;

	auto thread_func_a = [](mutex &a, mutex &b,const char *p) {
		while (1)
		{
			a.lock();
			b.lock();
			cout << p << endl;
			b.unlock();
			a.unlock();
		}
	};


	
	thread thread2(thread_func_a, std::ref(a2), std::ref(a1),"bbb");
	thread thread1(thread_func_a, std::ref(a1), std::ref(a2), "aaa");
	thread1.join();
	thread2.join();
}

static void test_std_lock()
{
	mutex a1, a2;

	auto thread_func_a = [](mutex &a, mutex &b, const char *p) {
		while (1)
		{
			unique_lock<mutex> locka(a, defer_lock);
			unique_lock<mutex> lockb(b, defer_lock);
			std::lock(locka, lockb);
			cout << p << endl;
			
		}
	};



	thread thread2(thread_func_a, std::ref(a2), std::ref(a1), "bbb");
	thread thread1(thread_func_a, std::ref(a1), std::ref(a2), "aaa");
	thread1.join();
	thread2.join();
}

static void test_std_call_once()
{
	auto f = []() {
		printf("cccc"); 
	};
	std::once_flag once;
	std::call_once(once, f);
	std::call_once(once, f);
}



static std::mutex for_cond;
static std::condition_variable cond;
static std::queue<int> q;

void push_thread()
{
	cout << __FUNCTION__ << endl;
	int i = 0;
	while (true)
	{
		std::this_thread::sleep_for(std::chrono::milliseconds(100));
		std::lock_guard<std::mutex> lg(for_cond);
		q.push(++i);
		cout << "push " << i << endl;
		cond.notify_one();
	}
}

void pop_thread()
{
	cout << __FUNCTION__ << endl;
	while (true)
	{
		std::unique_lock<std::mutex> lg(for_cond);
		// l:解锁lg
		// 2:阻塞住
		// 3:当其他线程 调用过notify_one后
		// 4:线程苏醒，重新锁住lg
		// 5:检查后面的函数（q是否我空）,条件不满足时，继续跳到1。条件满足则继续执行。


		cond.wait(lg, [] {return !q.empty(); });
		auto val_queue = q.front();
		q.pop();
		lg.unlock();
		cout << "pop "<<val_queue << endl;
	}
}
void test_condition_variable()
{
	thread t1(push_thread);
	thread t2(pop_thread);
	t1.join();
	t2.join();
}
static void test_thread()
{
	test_condition_variable();
	test_std_call_once();
	test_std_lock();// 使用 std::lock 避免死锁
	test_dead_lock();// 死锁
	test_lock_multi_lock();
	test_joinble();
	test_detach();
	
}