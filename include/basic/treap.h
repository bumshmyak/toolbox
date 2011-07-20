#ifndef _TOOLBOX_BASIC_TREAP_H_
#define _TOOLBOX_BASIC_TREAP_H_

#include <cassert>
#include <iterator>
#include <stdexcept>
#include <iostream>
#include <cstdlib>

template <class KeyType>
class Treap {
 private:
  class Node;
 public:
  class iterator;
  
 public:
  Treap();
  ~Treap();
  void insert(const KeyType& key);
  void erase(const KeyType& key);
  void erase(const iterator& position);
  iterator find(const KeyType& key);
  int size() const;
  bool empty() const;
  iterator begin();
  iterator end();
  KeyType minimum();
  KeyType maximum();
  void clear();
  void preorder_print();

  class iterator :
      public std::iterator<std::bidirectional_iterator_tag, KeyType> {
   public:
    explicit iterator(Treap* tree)
        : t(tree),
          p(NULL)
    { }
    explicit iterator(Treap* tree, Node* ptr)
        : t(tree),
          p(ptr)
    { }
    
    iterator& operator--() {
      p = t->get_predecessor(p);
      return *this;
    }
    
    iterator& operator++() {
      p = t->get_successor(p);
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
      return (t == rhs.t) && (p == rhs.p);
    }

    bool operator!=(const iterator& rhs) const {
      return (t != rhs.t) || (p != rhs.p);
    }
    
    const KeyType& operator*() const {
      return p->key;
    }
    
    const KeyType& operator->() const {
      return p->key;
    }

    friend class Treap<KeyType>;
    
   private:
    Treap<KeyType>* t;
    Node* p;
  };
  
 private:
  struct Node {
    Node* parent;
    Node* left;
    Node* right;
    KeyType key;
    int priority;

    explicit Node(const KeyType& k)
        : parent(NULL),
          left(NULL),
          right(NULL),
          key(k),
          priority(rand())
    { }
  };

  Node* get_successor(Node* position);
  Node* get_predecessor(Node* position);
  Node* get_minimum(Node* root);
  Node* get_maximum(Node* root);
  void clear(Node* root);
  void preorder_print(Node* root);

  static Node* merge(Node* lower_root, Node* greater_root);
  static void split(Node* root,
                    KeyType key,
                    Node*& lower_root,
                    Node*& greater_root);
  
  Node* root_;
  int elements_count_;
};

template <class KeyType>
Treap<KeyType>::Treap()
    : root_(NULL),
      elements_count_(0)
{ }

template <class KeyType>
Treap<KeyType>::~Treap() {
  clear();
}

template <class KeyType>
int Treap<KeyType>::size() const {
  return elements_count_;
}

template <class KeyType>
bool Treap<KeyType>::empty() const {
  return elements_count_ == 0;
}

template <class KeyType>
typename Treap<KeyType>::iterator
Treap<KeyType>::begin() {
  if (root_ == NULL) {
    return iterator(this);
  } else {
    return iterator(this, get_minimum(root_));
  }
}

template <class KeyType>
typename Treap<KeyType>::iterator
Treap<KeyType>::end() {
  return iterator(this);
};


template <class KeyType>
typename Treap<KeyType>::iterator Treap<KeyType>::find(
    const KeyType& key) {
  Node* root = root_;
  while (root != NULL && root->key != key) {
    if (key < root->key) {
      root = root->left;
    } else {
      root = root->right;
    }
  }

  return iterator(this, root);
}

template <class KeyType>
typename Treap<KeyType>::Node* Treap<KeyType>::merge(
    Node* lower_root,
    Node* greater_root) {
  if (lower_root == NULL) {
    return greater_root;
  }
  if (greater_root == NULL) {
    return lower_root;
  }
  if (lower_root->priority < greater_root->priority) {
    lower_root->right = merge(lower_root->right, greater_root);
    if (lower_root->right != NULL) {
      lower_root->right->parent = lower_root;
    }
    return lower_root;
  } else {
    greater_root->left = merge(lower_root, greater_root->left);
    if (greater_root->left != NULL) {
      greater_root->left->parent = greater_root;
    }
    return greater_root;
  }
}

