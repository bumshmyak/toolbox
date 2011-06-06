#include <cassert>
#include <vector>
#include <algorithm>
#include <iostream>

#include "graph/heap.h"
#include "gtest/gtest.h"

using std::vector;

TEST(GraphHeapTest, GraphVectorHeap) {
  const int HEAP_SIZE = 10;
  GraphVectorHeap<int> dummy_heap(HEAP_SIZE);
  EXPECT_TRUE(dummy_heap.empty());

  dummy_heap.push(0, 1);
  EXPECT_FALSE(dummy_heap.empty());

  EXPECT_EQ(0, dummy_heap.top());
  EXPECT_EQ(1, dummy_heap.get(0));
  dummy_heap.pop();
  EXPECT_TRUE(dummy_heap.empty());

  vector<int> keys(HEAP_SIZE);
  for (int i = 0; i < keys.size(); ++i) {
    keys[i] = i;
  }

  srand(42);
  std::random_shuffle(keys.begin(), keys.end());

  for (int i = 0; i < keys.size(); ++i) {
    dummy_heap.push(keys[i], i);
    EXPECT_FALSE(dummy_heap.empty());
    EXPECT_EQ(keys[0], dummy_heap.top());
    EXPECT_EQ(i, dummy_heap.get(keys[i]));
  }

  for (int i = 0; i < keys.size(); ++i) {
    dummy_heap.decrease_key(keys[i], i - 1);
    EXPECT_EQ(keys[0], dummy_heap.top());
  }

  for (int i = 0; i < keys.size(); ++i) {
    EXPECT_EQ(keys[i], dummy_heap.top());
    EXPECT_EQ(i -  1, dummy_heap.get(keys[i]));
    dummy_heap.pop();
  }

  EXPECT_TRUE(dummy_heap.empty());
}

TEST(GraphHeapTest, GraphKaryHeapStress) {
  const int HEAP_SIZE = 1000;
  const int MAX_KEY_VALUE = 1000000;

  vector<int> keys(HEAP_SIZE);
  for (int i = 0; i < keys.size(); ++i) {
    keys[i] = i;
  }

  srand(42);
  std::random_shuffle(keys.begin(), keys.end());

  vector<int> values(HEAP_SIZE);
  for (int i = 0; i < values.size(); ++i) {
    values[i] = rand() % MAX_KEY_VALUE;
  }

  GraphVectorHeap<int> dummy_heap(HEAP_SIZE);
  GraphKaryHeap<int> kary_heap(HEAP_SIZE);

  for (int i = 0; i < HEAP_SIZE; ++i) {
    dummy_heap.push(keys[i], values[i]);
    kary_heap.push(keys[i], values[i]);
    EXPECT_EQ(dummy_heap.top(), kary_heap.top());
    EXPECT_EQ(dummy_heap.get(dummy_heap.top()), kary_heap.get(kary_heap.top()));
  }

  for (int i = 0; i < HEAP_SIZE; ++i) {
    int drop_down = rand() % MAX_KEY_VALUE;
    dummy_heap.decrease_key(keys[i], dummy_heap.get(keys[i]) - drop_down);
    kary_heap.decrease_key(keys[i], kary_heap.get(keys[i]) - drop_down);
    EXPECT_EQ(dummy_heap.top(), kary_heap.top());
    EXPECT_EQ(dummy_heap.get(dummy_heap.top()), kary_heap.get(kary_heap.top()));
  }

  for (int i = 0; i < HEAP_SIZE; ++i) {
    EXPECT_EQ(dummy_heap.top(), kary_heap.top());
    EXPECT_EQ(dummy_heap.get(dummy_heap.top()), kary_heap.get(kary_heap.top()));
    dummy_heap.pop();
    kary_heap.pop();
  }

  EXPECT_TRUE(dummy_heap.empty());
  EXPECT_TRUE(kary_heap.empty());
}
