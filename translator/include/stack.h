#ifndef MATRIX_LIST_H
#define MATRIX_LIST_H

#include <iostream>
#include <vector>
#include <stdexcept>

template <typename T>
class Stack {
private:
    std::vector<T> data; // Хранилище элементов стека
    int last;            // Индекс последнего элемента

public:
    explicit Stack(int n) {
        if (n <= 0) {
            throw std::out_of_range("Invalid length");
        }
        data = std::vector<T>(n);
        last = -1;
    }


    T pop() {
        if (last < 0) {
            throw std::out_of_range("Stack is empty");
        }
        T topElement = data[last];
        last--;
        return topElement;
    }


    void push(T value) {
        if (last >= static_cast<int>(data.size()) - 1) {
            throw std::out_of_range("Stack overflow");
        }
        data[++last] = value;
    }


    T top() const {
        if (last < 0) {
            throw std::out_of_range("Stack is empty");
        }
        return data[last];
    }

    int size() const {
        return last + 1;
    }

    bool empty() const {
        return last < 0;
    }


    bool operator==(const Stack& other) const {
        if (last != other.last) {
            return false;
        }
        for (int i = 0; i <= last; ++i) {
            if (data[i] != other.data[i]) {
                return false;
            }
        }
        return true;
    }


    bool operator!=(const Stack& other) const {
        return !(*this == other);
    }
};

#endif 