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

    Smart_array& operator=(const Smart_array& any_arr) {
        if(this != &any_arr) {
            if(size != any_arr.size) {
                delete[] array;
                size = any_arr.size;
                logical_size = any_arr.logical_size;
                array = new int[size];
            }
            for(int i = 0; i < logical_size; ++i) {
                array[i] = any_arr.array[i];
            }
        }
        return *this;
    }

    void print_array() {
        for(int i = 0; i < size; ++i) {
            std::cout << array[i] << "\t";
        }
        std::cout << std::endl;
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

    Smart_array new_array(2);
    new_array.append(44);
    new_array.append(34);

    arr.print_array();

    arr = new_array;

    arr.print_array();

}
catch (const std::exception& ex) {
	std::cout << ex.what() << std::endl;
}


    return 0;
}