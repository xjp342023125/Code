#pragma once
#include <iostream>
#include <vector>
#include <algorithm>
#include <condition_variable>
#include <thread>
#include <chrono>
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
		t.detach();//Ҫ�����
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
static void test_thread()
{
	test_std_lock();// ʹ�� std::lock ��������
	test_dead_lock();// ����
	test_lock_multi_lock();
	test_joinble();
	test_detach();
	
}