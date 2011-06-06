#ifndef GRAPH_HEAP_H
#define GRAPH_HEAP_H

#include <algorithm>
#include <iostream>
#include <cassert>
#include <vector>

template <class T>
class GraphVectorHeap {
 public:
  explicit GraphVectorHeap(int keys_count);
  void push(int key, const T& value);
  T get(int key);
  int top();
  void pop();
  void decrease_key(int key, const T& value);
  bool empty() { return elements_set_count_ == 0; }
 private:
  std::vector<T> values_;
  std::vector<bool> is_set_;
  int elements_set_count_;
};

template <class T>
GraphVectorHeap<T>::GraphVectorHeap(int keys_count)
    : values_(keys_count),
      is_set_(keys_count),
      elements_set_count_(0)
  { }

template <class T>
void GraphVectorHeap<T>::push(int key, const T& value) {
  values_[key] = value;
  is_set_[key] = true;
  ++elements_set_count_;
}

template <class T>
T GraphVectorHeap<T>::get(int key) {
  assert(is_set_[key]);
  return values_[key];
}

template <class T>
int GraphVectorHeap<T>::top() {
  assert(!empty());
  int top_key = -1;
  for (int i = 0; i < values_.size(); ++i) {
    if (is_set_[i] && (top_key == -1 || values_[i] < values_[top_key])) {
      top_key = i;
    }
  }

  return top_key;
}

template <class T>
void GraphVectorHeap<T>::pop() {
  assert(!empty());
  is_set_[top()] = false;
  --elements_set_count_;
}

template <class T>
void GraphVectorHeap<T>::decrease_key(int key, const T& value) {
  assert(is_set_[key]);
  values_[key] = value;
}

template <class T, int K = 2>
class GraphKaryHeap {
 public:
  explicit GraphKaryHeap(int keys_count);
  void push(int key, const T& value);
  T get(int key);
  int top();
  void pop();
  void decrease_key(int key, const T& value);
  bool empty() { return heap_size_ == 0; }
 private:
  struct Node {
    int key;
    T value;

    Node() :
        key(-1)
      { }

    Node(int k, const T& v) :
        key(k),
        value(v)
      { }
    bool operator<(const Node& other) {
      if (value == other.value) {
        return key < other.key;
      } else {
        return value < other.value;
      }
    }
  };

  int parent(int heap_index) {
    return (heap_index - 1) / K;
  }

  int children_indexes_begin(int heap_index) {
    return heap_index * K + 1;
  }

  int children_indexes_end(int heap_index) {
    return std::min((heap_index + 1) * K + 1, heap_size_);
  }

  void sift_up(int heap_index);
  void sift_down(int heap_index);

  void output() {
    std::cout << std::endl;
    for (int i = 0; i < heap_size_; ++i) {
      std::cout << '(' << heap_[i].key << ';' << heap_[i].value << ')' << ' ';
    }
    std::cout << std::endl;
  }

  std::vector<Node> heap_;
  std::vector<int> key2index_;
  int heap_size_;
};

template <class T, int K>
GraphKaryHeap<T, K>::GraphKaryHeap(int keys_count)
    : heap_(keys_count),
      key2index_(keys_count, -1),
      heap_size_(0)
  { }

template <class T, int K>
void GraphKaryHeap<T, K>::sift_up(int heap_index) {
  while (heap_index != 0 &&
         heap_[heap_index] < heap_[parent(heap_index)]) {
    std::swap(key2index_[heap_[heap_index].key],
              key2index_[heap_[parent(heap_index)].key]);
    std::swap(heap_[heap_index], heap_[parent(heap_index)]);
    heap_index = parent(heap_index);
  }
}

template <class T, int K>
void GraphKaryHeap<T, K>::sift_down(int heap_index) {
  int children_begin = children_indexes_begin(heap_index);
  int children_end = children_indexes_end(heap_index);

  if (children_begin >= children_end) {
    return;
  }

  int min_child_index = std::min_element(
                          heap_.begin() + children_begin,
                          heap_.begin() + children_end) -
                        heap_.begin();

  if (heap_[min_child_index] < heap_[heap_index]) {
    std::swap(key2index_[heap_[heap_index].key],
              key2index_[heap_[min_child_index].key]);
    std::swap(heap_[heap_index], heap_[min_child_index]);
    sift_down(min_child_index);
  }
}

template <class T, int K>
void GraphKaryHeap<T, K>::push(int key, const T& value) {
  assert(heap_size_ < heap_.size());
  assert(key < key2index_.size());
  heap_[heap_size_] = Node(key, value);
  key2index_[key] = heap_size_;
  sift_up(heap_size_);
  ++heap_size_;
}

template <class T, int K>
int GraphKaryHeap<T, K>::top() {
  return heap_[0].key;
}

template <class T, int K>
T GraphKaryHeap<T, K>::get(int key) {
  assert(key >= 0 && key < key2index_.size());
  int heap_index = key2index_[key];
  assert(heap_index >= 0 && heap_index < heap_size_);
  return heap_[heap_index].value;
}

template <class T, int K>
void GraphKaryHeap<T, K>::pop() {
  assert(heap_size_ > 0);
  std::swap(heap_[0], heap_[heap_size_ - 1]);
  key2index_[heap_[heap_size_ - 1].key] = -1;
  key2index_[heap_[0].key] = 0;
  --heap_size_;
  sift_down(0);
}

template <class T, int K>
void GraphKaryHeap<T, K>::decrease_key(int key, const T& value) {
  assert(key >= 0 && key < key2index_.size());
  int heap_index = key2index_[key];
  assert(heap_index >= 0 && heap_index < heap_size_);
  heap_[heap_index].value = value;
  sift_up(heap_index);
}

#endif  // GRAPH_HEAP_H
