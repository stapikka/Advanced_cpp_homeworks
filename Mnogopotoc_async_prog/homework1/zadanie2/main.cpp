#include <iostream>
#include <vector>
#include <thread>
#include <chrono>
#include <algorithm>
#include <iomanip>

std::vector<int> parallel_sum(const std::vector<int>& vec1,
		const std::vector<int>& vec2,
		int num_threads) {

	if (vec1.size() != vec2.size()) {
		throw std::invalid_argument("Wrong vector size!");
	}

	std::vector<int> result(vec1.size());
	std::vector<std::thread> threads;

	size_t chunk_size = (vec1.size() + num_threads - 1) / num_threads;

	for (size_t i = 0; i < num_threads; ++i) {
		size_t start = i * chunk_size;
		size_t end = std::min((i + 1) * chunk_size, vec1.size());

		if (start >= vec1.size()) break;

		threads.emplace_back([&vec1, &vec2, &result, start, end]() {
			for (size_t j = start; j < end; ++j) {
				result[j] = vec1[j] + vec2[j];
			}
		});
	}

	for (auto& t : threads) {
		t.join();
	}

	return result;
}

int main() {
	std::vector<std::vector<double>> table(5, std::vector<double>(5));

	std::vector<size_t> element_counts = { 1000, 10000, 100000, 1000000 };

	std::vector<int> thread_counts = { 2, 4, 8, 16 };

    table[1][0] = thread_counts[0];
    table[2][0] = thread_counts[1];
    table[3][0] = thread_counts[2];
    table[4][0] = thread_counts[3];


	for (size_t col = 0; col < element_counts.size(); ++col) {
		size_t i = element_counts[col];
		std::vector<int> v1(i, 10);
		std::vector<int> v2(i, 10);

		for (size_t row = 0; row < thread_counts.size(); ++row) {
			int j = thread_counts[row];
			auto start = std::chrono::steady_clock::now();
			auto res = parallel_sum(v1, v2, j);
			auto end = std::chrono::steady_clock::now();

			std::chrono::duration<double> seconds = end - start;
			table[row + 1][col + 1] = seconds.count();
		}
	}

	std::cout << "\nResults: parallel_sum:\n";
	std::cout << "============================================================================\n";
	std::cout << std::fixed << std::setprecision(6);

	std::cout << std::setw(12) << "Threads";
	for (auto elem : element_counts) {
		std::cout << std::setw(15) << elem;
	}
	std::cout << std::endl;

	for (size_t row = 1; row < table.size(); ++row) {
		std::cout << std::setw(12) << static_cast<int>(table[row][0]);
		for (size_t col = 1; col < table[row].size(); ++col) {
			std::cout << std::setw(15) << table[row][col];
		}
		std::cout << std::endl;
	}



	return 0;
}