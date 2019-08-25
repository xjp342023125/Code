#pragma once
#include <iostream>
#include <vector>
#include <algorithm>
#include <map>
#include <memory>
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