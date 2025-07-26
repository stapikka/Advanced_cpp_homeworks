#include <iostream>
#include <vector>

template <typename T>
T square(T a) {
    return a * a;
}

template <typename T>
std::vector<T> square(std::vector<T> vec) {
    for(auto& elem : vec) {
        elem *= elem;
    }
    return vec;
}

template <typename T>
void printV(const std::vector<T>& vec) {
    for(const auto& i : vec) {
        std::cout << i << " ";
    }
    std::cout << std::endl;
}

int main() {
    int x = 4;
    std::cout << "x = " << x << std::endl;
    std::cout << "square(x) = " << square(x) << std::endl;

    std::vector<double> v = {-1.1, 4.3, 8.2};
    printV(v);
    std::vector<double> b = square(v);
    printV(b);
    

}