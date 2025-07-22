#include <iostream>
#include <vector>
 
class Counter {
  int count;
  int sum;
 
public:
  Counter() : count(0), sum(0) {}
  void operator()(int value) {
    if (value % 3 == 0) {
      sum += value;
      count++;
    }
  }
  int getSum() { return sum; }
  int getCount() { return count; }
};
 
int main() {
  Counter c;
  std::vector<int> v = {4, 1, 3, 6, 25, 54};
  for (const auto &el : v) {
    std::cout << el << " ";
  }
  std::cout << std::endl;
 
  for (auto &el : v) {
    c(el);
  }
  int count = c.getCount();
  int sum = c.getSum();
 
  std::cout << "Count: " << count << std::endl;
  std::cout << "Sum: " << sum << std::endl;
 
  return 0;
}