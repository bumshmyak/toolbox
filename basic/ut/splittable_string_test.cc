#include "basic/splittable_string.h"

#include <cstdlib>
#include <iostream>
#include <string>

#include "gtest/gtest.h"

using std::cout;
using std::cin;
using std::endl;
using std::vector;
using std::string;

TEST(SplittableStringTest, BasicOperations) {
  SplittableString<char> whole;
  whole.init("abc");
  ASSERT_EQ("abc", whole.tostring());
  ASSERT_EQ(3, whole.size());
  
  SplittableString<char> low, high;
  EXPECT_TRUE(low.empty());
  low.init("a");
  high.init("bc");
  EXPECT_EQ("a", low.tostring());
  EXPECT_EQ("bc", high.tostring());

  EXPECT_EQ(1, low.size());
  EXPECT_EQ(2, high.size());
  
  whole = low.append(high);
  EXPECT_EQ("abc", whole.tostring());
  EXPECT_EQ(3, whole.size());

  whole.split(2, low, high);
  EXPECT_EQ("ab", low.tostring());
  EXPECT_EQ("c", high.tostring());

  low.clear();
  high.clear();
  EXPECT_TRUE(high.empty());
}

TEST(SplittableStringTest, Stress) {
  const int STRING_SIZE = 1000;
  const int OPERATIONS_COUNT = 100;

  std::string correct_string;
  for (int i = 0; i < STRING_SIZE; ++i) {
    correct_string += ('a' + rand() % 26);
  }

  SplittableString<char> checked_string;
  checked_string.init(correct_string);
  ASSERT_EQ(correct_string, checked_string.tostring());
  ASSERT_EQ(STRING_SIZE, checked_string.size());

  for (int operation = 0; operation < OPERATIONS_COUNT; ++operation) {
    int position = rand() % STRING_SIZE;

    correct_string = correct_string.substr(position) +
        correct_string.substr(0, position);
    
    SplittableString<char> low;
    SplittableString<char> high;
    checked_string.split(position, low, high);
    checked_string = high.append(low);
    ASSERT_EQ(correct_string, checked_string.tostring());
    ASSERT_EQ(STRING_SIZE, checked_string.size());
  }

  checked_string.clear();
}
