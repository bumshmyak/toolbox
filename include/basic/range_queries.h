#ifndef _TOOLBOX_BASIC_RANGE_QUERIES_H_
#define _TOOLBOX_BASIC_RANGE_QUERIES_H_

#include <vector>
#include <iostream>
#include <cassert>
#include <algorithm>
#include <limits>
#include <string>
#include <functional>

template <typename T>
class MinTraits {
 public:
  typedef T ValueType;
  static T operation(const T& a, const T& b) {
    return std::min(a, b);
  }
  
  static T identity_element() {
    return std::numeric_limits<T>::max();
  }
};

template <typename T>
class SumTraits {
 public:
  typedef T ValueType;  
  static T operation(const T& a, const T& b) {
    return a + b;
  }
  
  static T inverse_element(const T& a) {
    return -a;
  }
  
  static T identity_element() {
    return 0;
  }
};

template <class OperationTraits>
class DynamicRangeQueryProcessor {
 public:
  typedef typename OperationTraits::ValueType ValueType;
  
  virtual void init(int size) = 0;

  // [left;right]
  virtual ValueType query(int left, int right) const = 0;
  virtual void update(int element_index, const ValueType& value) = 0;

  virtual int size() const = 0;

  virtual ~DynamicRangeQueryProcessor() {}
};



template <class OperationTraits>
class ElementwiseRangeQueryProcessor
    : public DynamicRangeQueryProcessor<OperationTraits> {
 public:
  typedef typename OperationTraits::ValueType ValueType;
  
  virtual void init(int size) {
    a.assign(size, OperationTraits::identity_element());
  }

  virtual ValueType query(int left, int right) const {
    ValueType range_value = OperationTraits::identity_element();
    for (int i = left; i <= right; ++i) {
      range_value = OperationTraits::operation(range_value, a[i]);
    }
    return range_value;
  }

  virtual void update(int element_index, const ValueType& value) {
    a[element_index] = value;
  }

  virtual int size() const {
    return a.size();
  }
 private:
  std::vector<int> a;
};

template <class OperationTraits>
class FenwickTree
    : public DynamicRangeQueryProcessor<OperationTraits> {
 public:
  typedef typename OperationTraits::ValueType ValueType;
  
  virtual void init(int size) {
    tree_.assign(size, OperationTraits::identity_element());
  }

  // apply OperationTraits::operation for elements at range [left;right]
  virtual ValueType query(int left, int right) const
    { return query(right) - query(left - 1); }

  // update element at position to value
  virtual void update(int position, const ValueType& value) {
    ValueType inverse_element =
        OperationTraits::inverse_element(query(position, position));
    ValueType difference = OperationTraits::operation(value, inverse_element);
    while (position < tree_.size()) {
      tree_[position] =
          OperationTraits::operation(tree_[position], difference);
      position = position | (position + 1);
    }
  }

  virtual int size() const {
    return tree_.size();
  }

 private:
  std::vector<ValueType> tree_;
  
  // apply OperationTraits::operation for elements at range [0;right]
  ValueType query(int right) const {
    ValueType range_value = OperationTraits::identity_element();
    while (right >= 0) {
      range_value = OperationTraits::operation(range_value,
                                              tree_[right]);
      right = (right & (right + 1)) - 1;
    }
    return range_value;
  }
};

template <class OperationTraits>
class SegmentTree
    : public DynamicRangeQueryProcessor<OperationTraits> {
 public:
  typedef typename OperationTraits::ValueType ValueType;
  
  SegmentTree()
      : size_(0)
  { }

  virtual void init(int size) {
    tree_.clear();
    size_ = size;
    tree_.resize(4 * size_);
    init_vertex(0, 0, size_ - 1);
  }
  
  virtual void update(int position, const ValueType& value) {
    assert(0 <= position);
    assert(position < size_);
    update_vertex(0, position, value);
  }
  
  virtual ValueType query(int left, int right) const {
    assert(left <= right);
    assert(0 <= left && left < size_);
    assert(0 <= right && right < size_);
    return query_vertex(0, left, right);
  }
  
  virtual int size() const {
    return size_;
  }

 private:
  struct Node {
    int left;
    int right;
    ValueType value;

    Node()
        : left(-1),
          right(-1)
    { }
    
    Node(int l, int r, const ValueType& v)
        : left(l),
          right(r),
          value(v)
    { }
  };

  void init_vertex(int root, int left, int right);
  void update_vertex(int root, int position, const ValueType& value);
  ValueType query_vertex(int root, int left, int right) const;
  
  std::vector<Node> tree_;
  int size_;
};


