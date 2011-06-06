#include "storage/persistent_top.h"

#include <sys/stat.h>
#include <errno.h>

#include <cstring>
#include <string>
#include <iostream>
#include <fstream>
#include <stdexcept>
#include <cassert>

namespace simple {

void PersistentTop::read_entry(std::istream& data_file, std::string& entry) {
  uint32_t entry_size;
  data_file.read(reinterpret_cast<char*>(&entry_size), sizeof(entry_size));
  entry.resize(entry_size);
  data_file.read(const_cast<char*>(entry.data()), entry_size);
  if (data_file.fail()) {
    throw std::runtime_error("can't read entry");
  }
}

void PersistentTop::write_entry(std::ostream& data_file,
                                const std::string& entry) {
  uint32_t entry_size = entry.size();
  data_file.write((const char*)&entry_size, sizeof(entry_size));
  data_file.write(entry.data(), entry.size());
  if (data_file.fail()) {
    throw std::runtime_error(filename_ + "can't write data to file");
  }
}

void PersistentTop::push(const std::string& entry) {
  std::fstream data_file(filename_.c_str());
  uint32_t entries_count;

  if (!data_file.is_open()) {
    // first push, no data file yet, create one
    data_file.clear();
    data_file.open(filename_.c_str(), std::ios_base::out);
    if (!data_file.is_open()) {
      throw std::runtime_error(filename_ + "can't be opened for write");
    }
    entries_count = 0;
  } else {
    data_file.read(reinterpret_cast<char*>(&entries_count),
                   sizeof(entries_count));
  }

  if (entries_count == max_entries_count_) {
    // skip oldest entry
    std::string forever_lost;
    read_entry(data_file, forever_lost);
    --entries_count;
  }

  std::vector< std::string > entries;

  for (int entry_index = 0; entry_index < entries_count; ++entry_index) {
    std::string entry;
    read_entry(data_file, entry);
    entries.push_back(entry);
  }

  // append new
  entries.push_back(entry);
  ++entries_count;

  // write back
  data_file.close();
  data_file.open(filename_.c_str(), std::ios_base::out | std::ios_base::trunc);
  data_file.write((const char*)&entries_count, sizeof(entries_count));
  for (int entry_index = 0; entry_index < entries_count; ++entry_index) {
    write_entry(data_file, entries[entry_index]);
  }
}
}

namespace batch {

uint64_t get_file_size(const std::string& filename) {
  struct stat file_info;

  int status = stat(filename.c_str(), &file_info);
  if (status != 0) {
    if (errno == ENOENT) {
      throw file_not_exists();
    } else {
      throw std::runtime_error(filename + " can't stat file");
    }
  }

  return file_info.st_size;
}

void PersistentTop::push(const std::string& entry) {
  uint64_t file_size = 0;
  bool file_exists = true;
  uint32_t entries_count = 0;
  uint32_t entry_size = entry.size();

  // get entries count
  {
    std::ifstream data_file(filename_.c_str());
    if (data_file.is_open()) {
      data_file.read(reinterpret_cast<char*>(&entries_count),
                     sizeof(entries_count));
    }
  }

  if (entries_count < 2 * max_entries_count_) {
    // add to end
    ++entries_count;
    std::ofstream data_file;
    data_file.open(filename_.c_str(), std::ios_base::out | std::ios_base::in);
    if (!data_file.is_open()) {
      data_file.clear();
      data_file.open(filename_.c_str(), std::ios_base::out);
    }
    data_file.seekp(0, std::ios_base::beg);
    data_file.write(reinterpret_cast<char*>(&entries_count),
                    sizeof(entries_count));

    data_file.seekp(0, std::ios_base::end);
    data_file.write(reinterpret_cast<char*>(&entry_size), sizeof(entry_size));
    data_file.write(entry.data(), entry_size);

    if (!data_file.good()) {
      throw std::runtime_error(filename_ + " can't write file");
    }
  } else {
    try {
      file_size = get_file_size(filename_);
    } catch(const file_not_exists& e) {
      file_exists = false;
    }

    file_size = std::max(file_size, (uint64_t)sizeof(entries_count));

    // load all and clear
    char* buf = new char[file_size + sizeof(entry_size) + entry_size];

    if (file_exists) {
      std::ifstream data_file(filename_.c_str());
      data_file.read(buf, file_size);
      if (!data_file.good()) {
        throw std::runtime_error(filename_ + " can't read file");
      }
      entries_count = *(reinterpret_cast<uint32_t*>(buf));
    }

    memcpy(buf + file_size, &entry_size, sizeof(entry_size));
    memcpy(buf + file_size + sizeof(entry_size), entry.data(), entry_size);
    ++entries_count;
    file_size += sizeof(entry_size) + entry_size;

    std::streamoff off = sizeof(entries_count);

    while (entries_count > max_entries_count_) {
      // skipping entries in the begining
      uint32_t entry_size = *(reinterpret_cast<uint32_t*>(buf + off));
      off += sizeof(entry_size) + entry_size;
      file_size -= (sizeof(entry_size) + entry_size);
      --entries_count;
    }

    // update count
    *reinterpret_cast<uint32_t*>(buf + off - sizeof(entries_count)) =
        entries_count;

    // write back
    std::ofstream data_file(filename_.c_str(),
                            std::ios_base::out | std::ios_base::trunc);
    data_file.write(buf + off - sizeof(entries_count), file_size);
    delete buf;

    if (!data_file.good()) {
      throw std::runtime_error(filename_ + " can't write file");
    }
  }
}
}

