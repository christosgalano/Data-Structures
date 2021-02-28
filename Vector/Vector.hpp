#include <cstddef>
#include <algorithm>
#include <stdexcept>
#include <iostream>

template <typename T>
class Vector {
private:
    T* data;

    size_t sz;
    size_t cap;

    static constexpr int min_cap {10};
    static constexpr int capacity_factor {2};

public:
    class Iterator;

    Vector() : data{}, sz{}, cap{min_cap}
    {
        data = new T[cap];
    }

    Vector(size_t size)
        : sz{size}, cap{ sz < min_cap ? min_cap : sz }
    {
        data = new T[cap];
    }

    Vector(size_t size, T value)
        : sz{size}, cap{ sz < min_cap ? min_cap : sz }
    {
        data = new int[cap];
        std::fill(data, data + sz, value);
    }

    ~Vector() { delete[] data; }

    void push_back(T value) {
        if (sz < cap)
            data[sz] = value;
        else {
            cap *= capacity_factor;

            T* temp = new T[cap];
            for (size_t i = 0; i < sz; ++i)
                temp[i] = data[i];
            
            temp[sz] = value;
            delete[] data;
            data = temp;
        }        
        ++sz;
    }

    void pop_back() {
        if (sz > 0)
            --sz;
    }

    size_t size()      const  { return sz;      }
    size_t capacity()  const  { return cap;     }
    bool   empty()     const  { return sz == 0; }

    Iterator begin() { return Iterator(data);      }
    Iterator end()   { return Iterator(data + sz); }

    T& operator[] (size_t index) {
        if (index >= sz)
            throw std::invalid_argument("invalid index");
        
        return data[index];
    }

    void clear() {
        delete[] data;
        sz = 0;
        cap = min_cap;
        data = new T[cap];
    }

    void print() const {
        for (size_t i = 0; i < sz; ++i)
            std::cout << data[i] << ' ';
    }
};

template <typename T>
class Vector<T>::Iterator {
private:
    T* data_ptr;
    friend class Vector;

public:
    Iterator() : data_ptr{} {}
    Iterator(T* in_ptr) : data_ptr{in_ptr} {}

    Iterator& operator++() {
        ++data_ptr;
        return *this;
    }

    Iterator operator++(int) {
        Iterator temp{*this};
        ++data_ptr;
        return temp;
    }

    bool operator==(const Iterator& rhs) const { return data_ptr == rhs.data_ptr; }
    bool operator!=(const Iterator& rhs) const { return data_ptr != rhs.data_ptr; }

    T& operator*() const { return *data_ptr; }
};