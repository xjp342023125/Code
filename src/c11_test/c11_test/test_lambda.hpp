#pragma once
#include <iostream>
using namespace std;

static void test_lambda1()
{
	[](int ccc) {
		cout << 111<<ccc<<endl;
	}(333);
}
static void test_lambda()
{
	test_lambda1();
}