template <class OperationTraits>
void SegmentTree<OperationTraits>::init_vertex(
    int root,
    int left,
    int right) {
  tree_[root] = Node(left, right, OperationTraits::identity_element());
  if (left < right) {
    int middle = left + (right - left) / 2;
    init_vertex(2 * root + 1, left, middle);
    init_vertex(2 * root + 2, middle + 1, right);
  }
}


template <class OperationTraits>
void SegmentTree<OperationTraits>::update_vertex(
    int root,
    int position,
    const ValueType& value) {
  Node& node = tree_[root];
  if (node.left == node.right) {
    assert(node.left == position);
    node.value = value;
  } else {
    int middle = node.left + (node.right - node.left) / 2;
    if (position <= middle) {
      update_vertex(2 * root + 1, position, value);
    } else {
      update_vertex(2 * root + 2, position, value);
    }
    node.value = OperationTraits::operation(
        tree_[2 * root + 1].value,
        tree_[2 * root + 2].value);
  }      
}

template <class OperationTraits>
typename OperationTraits::ValueType SegmentTree<OperationTraits>::query_vertex(
    int root,
    int left,
    int right) const {
  const Node& node = tree_[root];
  if (node.left == left && node.right == right) {
    return node.value;
  } else {
    ValueType result = OperationTraits::identity_element();
    int middle = node.left + (node.right - node.left) / 2;
    if (left <= middle) {
      result = OperationTraits::operation(result,
                                           query_vertex(2 * root + 1,
                                                        left,
                                                        std::min(middle,
                                                                 right)));
    }
    if (middle + 1 <= right) {
      result = OperationTraits::operation(result,
                                          query_vertex(2 * root + 2,
                                                       std::max(left,
                                                                middle + 1),
                                                       right));
    }
    
    return result;
  }
}

template <class Comparator>
class StaticRangeQueryProcessor {
 public:
  typedef typename Comparator::first_argument_type ValueType;
  
  virtual void init(const std::vector<ValueType>& values) = 0;

  // returns position in [left;right]
  virtual int query(int left, int right) const = 0;

  virtual int size() const = 0;

  virtual ~StaticRangeQueryProcessor() {}
};

template <class Comparator>
class SparseTable
    : public StaticRangeQueryProcessor<Comparator> {
 public:
  typedef typename Comparator::first_argument_type ValueType;
  
  virtual void init(const std::vector<ValueType>& values);

  // returns position [left;right]
  virtual int query(int left, int right) const;
  virtual int size() const {
    return values_.size();
  }
  
 private:
  std::vector< std::vector<int> > table_;
  std::vector< ValueType > values_;
};

int log2(int n) {
  int res = 0;
  while (n != 1) {
    n /= 2;
    ++res;
  }
  return res;
}

template <class Comparator>
void SparseTable<Comparator>::init(
    const std::vector<ValueType>& values) {
  values_ = values;
  table_.assign(values_.size(),
                std::vector<ValueType>(log2(values_.size()) + 1));

  for (int i = 0; i < values.size(); ++i) {
    table_[i][0] = i;
  }

  for (int j = 1; (1 << j) <= values_.size(); ++j) {
    for (int i = 0; i + (1 << j) <= values_.size(); ++i) {
      if (Comparator()(values_[table_[i][j - 1]],
                       values_[table_[i + (1 << (j - 1))][j - 1]])) {
        table_[i][j] = table_[i][j - 1];
      } else {
        table_[i][j] = table_[i + (1 << (j - 1))][j - 1];
      }
    }
  }
}

template <class Comparator>
int SparseTable<Comparator>::query(int left, int right) const {
  int length = log2(right - left + 1);
  assert(0 <= left && left < size());
  assert(0 <= right && right < size());
  assert(left <= right);
  assert(length < table_[0].size());
  if (Comparator()(values_[table_[left][length]],
                   values_[table_[right - (1 << length) + 1][length]])) {
    return table_[left][length];
  } else {
    return table_[right - (1 << length) + 1][length];
  }
}


