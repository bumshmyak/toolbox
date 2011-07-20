#ifndef _TOOLBOX_BASIC_SPLITTABLE_STRING_H_
#define _TOOLBOX_BASIC_SPLITTABLE_STRING_H_

#include <cassert>
#include <stdexcept>
#include <iostream>
#include <string>

template <class CharacterType>
class SplittableString {
 public:
  SplittableString();

  void init(const std::basic_string<CharacterType>& sequence);
  void clear();
  
  SplittableString append(SplittableString& another);

  void split(int position,
             SplittableString& lower_part,
             SplittableString& higher_part);
  
  int size() const {
    return size(root_);
  }
  
  bool empty() const {
    return size() == 0;
  }

  std::basic_string<CharacterType> tostring(); 
  
 private:
  struct Node {
    Node* parent;
    Node* left;
    Node* right;
    CharacterType character;
    int size;
    
    explicit Node(const CharacterType& c)
        : parent(NULL),
          left(NULL),
          right(NULL),
          character(c),
          size(1)
    { }
  };

  Node* get_maximum(Node* root);
  void clear(Node* root);
  std::basic_string<CharacterType> tostring(Node* root);

  void rotate_left(Node* position);
  void rotate_right(Node* position);

  void splay(Node* position);

  int size(Node* root) const {
    return root == NULL ? 0 : root->size;
  }
    
  Node* root_;
};

template <class CharacterType>
SplittableString<CharacterType>::SplittableString()
    : root_(NULL)
{ }


template <class CharacterType>
void SplittableString<CharacterType>::rotate_left(Node* position) {
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

  right_child->size = size(position);
  position->size = size(position->left) + size(position->right) + 1;
}

template <class CharacterType>
void SplittableString<CharacterType>::rotate_right(Node* position) {
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

  left_child->size = size(position);
  position->size = size(position->left) + size(position->right) + 1;
}

template <class CharacterType>
void SplittableString<CharacterType>::splay(Node* position) {
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

template <class CharacterType>
typename SplittableString<CharacterType>::Node*
SplittableString<CharacterType>::get_maximum(Node* root) {
  assert(root != NULL);
  while (root->right != NULL) {
    root = root->right;
  }
  splay(root);
  return root;
}

template <class CharacterType>
SplittableString<CharacterType>
SplittableString<CharacterType>::append(
    SplittableString<CharacterType>& another) {
  if (another.empty()) {
    return *this;
  }
  
  if (empty()) {
    root_ = another.root_;
  } else {
    splay(get_maximum(root_));
    root_->right = another.root_;
    another.root_->parent = root_;
    root_->size += another.root_->size;
  }

  return *this;
}


template <class CharacterType>
void SplittableString<CharacterType>::split(
    int position,
    SplittableString& lower_part,
    SplittableString& higher_part) {
  assert(0 <= position && position < size());
  Node* root = root_;
  while (position != size(root->left)) {
    if (position < size(root->left)) {
      root = root->left;
    } else {
      position -= size(root->left) + 1;
      root = root->right;
    }
  }

  splay(root);

  lower_part.root_ = root_->left;
  higher_part.root_ = root_;

  if (lower_part.root_ != NULL) {
    higher_part.root_->left = NULL;
    lower_part.root_->parent = NULL;
    higher_part.root_->size -= lower_part.root_->size;
  }
}

template <class CharacterType>
void SplittableString<CharacterType>::init(
    const std::basic_string<CharacterType>& sequence) {
  for (int i = 0; i < sequence.size(); ++i) {
    SplittableString<CharacterType> newbie;
    newbie.root_ = new Node(sequence[i]);
    append(newbie);
  }
}


template <class CharacterType>
void SplittableString<CharacterType>::clear() {
  if (root_ != NULL) {
    clear(root_);
    delete root_;
    root_ = NULL;
  }
}

template <class CharacterType>
void SplittableString<CharacterType>::clear(Node* root) {
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

template <class CharacterType>
std::basic_string<CharacterType> SplittableString<CharacterType>::tostring() {
  return tostring(root_);
}

template <class CharacterType>
std::basic_string<CharacterType>
SplittableString<CharacterType>::tostring(Node* root) {
  if (root == NULL) {
    return std::basic_string<CharacterType>();
  } else {
    return tostring(root->left) +
        root->character +
        tostring(root->right);
  }     
}


#endif  // _TOOLBOX_BASIC_SPLITTABLE_STRING_H_
