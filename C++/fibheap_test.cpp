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

	ASSERT_EQ(heap.size(), 3);
	ASSERT_EQ(heap.top(), "b");
	
	heap.pop();

	ASSERT_EQ(heap.size(), 2);
	ASSERT_EQ(heap.top(), "c");

	heap.pop();

	ASSERT_EQ(heap.size(), 1);
	ASSERT_EQ(heap.top(), "a");
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

	heap.decreaseKey("5", 101);
	ASSERT_EQ(heap.top(), "5");

	heap.decreaseKey("6", 102);
	ASSERT_EQ(heap.top(), "6");
}

TEST(FibHeap, CtorTests) {
	FibonacciHeap minHeap;
	for (int i = 0; i < 10; i++) {
		minHeap.push(to_string(i), i);
	}

	minHeap.pop();
	FibonacciHeap minHeapCopy(minHeap);

	ASSERT_EQ(minHeapCopy.size(), 9);
	ASSERT_EQ(minHeap.top(), "1");
	ASSERT_EQ(minHeapCopy.top(), minHeap.top());

	minHeapCopy.pop();
	
	// changes to copy are independent from original
	ASSERT_EQ(minHeapCopy.size(), minHeap.size() - 1);
	ASSERT_EQ(minHeap.top(), "1");
	ASSERT_EQ(minHeapCopy.top(), "2");

	// assignment op test
	FibonacciHeap minHeapCopyCopy = minHeapCopy;
	minHeapCopyCopy = minHeapCopy;
	ASSERT_EQ(minHeapCopyCopy.size(), minHeapCopy.size());
}

int main(int argc, char ** argv) {
	testing::InitGoogleTest(&argc, argv);
	return RUN_ALL_TESTS();
}