namespace advanced {

const char* PersistentTop::header_file_suffix = ".pth";
const char* PersistentTop::data_file_suffix[2] = {".0", ".1"};

void PersistentTop::write_header() {
  header_file_.write((const char*)&active_file_index_,
                     sizeof(active_file_index_));
  header_file_.write((const char*)&first_entry_index_,
                     sizeof(first_entry_index_));

  for (int data_file_index = 0; data_file_index < 2; ++data_file_index) {
    uint32_t entries_count = entries_offsets_[data_file_index].size();
    header_file_.write((const char*)&entries_count, sizeof(entries_count));

    for (int entry_index = 0; entry_index < entries_count; ++entry_index) {
      header_file_.write(
          (const char*)&entries_offsets_[data_file_index][entry_index],
          sizeof(entries_offsets_[data_file_index][entry_index]));
    }
  }
}

void PersistentTop::read_header() {
  header_file_.read(reinterpret_cast<char*>(&active_file_index_),
                    sizeof(active_file_index_));

  header_file_.read(reinterpret_cast<char*>(&first_entry_index_),
                    sizeof(first_entry_index_));

  for (int data_file_index = 0; data_file_index < 2; ++data_file_index) {
    uint32_t entries_count;
    header_file_.read(reinterpret_cast<char*>(&entries_count),
                      sizeof(entries_count));
    entries_offsets_[data_file_index].resize(entries_count);

    for (int entry_index = 0; entry_index < entries_count; ++entry_index) {
      header_file_.read(
          reinterpret_cast<char*>(
              &entries_offsets_[data_file_index][entry_index]),
          sizeof(entries_offsets_[data_file_index][entry_index]));
    }
  }
}

void PersistentTop::open(const std::string& filename) {
  // open header file
  {
    filename_ = filename;
    std::string header_file_path = filename_ + header_file_suffix;
    header_file_.open(header_file_path.c_str());

    if (!header_file_.is_open()) {
      // create new
      header_file_.clear();
      header_file_.open(header_file_path.c_str(), std::ios_base::out);
      if (!header_file_.is_open()) {
        throw std::runtime_error(header_file_path + " can't be opened");
      }

      // write header
      write_header();
      if (header_file_.fail()) {
        throw std::runtime_error(header_file_path + " can't write header");
      }

      // reopen read/write
      header_file_.close();
      header_file_.open(header_file_path.c_str());
      if (!header_file_.is_open()) {
        throw std::runtime_error(header_file_path + " can't be opened");
      }
    } else {
      read_header();
      if (header_file_.fail()) {
        throw std::runtime_error(header_file_path + " can't read header");
      }
    }
  }

  // open data files
  {
    for (int data_file_index = 0; data_file_index < 2; ++data_file_index) {
      std::string data_file_path =
          filename_ + data_file_suffix[data_file_index];
      data_files_[data_file_index].open(data_file_path.c_str());

      if (!data_files_[data_file_index].is_open()) {
        // create new
        data_files_[data_file_index].clear();
        data_files_[data_file_index].open(data_file_path.c_str(),
                                          std::ios_base::out);
        if (!data_files_[data_file_index].is_open()) {
          throw std::runtime_error(data_file_path + " can't be opened");
        }

        // reopen read/write
        data_files_[data_file_index].close();
        data_files_[data_file_index].open(data_file_path.c_str());
        if (!data_files_[data_file_index].is_open()) {
          throw std::runtime_error(data_file_path + " can't be opened");
        }
      }
    }
  }
}

void PersistentTop::close() {
  header_file_.close();
  for (int data_file_index = 0; data_file_index < 2; ++data_file_index) {
    data_files_[data_file_index].close();
  }
}

void PersistentTop::push(const std::string& entry) {
  if (entries_offsets_[active_file_index_].size() == max_entries_count_) {
    active_file_index_ ^= 1;
    std::string data_file_path =
        filename_ + data_file_suffix[active_file_index_];

    entries_offsets_[active_file_index_].clear();
    data_files_[active_file_index_].close();
    data_files_[active_file_index_].open(data_file_path.c_str(),
                                         std::ios_base::in |
                                         std::ios_base::out |
                                         std::ios_base::trunc);
    if (!data_files_[active_file_index_].is_open()) {
      throw std::runtime_error("can't reopen data file");
    }
  }

  data_files_[active_file_index_].seekp(0, std::ios_base::end);
  uint64_t entry_offset = data_files_[active_file_index_].tellp();

  uint32_t entry_size = entry.size();

  data_files_[active_file_index_].write((const char*)&entry_size,
                                        sizeof(entry_size));
  data_files_[active_file_index_].write(entry.c_str(), entry_size);

  if (data_files_[active_file_index_].fail()) {
    throw std::runtime_error("can't push entry");
  }

  entries_offsets_[active_file_index_].push_back(entry_offset);

  int total_entries = entries_offsets_[active_file_index_].size()
                      + entries_offsets_[active_file_index_ ^ 1].size()
                      - first_entry_index_;
  assert(total_entries <= (max_entries_count_ + 1));

  if (total_entries == (max_entries_count_ + 1)) {
    first_entry_index_ = (first_entry_index_ + 1) % max_entries_count_;
  }
}
}
