#include "basic/range_queries.h"

#include <cstdlib>
#include <iostream>

#include "gtest/gtest.h"

TEST(ElementwiseRangeProcessorTest, Basic) {
  const int element_count = 10;
  ElementwiseRangeQueryProcessor< SumTraits<int> >
      range_processor(element_count);

  // change one by one
  for (int element_index = 0; element_index < element_count; ++element_index) {
    range_processor.update(element_index, 1);
    for (int i = 0; i < element_count; ++i) {
      for (int j = i; j < element_count; ++j) {
        if ((i <= element_index) && (element_index <= j)) {
          EXPECT_EQ(1, range_processor.query(i, j));
        } else {
          EXPECT_EQ(0, range_processor.query(i, j));
        }
      }
    }
    range_processor.update(element_index, 0);
  }
}

TEST(FenwickTreeTest, ElementwiseStress) {
  const int test_count = 100;
  const int max_elements_count = 1000;
  const int max_value = 1000000;
  const int queries_count = 100;

  srand(42);
  for (int test = 0; test < test_count; ++test) {
    int n = rand() % max_elements_count + 1;

    FenwickTree< SumTraits<int> > fenwick_tree(n);
    ElementwiseRangeQueryProcessor< SumTraits<int> > dummy(n);

    for (int q = 0; q < queries_count; ++q) {
      int k = rand() % n;
      int v = rand() % max_value;
      dummy.update(k, v);
      fenwick_tree.update(k, v);
      int l = rand() % n;
      int r = l + rand() % (n - l);
      EXPECT_EQ(dummy.query(l, r), fenwick_tree.query(l, r));
    }
  }
}
