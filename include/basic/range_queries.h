#ifndef _TOOLBOX_BASIC_RANGE_QUERIES_H_
#define _TOOLBOX_BASIC_RANGE_QUERIES_H_

#include <vector>
#include <iostream>

// group structure interface
template <typename T>
class ElementTraits {
 public:
  typedef T value_type;
  virtual T operation(const T& a, const T& b) const = 0;
  virtual T inverse_element(const T& a) const = 0;
  virtual T identity_element() const = 0;

  virtual ~ElementTraits() {}
};

// addition group implementation
template <typename T>
class SumTraits : ElementTraits<T> {
 public:
  typedef T value_type;
  T operation(const T& a, const T& b) const
    { return a + b; }
  T inverse_element(const T& a) const
    { return -a; }
  T identity_element() const
    { return 0; }
};

// range query processor interface
template <class ElementTraits>
class RangeQueryProcessor {
 public:
  typedef typename ElementTraits::value_type ElementType;

  // [left;right]
  virtual ElementType query(int left, int right) = 0;
  virtual void update(int element_index, const ElementType& value) = 0;

  virtual ~RangeQueryProcessor() {}
};

template <class ElementTraits>
class ElementwiseRangeQueryProcessor : RangeQueryProcessor<ElementTraits> {
 public:
  typedef typename ElementTraits::value_type ElementType;

  explicit ElementwiseRangeQueryProcessor(int n) {
    a.resize(n);
    std::fill(a.begin(), a.end(), element_traits_.identity_element());
  }

  ElementType query(int left, int right) {
    ElementType range_value = element_traits_.identity_element();
    for (int i = left; i <= right; ++i) {
      range_value = element_traits_.operation(range_value, a[i]);
    }
    return range_value;
  }

  void update(int element_index, const ElementType& value) {
    a[element_index] = value;
  }
 private:
  std::vector<int> a;
  ElementTraits element_traits_;
};

template <class ElementTraits>
class FenwickTree {
 public:
  typedef typename ElementTraits::value_type ElementType;

  explicit FenwickTree(int size) :
      fenwick_tree_(size, element_traits_.identity_element())
    { }

  // apply ElementTraits::operation for elements at range [left;right]
  ElementType query(int left, int right) const
    { return query(right) - query(left - 1); }

  // update element at position to value
  void update(int position, const ElementType& value) {
    ElementType inverse_element =
        element_traits_.inverse_element(query(position, position));
    ElementType difference = element_traits_.operation(value, inverse_element);
    while (position < fenwick_tree_.size()) {
      fenwick_tree_[position] =
          element_traits_.operation(fenwick_tree_[position], difference);
      position = position | (position + 1);
    }
  }

  int size() const
    { return fenwick_tree_.size(); }
 private:
  std::vector<ElementType> fenwick_tree_;
  ElementTraits element_traits_;

  // apply ElementTraits::operation for elements at range [0;right]
  ElementType query(int right) const {
    ElementType range_value = element_traits_.identity_element();
    while (right >= 0) {
      range_value = element_traits_.operation(range_value,
                                              fenwick_tree_[right]);
      right = (right & (right + 1)) - 1;
    }
    return range_value;
  }
};

#endif  // _TOOLBOX_BASIC_RANGE_QUERIES_H_
