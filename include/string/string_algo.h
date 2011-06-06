#ifndef _TOOLBOX_STRING_STRING_ALGO_H_
#define _TOOLBOX_STRING_STRING_ALGO_H_

#include <algorithm>
#include <vector>

namespace bicycle {

template <class RandomAccessIterator>
int lis_dummy(RandomAccessIterator first, RandomAccessIterator last) {
  int size = std::distance(first, last);

  // d[i] - length of maximal increasing subsequence ending in *(first + i)
  std::vector<int> d(size, 1);
  int res = 0;

  for (int i = 0; i < size; ++i) {
    for (int j = 0; j < i; ++j) {
      if (*(first + j) < *(first + i)) {
        d[i] = std::max(d[i], d[j] + 1);
      }
    }
    res = std::max(res, d[i]);
  }

  return res;
}
}

#endif  // TOOLBOX_STRING_STRING_ALGO_H_
