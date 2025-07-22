#include <iostream>
#include <vector>

template <typename T>
T square(T a) {
    return a * a;
}

template <>
std::vector<int> square(std::vector<int> vec) {
    for(auto& elem : vec) {
        elem *= elem;
    }
    return vec;
}

void printV(const std::vector<int>& vec) {
    for(const auto& i : vec) {
        std::cout << i << " ";
    }
    std::cout << std::endl;
}

int main() {
    int x = 4;
    std::cout << "x = " << x << std::endl;
    std::cout << "square(x) = " << square(x) << std::endl;

    std::vector<int> v = {-1, 4, 8};
    printV(v);
    std::vector<int> b = square(v);
    printV(b);
    

}