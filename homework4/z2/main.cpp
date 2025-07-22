#include <iostream>
 
template <typename T> 
class Table {
  int rows;
  int columns;
  T **array;
 
public:
  Table(int r = 0, int c = 0) : rows(r), columns(c) {
    array = new T *[rows];
    for (int i = 0; i < rows; ++i) {
      array[i] = new T[columns];
    }
    for (int i = 0; i < rows; ++i) {
      for (int j = 0; j < columns; ++j) {
        array[i][j] = 0;
      }
    }
  }
  class Proxy {
    T *arRows;
 
  public:
    Proxy(T *ar) : arRows(ar) {}
    T &operator[](int c) { return arRows[c]; }
  };
  Proxy operator[](int r) { return Proxy(array[r]); }
  ~Table() {
    for (int i = 0; i < rows; ++i) {
      delete[] array[i];
    }
    delete[] array;
  }
 
  Table &operator=(const Table &other) {
    if (this == &other)
      return *this;
    for (int i = 0; i < rows; ++i) {
      delete[] array[i];
    }
    delete[] array;
    rows = other.rows;
    columns = other.columns;
    array = new T *[rows];
    for (int i = 0; i < rows; ++i) {
      array[i] = new T[columns];
    }
    for (int i = 0; i < rows; ++i) {
      for (int j = 0; j < columns; ++j) {
        array[i][j] = other.array[i][j];
      }
    }
    return *this;
  }
};
 
int main() {
  Table<int> t(2, 3);
 
  t[0][0] = 1;
 
  std::cout << t[0][0] << std::endl;
}