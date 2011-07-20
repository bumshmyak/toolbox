#ifndef _TOOLBOX_BASIC_RB_TREE_H_
#define _TOOLBOX_BASIC_RB_TREE_H_

#include <cassert>
#include <iterator>
#include <stdexcept>
#include <iostream>

template <class KeyType>
class RBTree {
 private:
  class Node;
 public:
  class iterator;
  
 public:
  RBTree();
  ~RBTree();
  void insert(const KeyType& key);
  void erase(const KeyType& key);
  void erase(const iterator& position);
  iterator find(const KeyType& key);
  int size() const;
  bool empty() const;
  iterator begin() const;
  iterator end() const;  // -- does't work
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
      iterator result(*this);
      operator++();
      return result;
    }
    
    iterator operator--(int) {  // NOLINT
      iterator result(*this);
      operator--();
      return result;
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
    
    const KeyType* operator->() const {
      return &(p->key);
    }

    friend class RBTree<KeyType>;
    
   private:
    Node* p;
  };
  
 private:
  struct Node {
    enum ColorType {
      BLACK = 0,
      RED = 1
    };
    
    Node* parent;
    Node* left;
    Node* right;
    KeyType key;
    ColorType color;

    explicit Node(const KeyType& k)
        : parent(NULL),
          left(NULL),
          right(NULL),
          key(k),
          color(BLACK)
    { }
  };

  static Node* get_successor(Node* position);
  static Node* get_predecessor(Node* position);
  static Node* get_minimum(Node* root);
  static Node* get_maximum(Node* root);
  static void clear(Node* root);
  static void preorder_print(Node* root);

  void rotate_left(Node* position);
  void rotate_right(Node* position);

  void insert_fixup(Node* position);
  static typename Node::ColorType get_color(Node* position);
    
  Node* root_;
  int elements_count_;
};

template <class KeyType>
RBTree<KeyType>::RBTree()
    : root_(NULL),
      elements_count_(0)
{ }

template <class KeyType>
RBTree<KeyType>::~RBTree() {
  clear();
}

template <class KeyType>
int RBTree<KeyType>::size() const {
  return elements_count_;
}

template <class KeyType>
bool RBTree<KeyType>::empty() const {
  return elements_count_ == 0;
}

template <class KeyType>
typename RBTree<KeyType>::iterator
RBTree<KeyType>::begin() const {
  if (root_ == NULL) {
    return iterator();
  } else {
    return iterator(get_minimum(root_));
  }
}

template <class KeyType>
typename RBTree<KeyType>::iterator
RBTree<KeyType>::end() const {
  return iterator();
};


template <class KeyType>
typename RBTree<KeyType>::iterator RBTree<KeyType>::find(
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
void RBTree<KeyType>::rotate_left(Node* position) {
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
void RBTree<KeyType>::rotate_right(Node* position) {
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
void RBTree<KeyType>::insert(const KeyType& key) {
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
  root->color = Node::RED;
  insert_fixup(root);
}

template <class KeyType>
typename RBTree<KeyType>::Node::ColorType RBTree<KeyType>::get_color(
    Node* position) {
  if (position == NULL) {
    return Node::BLACK;
  } else {
    return position->color;
  }
}

template <class KeyType>
void RBTree<KeyType>::insert_fixup(Node* position) {
  while (get_color(position->parent) == Node::RED) {
    Node* ancle;
    if (position->parent->parent->left == position->parent) {
      ancle = position->parent->parent->right;
      if (get_color(ancle) == Node::BLACK) {
        if (position == position->parent->right) {
          position = position->parent;
          rotate_left(position);
        }
        position->parent->color = Node::BLACK;
        position->parent->parent->color = Node::RED;
        rotate_right(position->parent->parent);
      }
    } else {
      ancle = position->parent->parent->left;
      if (get_color(ancle) == Node::BLACK) {
        if (position == position->parent->left) {
          position = position->parent;
          rotate_right(position);
        }
        position->parent->color = Node::BLACK;
        position->parent->parent->color = Node::RED;
        rotate_left(position->parent->parent);
      } 
    }
    
    if (get_color(ancle) == Node::RED) {
      ancle->color = Node::BLACK;
      position->parent->color = Node::BLACK;
      position->parent->parent->color = Node::RED;
      position = position->parent->parent;
    }
  }
  root_->color = Node::BLACK;
}

template <class KeyType>
void RBTree<KeyType>::erase(const KeyType& key) {
  erase(find(key));
}

template <class KeyType>
void RBTree<KeyType>::erase(const iterator& position) {
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
typename RBTree<KeyType>::Node*
RBTree<KeyType>::get_minimum(Node* root) {
  assert(root != NULL);
  while (root->left != NULL) {
    root = root->left;
  }
  return root;
}

template <class KeyType>
typename RBTree<KeyType>::Node*
RBTree<KeyType>::get_maximum(Node* root) {
  assert(root != NULL);
  while (root->right != NULL) {
    root = root->right;
  }
  return root;
}


template <class KeyType>
KeyType RBTree<KeyType>::maximum() const {
  if (empty()) {
    throw std::runtime_error("tree is empty");
  }
  return get_maximum(root_)->key;
}

template <class KeyType>
KeyType RBTree<KeyType>::minimum() const {
  if (empty()) {
    throw std::runtime_error("tree is empty");
  }
  return get_minimum(root_)->key;
}

template <class KeyType>
typename RBTree<KeyType>::Node*
RBTree<KeyType>::get_successor(Node* position) {
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
typename RBTree<KeyType>::Node*
RBTree<KeyType>::get_predecessor(Node* position) {
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
void RBTree<KeyType>::clear() {
  if (root_ != NULL) {
    clear(root_);
    delete root_;
    root_ = NULL;
    elements_count_ = 0;
  }
}

template <class KeyType>
void RBTree<KeyType>::clear(Node* root) {
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
void RBTree<KeyType>::preorder_print() {
  if (root_ != NULL) {
    preorder_print(root_);
    std::cout << std::endl;
  } else {
    std::cout << "()" << std::endl;
  }
}

template <class KeyType>
void RBTree<KeyType>::preorder_print(Node* root) {
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
 
#endif  // _TOOLBOX_BASIC_RB_TREE_H_