template <class KeyType>
void Treap<KeyType>::split(Node* root,
                           KeyType key,
                           Node*& lower_root,
                           Node*& greater_root) {
  if (root == NULL) {
    lower_root = NULL;
    greater_root = NULL;
    return;
  }
  
  if (key == root->key) {
    greater_root = root->right;
    if (greater_root != NULL) {
      greater_root->parent = NULL;
    }
    
    root->right = NULL;
    lower_root = root;
  } else if (key < root->key) {
    if (root->left == NULL) {
      lower_root = NULL;
      greater_root = root;
    } else {
      split(root->left, key, lower_root, greater_root);
      if (lower_root != NULL) {
        lower_root->parent = NULL;
      }
      root->left = greater_root;
      if (greater_root != NULL) {
        greater_root->parent = root;
      }
      greater_root = root;
    }
  } else {  // key > root->key
    split(root->right, key, lower_root, greater_root);
    if (greater_root != NULL) {
      greater_root->parent = NULL;
    }
    root->right = lower_root;
    if (lower_root != NULL) {
      lower_root->parent = root;
    }
    lower_root = root;
  }
}
  


template <class KeyType>
void Treap<KeyType>::insert(const KeyType& key) {
  Node* lower_root;
  Node* greater_root;
  split(root_, key, lower_root, greater_root);
  root_ = merge(merge(lower_root, new Node(key)), greater_root);
  ++elements_count_;
}

template <class KeyType>
void Treap<KeyType>::erase(const KeyType& key) {
  erase(find(key));
}

template <class KeyType>
void Treap<KeyType>::erase(const iterator& position) {
  Node* pos = position.p;
  if (pos == NULL) {
    return;
  }

  Node* pos_substitute = merge(pos->left, pos->right);
  if (pos->parent == NULL) {
    root_ = pos_substitute;
  } else {
    if (pos->parent->left == pos) {
      pos->parent->left = pos_substitute;
    } else {
      pos->parent->right = pos_substitute;
    }
  }
  if (pos_substitute != NULL) {
    pos_substitute->parent = pos->parent;
  }
  delete pos;
  --elements_count_;
}

template <class KeyType>
typename Treap<KeyType>::Node*
Treap<KeyType>::get_minimum(Node* root) {
  assert(root != NULL);
  while (root->left != NULL) {
    root = root->left;
  }
  return root;
}

template <class KeyType>
typename Treap<KeyType>::Node*
Treap<KeyType>::get_maximum(Node* root) {
  assert(root != NULL);
  while (root->right != NULL) {
    root = root->right;
  }
  return root;
}


template <class KeyType>
KeyType Treap<KeyType>::maximum() {
  if (empty()) {
    throw std::runtime_error("tree is empty");
  }
  return get_maximum(root_)->key;
}

template <class KeyType>
KeyType Treap<KeyType>::minimum() {
  if (empty()) {
    throw std::runtime_error("tree is empty");
  }
  return get_minimum(root_)->key;
}

template <class KeyType>
typename Treap<KeyType>::Node*
Treap<KeyType>::get_successor(Node* position) {
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
typename Treap<KeyType>::Node*
Treap<KeyType>::get_predecessor(Node* position) {
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
void Treap<KeyType>::clear() {
  if (root_ != NULL) {
    clear(root_);
    delete root_;
    root_ = NULL;
    elements_count_ = 0;
  }
}

template <class KeyType>
void Treap<KeyType>::clear(Node* root) {
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
void Treap<KeyType>::preorder_print() {
  if (root_ != NULL) {
    preorder_print(root_);
    std::cout << std::endl;
  } else {
    std::cout << "()" << std::endl;
  }
}

template <class KeyType>
void Treap<KeyType>::preorder_print(Node* root) {
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
 
#endif  // _TOOLBOX_BASIC_TREAP_H_
