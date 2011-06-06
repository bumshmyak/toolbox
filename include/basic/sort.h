#ifndef TOOLBOX_BASIC_SORT_H_
#define TOOLBOX_BASIC_SORT_H_

#include <algorithm>
#include <iterator>

namespace bicycle {

// checks whether [first;last) is ordered using <
template <class ForwardIterator>
bool is_ordered(ForwardIterator first, ForwardIterator last) {
  while (first != last) {
    ForwardIterator prefirst = first;
    ++first;
    if (first != last && *prefirst > *first) {
      return false;
    }
  }

  return true;
}

// merges [first;middle) with [middle;last) using <
template <class RandomAccessIterator>
void merge(RandomAccessIterator first,
           RandomAccessIterator middle,
           RandomAccessIterator last,
           RandomAccessIterator work_area_first) {
  RandomAccessIterator left = first;
  RandomAccessIterator right = middle;
  RandomAccessIterator work_area_last = work_area_first;

  while (left < middle && right < last) {
    if (*left < *right) {
      std::swap(*left++, *work_area_last++);
    } else {
      std::swap(*right++, *work_area_last++);
    }
  }

  work_area_last = std::copy(right,
                             last,
                             std::copy(left, middle, work_area_last));

  std::copy(work_area_first, work_area_last, first);
}

// sort [first;last) using work_area_first
template <class RandomAccessIterator>
void merge_sort(RandomAccessIterator first,
                RandomAccessIterator last,
                RandomAccessIterator work_area_first) {
  int size = std::distance(first, last);
  for (int sorted_size = 1; sorted_size < size; sorted_size *= 2) {
    for (RandomAccessIterator begin = first;
         begin + sorted_size < last;
         begin += 2 * sorted_size) {
      merge(begin, begin + sorted_size,
            std::min(last, begin + 2 * sorted_size),
            work_area_first);
    }
  }
}

template <class RandomAccessIterator>
void inplace_merge_sort(RandomAccessIterator first, RandomAccessIterator last) {
  int size = std::distance(first, last);
  merge_sort(first + size / 2 + size % 2, last, first);
  size -= size / 2;
  while (size > 1) {
    merge_sort(first, first + size / 2, first + size / 2);
    merge(first, first + size / 2, first + size, last, first + size / 2);
    size -= size / 2;
  }

  // insert last one
  {
    RandomAccessIterator next = first;
    RandomAccessIterator prev = next++;
    while (next != last) {
      if (*prev > *next) {
        std::swap(*prev, *next);
        prev = next++;
      } else {
        break;
      }
    }
  }
}

// distance(first, last) >= 2
template <class ForwardIterator>
ForwardIterator lomuto_partition(ForwardIterator first, ForwardIterator last) {
  typename ForwardIterator::value_type pivot = *first;
  ForwardIterator lower = first;

  for (ForwardIterator i = first + 1; i != last; ++i) {
    if (*i < pivot) {
      ++lower;
      std::swap(*lower, *i);
    }
  }
  std::swap(*first, *lower);

  return lower;
}

template <class RandomAccessIterator>
void quick_sort(RandomAccessIterator first, RandomAccessIterator last) {
  if (first + 1 < last) {
    int n = std::distance(first, last);
    std::swap(*first, *(first + rand() % n));
    RandomAccessIterator pivot_iterator = lomuto_partition(first, last);
    quick_sort(first, pivot_iterator);
    quick_sort(pivot_iterator + 1, last);
  }
}

template <class ForwardIterator>
void min_and_max_element(
    ForwardIterator first,
    ForwardIterator last,
    ForwardIterator& min_iterator,
    ForwardIterator& max_iterator) {
  int n = std::distance(first, last);
  if (n == 0) {
    return;
  }

  // init
  if (n % 2) {
    min_iterator = first;
    max_iterator = first;
  } else {
    min_iterator = first;
    ++first;
    max_iterator = first;
    if (*min_iterator > *max_iterator) {
      std::swap(min_iterator, max_iterator);
    }
  }
  ++first;

  while (first != last) {
    ForwardIterator next = first;
    ++next;
    if (*first < *next) {
      if (*first < *min_iterator) {
        min_iterator = first;
      }
      if (*next > *max_iterator) {
        max_iterator = next;
      }
    } else {
      if (*next < *min_iterator) {
        min_iterator = next;
      }
      if (*first > *max_iterator) {
        max_iterator = first;
      }
    }
    first = next;
    ++first;
  }
}

// finds k-th order statistics in 0-indexing
template <class RandomAccessIterator>
RandomAccessIterator randomized_select(
    RandomAccessIterator first,
    RandomAccessIterator last,
    int k) {
  while (first < last) {
    int n = std::distance(first, last);
    std::swap(*first, *(first + rand() % n));
    RandomAccessIterator pivot_iterator = lomuto_partition(first, last);
    int pivot_index = std::distance(first, pivot_iterator);
    if (k == pivot_index) {
      return pivot_iterator;
    } else if (k > pivot_index) {
      first = pivot_iterator + 1;
      k -= (pivot_index + 1);
    } else {
      last = pivot_iterator;
    }
  }

  return first;
}
};  // bycycle ns

#endif  // TOOLBOX_BASIC_SORT_H_
