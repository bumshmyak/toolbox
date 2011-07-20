#include "basic/sort.h"

#include <cstdlib>
#include <iostream>
#include <algorithm>
#include <vector>

#include "gtest/gtest.h"

using std::cout;
using std::cerr;
using std::endl;
using std::vector;

TEST(InplaceMergeSort, Pemutations) {
  const int ELEMENTS_COUNT = 7;
  vector<int> a(ELEMENTS_COUNT);
  for (int i = 0; i < ELEMENTS_COUNT; ++i) {
    a[i] = i;
  }

  do {
    vector<int> b = a;
    bicycle::inplace_merge_sort(b.begin(), b.end());
    ASSERT_TRUE(bicycle::is_ordered(b.begin(), b.end()));
  } while (std::next_permutation(a.begin(), a.end()));
}
 
TEST(InplaceMergeSortTest, Stress) {
  const int TEST_COUNT = 1000;
  const int MAX_VALUE = 100;

  for (int test = 0; test < TEST_COUNT; ++test) {
    int n = test + 1;
    std::vector<int> a(n);
    for (int i = 0; i < n; ++i) {
      a[i] = rand() % MAX_VALUE;
    }

    bicycle::inplace_merge_sort(a.begin(), a.end());
    EXPECT_TRUE(bicycle::is_ordered(a.begin(), a.end()));
  }
}

TEST(QuickSortTest, Pemutations) {
  const int ELEMENTS_COUNT = 5;
  vector<int> a(ELEMENTS_COUNT);
  for (int i = 0; i < ELEMENTS_COUNT; ++i) {
    a[i] = i;
  }

  do {
    vector<int> b = a;
    cout << "---" << endl;
    bicycle::quick_sort(b.begin(), b.end());
    if (!bicycle::is_ordered(b.begin(), b.end())) {
      for (int i = 0; i < b.size(); ++i) {
        cout << b[i] << ' ';
      }
      cout << endl;
      break;
    }
  } while (std::next_permutation(a.begin(), a.end()));
}


TEST(QuickSortTest, Stress) {
  const int TEST_COUNT = 1000;
  const int MAX_VALUE = 100;

  for (int test = 0; test < TEST_COUNT; ++test) {
    int n = test + 1;
    std::vector<int> a(n);
    for (int i = 0; i < n; ++i) {
      a[i] = rand() % MAX_VALUE;
    }

    bicycle::quick_sort(a.begin(), a.end());

    ASSERT_TRUE(bicycle::is_ordered(a.begin(), a.end()));
  }
}

TEST(QuickSortTest, AllEqual) {
  vector<int> a(100000);
  bicycle::quick_sort(a.begin(), a.end());
  EXPECT_TRUE(bicycle::is_ordered(a.begin(), a.end()));
}


TEST(OrderStatisticsTest, MiniMax) {
  const int TEST_COUNT = 1000;
  const int MAX_VALUE = 100;

  for (int test = 0; test < TEST_COUNT; ++test) {
    int n = test + 1;
    std::vector<int> a(n);
    for (int i = 0; i < n; ++i) {
      a[i] = rand() % MAX_VALUE;
    }

    std::vector<int>::iterator min_iterator;
    std::vector<int>::iterator max_iterator;

    bicycle::min_and_max_element(a.begin(), a.end(),
                                 min_iterator,
                                 max_iterator);
    EXPECT_EQ(*std::min_element(a.begin(), a.end()), *min_iterator);
    EXPECT_EQ(*std::max_element(a.begin(), a.end()), *max_iterator);
  }
}

TEST(OrderStatisticsTest, KthElement) {
  const int TEST_COUNT = 1000;
  const int MAX_VALUE = 100;

  for (int test = 0; test < TEST_COUNT; ++test) {
    int n = test + 1;
    std::vector<int> a(n);
    for (int i = 0; i < n; ++i) {
      a[i] = rand() % MAX_VALUE;
    }
    int k = rand() % n;
    int kth_element = *bicycle::randomized_select(a.begin(), a.end(), k);
    std::sort(a.begin(), a.end());
    EXPECT_EQ(a[k], kth_element);
  }
}
