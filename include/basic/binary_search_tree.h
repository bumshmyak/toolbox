#ifndef _TOOLBOX_BASIC_BINARY_SEARCH_TREE_H_
#define _TOOLBOX_BASIC_BINARY_SEARCH_TREE_H_

#include <cassert>
#include <iterator>
#include <stdexcept>
#include <iostream>

template <class KeyType>
class BinarySearchTree {
 private:
  class Node;
 public:
  class iterator;
  
 public:
  BinarySearchTree();
  ~BinarySearchTree();
  iterator insert(const KeyType& key);
  void erase(const KeyType& key);
  void erase(const iterator& position);
  iterator find(const KeyType& key);
  int size() const;
  bool empty() const;
  iterator begin() const;
  iterator end() const;
  KeyType minimum() const;
  KeyType maximum() const;
  void clear();
  void preorder_print();

  class iterator :
      public std::iterator<std::bidirectional_iterator_tag, KeyType> {
   public:
    iterator()
        : p(NULL)
    { }
    explicit iterator(Node* ptr)
        : p(ptr)
    { }
    
    iterator& operator--() {
      p = get_predecessor(p);
      return *this;
    }
    
    iterator& operator++() {
      p = get_successor(p);
      return *this;
    }
    
    iterator operator++(int) {  // NOLINT
      iterator tmp(*this);
      operator++();
      return tmp;
    }
    
    iterator operator--(int) {  // NOLINT
      iterator tmp(*this);
      operator--();
      return tmp;
    }
    
    bool operator==(const iterator& rhs) const {
      return p == rhs.p;
    }

    bool operator!=(const iterator& rhs) const {
      return p != rhs.p;
    }
    
    const KeyType& operator*() const {
      return p->key;
    }
    
    const KeyType& operator->() const {
      return p->key;
    }

    friend class BinarySearchTree<KeyType>;
    
   private:
    Node* p;
  };
  
 private:
  struct Node {
    Node* parent;
    Node* left;
    Node* right;
    KeyType key;

    explicit Node(const KeyType& k)
        : parent(NULL),
          left(NULL),
          right(NULL),
          key(k)
    { }
  };

  static Node* get_successor(Node* position);
  static Node* get_predecessor(Node* position);
  static Node* get_minimum(Node* root);
  static Node* get_maximum(Node* root);
  static void clear(Node* root);
  static void preorder_print(Node* root);
    
  Node* root_;
  int elements_count_;
};

template <class KeyType>
BinarySearchTree<KeyType>::BinarySearchTree()
    : root_(NULL),
      elements_count_(0)
{ }

template <class KeyType>
BinarySearchTree<KeyType>::~BinarySearchTree() {
  clear();
}

template <class KeyType>
int BinarySearchTree<KeyType>::size() const {
  return elements_count_;
}

template <class KeyType>
bool BinarySearchTree<KeyType>::empty() const {
  return elements_count_ == 0;
}

template <class KeyType>
typename BinarySearchTree<KeyType>::iterator
BinarySearchTree<KeyType>::begin() const {
  if (root_ == NULL) {
    return iterator();
  } else {
    return iterator(get_minimum(root_));
  }
}

template <class KeyType>
typename BinarySearchTree<KeyType>::iterator
BinarySearchTree<KeyType>::end() const {
  return iterator();
};


template <class KeyType>
typename BinarySearchTree<KeyType>::iterator BinarySearchTree<KeyType>::find(
    const KeyType& key) {
  Node* root = root_;
  while (root != NULL && root->key != key) {
    if (key < root->key) {
      root = root->left;
    } else {
      root = root->right;
    }
  }
  
  return iterator(root);
}

template <class KeyType>
typename BinarySearchTree<KeyType>::iterator BinarySearchTree<KeyType>::insert(
    const KeyType& key) {
  Node* root = root_;
  Node* root_parent = NULL;
  bool is_left_child = true;
  while (root != NULL) {
    root_parent = root;
    if (key < root->key) {
      is_left_child = true;
      root = root->left;
    } else {
      is_left_child = false;
      root = root->right;
    }
  }

  root = new Node(key);
  root->parent = root_parent;

  if (root_parent == NULL) {
    root_ = root;
  } else if (is_left_child) {
    root_parent->left = root;  
  } else {
    root_parent->right = root;
  }

  ++elements_count_;

  return iterator(root);
}

template <class KeyType>
void BinarySearchTree<KeyType>::erase(const KeyType& key) {
  erase(find(key));
}

