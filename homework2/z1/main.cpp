#include <iostream>
#include <stdexcept>

class Smart_array {
private:
    int* array;
    int size;
    int logical_size = 0;
public:
    Smart_array(int arr_size) : size(arr_size) {
        array = new int[size];
        for (int i = 0; i < size; ++i) {
            array[i] = 0;
        }
    }

    int get_index(int index) {
        if(index >= size) {
            throw std::runtime_error("Wrong index");
        }
        return array[index];
    }

    void append(int new_val) {
        if(logical_size < size) {
            array[logical_size++] = new_val;
            return;
        }
        if(logical_size == size) {
            int new_size = (size == 0) ? 1 : size *= 2;
            int* new_array = new int[(new_size)];
            for(int i = 0; i < logical_size; ++i) {
                new_array[i] = array[i];
            }
            delete[] array;
            array = new_array;
            new_array = nullptr;
            array[logical_size++] = new_val;
            return;
        }
    }

    ~Smart_array() {
        delete[] array;
    }
};


int main() {
    try {
	Smart_array arr(5);
	arr.append(1);
	arr.append(4);
	arr.append(155);
	arr.append(14);
	arr.append(15);
	std::cout << arr.get_index(1) << std::endl;
}
catch (const std::exception& ex) {
	std::cout << ex.what() << std::endl;
}


    return 0;
}