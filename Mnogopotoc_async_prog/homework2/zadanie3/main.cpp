#include<iostream>
#include<thread>
#include<mutex>
#include<string>
#include <utility>


class Data {
	int value;
	std::mutex mtx;
public:
	Data(int val = 0) : value(val) {}

	Data(const Data&) = delete;
	Data& operator=(const Data&) = delete;

	void set_value(int val) {
		std::lock_guard<std::mutex> grd(mtx);
		value = val;
	}

	int get_value() {
		std::lock_guard<std::mutex> grd(mtx);
		return value;
	}

	std::mutex& get_mutex() { return mtx; }

	void print(std::string name) {
		std::lock_guard<std::mutex> grd(mtx);
		std::cout << name << " = " << value << std::endl;
	}

	int& direct_value() { return value; }
};

// lock
void swap_lock(Data& d1, Data& d2) {
	std::lock(d1.get_mutex(), d2.get_mutex());

	std::lock_guard<std::mutex> lock_d1(d1.get_mutex(), std::adopt_lock);
	std::lock_guard<std::mutex> lock_d2(d2.get_mutex(), std::adopt_lock);

	std::swap(d1.direct_value(), d2.direct_value());
}

//scoped_lock

// unique
void swap_unique_lock(Data& d1, Data& d2) {
	// Создаем unique_lock без немедленного захвата мьютексов
	std::unique_lock<std::mutex> lock_d1(d1.get_mutex(), std::defer_lock);
	std::unique_lock<std::mutex> lock_d2(d2.get_mutex(), std::defer_lock);

	std::lock(lock_d1, lock_d2);

	std::swap(d1.direct_value(), d2.direct_value());
}

void test_swap() {
	Data data1(100);
	Data data2(200);

	data1.print("data1");
	data2.print("data2");

	swap_unique_lock(data1, data2);

	std::cout << "After swap: " << std::endl;

	data1.print("data1");
	data2.print("data2");


}

void test_concurrent_swap() {
	Data dataA(1000);
	Data dataB(2000);

	std::cout << "\nTesting concurrent swaps:" << std::endl;
	std::cout << "Initial values:" << std::endl;
	dataA.print("dataA");
	dataB.print("dataB");

	std::thread t1([&dataA, &dataB]() {
		for (int i = 0; i < 4; ++i) {
			swap_lock(dataA, dataB);
			std::cout << "\nafter " << i << " iteration: " << std::endl;
			dataA.print("dataA");
			dataB.print("dataB");
			std::this_thread::sleep_for(std::chrono::milliseconds(1000));
		}
	});
	std::cout << std::endl;
	std::cout << std::endl;
	std::cout << std::endl;

	std::thread t2([&dataA, &dataB]() {
		for (int i = 0; i < 3; ++i) {
			swap_unique_lock(dataA, dataB);
			std::cout << "\nTHREAD t2 after " << i << " iteration: " << std::endl;
			dataA.print("dataA");
			dataB.print("dataB");
			std::this_thread::sleep_for(std::chrono::milliseconds(3000));
		}
	});

	t1.join();
	t2.join();

	std::cout << "After concurrent swaps:" << std::endl;
	dataA.print("dataA");
	dataB.print("dataB");
}

int main() {

	test_concurrent_swap();

	return 0;
}