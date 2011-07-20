#include "basic/hash.h"

#include <set>
#include <string>
#include <vector>
#include <iostream>

#include "gtest/gtest.h"

using std::vector;
using std::string;
using std::set;
using std::cout;
using std::cerr;
using std::endl;

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

TEST(HashTest, InitIdeal) {
  const int MAX_KEY = 1000000000;
  for (int keys_count = 2; keys_count <= 100; ++keys_count) {
    vector<int> keys;
    while (keys.size() != keys_count) {
      int key;
      do {
        key = rand() % (MAX_KEY + 1);
        if (rand() % 2) {
          key = -key;
        }
      } while (std::find(keys.begin(), keys.end(), key) != keys.end());
      keys.push_back(key);
    }
    bicycle::HashTable<int> fixed_set;
    fixed_set.init_ideal(keys);
    for (int key_index = 0; key_index < keys.size(); ++key_index) {
      EXPECT_TRUE(fixed_set.contains(keys[key_index]));
    }
  }
}
 
TEST(HashTest, FixedSet) {
  const int MAX_KEY = 1000000000;
  const int KEYS_COUNT = 100000;
  
  set<int> key_set;
  for (int i = 0; i < KEYS_COUNT; ++i) {
    int key = rand() % (MAX_KEY + 1);
    if (rand() % 2) {
      key = -key;
    }
    key_set.insert(key);
  }
  vector<int> keys(key_set.begin(), key_set.end());
  
  bicycle::FixedSet<int> fixed_set;
  fixed_set.init(keys);
    
  for (int i = 0; i < keys.size(); ++i) {
    EXPECT_TRUE(fixed_set.contains(keys[i]));
  }

  for (int i = 0; i < 10 * KEYS_COUNT; ++i) {
    int key = rand() % MAX_KEY;
    EXPECT_EQ(key_set.find(key) != key_set.end(),
              fixed_set.contains(key));
  }
}

TEST(HashTest, OrderedFixedSetDirectOrder) {
  const int KEYS_COUNT = 1000;

  vector<int> keys(KEYS_COUNT);
  vector< std::pair<const void*, int> > serialized_keys(KEYS_COUNT);  

  for (int i = 0; i < KEYS_COUNT; ++i) {
    keys[i] = i;
    serialized_keys[i] = std::make_pair(reinterpret_cast<const void*>(&keys[i]),
                                        sizeof(keys[i]));
  }

  bicycle::OrderedFixedSet fixed_set;

  fixed_set.init(serialized_keys);
  for (int i = 0; i < KEYS_COUNT; ++i) {
    ASSERT_EQ(i, fixed_set.get_index(reinterpret_cast<const void*>(&keys[i]),
                                     sizeof(keys[i])));
  }

  int non_existant_key = KEYS_COUNT;

  ASSERT_EQ(-1, fixed_set.get_index(
      reinterpret_cast<const void*>(&non_existant_key),
      sizeof(non_existant_key)));
}

TEST(HashTest, OrderedFixedSetReversedOrder) {
  const int KEYS_COUNT = 10000;

  vector<int> keys(KEYS_COUNT);
  vector< std::pair<const void*, int> > serialized_keys(KEYS_COUNT);  

  for (int i = 0; i < KEYS_COUNT; ++i) {
    keys[i] = KEYS_COUNT - 1 - i;
    serialized_keys[i] = std::make_pair(reinterpret_cast<const void*>(&keys[i]),
                                        sizeof(keys[i]));
  }

  bicycle::OrderedFixedSet fixed_set;

  fixed_set.init(serialized_keys);
  for (int i = 0; i < KEYS_COUNT; ++i) {
    ASSERT_EQ(i, fixed_set.get_index(reinterpret_cast<const void*>(&keys[i]),
                                     sizeof(keys[i])));
  }

  int non_existant_key = KEYS_COUNT;

  ASSERT_EQ(-1, fixed_set.get_index(
      reinterpret_cast<const void*>(&non_existant_key),
      sizeof(non_existant_key)));
}

TEST(HashTest, OrderedFixedSetRandomKeys) {
  const int KEYS_COUNT = 1000;

  vector<int> keys(KEYS_COUNT);
  vector< std::pair<const void*, int> > serialized_keys(KEYS_COUNT);  

  for (int i = 0; i < KEYS_COUNT; ++i) {
    keys[i] = rand();
    serialized_keys[i] = std::make_pair(reinterpret_cast<const void*>(&keys[i]),
                                        sizeof(keys[i]));
  }

  bicycle::OrderedFixedSet fixed_set;

  fixed_set.init(serialized_keys);
  for (int i = 0; i < KEYS_COUNT; ++i) {
    ASSERT_EQ(i, fixed_set.get_index(reinterpret_cast<const void*>(&keys[i]),
                                     sizeof(keys[i])));
  }

  int non_existant_key = KEYS_COUNT;

  ASSERT_EQ(-1, fixed_set.get_index(
      reinterpret_cast<const void*>(&non_existant_key),
      sizeof(non_existant_key)));
}
