#include <gtest/gtest.h>
#include "fibheap.h"

using namespace std;

TEST(FibHeap, EmptyTest) {
	FibonacciHeap heap;
	ASSERT_TRUE(heap.empty());
}

int main(int argc, char ** argv) {
	testing::InitGoogleTest(&argc, argv);
	return RUN_ALL_TESTS();
}
