#pragma once

#include <algorithm>
#include <cstddef>
#include <initializer_list>
#include <iterator>
#include <new>
#include <stdexcept>
#include <utility>

namespace data_structures {

template <typename T> class Vector_Iterator;
template <typename T> class Const_Vector_Iterator;

template <typename T>
class Vector {
public:
    using iterator = Vector_Iterator<T>;
    using const_iterator = Const_Vector_Iterator<T>;

    Vector();
    explicit Vector(std::size_t size);
    Vector(std::size_t size, const T& value);
    explicit Vector(const std::initializer_list<T>& values);
    Vector(const Vector& vec);
    Vector(Vector&& vec) noexcept;
    ~Vector();

    void push_back(const T& value);
    void push_back(T&& value);
    void pop_back();

    template <typename ... Args>
    T& emplace_back(Args&& ... args);

    iterator erase(iterator pos);
    iterator insert(iterator pos, const T& value);
    iterator find(const T& key);

    T& front();
    T& back();
    T& at(std::size_t index);

    std::size_t size()      const  { return sz;      }
    std::size_t capacity()  const  { return cap;     }
    bool empty()            const  { return sz == 0; }

    void resize(std::size_t n);
    void resize(std::size_t n, const T& val);
    void reserve(std::size_t n);

    void clear();
    void swap(Vector& vec) noexcept;

    T&       operator[] (std::size_t index);
    const T& operator[] (std::size_t index) const;
    Vector&  operator=  (const Vector& rhs);
    Vector&  operator=  (Vector&& rhs) noexcept;

    iterator begin() { return iterator{data};      }
    iterator end()   { return iterator{data + sz}; }

