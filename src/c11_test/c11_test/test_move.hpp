#pragma once
#include <iostream>
#include <vector>
#include <algorithm>
#include <map>
#include <condition_variable>
using namespace std;

class A
{
public:
	A()
	{
		cout << "new A" << endl;
	}
	~A()
	{
		cout << "del A" << endl;
	}

	A(const A& r)
	{
		cout << "copy A" << endl;
	}
	A(const A&& r)
	{
		cout << "&& copy A" << endl;
	}

	A& operator=(const A& r)
	{
		cout << "= A" << endl;
	}
	A& operator=(const A&& r)
	{
		cout << "&& = A" << endl;
	}
	string name;
};

static A make_A()
{
	cout << ""<<endl;
	return A();
}

static void test_move()
{
	A aa = make_A();
	A bb = std::move(aa);

	A cc(std::move(aa));
	A dd(make_A());
}