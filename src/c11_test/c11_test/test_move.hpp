#pragma once
#include <iostream>
#include <vector>
#include <algorithm>
#include <map>
#include <condition_variable>
#include "test_common.hpp"
using namespace std;



static A make_A()
{
	cout << ""<<endl;
	return A();
}

static void test_move()
{
	A aa = make_A();
	A bb = std::move(aa);
	A ee(bb);
	A cc(std::move(aa));
	A dd(make_A());
}