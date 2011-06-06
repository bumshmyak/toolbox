#include "basic/hash.h"

#include <set>
#include <string>
#include <vector>

#include "gtest/gtest.h"


using std::vector;
using std::string;
using std::set;

TEST(HashTest, HashTable) {
  const int TABLE_SIZE = 1024;
  bicycle::HashTable<int> hash_table(TABLE_SIZE);

  EXPECT_TRUE(hash_table.empty());

  hash_table.insert(1);
  EXPECT_TRUE(hash_table.contains(1));
  EXPECT_EQ(1, hash_table.size());

  hash_table.insert(1);
  EXPECT_TRUE(hash_table.contains(1));
  EXPECT_EQ(1, hash_table.size());

  hash_table.erase(1);
  EXPECT_FALSE(hash_table.contains(1));
  EXPECT_TRUE(hash_table.empty());

  // try to erase non existent
  hash_table.erase(2);
}

TEST(HashTest, SequentalStoreAndRemove) {
  const int TABLE_SIZE = 1024;
  const int ELEMENTS_COUNT = 10 * 1024;
  bicycle::HashTable<int> hash_table;
  hash_table.init(TABLE_SIZE);

  // store
  for (int i = 0; i < ELEMENTS_COUNT; ++i) {
    hash_table.insert(i);
    EXPECT_EQ(i + 1, hash_table.size());
    EXPECT_TRUE(hash_table.contains(i));
    EXPECT_FALSE(hash_table.contains(i + 1));
  }

  // remove
  for (int i = 0; i < ELEMENTS_COUNT; ++i) {
    hash_table.erase(i);
    EXPECT_EQ(ELEMENTS_COUNT - i - 1, hash_table.size());
    EXPECT_FALSE(hash_table.contains(i));
  }
}