template <class Comparator>
class BenderFarachColton
    : public StaticRangeQueryProcessor<Comparator> {
 public:
  typedef typename Comparator::first_argument_type ValueType;
  
  virtual void init(const std::vector<ValueType>& values);

  // returns position [left;right]
  virtual int query(int left, int right) const;
  virtual int size() const {
    return size_;
  }
  
 private:
  struct Node {
    Node* left;
    Node* right;
    Node* parent;
    int key;
    ValueType priority;

    Node()
        : left(NULL),
          right(NULL),
          parent(NULL),
          key(-1)
    { }

    Node(int k, const ValueType& p)
        : left(NULL),
          right(NULL),
          parent(NULL),
          key(k),
          priority(p)
    { }
  };
  
  int get_part_code(int start, int length) const;
  void find_heights(const std::vector<ValueType>& values);
  void traverse_tree(Node* root, int height);
  void check_treap(Node* root);
  
  int block_size_;
  int size_;

  std::vector<int> keys_; 
  std::vector<int> heights_;
  std::vector<int> first_occurences_;
  std::vector<ValueType> block_minimums_;
  std::vector<int> block_minimums_positions_;
  SparseTable< std::less<int> > sparse_table_;
  std::vector< std::vector<int> > minimum_offsets_;
};


// for debugging only
template <class Comparator>
void BenderFarachColton<Comparator>::check_treap(Node* root) {
  if (root == NULL) {
    return;
  }
  if (root->left != NULL) {
    assert(root->left->key < root->key);
    assert(!Comparator()(root->left->priority, root->priority));
    check_treap(root->left);
  }
  if (root->right != NULL) {
    assert(root->right->key > root->key);
    assert(!Comparator()(root->right->priority, root->priority));
    check_treap(root->right);
  }
}


template <class Comparator>
void BenderFarachColton<Comparator>::traverse_tree(Node* root, int height) {
  first_occurences_[root->key] = heights_.size();
  keys_.push_back(root->key);
  heights_.push_back(height);
  if (root->left != NULL) {
    traverse_tree(root->left, height + 1);
    keys_.push_back(root->key);
    heights_.push_back(height);
  }
  if (root->right != NULL) {
    traverse_tree(root->right, height + 1);
    keys_.push_back(root->key);
    heights_.push_back(height);
  }
}


template <class Comparator>
void BenderFarachColton<Comparator>::find_heights(
    const std::vector<ValueType>& values) {
  Node* root = new Node(0, values[0]);
  Node* rightmost_node = root;
  for (int key = 1; key < values.size(); ++key) {
    while (Comparator()(values[key], rightmost_node->priority) &&
           rightmost_node->parent != NULL) {
      rightmost_node = rightmost_node->parent;
    }
    
    Node* newbie = new Node(key, values[key]);
    if (Comparator()(values[key], rightmost_node->priority)) {  
      newbie->left = root;
      root->parent = newbie;
      root = newbie;
      rightmost_node = root;
    } else {
      newbie->left = rightmost_node->right;
      if (rightmost_node->right != NULL) {
        rightmost_node->right->parent = newbie;
      }
      rightmost_node->right = newbie;
      newbie->parent = rightmost_node;
      rightmost_node = newbie;
    }
  }

  check_treap(root);

  first_occurences_.assign(values.size(), -1);
  traverse_tree(root, 0);
}

