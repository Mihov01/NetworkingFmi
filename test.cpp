# include "parralel_quickSort.hpp"
# include <vector>
#include <iostream>
#include <chrono>
#include <fstream>
# include<algorithm>

int main() {
  // Record the start time
  std::vector<int> arr;
  std::vector<int> arr1;
  std::ifstream in ("array.txt");
  int x ;
  while(in >> x)
  {
    arr.push_back(x);
  }
  arr1= arr;
  auto start = std::chrono::steady_clock::now();


  ParallelQuickSort(arr.begin(), arr.end(), std::less<int>(), std::thread::hardware_concurrency());


  auto end = std::chrono::steady_clock::now();

  auto elapsed = std::chrono::duration_cast<std::chrono::milliseconds>(end - start);

  std::cout << "Elapsed time Parallel quickSort: " << elapsed.count() << " milliseconds" << std::endl;
  std::cout << "\n";

  auto start1 = std::chrono::steady_clock::now();


  std::sort(arr1.begin(), arr1.end());


  auto end1 = std::chrono::steady_clock::now();

  auto elapsed1 = std::chrono::duration_cast<std::chrono::milliseconds>(end1 - start1);

  std::cout << "Elapsed time std::sort(): " << elapsed1.count() << " milliseconds" << std::endl;
  std::cout << "\n";

  return 0;
}
