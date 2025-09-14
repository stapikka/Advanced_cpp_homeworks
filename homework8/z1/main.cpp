#include <iostream>
#include <vector>
#include <algorithm>

void printvec(std::vector<int>& vector) {
    for(const auto& el : vector) {
        std::cout << el << " ";
    }
    std::cout << std::endl;
}

void deleteDubl(std::vector<int>& vector) {
    std::sort(vector.begin(), vector.end());
    auto it = std::unique(vector.begin(), vector.end());
    vector.erase(it, vector.end());
}

int main() {
    std::vector<int> vec = {1, 1, 2, 5, 6, 1, 2, 4};
    printvec(vec);
    deleteDubl(vec);
    printvec(vec);
}