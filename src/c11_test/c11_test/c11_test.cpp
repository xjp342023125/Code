// c11_test.cpp : Defines the entry point for the console application.
//

#include "stdafx.h"
#include "test_temp.hpp"
#include "test_lambda.hpp"
#include "test_thread.hpp"
#include "test_move.hpp"
#include "test_async.hpp"
#include "future.hpp"
int main()
{
	test_future();
	test_async();
	test_move();
	test_lambda();
	test_temp();
	test_thread();
    return 0;
}

