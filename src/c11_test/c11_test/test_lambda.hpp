#pragma once
#include <iostream>
#include <vector>
#include <algorithm>
#include <map>
#include <condition_variable>
using namespace std;

static void test_lambda1()
{
	[](int ccc) {
		cout << 111<<ccc<<endl;
	}(333);
}

static void test_lambda2()
{
	std::vector<int> data{ 1,2,3,4,555 };
	for_each(data.begin(), data.end(), [](int i) { cout << i << endl; });
}
static void test_lambda3()
{
	condition_variable cond;
}

// use loacl var by copy
std::function<int(int)> make_iffseter(int off)
{
	//return [](int j) {return off + j;};// error C3493: 'off' cannot be implicitly captured because no default capture mode has been specified
	return [=](int j) {return off + j; };
}
static void test_lambda4()
{
	auto off_30 = make_iffseter(30);
	cout << off_30(100) << endl;
}

// use loacl var by ref
static void test_lambda5()
{
	int off = 42;
	auto f1 = [&](int j) { return off + j; };
	off = 100;
	auto f2 = [&](int j) { return off + j; };
	cout << f1(10) << endl;
	cout << f1(10) << endl;
}

// some copy,some ref
static void test_lambda6()
{
	int a = 1, b = 10, c = 100;
	auto f = [=, &b, &c] {return a + b + c; };
	b = 20;
	cout << f() << endl;
}

// use this
static void test_lambda7()
{
	auto do_vec = [](vector<int> &vec) {
		for_each(vec.begin(), vec.end(), [](int &i) {i += 10; });
	};

	vector<int> iii{ 1,2,3,4 };
	do_vec(iii);
}
static void map_merge(map<int, int>& all, map<int, int>& toadd)
{
	for_each(toadd.begin(), toadd.end(),
		[&](map<int, int>::reference it) {all[it.first] = all[it.first] + it.second; });
}
static void test_map_merge()
{
	map<int, int> all;
	map<int, int> toadd{ {1,1},{ 2,2 } ,{ 3,3 } };
	map_merge(all, toadd);
	map_merge(all, toadd);
	toadd.insert({ 100,100 });
	map_merge(all, toadd);

}
static void test_lambda()
{
	test_map_merge();
	test_lambda1();
	test_lambda2();
	test_lambda4();
	test_lambda5();
	test_lambda6();
	test_lambda7();
}