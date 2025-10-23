#include<iostream>
#include<vector>
#include<future>
#include<iterator>
#include<algorithm>

template<typename Iterator, typename Func>
void paral_for_each(Iterator first, Iterator last, Func&& func, size_t treshold = 100) {
	size_t size = std::distance(first, last);

	if (size <= treshold) {
		std::for_each(first, last, func);
		return;
	}

	Iterator mid = first;
	std::advance(mid, size / 2);

	auto left_future = std::async(std::launch::async, [first, mid, &func, treshold]() {
		paral_for_each(first, mid, func, treshold);
	});

	auto right_future = std::async(std::launch::async, [mid, last, &func, treshold]() {
		paral_for_each(mid, last, func, treshold);
	});

	left_future.get();
	right_future.get();
}

int main() {
	std::vector<int> vec(1000, 1);
	
	paral_for_each(vec.begin(), vec.end(), [](int& n) { n *= 2; });

	for (const auto& el : vec) {
		std::cout << el << " ";
	}
	std::cout << std::endl;

	return 0;
}