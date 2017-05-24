// c11_smart_ptr_test.cpp : Defines the entry point for the console application.
//

#include "stdafx.h"
#include <memory>
#include <iostream>
#include <vector>
#include <list>
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
int main()
{
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

