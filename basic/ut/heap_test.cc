#include "basic/heap.h"

#include <cassert>
#include <vector>
#include <algorithm>
#include <iostream>
#include <queue>
#include <functional>

#include "gtest/gtest.h"

using std::priority_queue;
using std::vector;

TEST(HeapTest, PopAfterPush) {
  const int HEAP_SIZE = 100000;
  const int MAX_KEY_VALUE = 1000000000;

  vector<int> keys(HEAP_SIZE);
  for (int i = 0; i < keys.size(); ++i) {
    keys[i] = rand() % MAX_KEY_VALUE;
  }

  // min heaps
  KaryHeap<int> kary_heap;
  FibonacciHeap<int> fibonacci_heap;
  priority_queue<int, std::vector<int>, std::greater<int> > correct_heap;

  for (int i = 0; i < HEAP_SIZE; ++i) {
    correct_heap.push(keys[i]);
    kary_heap.push(keys[i]);
    fibonacci_heap.push(keys[i]);

    EXPECT_EQ(correct_heap.top(), kary_heap.top());
    EXPECT_EQ(correct_heap.top(), fibonacci_heap.top());

    EXPECT_EQ(correct_heap.size(), kary_heap.size());
    EXPECT_EQ(correct_heap.size(), fibonacci_heap.size());
  }

  for (int i = 0; i < HEAP_SIZE; ++i) {
    EXPECT_EQ(correct_heap.size(), kary_heap.size());
    EXPECT_EQ(correct_heap.size(), fibonacci_heap.size());

    EXPECT_EQ(correct_heap.top(), kary_heap.top());
    EXPECT_EQ(correct_heap.top(), fibonacci_heap.top());

    correct_heap.pop();
    kary_heap.pop();
    fibonacci_heap.pop();
  }

  EXPECT_TRUE(correct_heap.empty());
  EXPECT_TRUE(kary_heap.empty());
  EXPECT_TRUE(fibonacci_heap.empty());
}

TEST(HeapTest, MixedPushPop) {
  const int ITERATION_COUNT = 10;
  const int MAX_PUSHED = 100;
  const int MAX_KEY_VALUE = 1000;

  // min heaps
  KaryHeap<int> kary_heap;
  FibonacciHeap<int> fibonacci_heap;
  priority_queue<int, std::vector<int>, std::greater<int> > correct_heap;

  for (int i = 0; i < ITERATION_COUNT; ++i) {
    int push_count = rand() % MAX_PUSHED + 1;
    int pop_count = rand() % push_count;
    for (int j = 0; j < push_count; ++j) {
      int element = rand() % MAX_KEY_VALUE;
      correct_heap.push(element);
      kary_heap.push(element);
      fibonacci_heap.push(element);
      EXPECT_EQ(correct_heap.top(), kary_heap.top());
      EXPECT_EQ(correct_heap.top(), fibonacci_heap.top());
    }

    for (int j = 0; j < pop_count; ++j) {
      EXPECT_EQ(correct_heap.top(), kary_heap.top());
      EXPECT_EQ(correct_heap.top(), fibonacci_heap.top());
      correct_heap.pop();
      kary_heap.pop();
      fibonacci_heap.pop();
    }
  }
}
