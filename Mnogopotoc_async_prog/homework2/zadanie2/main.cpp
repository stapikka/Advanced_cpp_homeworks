#include<iostream>
#include<string>
#include<thread>
#include<random>
#include<mutex>
#include<chrono>
#include<vector>

std::mutex mtx;

void printBar(char symbol, int thread_id, int width, int total_threads) {
	std::random_device rd;
	std::mt19937 gen(rd());
	std::uniform_int_distribution<> dist_ms(100, 1000);

	auto start = std::chrono::steady_clock::now();

	

	for (int i = 0; i < width; ++i) {
		auto random_duration = std::chrono::milliseconds(dist_ms(gen));
		std::this_thread::sleep_for(random_duration);

		std::unique_lock<std::mutex> lock(mtx);

		// Перемещаем курсор на соответствующую строку

		std::cout << "\033[" << (1 + thread_id) << ";1H";
		std::cout << "Thread " << thread_id << ": ";

		// Выводим прогресс
		for (int j = 0; j <= i; ++j) {
			std::cout << symbol;
		}
		std::cout.flush();
	}
	auto end = std::chrono::steady_clock::now();
	std::chrono::duration<double> duration = end - start;

	std::unique_lock<std::mutex> lock(mtx);
	int result_line = 2 + total_threads + thread_id;
	std::cout << "\033[" << result_line << ";1H";
	std::cout << "Thread " << thread_id << " completed in: " << duration.count() << " seconds" << std::endl;
}

int main() {

	int num_bars = 3;
	int width = 10;

	std::cout << "\033[2J\033[1;1HStarting " << num_bars << " parallel progress bars:\n";
	std::cout.flush();

	std::vector<std::thread> threads;

	for (int i = 0; i < num_bars; ++i) {
		
		const char symbol = '+';
		threads.push_back(std::thread(printBar, symbol, i + 1, width, num_bars));
	}

	for (auto &t : threads) {
		t.join();
	}

	std::cout << "\033[" << (3 + num_bars * 2 + 1) << ";1HAll " << num_bars << " threads completed!" << std::endl;
	return 0;
}