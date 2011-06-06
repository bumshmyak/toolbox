#ifndef _TOOLBOX_BASIC_DSU_H_
#define _TOOLBOX_BASIC_DSU_H_

#include <vector>

class DisjointSetUnion {
 public:
  explicit DisjointSetUnion(int elements_count);

  int get_root(int key);
  void unite(int first_key, int second_key);
  int get_components_count() const
    { return components_count_; }
 private:
  struct element_type {
    int parent;
    int rank;
  };

  std::vector< element_type > elements_;
  int components_count_;
};


#endif  // _TOOLBOX_BASIC_DSU_H_
