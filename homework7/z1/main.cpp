#include <iostream>
#include <unordered_map>
#include <vector>
#include <string>
#include <algorithm>

int main() {

    std::string text;
    std::cout << "Enter text" << std::endl;
    std::getline(std::cin, text);
    
    std::unordered_map<char, int> freqMap;
    for(char c : text) {
        freqMap [c]++;
    }

    std::vector<std::pair<char, int>> sortedMap(freqMap.begin(), freqMap.end());

    std::sort(sortedMap.begin(), sortedMap.end(), [](const auto& a, const auto& b){return a.second > b.second;});

    for(const auto& el : sortedMap) {
        std::cout << el.first << " " << el.second << std::endl;
    }
}