    const_iterator cbegin() const { return const_iterator{data};      }
    const_iterator cend()   const { return const_iterator{data + sz}; }

private:
    T* data;
    std::size_t sz;
    std::size_t cap;
    static constexpr short min_cap {10};
    static constexpr short capacity_factor {2};
    void p_realloc(std::size_t n);
};


template <typename T>
Vector<T>::Vector() 
    : sz{}, cap{min_cap}
{
    data = (T*)::operator new(cap * sizeof(T));
}


template <typename T>
Vector<T>::Vector(std::size_t size)
    : sz{size}, cap{ size < min_cap ? min_cap : size }
{
    data = (T*)::operator new(cap * sizeof(T));
}


template <typename T>
Vector<T>::Vector(std::size_t size, const T& value)
    : sz{size}, cap{ sz < min_cap ? min_cap : sz }
{
    data = (T*)::operator new(cap * sizeof(T));
    for (std::size_t i = 0; i < sz; ++i)
        new (&data[i]) T(value);
}


template <typename T>
Vector<T>::Vector(const std::initializer_list<T>& values)
    : sz{values.size()}, cap{ sz < min_cap ? min_cap : sz }
{
    data = (T*)::operator new(cap * sizeof(T));
    std::size_t i {};
    for (auto v : values)
        new (&data[i++]) T(v);
}


template <typename T>
Vector<T>::Vector(const Vector& vec) 
    : sz{vec.sz}, cap{vec.cap}
{
    data = (T*)::operator new(cap * sizeof(T));
    for (std::size_t i = 0; i < sz; ++i)
        new (&data[i]) T(vec[i]);
}


template <typename T>
Vector<T>::Vector(Vector&& vec) noexcept
    : data{}, sz{}, cap{min_cap}
{
    vec.swap(*this);
}


template <typename T>
Vector<T>::~Vector() {
    clear();
    ::operator delete(data, cap * sizeof(T));
}


template <typename T>
void Vector<T>::p_realloc(std::size_t n) {
    T* new_data = (T*)::operator new(n * sizeof(T));

    std::size_t old_sz = sz;
    if (n < sz)
        sz = n;

    for (std::size_t i = 0; i < sz; ++i) 
        new (&new_data[i]) T(std::move(data[i]));

    for (std::size_t i = 0; i < old_sz; ++i)
        data[i].~T();

    ::operator delete(data, cap * sizeof(T));
    data = new_data;
    cap = n;
}


template <typename T>
void Vector<T>::push_back(const T& value) {
    if (sz >= cap)
        p_realloc(cap * capacity_factor);
    new (&data[sz]) T(value);
    ++sz;
}


template <typename T>
void Vector<T>::push_back(T&& value) {
    if (sz >= cap)
        p_realloc(cap * capacity_factor);
    new (&data[sz]) T(std::move(value));
    ++sz;
}


template <typename T>
void Vector<T>::pop_back() {
    if (sz > 0) {
        --sz;
        data[sz].~T();
    }
}


template<typename T>
template<typename ... Args>
T& Vector<T>::emplace_back(Args&& ... args) {
    if (sz >= cap)
        p_realloc(cap * capacity_factor);
    new (&data[sz]) T(std::forward<Args>(args)...);
    return data[sz++];
}


template<typename T>
typename Vector<T>::iterator Vector<T>::erase(iterator pos) {
    if (pos == end())
        throw std::invalid_argument("invalid position");

    std::size_t i {};

    for (auto iter = begin(); iter != pos; ++iter, ++i);
    
    std::size_t index {i};

    for (auto iter = pos + 1; iter != end(); ++iter, ++i)
        data[i] = data[i + 1];

    --sz;
    data[sz].~T();

    if (index >= sz)
        index = 0;

    return iterator{data + index};
}


template<typename T>
typename Vector<T>::iterator Vector<T>::insert(iterator pos, const T& value){
    if (pos == end())
        throw std::invalid_argument("invalid position");

    if (sz >= cap) {
        std::size_t i {};
        cap *= capacity_factor;
        
        T* temp = (T*)::operator new(cap * sizeof(T));

        for (auto iter = begin(); iter != pos; ++iter, ++i)
            new (&temp[i])  T(std::move(data[i]));
        new (&temp[i]) T(value);

        std::size_t index {i++};
        
        for (std::size_t j = 0; j < sz - index; ++j, ++i)
            new (&temp[i]) T(std::move(data[i - 1]));


        for (std::size_t i = 0; i < sz; ++i)
            data[i].~T();

        ::operator delete(data, cap * sizeof(T));
        data = temp;
        ++sz;

        return iterator{data + index};
    }

    new (&data[sz]) T(std::move(data[sz - 1]));

    std::size_t i {1};
    for(auto iter = end() - 1; iter != pos; --iter, ++i)
        data[sz - i] = std::move(data[sz - i - 1]);

    *pos = value;
    ++sz;

    return pos;
}


template <typename T>
T& Vector<T>::front() {
    if (!sz)
        throw std::runtime_error("vector is empty");
    return data[0];
}


template <typename T>
T& Vector<T>::back() {
    if (!sz)
        throw std::runtime_error("vector is empty");
    return data[sz - 1];
}


template <typename T>
T& Vector<T>::at(std::size_t index) {
    if (!sz)
        throw std::runtime_error("vector is empty");
    else if (index >= sz)
        throw std::invalid_argument("invalid index");
    return data[index];
}


// operator[] overloadata_structures do not check that index is not >= sz.
// If the user wants an exception to be thrown if the index is invalid he should use at(std::size_t).
template <typename T>
T& Vector<T>::operator[] (std::size_t index) {
    return data[index];    
}


template <typename T>
const T& Vector<T>::operator[] (std::size_t index) const {
    return data[index];    
}


// Because self assignment happens so rarely we don't check that this != &rhs
template <typename T>
Vector<T>& Vector<T>::operator=(const Vector& rhs) {
    Vector temp{rhs};    // Exceptions may occur at this state so we create a temp vector and then swap it with *this
    temp.swap(*this);    
    return *this;
}


template <typename T>
Vector<T>& Vector<T>::operator=(Vector&& rhs) noexcept {
    rhs.swap(*this);
    return *this;
}


template <typename T>
bool operator==(const Vector<T>& lhs, const Vector<T>& rhs) {
    if (lhs.size() != rhs.size())
        return false;

    for (std::size_t i = 0; i < lhs.size(); ++i)
        if (lhs[i] != rhs[i])
            return false;
        
    return true;   
}


template <typename T>
bool operator!=(const Vector<T>& lhs, const Vector<T>& rhs) {
    return !(lhs == rhs);
}


template <typename T>
void Vector<T>::clear() {
    for (std::size_t i = 0; i < sz; ++i)
        data[i].~T();
    sz = 0;
}


// Resizes the container so that it contains n elements
template<typename T>
void Vector<T>::resize(std::size_t n) {
    if (n < sz) {
        for (std::size_t i = n; i < sz; ++i)
            data[i].~T();
        sz = n;
    }
    else if (n > cap)
        p_realloc(n);
    else {
        std::size_t diff {n - sz};
        for (std::size_t i = 0; i < diff; ++i)
            push_back(T{});
    }
}


template<typename T>
void Vector<T>::resize(std::size_t n, const T& val) {
    if (n < sz) {
        for (std::size_t i = n; i < sz; ++i)
            data[i].~T();
        sz = n;
    }
    else if (n > cap)
        p_realloc(n);
    else {
        std::size_t diff {n - sz};
        for (std::size_t i = 0; i < diff; ++i)
            push_back(val);
    }
}


// Requests that the vector capacity be at least enough to contain n elements
template<typename T>
void Vector<T>::reserve(std::size_t n) {
    if (n > cap)
        p_realloc(n);
}


template <typename T>
void Vector<T>::swap(Vector<T>& vec) noexcept {
    std::swap(sz,   vec.sz);
    std::swap(cap,  vec.cap);
    std::swap(data, vec.data);
}


template <typename T>
void swap(Vector<T>& lhs, Vector<T>& rhs) {
    lhs.swap(rhs);
}


template <typename T>
typename Vector<T>::iterator Vector<T>::find(const T& key) {
    for (auto iter = begin(); iter != end(); ++iter)
        if (*iter == key)
            return iter;
    return end();
}


template <typename T>
class Vector_Iterator {
private:
    T* data_ptr;
    friend class Vector<T>;
public:
    using iterator_category = std::random_access_iterator_tag;
    using difference_type = std::ptrdiff_t;
    using value_type = T;
    using pointer = value_type*;
    using reference = value_type&;

