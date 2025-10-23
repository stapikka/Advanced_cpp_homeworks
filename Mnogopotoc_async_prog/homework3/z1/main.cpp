#include<iostream>
#include<vector>
#include<future>

// Поиск минимального индекса
std::future<size_t> minimum_index(const std::vector<int>& vec, size_t start) {
	std::promise<size_t> promise;
	std::future<size_t> future = promise.get_future();

	std::async(std::launch::async, [promise = std::move(promise), &vec, start]() mutable {
		size_t min_index = start;
		for (size_t i = start + 1; i < vec.size(); ++i) {
			if (vec[i] < vec[min_index])
				min_index = i;
		}
		promise.set_value(min_index);
	});

	return future;
}

// Сортировка
void sort(std::vector<int>& v) {
	int n = v.size();
	for (int i = 0; i < n - 1; ++i) {
		std::future<size_t> future = minimum_index(v, i);

		size_t min_index = future.get();

		if (min_index != i)
			std::swap(v[i], v[min_index]);

		std::cout << "Step " << i << " in position: " << min_index << std::endl;
	}
}

void print(const std::vector<int>& vector) {
	for (const auto& el : vector) {
		std::cout << el << " ";
	}
	std::cout << std::endl;
}

int main() {
	
	std::vector<int> vec{ 1, 2, 5, 7, 3, 9, 4, 6, 15, 0 };
	print(vec);
	sort(vec);
	print(vec);


	return 0;
}