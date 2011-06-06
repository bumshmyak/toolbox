#include <basic/dsu.h>

DisjointSetUnion::DisjointSetUnion(int elements_count)
    : elements_(elements_count),
      components_count_(elements_count) {
  for (int element_index = 0; element_index < elements_count; ++element_index) {
    elements_[element_index].parent = -1;
    elements_[element_index].rank = 0;
  }
}

int DisjointSetUnion::get_root(int key) {
  int root = key;
  while (elements_[root].parent != -1) {
    root = elements_[root].parent;
  }

  // path compression
  if (key != root) {
    while (elements_[key].parent != root) {
      int parent = elements_[key].parent;
      elements_[key].parent = root;
      key = parent;
    }
  }

  return root;
}

void DisjointSetUnion::unite(int first_key, int second_key) {
  int first_root = get_root(first_key);
  int second_root = get_root(second_key);

  if (first_root != second_root) {
    if (elements_[first_root].rank < elements_[second_root].rank) {
      elements_[first_root].parent = second_root;
    } else {
      elements_[second_root].parent = first_root;
      if (elements_[first_root].rank == elements_[second_root].rank) {
        ++elements_[first_root].rank;
      }
    }
    --components_count_;
  }
}
