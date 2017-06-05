// c11_smart_ptr_test.cpp : Defines the entry point for the console application.
//

#include "stdafx.h"
#include <memory>
#include <iostream>
#include <vector>
#include <list>
#include <iterator>
#include <new>
#include <algorithm>

using namespace std;





void test_hide_func();
void test_cast();
void test_construct();
void test_place_new();
void test_destructor();
int main()
{
	string a = "1234";
	string b(a);
	cout << a.c_str();
	cout << b.c_str();

	vector<int> a1{ 1,2,2,34 };
	vector<int> a2{ 111,112,1134 };
	auto iii = std::remove(a1.begin(), a1.end(), 2);

	copy(a1.begin(), a1.end(), inserter(a2,a2.begin()));
	//test_hide_func();
	//test_construct();
	//test_place_new();
	//test_destructor();
	test_cast();
	
    return 0;
}
//=====================================================
template<class _Ty,class _Alloc = allocator<_Ty> >
class vectorex :public vector<_Ty, _Alloc>
{

};



class Array
{
public:
	class ArraySize
	{
	public:
		ArraySize(int n):m_n(n)
		{}

		int m_n;
	};

	Array(ArraySize size)
	{
		m_n = size.m_n;
	}
	int m_n;
};

void test_construct()
{
	Array a1(3);
	//if (a1 == 3)
	//if (3 == a1)
	{
	}
}
//=====================================================



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

	virtual void test_hide()const
	{
		cout << __FUNCTION__ << endl;
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

	virtual void test_hide()
	{
		cout << __FUNCTION__ << endl;
	}
};
void test_hide_func()
{
	A *p = new B;
	p->test_hide();
}

void test_empty_class_size()
{
	cout << sizeof(A);
}

void test_share_ptr_convert()
{
	{
		cout << std::is_convertible<B*, A*>::value;
		std::shared_ptr<B> bbb(new B);
		std::shared_ptr<A> aaa(bbb);

		std::shared_ptr<A> aaa2 = bbb;

		is_base_of<A, B>;
	}
	{
		shared_ptr<A> aaa(new A);
		//shared_ptr<B> bbb(aaa); // 无法转换
	}
}
//=====================================================

void *operator new(size_t, void *p)
{
	return p;
}
void test_place_new()
{
	char sz[100];
	A *p2 = new (sz) A;
	//delete p2;
}
//=====================================================
class AA
{
public:
	virtual ~AA()
	{

	}
	virtual void ccc() = 0;
	int a;
};

class BB :virtual public AA
{
public:
	virtual void ccc()
	{

	}
	int aa;
};

void test_cast()
{
	// 至少一个虚函数才可以
	AA *aa = new BB;
	BB* bb = dynamic_cast<BB*>(aa);
	cout << aa << endl;
	cout << dynamic_cast<BB*>(aa) << endl;

	// 虚继承，转化时，地址不一样了
	
}
//=====================================================
class AAA
{
public:
	virtual ~AAA()
	{
		throw "hhh";
	}
};

void test_destructor()
{
	try
	{
		AAA bb;
	}
	catch (const char *p)
	{
		cout << p << endl;
	}


	try
	{
		AAA aa;
		throw "ccc";
	}
	catch (const char *p)
	{
		cout << p << endl;
	}
}
////////////////////////////////////
class AA3
{
private:
	virtual ~AA3()
	{
		throw "hhh";
	}
};

void test_must_new()
{
	//AA3 aa3; 只能new出来
}