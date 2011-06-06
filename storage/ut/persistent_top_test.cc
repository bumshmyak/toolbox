#include "storage/persistent_top.h"

#include <stdexcept>
#include <iostream>
#include <sstream>

#include "gtest/gtest.h"


TEST(SimplePersistentTopTest, PushAndList) {
  const unsigned int max_entries_count = 100;
  simple::PersistentTop top("test.pt", max_entries_count);

  try {
    for (unsigned int i = 1; i <= 5 * max_entries_count; ++i) {
      // push
      {
        std::ostringstream oss;
        oss << i;
        top.push(oss.str());
      }

      std::vector< std::string > entries;
      top.list(std::back_inserter(entries));
      EXPECT_EQ(std::min(i, max_entries_count), entries.size());

      for (unsigned int j = 0; j < entries.size(); ++j) {
        std::string entry;
        {
          std::ostringstream oss;
          oss << i - entries.size() + j + 1;
          entry = oss.str();
        }
        EXPECT_EQ(entry, entries[j]);
      }
    }
  } catch(const std::runtime_error& e) {
    EXPECT_TRUE(false);
  }

  EXPECT_EQ(0, system("rm test.pt"));
}

TEST(BatchPersistentTopTest, FileSize) {
  const uint64_t FILE_SIZE = 1024;
  char buf[FILE_SIZE];

  {
    std::ofstream out("test.pt");
    out.write(buf, FILE_SIZE);
    EXPECT_TRUE(out.good());
  }

  try {
    EXPECT_EQ(FILE_SIZE, batch::get_file_size("test.pt"));
  } catch(...) {
  }

  EXPECT_EQ(0, system("rm test.pt"));
}

TEST(BatchPersistentTopTest, PushAndList) {
  const unsigned int max_entries_count = 100;
  batch::PersistentTop top("test.pt", max_entries_count);

  try {
    for (unsigned int i = 1; i <= 5 * max_entries_count; ++i) {
      // push
      {
        std::ostringstream oss;
        oss << i;
        top.push(oss.str());
      }

      std::vector< std::string > entries;
      top.list(std::back_inserter(entries));
      EXPECT_EQ(std::min(i, max_entries_count), entries.size());

      for (unsigned int j = 0; j < entries.size(); ++j) {
        std::string entry;
        {
          std::ostringstream oss;
          oss << i - entries.size() + j + 1;
          entry = oss.str();
        }
        EXPECT_EQ(entry, entries[j]);
      }
    }
  } catch(const std::runtime_error& e) {
    EXPECT_TRUE(false);
  }

  EXPECT_EQ(0, system("rm test.pt"));
}
