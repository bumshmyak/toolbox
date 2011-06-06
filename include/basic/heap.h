#ifndef _TOOLBOX_BASIC_HEAP_H_
#define _TOOLBOX_BASIC_HEAP_H_

#include <cassert>
#include <vector>
#include <algorithm>
#include <iostream>
#include <cmath>
#include <string>

template <class KeyType>
class PriorityQueueInterface {
 public:
  virtual void push(const KeyType& key) = 0;
  virtual KeyType top() = 0;
  virtual void pop() = 0;
  virtual int size() = 0;
  virtual bool empty() = 0;
  virtual ~PriorityQueueInterface() { }
};

template <class KeyType, int K = 2>
class KaryHeap : public PriorityQueueInterface<KeyType> {
 public:
  virtual void push(const KeyType& key);
  virtual KeyType top();
  virtual void pop();
  virtual int size() { return heap_.size(); }
  virtual bool empty() { return heap_.empty(); }
  void output();
 private:
  std::vector<KeyType> heap_;

  int parent(int heap_index) {
    return (heap_index - 1) / K;
  }

  int children_indexes_begin(int heap_index) {
    return heap_index * K + 1;
  }

  int children_indexes_end(int heap_index) {
    return std::min((heap_index + 1) * K + 1, static_cast<int>(heap_.size()));
  }

  void sift_up(int heap_index);
  void sift_down(int heap_index);
};

template <class KeyType, int K>
void KaryHeap<KeyType, K>::push(const KeyType& key) {
  heap_.push_back(key);
  sift_up(heap_.size() - 1);
}

template <class KeyType, int K>
KeyType KaryHeap<KeyType, K>::top() {
  return heap_[0];
}

template <class KeyType, int K>
void KaryHeap<KeyType, K>::pop() {
  std::swap(heap_[0], heap_[heap_.size() - 1]);
  heap_.pop_back();
  sift_down(0);
}


template <class KeyType, int K>
void KaryHeap<KeyType, K>::sift_up(int heap_index) {
  while (heap_index != 0 &&
         heap_[heap_index] < heap_[parent(heap_index)]) {
    std::swap(heap_[heap_index], heap_[parent(heap_index)]);
    heap_index = parent(heap_index);
  }
}

template <class KeyType, int K>
void KaryHeap<KeyType, K>::sift_down(int heap_index) {
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
    std::swap(heap_[heap_index], heap_[min_child_index]);
    sift_down(min_child_index);
  }
}

template <class KeyType, int K>
void KaryHeap<KeyType, K>::output() {
  std::cout << "---" << std::endl;
  for (int i = 0; i < heap_.size(); ++i) {
    std::cout << heap_[i] << ' ';
  }
  std::cout << std::endl;
  std::cout << "---" << std::endl;
}

template <class KeyType>
class FibonacciHeap : public PriorityQueueInterface<KeyType> {
 public:
  FibonacciHeap()
      : min_root_(NULL),
        elements_count_(0)
  { }
  ~FibonacciHeap();

  virtual void push(const KeyType& key);
  virtual KeyType top() { return min_root_->key; }
  virtual void pop();
  virtual int size() { return elements_count_; }
  virtual bool empty() { return elements_count_ == 0; }
  void output();
 private:
  struct Node {
    Node* next;
    Node* prev;
    Node* child;
    int degree;
    KeyType key;

    explicit Node(const KeyType& k)
        : next(this),
          prev(this),
          child(NULL),
          degree(0),
          key(k)
    { }
  };

  // merge two circular lists
  Node* merge(Node* first, Node* second);
  void remove(Node* element);
  void release(Node* root);
  Node* link(Node* first, Node* second);
  void output(Node* root, int depth);

  Node* min_root_;
  int elements_count_;
};

template <class KeyType>
FibonacciHeap<KeyType>::~FibonacciHeap() {
  release(min_root_);
}

template <class KeyType>
void FibonacciHeap<KeyType>::release(Node* root) {
  if (root == NULL) {
    return;
  }
  while (root->next != root) {
    release(root->child);
    Node* root_next = root->next;
    remove(root);
    delete root;
    root = root_next;
  };
  delete root;
}

template <class KeyType>
typename FibonacciHeap<KeyType>::Node* FibonacciHeap<KeyType>::merge(
    Node* first,
    Node* second) {
  if (first == NULL) {
    return second;
  }
  if (second == NULL) {
    return first;
  }

  first->next->prev = second->prev;
  second->prev->next = first->next;
  first->next = second;
  second->prev = first;

  if (first->key < second->key) {
    return first;
  } else {
    return second;
  }
}

template <class KeyType>
void FibonacciHeap<KeyType>::remove(Node* element) {
  element->prev->next = element->next;
  element->next->prev = element->prev;
}

template <class KeyType>
typename FibonacciHeap<KeyType>::Node* FibonacciHeap<KeyType>::link(
    Node* first,
    Node* second) {
  if (first->key < second->key) {
    std::swap(first, second);
  }
  // second < first: link first to second
  remove(first);
  first->next = first;
  first->prev = first;
  second->child = merge(second->child, first);
  ++(second->degree);
  return second;
}


template <class KeyType>
void FibonacciHeap<KeyType>::output(Node* root, int depth) {
  if (root == NULL) {
    return;
  }
  Node* current = root;
  do {
    std::cout << std::string(depth, ' ') << current->key << std::endl;
    output(current->child, depth + 2);
    current = current->next;
  } while (current != root);
}


template <class KeyType>
void FibonacciHeap<KeyType>::output() {
  std::cout << "---" << std::endl;
  output(min_root_, 0);
  std::cout << "---" << std::endl;
}

template <class KeyType>
void FibonacciHeap<KeyType>::push(const KeyType& key) {
  Node* root = new Node(key);
  min_root_ = merge(min_root_, root);
  ++elements_count_;
}

template <class KeyType>
void FibonacciHeap<KeyType>::pop() {
  assert(!empty());
  Node* min_root_child = min_root_->child;
  Node* min_root_sibling = min_root_->next;
  if (min_root_sibling == min_root_) {
    min_root_sibling = NULL;
  }

  remove(min_root_);
  delete min_root_;
  --elements_count_;

  min_root_ = merge(min_root_child, min_root_sibling);
  if (min_root_ == NULL) {
    return;
  }

  // get roots count
  int roots_count = 0;
  {
    Node* begin = min_root_;
    Node* current = begin;
    do {
      ++roots_count;
      current = current->next;
    } while (current != begin);
  }

  // make only one tree of each degree
  std::vector<Node*> trees(static_cast<int>(log2(elements_count_)) + 1, NULL);
  Node* current = min_root_;
  for (int i = 0; i < roots_count; ++i) {
    Node* next = current->next;
    while (trees[current->degree] != NULL) {
      int degree = current->degree;
      current = link(current, trees[current->degree]);
      trees[degree] = NULL;
    }
    trees[current->degree] = current;
    current = next;
  }

  // find min
  min_root_ = NULL;
  for (int i = 0; i < trees.size(); ++i) {
    if ((trees[i] != NULL) &&
        (min_root_ == NULL || trees[i]->key < min_root_->key)) {
      min_root_ = trees[i];
    }
  }
}

#endif  //  _TOOLBOX_BASIC_HEAP_H_
