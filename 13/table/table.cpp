#include <iostream>
#include <unordered_map>
#include <string>
#include <cassert>

template<typename T, T default_value>
class Matrix {
private:
    struct Hash {
        std::size_t operator()(const std::pair<int, int>& k) const {
            return std::hash<int>()(k.first) ^ std::hash<int>()(k.second);
        }
    };

    T def_val;
    std::unordered_map<std::pair<int, int>, T, Hash> table;
public:
    Matrix() : def_val{ default_value } {};
    ~Matrix() {};

    T operator()(int row, int col) {
        T value;
        try {
            value = table.at(std::make_pair(row, col));
        }
        catch (const std::out_of_range&) {
            return def_val;
        }
        return value;
    }

    void set(int row, int col, T val) {
        table[std::make_pair(row, col)] = val;
    }

    int get_size() {
        return table.size();
    }

    T get_default_value() {
        return def_val;
    }
};

int main() {
    Matrix<int, -1> matrix; // бесконечная матрица int заполнена значениями -1
    assert(matrix.get_size() == 0); // все ячейки свободны
    auto a = matrix(0, 0);
    assert(a == -1);
    assert(matrix.get_size() == 0);
    matrix.set(100, 100, 314);
    assert(matrix(100, 100) == 314);
    assert(matrix.get_size() == 1);
    matrix.set(100, 100, 312);
    a = matrix(0, 0);
    assert(a == -1);

    //-------------------------------------------------------------------------//

    Matrix<int, 0> mat;
    for (int i = 0; i < 10; ++i) {
        mat.set(i, i, i);
        mat.set(10 - i - 1, i, i);
    }

    for (int i = 0; i < 10; ++i) {
        int j = 0;
        if (i % 2 == 0)
            j = 1;
        for(; j < 10; j+=2) {
            mat.set(i, j, 5);
        }
        if (i == 0 || i > 8)
            continue;
        for (int q = 1; q < 9; ++q) {
            std::cout << mat(i, q) << ' ';
        }
        std::cout << '\n';
    }

    //-------------------------------------------------------------------------//

    std::cout << '\n';
    Matrix<char, 'a'> mat2;
    for (int i = 0; i < 10; ++i) {
        mat2.set(i, i, 'Z' - i);
        mat2.set(10 - i - 1, i, 'Z' - i);
    }

    for (int i = 0; i < 10; ++i) {
        int j = 0;
        if (i % 2 == 0)
            j = 1;
        for (; j < 10; j += 2) {
            mat2.set(i, j, '~');
        }
        for (int q = 0; q < 10; ++q) {
            std::cout << mat2(i, q) << ' ';
        }
        std::cout << '\n';
    }
}