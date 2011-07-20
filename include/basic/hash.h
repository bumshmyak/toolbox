#ifndef _TOOLBOX_BASIC_HASH_H_
#define _TOOLBOX_BASIC_HASH_H_

#include <vector>
#include <list>
#include <stdexcept>
#include <algorithm>
#include <iostream>
#include <cassert>
#include <utility>

#include <ext/slist> // NOLINT

namespace bicycle {

template <class KeyType>
unsigned int hash(const KeyType& key);

template <>
unsigned int hash(const int& key) {
  if (key < 0) {
    return key + 2000000010;
  } else {
    return key;
  }
}

const long long BIG_PRIME_FOR_HASH = 2147483647;

template <class KeyType>
class HashTable {
 public:
  HashTable()
      : elements_count_(0),
        a_(0),
        b_(0)
  { }
  explicit HashTable(int buckets_count) { init(buckets_count); }

  void init(int buckets_count);

  // creates hash with no collisions and (keys.size())^2
  // buckets count
  void init_ideal(const std::vector<KeyType>& keys);

  int size() const { return elements_count_; }
  bool empty() const { return elements_count_ == 0; }
  void insert(const KeyType& key);
  bool contains(const KeyType& key) const;
  void erase(const KeyType& key);

 private:
  int get_bucket_index(const KeyType& key) const {
    return ((a_ * hash(key) + b_) % BIG_PRIME_FOR_HASH) % buckets_.size();
  }

  void generate_random_params() {
    a_ = rand() % BIG_PRIME_FOR_HASH;
    b_ = rand() % BIG_PRIME_FOR_HASH;
  }

  std::vector< std::list<KeyType> > buckets_;
  int elements_count_;
  long long a_;
  long long b_;
};

template <class KeyType>
void HashTable<KeyType>::init(int buckets_count) {
  elements_count_ = 0;
  generate_random_params();
  buckets_.clear();
  buckets_.resize(buckets_count);
}

template <class KeyType>
void HashTable<KeyType>::insert(const KeyType& key) {
  if (buckets_.empty()) {
    throw std::runtime_error("hash table not initialized");
  }
  std::list<KeyType>& bucket = buckets_[get_bucket_index(key)];
  typename std::list<KeyType>::iterator key_iterator =
      std::find(bucket.begin(), bucket.end(), key);
  if (key_iterator == bucket.end()) {
    ++elements_count_;
    bucket.push_back(key);
  }
}

template <class KeyType>
bool HashTable<KeyType>::contains(const KeyType& key) const {
  if (buckets_.empty()) {
    throw std::runtime_error("hash table not initialized");
  }
  const std::list<KeyType>& bucket = buckets_[get_bucket_index(key)];
  typename std::list<KeyType>::const_iterator key_iterator =
      std::find(bucket.begin(), bucket.end(), key);
  return key_iterator != bucket.end();
}

template <class KeyType>
void HashTable<KeyType>::erase(const KeyType& key) {
  if (buckets_.empty()) {
    throw std::runtime_error("hash table not initialized");
  }
  std::list<KeyType>& bucket = buckets_[get_bucket_index(key)];
  typename std::list<KeyType>::iterator key_iterator =
      std::find(bucket.begin(), bucket.end(), key);
  if (key_iterator != bucket.end()) {
    --elements_count_;
    bucket.erase(key_iterator);
  }
}

// for debugging only
template <class KeyType>
bool have_duplicates(const std::vector<KeyType>& keys) {
  std::vector<KeyType> keys_copy = keys;
  std::sort(keys_copy.begin(), keys_copy.end());
  return std::unique(keys_copy.begin(), keys_copy.end()) != keys_copy.end();
}

template <class KeyType>
void HashTable<KeyType>::init_ideal(const std::vector<KeyType>& keys) {
  bool have_collisions = false;
  do {
    buckets_.clear();
    buckets_.resize(keys.size() * keys.size());
    elements_count_ = 0;
    generate_random_params();
    have_collisions = false;
    for (int key_index = 0;
         key_index < keys.size() && !have_collisions;
         ++key_index) {
      std::list<KeyType>& bucket = buckets_[get_bucket_index(keys[key_index])];
      if (!bucket.empty()) {
        have_collisions = true;
      } else {
        bucket.push_back(keys[key_index]);
        ++elements_count_;
      }
    }
  } while (have_collisions);
}

template <class KeyType>
class FixedSet {
 public:
  FixedSet()
      : a_(0),
        b_(0)
  { }
  void init(const std::vector<KeyType>& keys);
  bool contains(const KeyType& key) const;
 private:
  int get_table_index(const KeyType& key) const {
    return ((a_ * hash(key) + b_) % BIG_PRIME_FOR_HASH) % tables_.size();
  }

  void generate_random_params() {
    a_ = rand() % BIG_PRIME_FOR_HASH;
    b_ = rand() % BIG_PRIME_FOR_HASH;
  }
  
  std::vector< HashTable<KeyType> > tables_;
  long long int a_;
  long long int b_;
};

template <class KeyType>
void FixedSet<KeyType>::init(const std::vector<KeyType>& keys) {
  tables_.clear();
  tables_.resize(keys.size());
  
  std::vector< std::vector<KeyType> > buckets;
  long long square_sum = 0;

  do {
    buckets.clear();
    buckets.resize(keys.size());
    generate_random_params();
    
    for (int key_index = 0; key_index < keys.size(); ++key_index) {
      buckets[get_table_index(keys[key_index])].push_back(keys[key_index]);
    }

    square_sum = 0;
    for (int key_index = 0; key_index < keys.size(); ++key_index) {
      long long buckets_size = buckets[key_index].size();
      square_sum += buckets_size * buckets_size;
    }
  } while (square_sum > 4 * keys.size());

  for (int key_index = 0; key_index < keys.size(); ++key_index) {
    if (!buckets[key_index].empty()) {
      tables_[key_index].init_ideal(buckets[key_index]);
    }
  } 
}

template <class KeyType>
bool FixedSet<KeyType>::contains(const KeyType& key) const {
  const HashTable<KeyType>& table = tables_[get_table_index(key)];
  if (table.empty()) {
    return false;
  } else {
    return table.contains(key);
  }
}

class OrderedFixedSet {
 public:
  OrderedFixedSet() :
      vertices_count_(0)
  { }
  
  void init(const std::vector< std::pair<const void*, int> >& keys);
  int get_index(const void* key_data, int key_length) const;
 private:
  struct Arc {
    int head;
    int index;
    Arc() :
        head(-1),
        index(-1)
    { }
    Arc(int h, int i) :
        head(h),
        index(i)
    { }

    bool operator==(const Arc& another) {
      return head == another.head && index == another.index;
    }
  };
  
  typedef std::list<Arc> AdjacencyListType; // NOLINT
  typedef std::vector<AdjacencyListType> GraphType;

  void generate_random_tables();
  int get_hash_by_key(const void* key_data,
                      int key_length,
                      int table_index) const;
  
  void build_graph(GraphType& graph);
  bool bfs(const GraphType& graph, int start);
  bool assign_labels_to_vertices(const GraphType& graph);

  std::vector< std::pair<const void*, int> > keys_;
  long long random_char_table_[2][256];
  std::vector<long long> random_position_;
  int vertices_count_;
  std::vector<int> label_;
};


};  // bicycle namespace

#endif  //  _TOOLBOX_BASIC_HASH_H_
