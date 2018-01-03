#pragma once
#include "test_common.hpp"

static void test_default_delete()
{
	std::shared_ptr<A> a_ptr(new A[3], std::default_delete<A[]>());
}