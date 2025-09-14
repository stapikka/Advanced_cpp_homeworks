#include <iostream>
#include <vector>
#include <set>


int main() {

    int count{};

    std::cin >> count;
    std::vector<int> vec(count);

    for(int i = 0; i < count; ++i) {
        std::cin >> vec.at(i);
    }

    std::set<int, std::greater<int>> sortedVec(vec.begin(), vec.end());
    for(const auto& i : sortedVec) {
        std::cout << i << " ";
    }
    std::cout << std::endl;

}