    Vector_Iterator() : data_ptr{} {}
    explicit Vector_Iterator(T* in_ptr) : data_ptr{in_ptr} {}

    Vector_Iterator(const Vector_Iterator&) = default;
    ~Vector_Iterator() = default;

    Vector_Iterator& operator++() {
        ++data_ptr;
        return *this;
    }

    Vector_Iterator operator++(int) {
        Vector_Iterator temp{*this};
        ++data_ptr;
        return temp;
    }

    Vector_Iterator& operator+=(const difference_type& d) {
        data_ptr += d;
        return *this;
    }

    Vector_Iterator operator+(const difference_type& d) {
        data_ptr += d;
        return *this;
    }

    Vector_Iterator& operator--() {
        --data_ptr;
        return *this;
    }

    Vector_Iterator operator--(int) {
        Vector_Iterator temp{*this};
        --data_ptr;
        return temp;
    }

    Vector_Iterator& operator-=(const difference_type& d) {
        data_ptr -= d;
        return *this;
    }

    Vector_Iterator operator-(const difference_type& d) {
        data_ptr -= d;
        return *this;
    }

    bool operator==(const Vector_Iterator& rhs) const { return data_ptr == rhs.data_ptr; }
    bool operator!=(const Vector_Iterator& rhs) const { return data_ptr != rhs.data_ptr; }

    reference operator*() const { return *data_ptr; }

    pointer operator->()  const { return data_ptr;  }

    void swap(Vector_Iterator& other) { std::swap(data_ptr, other.data_ptr); }
};


template <typename T>
class Const_Vector_Iterator {
private:
    T* data_ptr;
    friend class Vector<T>;
public:
    using iterator_category = std::random_access_iterator_tag;
    using difference_type = std::ptrdiff_t;
    using value_type = const T;
    using pointer = value_type*;
    using reference = value_type&;

    Const_Vector_Iterator() : data_ptr{} {}
    explicit Const_Vector_Iterator(T* in_ptr) : data_ptr{in_ptr} {}

    Const_Vector_Iterator(const Const_Vector_Iterator&) = default;
    ~Const_Vector_Iterator() = default;

    Const_Vector_Iterator& operator++() {
        ++data_ptr;
        return *this;
    }

    Const_Vector_Iterator operator++(int) {
        Const_Vector_Iterator temp{*this};
        ++data_ptr;
        return temp;
    }

    Const_Vector_Iterator& operator+=(const difference_type& d) {
        data_ptr += d;
        return *this;
    }

    Const_Vector_Iterator operator+(const difference_type& d) {
        data_ptr += d;
        return *this;
    }

    Const_Vector_Iterator& operator--() {
        --data_ptr;
        return *this;
    }

    Const_Vector_Iterator operator--(int) {
        Const_Vector_Iterator temp{*this};
        --data_ptr;
        return temp;
    }

    Const_Vector_Iterator& operator-=(const difference_type& d) {
        data_ptr -= d;
        return *this;
    }
    
    Const_Vector_Iterator operator-(const difference_type& d) {
        data_ptr -= d;
        return *this;
    }

    bool operator==(const Const_Vector_Iterator& rhs) const { return data_ptr == rhs.data_ptr; }
    bool operator!=(const Const_Vector_Iterator& rhs) const { return data_ptr != rhs.data_ptr; }

    reference operator*() const { return *data_ptr; }

    pointer operator->()  const { return data_ptr;  }
};

}