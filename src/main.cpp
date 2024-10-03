#include <iostream>
#include <ranges>
#include <vector>

int main() {
  std::vector<int> vec = {1, 2, 3, 4, 5};
  for (int n : vec | std::views::reverse) {
    std::cout << n << " ";
  }
  std::cout << "\n";
  return 0;
}
