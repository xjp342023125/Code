// c11_smart_ptr_test.cpp : Defines the entry point for the console application.
//

#include "stdafx.h"
#include <memory>
#include <iostream>
#include <vector>
#include <list>
#include <algorithm>
using namespace std;


class A
{
public:
	A()
	{
		cout << "new A";
	}
	~A()
	{
		cout << "del A";
	}
};

class B :public A
{
public:
	B()
	{
		cout << "new B";
	}
	~B()
	{
		cout << "del B";
	}
};

void p_vec(vector<int> &vec)
{
	cout << "====" << endl;

	for (auto i : vec)
	{
		cout << i << endl;
	}
}

void test_reverse_it()
{
	
	vector<int> vec{ 1,2,3,4,5 };
	p_vec(vec);

	vector<int>::reverse_iterator rit = find(vec.rbegin(), vec.rend(), 3);
	//vec.insert(rit.base(), 999);
	vec.erase((++rit).base());
	p_vec(vec);
};

int main()
{
	test_reverse_it();
	string ABBA("1234");
	string BaaB(ABBA);
	{
		cout << std::is_convertible<B*, A*>::value;
		std::shared_ptr<B> bbb(new B);
		std::shared_ptr<A> aaa(bbb);

		std::shared_ptr<A> aaa2 = bbb;
	}
	{
		shared_ptr<A> aaa(new A);
		//shared_ptr<B> bbb(aaa); // ÎÞ·¨×ª»»
	}
	
    return 0;
}