template <class KeyType>
void BinarySearchTree<KeyType>::erase(const iterator& position) {
  Node* pos = position.p;
  if (pos == NULL) {
    return;
  }

  if (pos->left == NULL || pos->right == NULL) {
    Node* child;
    if (pos->left != NULL) {
      child = pos->left;
    } else {
      child = pos->right;
    }
    if (child != NULL) {
      child->parent = pos->parent;
    }
    
    if (pos->parent == NULL) {
      root_ = child;
    } else {
      if (pos->parent->left == pos) {
        pos->parent->left = child;
      } else {
        pos->parent->right = child;
      }
    }
  } else {
    Node* successor = get_successor(pos);
    
    if (successor->parent->left == successor) {
      successor->parent->left = successor->right;
    } else {
      successor->parent->right = successor->right;
    }

    if (successor->right != NULL) {
      successor->right->parent = successor->parent;
    }
    
    successor->left = pos->left;
    if (successor->left != NULL) {
      successor->left->parent = successor;
    }

    successor->right = pos->right;
    if (successor->right != NULL) {
      successor->right->parent = successor;
    }
    
    successor->parent = pos->parent;

    if (pos->parent != NULL) {
      if (pos->parent->left == pos) {
        successor->parent->left = successor;
      } else {
        successor->parent->right = successor;
      }
    } else {
      root_ = successor;
    }
  }
  delete pos;
  --elements_count_;
  if (elements_count_ == 0) {
    root_ = NULL;
  }
}

template <class KeyType>
typename BinarySearchTree<KeyType>::Node*
BinarySearchTree<KeyType>::get_minimum(Node* root) {
  assert(root != NULL);
  while (root->left != NULL) {
    root = root->left;
  }
  return root;
}

template <class KeyType>
typename BinarySearchTree<KeyType>::Node*
BinarySearchTree<KeyType>::get_maximum(Node* root) {
  assert(root != NULL);
  while (root->right != NULL) {
    root = root->right;
  }
  return root;
}


template <class KeyType>
KeyType BinarySearchTree<KeyType>::maximum() const {
  if (empty()) {
    throw std::runtime_error("tree is empty");
  }
  return get_maximum(root_)->key;
}

template <class KeyType>
KeyType BinarySearchTree<KeyType>::minimum() const {
  if (empty()) {
    throw std::runtime_error("tree is empty");
  }
  return get_minimum(root_)->key;
}

template <class KeyType>
typename BinarySearchTree<KeyType>::Node*
BinarySearchTree<KeyType>::get_successor(Node* position) {
  if (position == NULL) {
    return NULL;
  }
  if (position->right != NULL) {
    return get_minimum(position->right);
  } else {
    while (position->parent != NULL &&
           position->parent->right == position) {
      position = position->parent;
    }
    return position->parent;
  }
}

template <class KeyType>
typename BinarySearchTree<KeyType>::Node*
BinarySearchTree<KeyType>::get_predecessor(Node* position) {
  if (position == NULL) {
    return NULL;
  }
  if (position->left != NULL) {
    return get_maximum(position->left);
  } else {
    while (position->parent != NULL &&
           position->parent->left == position) {
      position = position->parent;
    }
    return position->parent;
  }
}

template <class KeyType>
void BinarySearchTree<KeyType>::clear() {
  if (root_ != NULL) {
    clear(root_);
    delete root_;
    root_ = NULL;
    elements_count_ = 0;
  }
}

template <class KeyType>
void BinarySearchTree<KeyType>::clear(Node* root) {
  assert(root != NULL);
  if (root->left != NULL) {
    clear(root->left);
    delete root->left;
  }
  
  if (root->right != NULL) {
    clear(root->right);
    delete root->right;
  }
}

template <class KeyType>
void BinarySearchTree<KeyType>::preorder_print() {
  if (root_ != NULL) {
    preorder_print(root_);
    std::cout << std::endl;
  } else {
    std::cout << "()" << std::endl;
  }
}

template <class KeyType>
void BinarySearchTree<KeyType>::preorder_print(Node* root) {
  std::cout << "( " << root->key;
  if (root->left != NULL) {
    std::cout << " L";
    preorder_print(root->left);
  }
  if (root->right != NULL) {
    std::cout << " R";
    preorder_print(root->right);
  }
  std::cout << " )";
}
 
#endif  // _TOOLBOX_BASIC_BINARY_SEARCH_TREE_H_
