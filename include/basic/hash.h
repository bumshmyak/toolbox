#ifndef _TOOLBOX_BASIC_HASH_H_
#define _TOOLBOX_BASIC_HASH_H_

#include <vector>
#include <list>
#include <stdexcept>
#include <algorithm>

namespace bicycle {

template <class KeyType>
unsigned int hash(const KeyType& key);

template <>
unsigned int hash(const int& key) {
  return key;
}

template <class KeyType>
class HashTable {
 public:
  HashTable() { }
  explicit HashTable(int buckets_count) { init(buckets_count); }

  void init(int buckets_count);

  int size() const { return elements_count_; }
  bool empty() const { return elements_count_ == 0; }
  void insert(const KeyType& key);
  bool contains(const KeyType& key) const;
  void erase(const KeyType& key);

 private:
  // In theory i must first do calculation mod p
  // where p is prime and p > buckets_.size()
  // (fixed big prime number will be enough)
  // Here i simple make calculation mod 2^32
  // WARN: check this for perfomace issues
  unsigned int get_bucket_index(const KeyType& key) const {
    return (a_ * hash(key) + b_) % buckets_.size();
  }


  std::vector< std::list<KeyType> > buckets_;
  int elements_count_;
  unsigned int a_;
  unsigned int b_;
};

template <class KeyType>
void HashTable<KeyType>::init(int buckets_count) {
  elements_count_ = 0;
  a_ = rand();
  b_ = rand();
  buckets_.clear();
  buckets_.resize(buckets_count);
}

template <class KeyType>
void HashTable<KeyType>::insert(const KeyType& key) {
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
  const std::list<KeyType>& bucket = buckets_[get_bucket_index(key)];
  typename std::list<KeyType>::const_iterator key_iterator =
      std::find(bucket.begin(), bucket.end(), key);
  return key_iterator != bucket.end();
}

template <class KeyType>
void HashTable<KeyType>::erase(const KeyType& key) {
  std::list<KeyType>& bucket = buckets_[get_bucket_index(key)];
  typename std::list<KeyType>::iterator key_iterator =
      std::find(bucket.begin(), bucket.end(), key);
  if (key_iterator != bucket.end()) {
    --elements_count_;
    bucket.erase(key_iterator);
  }
}

template <class KeyType>
class FixedSet {
 public:
  FixedSet();
  void init(const std::vector<KeyType>& keys);
  bool contains(const KeyType& key) const;
};

template <class KeyType>
void FixedSet<KeyType>::init(const std::vector<KeyType>& keys) {
}

template <class KeyType>
bool FixedSet<KeyType>::contains(const KeyType& key) const {
  return true;
}

};  // bicycle namespace

#endif  //  _TOOLBOX_BASIC_HASH_H_