template <class Comparator>
void BenderFarachColton<Comparator>::init(
    const std::vector<ValueType>& values) {
  // initialization
  size_ = values.size();
  find_heights(values);
  block_size_ = std::max(1, log2(heights_.size()) / 2);
  assert(block_size_ < 31);
  
  int blocks_count = heights_.size() / block_size_;
  if (heights_.size() % block_size_ != 0) {
    ++blocks_count;
  }

  // get minimums of blocks
  block_minimums_.resize(blocks_count); 
  block_minimums_positions_.resize(blocks_count);
  int block_begin = 0;
  for (int block_index = 0; block_index < blocks_count; ++block_index) {
    int block_end = std::min(block_begin + block_size_,
                             static_cast<int>(heights_.size()));
    block_minimums_[block_index] = heights_[block_begin];
    block_minimums_positions_[block_index] = block_begin;
    for (int inner_index = block_begin + 1;
         inner_index < block_end;
         ++inner_index) {
      if (heights_[inner_index] < block_minimums_[block_index]) {
        block_minimums_[block_index] = heights_[inner_index];
        block_minimums_positions_[block_index] = inner_index;
      }
    }
    block_begin += block_size_; 
  }

  sparse_table_.init(block_minimums_);

  std::vector< std::vector<int> > part_minimums;
  std::vector< std::vector<int> > part_values;
  
  minimum_offsets_.push_back(std::vector<int>());
  part_minimums.push_back(std::vector<int>());
  part_values.push_back(std::vector<int>());
  
  minimum_offsets_.push_back(std::vector<int>(1, 0));
  part_minimums.push_back(std::vector<int>(1, 0));
  part_values.push_back(std::vector<int>(1, 0));

  for (int length = 2; length < block_size_; ++length) {
    minimum_offsets_.push_back(std::vector<int>(1 << (length - 1), 0));
    part_minimums.push_back(std::vector<int>(1 << (length - 1), 0));
    part_values.push_back(std::vector<int>(1 << (length - 1), 0));
    for (int code = 0; code < (1 << (length - 1)); ++code) {
      if (code & (1 << (length - 2))) {
        int previous_code = code & ~(1 << (length - 2));
        part_values[length][code] =
            part_values[length - 1][previous_code] - 1;
        if (part_values[length][code] <
                         part_minimums[length - 1][previous_code]) {
          part_minimums[length][code] = part_values[length][code];
          minimum_offsets_[length][code] = length - 1;
        } else {
          part_minimums[length][code] =
              part_minimums[length - 1][previous_code];
          minimum_offsets_[length][code] =
              minimum_offsets_[length - 1][previous_code];
        }
      } else {
        part_values[length][code] = part_values[length - 1][code] + 1;
        part_minimums[length][code] = part_minimums[length - 1][code];
        minimum_offsets_[length][code] = minimum_offsets_[length - 1][code];
      }
    }
  }                         
}

template <class Comparator>
int BenderFarachColton<Comparator>::get_part_code(int start, int length) const {
  int code = 0;
  for (int offset = 0; offset + 1 < length; ++offset) {
    if (heights_[start + offset + 1] < heights_[start + offset]) {
      code |= (1 << offset);
    }
  }
  return code;
}

template <class Comparator>
int BenderFarachColton<Comparator>::query(int left, int right) const {
  left = first_occurences_[left];
  right = first_occurences_[right];
  if (left > right) {
    std::swap(left, right);
  }

  int min_index = left;
  int left_block = left / block_size_;
  int left_part_length = 0;
  if (left % block_size_ != 0) {
    ++left_block;
    left_part_length = std::min(right + 1, left_block * block_size_) - left;
    int left_part_code = get_part_code(left, left_part_length);
    min_index = left + minimum_offsets_[left_part_length][left_part_code];
  }

  int right_block = (right - block_size_ + 1) / block_size_;
  int right_part_length = 0;
  if (right % block_size_ != block_size_ - 1) {
    int right_part_start;
    if (right < block_size_) {
      right_part_start = left;
    } else {
      right_part_start = std::max(left, (right_block + 1) * block_size_);
    }
    right_part_length = right - right_part_start + 1;
    int right_part_code = get_part_code(right_part_start,
                                  right_part_length);

    int min_in_tail_index =
        right_part_start +
        minimum_offsets_[right_part_length][right_part_code];
    if (heights_[min_in_tail_index] < heights_[min_index]) {
      min_index = min_in_tail_index;
    }
  }

  int total_length = right - left + 1;
  if (total_length - left_part_length - right_part_length >= block_size_) {
    int min_index_in_blocks =
        block_minimums_positions_[sparse_table_.query(left_block,
                                                      right_block)];
    if (heights_[min_index_in_blocks] < heights_[min_index]) {
      min_index = min_index_in_blocks;
    }
  }

  return keys_[min_index];
}

#endif  // _TOOLBOX_BASIC_RANGE_QUERIES_H_
