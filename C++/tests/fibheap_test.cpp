#include <bits/stdc++.h>
#include <gtest/gtest.h>
#include "fibheap.h"

using namespace std;

TEST(FibHeap, EmptyTest) {
	FibonacciHeap heap;
	ASSERT_TRUE(heap.empty());
	ASSERT_EQ(heap.size(), 0);
}

TEST(FibHeap, PushTest) {
	FibonacciHeap heap;
	heap.push("a", 5);
	heap.push("b", -5);
	heap.push("c", 0);

	ASSERT_EQ(heap.size(), 3);
	ASSERT_FALSE(heap.empty());
	ASSERT_EQ(heap.top(), "b");
}

TEST(FibHeap, PopTest) {
	FibonacciHeap heap;
	heap.push("a", 5);
	heap.push("b", -5);
	heap.push("c", 0);

	ASSERT_EQ(heap.top(), "b");
	
	heap.pop();

	ASSERT_EQ(heap.size(), 2);
	ASSERT_EQ(heap.top(), "c");
}

TEST(FibHeap, DecreaseKeyTest) {
	FibonacciHeap heap;
	for (int i = 0; i < 10; i++) {
		heap.push(to_string(i), i);
	}

	string key = "3";
	heap.decreaseKey(key, -100);

	ASSERT_EQ(heap.size(), 10);
	ASSERT_EQ(heap.top(), key);
}

TEST(FibHeap, IntegrationTest) {
	FibonacciHeap heap(true);
	for (int i = 0; i < 100; i++) {
		heap.push(to_string(i), i);
	}

	ASSERT_EQ(heap.size(), 100);
	ASSERT_EQ(heap.top(), "99");
	
	heap.pop();

	ASSERT_EQ(heap.top(), "98");

	heap.decreaseKey("50", 200);
	ASSERT_EQ(heap.top(), "50");
	heap.pop();

	heap.decreaseKey("0", 200);
	ASSERT_EQ(heap.top(), "0");
	heap.pop();
	
	heap.decreaseKey("98", 200);
	ASSERT_EQ(heap.top(), "98");
	heap.pop();
	
	heap.decreaseKey("9", 200);
	ASSERT_EQ(heap.top(), "9");
}

int main(int argc, char ** argv) {
	testing::InitGoogleTest(&argc, argv);
	return RUN_ALL_TESTS();
}
