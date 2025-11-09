#include <iostream>
#include <chrono>
#include <thread>

int COUNT = 0;

void client() {
	for (int i = 0; i < 100; ++i) {
		std::cout << "Client " << i << ":" << std::endl;
		COUNT++;
		std::this_thread::sleep_for(std::chrono::milliseconds(10));
		std::cout << "COUNT = " << COUNT << std::endl;
	}
}

void window() {
	while (COUNT == 0) {
        std::this_thread::sleep_for(std::chrono::milliseconds(10));
    }
    
    while (COUNT > 0) {
        --COUNT;
        std::cout << "Window processed. COUNT = " << COUNT << std::endl;
        std::this_thread::sleep_for(std::chrono::milliseconds(20));
    }
}



int main() {

	std::cout << COUNT << std::endl;

	std::thread t1(client);
	std::thread t2(window);

	if (t1.joinable()) {
		t1.join();
	}

	if (t2.joinable()) {
		t2.join();
	}




	return 0;
}