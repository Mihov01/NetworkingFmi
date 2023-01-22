#include <bits/stdc++.h>
template <typename Iterator, typename Compare>
void ParallelQuickSort(Iterator begin, Iterator end, Compare cmp, size_t num_threads) {
  // Base case: if the range is empty or has only one element, it is already sorted
  if (begin == end || std::next(begin) == end) {
    return;
  }

  // Choose a pivot element and partition the range around it
  Iterator pivot = std::next(begin, std::distance(begin, end) / 2);
  std::nth_element(begin, pivot, end, cmp);
  Iterator middle1 = std::partition(begin, end, [&](const auto & x) { return cmp(x, *pivot); });
  Iterator middle2 = std::partition(middle1, end, [&](const auto& x) { return !cmp(*pivot, x); });

  // Sort the two partitions in parallel using a fixed number of threads
  std::vector<std::thread> threads;
  if (num_threads > 1) {
    threads.emplace_back([&] {
      ParallelQuickSort(begin, middle1, cmp, num_threads / 2);
    });
    threads.emplace_back([&] {
      ParallelQuickSort(middle2, end, cmp, num_threads / 2);
    });
  } else {
    ParallelQuickSort(begin, middle1, cmp, 1);
    ParallelQuickSort(middle2, end, cmp, 1);
  }

  // Wait for the threads to complete
  for (auto& thread : threads) {
    thread.join();
  }
}