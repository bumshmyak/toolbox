#ifndef _TOOLBOX_BASIC_SPLAY_TREE_H_
#define _TOOLBOX_BASIC_SPLAY_TREE_H_

#include <cassert>
#include <iterator>
#include <stdexcept>
#include <iostream>

template <class KeyType>
class SplayTree {
 private:
  class Node;
 public:
  class iterator;
  
 public:
  SplayTree();
  ~SplayTree();
  void insert(const KeyType& key);
  void erase(const KeyType& key);
  void erase(const iterator& position);
  iterator find(const KeyType& key);
  int size() const;
  bool empty() const;
  iterator begin();
  iterator end();  // -- does't work
  KeyType minimum();
  KeyType maximum();
  void clear();
  void preorder_print();

  class iterator :
      public std::iterator<std::bidirectional_iterator_tag, KeyType> {
   public:
    explicit iterator(SplayTree* tree)
        : t(tree),
          p(NULL)
    { }
    explicit iterator(SplayTree* tree, Node* ptr)
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
    
    const KeyType* operator->() const {
      return &(p->key);
    }

    friend class SplayTree<KeyType>;
    
   private:
    SplayTree<KeyType>* t;
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

  Node* get_successor(Node* position);
  Node* get_predecessor(Node* position);
  Node* get_minimum(Node* root);
  Node* get_maximum(Node* root);
  void clear(Node* root);
  void preorder_print(Node* root);

  void rotate_left(Node* position);
  void rotate_right(Node* position);

  void splay(Node* position);
    
  Node* root_;
  int elements_count_;
};

template <class KeyType>
SplayTree<KeyType>::SplayTree()
    : root_(NULL),
      elements_count_(0)
{ }

template <class KeyType>
SplayTree<KeyType>::~SplayTree() {
  clear();
}

template <class KeyType>
int SplayTree<KeyType>::size() const {
  return elements_count_;
}

template <class KeyType>
bool SplayTree<KeyType>::empty() const {
  return elements_count_ == 0;
}

template <class KeyType>
typename SplayTree<KeyType>::iterator
SplayTree<KeyType>::begin() {
  if (root_ == NULL) {
    return iterator(this);
  } else {
    return iterator(this, get_minimum(root_));
  }
}

template <class KeyType>
typename SplayTree<KeyType>::iterator
SplayTree<KeyType>::end() {
  return iterator(this);
};


template <class KeyType>
typename SplayTree<KeyType>::iterator SplayTree<KeyType>::find(
    const KeyType& key) {
  Node* root = root_;
  while (root != NULL && root->key != key) {
    if (key < root->key) {
      root = root->left;
    } else {
      root = root->right;
    }
  }

  splay(root);
  return iterator(this, root);
}

template <class KeyType>
void SplayTree<KeyType>::rotate_left(Node* position) {
  assert(position != NULL);
  assert(position->right != NULL);

  Node* right_child = position->right;
  
  position->right = right_child->left;
  if (right_child->left != NULL) {
    right_child->left->parent = position;
  }

  right_child->parent = position->parent;
  if (position->parent == NULL) {
    root_ = right_child;
  } else if (position == position->parent->left) {
    position->parent->left = right_child;
  } else {
    position->parent->right = right_child;
  }
  right_child->left = position;
  position->parent = right_child;
}

template <class KeyType>
void SplayTree<KeyType>::rotate_right(Node* position) {
  assert(position != NULL);
  assert(position->left != NULL);

  Node* left_child = position->left;
  position->left = left_child->right;
  if (left_child->right != NULL) {
    left_child->right->parent = position;
  }

  left_child->parent = position->parent;
  if (position->parent == NULL) {
    root_ = left_child;
  } else if (position->parent->left == position) {
    position->parent->left = left_child;
  } else {
    position->parent->right = left_child;
  }
  left_child->right = position;
  position->parent = left_child;
}

template <class KeyType>
void SplayTree<KeyType>::splay(Node* position) {
  if (position == NULL) {
    return;
  }

  while (position->parent != NULL &&
         position->parent->parent != NULL) {
    if (position->parent->left == position) {
      if (position->parent->parent->left == position->parent) {
        // zig-zig
        rotate_right(position->parent->parent);
        rotate_right(position->parent);
      } else {
        // zig-zag
        rotate_right(position->parent);
        rotate_left(position->parent);
      }
    } else {
      if (position->parent->parent->right == position->parent) {
        // zig-zig
        rotate_left(position->parent->parent);
        rotate_left(position->parent);
      } else {
        // zig-zag
        rotate_left(position->parent);
        rotate_right(position->parent);
      }
    }
  }

  // zig
  if (position->parent != NULL) {
    if (position->parent->left == position) {
      rotate_right(position->parent);
    } else {
      rotate_left(position->parent);
    }
  }
}

template <class KeyType>
void SplayTree<KeyType>::insert(const KeyType& key) {
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
  splay(root);
}

template <class KeyType>
void SplayTree<KeyType>::erase(const KeyType& key) {
  erase(find(key));
}

template <class KeyType>
void SplayTree<KeyType>::erase(const iterator& position) {
  Node* pos = position.p;
  if (pos == NULL) {
    return;
  }
  splay(pos);

  Node* left_child = root_->left;
  Node* right_child = root_->right;

  delete root_;
  --elements_count_;

  if (left_child != NULL) {
    left_child->parent = NULL;
  }
  if (right_child != NULL) {
    right_child->parent = NULL;
  }

  if (left_child == NULL) {
    root_ = right_child;
  } else {
    root_ = left_child;
    Node* max_element = left_child;
    while (max_element->right != NULL) {
      max_element = max_element->right;
    }
    splay(max_element);
    root_->right = right_child;
    if (right_child != NULL) {
      right_child->parent = root_;
    }
  }
}

template <class KeyType>
typename SplayTree<KeyType>::Node*
SplayTree<KeyType>::get_minimum(Node* root) {
  assert(root != NULL);
  while (root->left != NULL) {
    root = root->left;
  }
  splay(root);
  return root;
}

template <class KeyType>
typename SplayTree<KeyType>::Node*
SplayTree<KeyType>::get_maximum(Node* root) {
  assert(root != NULL);
  while (root->right != NULL) {
    root = root->right;
  }
  splay(root);
  return root;
}


template <class KeyType>
KeyType SplayTree<KeyType>::maximum() {
  if (empty()) {
    throw std::runtime_error("tree is empty");
  }
  return get_maximum(root_)->key;
}

template <class KeyType>
KeyType SplayTree<KeyType>::minimum() {
  if (empty()) {
    throw std::runtime_error("tree is empty");
  }
  return get_minimum(root_)->key;
}

template <class KeyType>
typename SplayTree<KeyType>::Node*
SplayTree<KeyType>::get_successor(Node* position) {
  if (position == NULL) {
    return NULL;
  }
  splay(position);
  if (position->right != NULL) {
    return get_minimum(position->right);
  } else {
    return NULL;
  }
}

template <class KeyType>
typename SplayTree<KeyType>::Node*
SplayTree<KeyType>::get_predecessor(Node* position) {
  if (position == NULL) {
    return NULL;
  }
  splay(position);
  if (position->left != NULL) {
    return get_maximum(position->left);
  } else {
    return NULL;
  }
}

template <class KeyType>
void SplayTree<KeyType>::clear() {
  if (root_ != NULL) {
    clear(root_);
    delete root_;
    root_ = NULL;
    elements_count_ = 0;
  }
}

template <class KeyType>
void SplayTree<KeyType>::clear(Node* root) {
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
void SplayTree<KeyType>::preorder_print() {
  if (root_ != NULL) {
    preorder_print(root_);
    std::cout << std::endl;
  } else {
    std::cout << "()" << std::endl;
  }
}

template <class KeyType>
void SplayTree<KeyType>::preorder_print(Node* root) {
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
 
#endif  // _TOOLBOX_BASIC_SPLAY_TREE_H_
