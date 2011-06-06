#include "basic/dsu.h"

#include <cstdlib>
#include <iostream>

#include "gtest/gtest.h"


TEST(DSUTest, SequentialUnite) {
  const int ELEMENTS_COUNT = 100;
  DisjointSetUnion dsu(ELEMENTS_COUNT);

  for (int i = 0; i < ELEMENTS_COUNT; ++i) {
    for (int j = i; j < ELEMENTS_COUNT; ++j) {
      EXPECT_EQ(j, dsu.get_root(j));
    }
    if (i > 0) {
      dsu.unite(i, i - 1);
    }
    for (int j = 1; j <= i; ++j) {
      EXPECT_EQ(dsu.get_root(0), dsu.get_root(j));
    }
  }
}

TEST(DSUTest, Stress) {
  const int ELEMENTS_COUNT = 100;
  const int QUERIES_COUNT = 1000;
  DisjointSetUnion dsu(ELEMENTS_COUNT);

  srand(42);

  for (int i = 0; i < QUERIES_COUNT; ++i) {
    int first_key = rand() % ELEMENTS_COUNT;
    int second_key = rand() % ELEMENTS_COUNT;

    dsu.unite(first_key, second_key);
    EXPECT_EQ(dsu.get_root(first_key), dsu.get_root(second_key));
  }
}
