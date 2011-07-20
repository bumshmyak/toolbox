#include "basic/range_queries.h"

#include <cstdlib>
#include <iostream>
#include <functional>

#include "gtest/gtest.h"

using std::cout;
using std::cin;
using std::endl;
using std::vector;

TEST(RangeQueriesTest, Elementwise) {
  const int ELEMENTS_COUNT = 10;
  
  ElementwiseRangeQueryProcessor< SumTraits<int> >
      sum_processor;

  sum_processor.init(ELEMENTS_COUNT);
  // change one by one
  for (int element_index = 0; element_index < ELEMENTS_COUNT; ++element_index) {
    sum_processor.update(element_index, 1);
    for (int i = 0; i < ELEMENTS_COUNT; ++i) {
      for (int j = i; j < ELEMENTS_COUNT; ++j) {
        if ((i <= element_index) && (element_index <= j)) {
          EXPECT_EQ(1, sum_processor.query(i, j));
        } else {
          EXPECT_EQ(0, sum_processor.query(i, j));
        }
      }
    }
    sum_processor.update(element_index, 0);
  }

  ElementwiseRangeQueryProcessor< MinTraits<int> >
      min_processor;
  
  min_processor.init(ELEMENTS_COUNT);
  
  for (int element_index = 0; element_index < ELEMENTS_COUNT; ++element_index) {
    min_processor.update(element_index, 1);
  }

  for (int element_index = 0; element_index < ELEMENTS_COUNT; ++element_index) {
    min_processor.update(element_index, 0);
    for (int i = 0; i < ELEMENTS_COUNT; ++i) {
      for (int j = i; j < ELEMENTS_COUNT; ++j) {
        if ((i <= element_index) && (element_index <= j)) {
          EXPECT_EQ(0, min_processor.query(i, j));
        } else {
          EXPECT_EQ(1, min_processor.query(i, j));
        }
      }
    }
    min_processor.update(element_index, 1);
  }
}

TEST(RangeQueriesTest, SegmentTree) {
  const int ELEMENTS_COUNT = 10;
  
  SegmentTree< SumTraits<int> >
      sum_processor;

  sum_processor.init(ELEMENTS_COUNT);

  // change one by one
  for (int element_index = 0; element_index < ELEMENTS_COUNT; ++element_index) {
    sum_processor.update(element_index, 1);
    for (int i = 0; i < ELEMENTS_COUNT; ++i) {
      for (int j = i; j < ELEMENTS_COUNT; ++j) {
        if ((i <= element_index) && (element_index <= j)) {
          EXPECT_EQ(1, sum_processor.query(i, j));
        } else {
          EXPECT_EQ(0, sum_processor.query(i, j));
        }
      }
    }
    sum_processor.update(element_index, 0);
  }

  SegmentTree< MinTraits<int> >
      min_processor;
  
  min_processor.init(ELEMENTS_COUNT);
  
  for (int element_index = 0; element_index < ELEMENTS_COUNT; ++element_index) {
    min_processor.update(element_index, 1);
  }

  for (int element_index = 0; element_index < ELEMENTS_COUNT; ++element_index) {
    min_processor.update(element_index, 0);
    for (int i = 0; i < ELEMENTS_COUNT; ++i) {
      for (int j = i; j < ELEMENTS_COUNT; ++j) {
        if ((i <= element_index) && (element_index <= j)) {
          EXPECT_EQ(0, min_processor.query(i, j));
        } else {
          EXPECT_EQ(1, min_processor.query(i, j));
        }
      }
    }
    min_processor.update(element_index, 1);
  }
}


TEST(RangeQueriesTest, RandomStress) {
  const int test_count = 100;
  const int max_elements_count = 1000;
  const int max_value = 1000000;
  const int queries_count = 100;

  for (int test = 0; test < test_count; ++test) {
    int n = rand() % max_elements_count + 1;

    SegmentTree< SumTraits<int> > segment_tree;
    FenwickTree< SumTraits<int> > fenwick_tree;
    ElementwiseRangeQueryProcessor< SumTraits<int> > dummy;

    segment_tree.init(n);
    fenwick_tree.init(n);
    dummy.init(n);

    for (int q = 0; q < queries_count; ++q) {
      int k = rand() % n;
      int v = rand() % max_value;
      dummy.update(k, v);
      fenwick_tree.update(k, v);
      segment_tree.update(k, v);
      int l = rand() % n;
      int r = l + rand() % (n - l);
      EXPECT_EQ(dummy.query(l, r), fenwick_tree.query(l, r));
      EXPECT_EQ(dummy.query(l, r), segment_tree.query(l, r));
    }
  }
}

TEST(RangeQueriesTest, SparseTable) {
  const int ELEMENTS_COUNT = 100;
  vector<int> values(ELEMENTS_COUNT);
  for (int i = 0; i < ELEMENTS_COUNT; ++i) {
    values[i] = i;
  }

  SparseTable< std::less<int> > sparse_table;
  sparse_table.init(values);

  for (int i = 0; i < ELEMENTS_COUNT; ++i) {
    for (int j = i; j < ELEMENTS_COUNT; ++j) {
      EXPECT_EQ(i, sparse_table.query(i, j));
    }
  }
}

TEST(RangeQueriesTest, SparseTableStress) {
  const int MAX_VALUE = 100000;

  for (int elements_count = 1; elements_count < 100; ++elements_count) {
    vector<int> values(elements_count);
    for (int i = 0; i < elements_count; ++i) {
      values[i] = rand() % MAX_VALUE;
    }

    SparseTable< std::less<int> > sparse_table_;
    sparse_table_.init(values);
    for (int left = 0; left < elements_count; ++left) {
      for (int right = left; right < elements_count; ++right) {
        int expected = values[left];
        for (int middle = left + 1; middle <= right; ++middle) {
          if (values[middle] < expected) {
            expected = values[middle];
          }
        }
        int acquired = values[sparse_table_.query(left, right)];
        ASSERT_EQ(acquired, expected);
      }
    }
  }
}

TEST(RangeQueriesTest, BenderFarachColtonStress) {
  const int MAX_VALUE = 100;

  for (int elements_count = 1; elements_count < 100; ++elements_count) {
    vector<int> values(elements_count);
    for (int i = 0; i < elements_count; ++i) {
      values[i] = rand() % MAX_VALUE;
    }

    BenderFarachColton< std::greater<int> > bfc;
    bfc.init(values);
    for (int left = 0; left < elements_count; ++left) {
      for (int right = left; right < elements_count; ++right) {
        int expected = values[left];
        for (int middle = left + 1; middle <= right; ++middle) {
          if (values[middle] > expected) {
            expected = values[middle];
          }
        }
        int acquired = values[bfc.query(left, right)];
        ASSERT_EQ(expected, acquired);
      }
    }
  }
}
