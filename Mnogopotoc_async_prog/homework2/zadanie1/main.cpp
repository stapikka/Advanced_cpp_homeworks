#include <iostream>
#include <chrono>
#include <thread>
#include <atomic>


std::atomic<int> COUNT{ 0 };
std::atomic<bool> ALL_CLIENTS_ADDED{ false };

void client() {
	for (int i = 0; i < 5; ++i) {
		std::cout << "Client " << i << " added!" << std::endl;
		COUNT.fetch_add(1, std::memory_order_relaxed);
		//COUNT.fetch_add(1, std::memory_order_seq_cst);
		std::this_thread::sleep_for(std::chrono::milliseconds(1000));
		std::cout << "COUNT = " << COUNT << std::endl;
	}
	ALL_CLIENTS_ADDED.store(true);
}

void window() {

	std::this_thread::sleep_for(std::chrono::milliseconds(1000));
	while (!ALL_CLIENTS_ADDED.load() || COUNT.load() > 0) {
		int expected = COUNT.load();

		while (expected > 0) {
			if (COUNT.compare_exchange_weak(expected, expected - 1)) {
				std::cout << "Window working ..." << std::endl;
				std::this_thread::sleep_for(std::chrono::milliseconds(2000));
				std::cout << "COUNT = " << COUNT << std::endl;
				break;
			}
		}
		
		if (COUNT.load() == 0 && !ALL_CLIENTS_ADDED.load()) {
			std::this_thread::sleep_for(std::chrono::milliseconds(500));
		}
	}
	std::cout << "Window finished!" << std::endl;
}



int main() {

	std::cout << "COUNT.is_lock_free() - " << COUNT.is_lock_free() << std::endl;
	std::cout << "COUNT.load() - " << COUNT.load() << std::endl;

	std::thread t1(client);
	std::thread t2(window);

	if (t1.joinable()) {
		t1.join();
	}

	if (t2.joinable()) {
		t2.join();
	}


	std::cout << "COUNT.load() - " << COUNT.load() << std::endl;

	return 0;
}