#pragma once
#include <future>
#include <iostream>
#include <thread>
using namespace std;
static int get_int()
{
	cout <<__FUNCTION__<< this_thread::get_id() << endl;

	return time(NULL);
}

static int get_int_ex(int a)
{
	return a*a;
}
static void  test_future()
{
	std::future<int> answer = std::async(get_int);

	cout << answer.get() << "\n";;

	{
		std::future<int> answer = std::async(get_int_ex,10);

		cout << answer.get()<<"\n";
	}
	cout << this_thread::get_id() << endl;
}