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
		t.detach();//ÒªÓÐÕâ¾ä
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

static void test_thread()
{
	test_joinble();
	test_detach();
	
}