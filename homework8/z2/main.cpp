#include <iostream>
#include <vector>
#include <list>
#include <set>
#include <string>

template <typename T>
void print_container(const T& container) {
    for(const auto& el : container) {
        std:: cout << el << " ";
    }
    std::cout << std::endl;
}

int main() {

    std::vector<std::string> vec = { "one", "two", "three", "four" };
    print_container(vec);

    std::set<std::string> test_set = { "one", "two", "three", "four" };
    print_container(test_set);

    std::list<std::string> test_list = { "one", "two", "three", "four" };
    print_container(test_list);
}