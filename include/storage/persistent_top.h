/*
 * PersistentTop is disk based data structure wich holds
 * limited number of string entries. When new entry is
 * added while PersistentTop is full the oldest entry
 * is removed.
 * Basic interface:
 *   - create(filename, max_entries_count)
 *   - push(entry)
 *   - list all entries
 * PersistentTop is suitable to represent cycle queues
 * such as logs, journals.
 */


#ifndef TOOLBOX_STORAGE_PERSISTENT_TOP_H_
#define TOOLBOX_STORAGE_PERSISTENT_TOP_H_

#include <string>
#include <fstream>
#include <iostream>
#include <vector>
#include <stdint.h>
#include <cassert>

#include <utility>
#include <stdexcept>

namespace simple {

class PersistentTop {
  public:
    PersistentTop(const std::string& filename,
                  int max_entries_count) :
        filename_(filename),
        max_entries_count_(max_entries_count)
      { }

    void push(const std::string& entry);
    
    template <class BackInsertIterator>
    void list(BackInsertIterator);
  private:
    std::string filename_;
    int max_entries_count_;

    void read_entry(std::istream& data_file, std::string& entry);
    void write_entry(std::ostream& data_file, const std::string& entry);
};

template <class BackInsertIterator>
void PersistentTop::list(BackInsertIterator entries_inserter)
{
  std::ifstream data_file(filename_.c_str());
  if (!data_file.is_open()) {
    throw std::runtime_error(filename_ + " can't be opened for read");
  }

  uint32_t entries_count;
  data_file.read((char*)&entries_count, sizeof(entries_count));
  for (unsigned int entry_index = 0; entry_index < entries_count; ++entry_index) {
    std::string entry;
    read_entry(data_file, entry);
    *entries_inserter++ = entry;
  }
}

}


namespace batch {

struct file_not_exists :
  public std::exception
{ };

uint64_t get_file_size(const std::string& filename);

class PersistentTop {
  public:
    PersistentTop(const std::string& filename,
                  int max_entries_count) :
        filename_(filename),
        max_entries_count_(max_entries_count)
      { }

    void push(const std::string& entry);

    template <class BackInsertIterator>
    void list(BackInsertIterator);

    void set_max_entries_count(int max_entries_count) 
      { max_entries_count_ = max_entries_count; }

    int get_max_entries_count() const
      { return max_entries_count_; }
  private:
    std::string filename_;
    int max_entries_count_;
};

template <class BackInsertIterator>
void PersistentTop::list(BackInsertIterator entries_inserter)
{
  std::ifstream data_file(filename_.c_str());
  if (!data_file.is_open()) {
    throw std::runtime_error(filename_ + " can't be opened for read");
  }

  uint64_t file_size = get_file_size(filename_);
  char* buf = new char[file_size];
  data_file.read(buf, file_size);
  if (!data_file.good()) {
    delete buf;
    throw std::runtime_error(filename_ + " can't read file");
  }

  std::streamoff off = 0; 
  uint32_t entries_count = *(uint32_t*)(buf + off);
  off += sizeof(entries_count);

  try {
    for (int entry_index = 0; entry_index < entries_count; ++entry_index) {
      uint32_t entry_size = *(uint32_t*)(buf + off);
      off += sizeof(entry_size);
      if (entry_index + max_entries_count_ >= entries_count) {
        *entries_inserter++ = std::string(buf + off, entry_size); 
      }
      off += entry_size;
    }
  } catch(...) {
    delete buf;
    throw;
  }

  delete buf;
}

}

namespace advanced {

class PersistentTop {
  public:
    PersistentTop(int max_entries_count = 0) :
        max_entries_count_(max_entries_count)
      { }

    ~PersistentTop() { close(); }

    // precondition: !is_open()
    void open(const std::string& filename);

    bool is_open() { return header_file_.is_open(); }
    void close();

    // precondition: is_open()
    void push(const std::string& entry);
    
    // precondition: is_open()
    template <class BackInsertIterator>
    void list(BackInsertIterator);

  private:
    static const char* header_file_suffix;
    static const char* data_file_suffix[2];

    int max_entries_count_;
    std::string filename_;
    std::fstream header_file_;
    std::fstream data_files_[2];

    uint32_t active_file_index_;
    uint32_t first_entry_index_;
    std::vector< uint64_t > entries_offsets_[2];

    // precondition: is_open()
    void read_header();
    // precondition: is_open()
    void write_header(); 
};

template <class BackInsertIterator>
void PersistentTop::list(BackInsertIterator entries_iterator)
{
  int total_entries = entries_offsets_[active_file_index_].size()
                      + entries_offsets_[active_file_index_ ^ 1].size()
                      - first_entry_index_;
int current_file_index = active_file_index_;
  int entry_index = first_entry_index_;

  assert(total_entries <= max_entries_count_);

  for (int read_entries = 0; read_entries < total_entries; ++read_entries) {
    // read entry
    std::string entry;
    {
      uint32_t entry_size;
      data_files_[current_file_index].read((char*)&entry_size, sizeof(entry_size));
      entry.resize(entry_size);
      data_files_[current_file_index].read(const_cast<char*>(entry.data()), entry_size);
    }

    *entries_iterator++ = entry;
    ++entry_index; 
    if (entry_index == max_entries_count_) {
      entry_index = 0;
      current_file_index ^= 1;
    }
  }
}

}

#endif // TOOLBOX_STORAGE_PERSISTENT_TOP_H_
