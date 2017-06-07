#pragma once

#include <iostream>
using namespace std;
template<int n>
struct Factorial
{
	enum e_val
	{
		val = n*Factorial<n - 1>::val
	};
};

template<>
struct Factorial<0>
{
	enum e_val
	{
		val = 1
	};
};



int test_temp()
{
	//test_new_err_handle();
	cout << Factorial<3>::val;
	return 0;
}