#include <utility>
#include <algorithm>
#include <stdexcept>
#include <initializer_list>

template <typename T>
class Vector {
private:
    T* data;
    std::size_t sz;
    std::size_t cap;
    static constexpr short min_cap {10};
    static constexpr short capacity_factor {2};
public:
    class Iterator; // Forward declaration

    // Constructors - Destructor
    Vector() : sz{}, cap{min_cap}
    {
        data = new T[cap];
    }

    explicit Vector(std::size_t size)
        : sz{size}, cap{ sz < min_cap ? min_cap : sz }
    {
        data = new T[cap];
    }

    Vector(std::size_t size, const T& value)
        : sz{size}, cap{ sz < min_cap ? min_cap : sz }
    {
        data = new T[cap];
        std::fill(data, data + sz, value);
    }

    Vector(const std::initializer_list<T>& values)
        : sz{values.size()}, cap{ sz < min_cap ? min_cap : sz }
    {
        data = new T[cap];
        std::size_t i {};
        for (auto v : values)
            data[i++] = v;
    }

    Vector(const Vector<T>& vec) 
        : sz{vec.sz}, cap{vec.cap}
    {
        data = new T[cap];
        std::copy(vec.data, vec.data + sz, data);
    }

    Vector(Vector<T>&& vec)
        : data{}, sz{}, cap{min_cap}
    {
        vec.swap(*this);
    }

    ~Vector() { delete[] data; }

    // MethodsW
    void push_back(const T& value);
    void push_back(T&& value);
    void pop_back();

    T& front();
    T& back();
    T& at(std::size_t index);

    std::size_t size()      const  { return sz;      }
    std::size_t capacity()  const  { return cap;     }

    bool empty() const  { return sz == 0; }
    void clear();

    void swap(Vector<T>& vec);

    // Overloads
    T&          operator[] (std::size_t index);
    const T&    operator[] (std::size_t index) const;
    Vector<T>&  operator=  (const Vector<T>& rhs);
    Vector<T>&  operator=  (Vector<T>&& rhs) noexcept;

    // Iterator methods
    Iterator begin() { return Iterator{data};      }
    Iterator end()   { return Iterator{data + sz}; }
    Iterator find(const T& key);

};

template <typename T>
void Vector<T>::push_back(const T& value) {
    if (sz == cap) {
        cap *= capacity_factor;

        T* old = data;
        data = new T[cap];
        std::copy(old, old + sz, data);

        delete[] old;
    }        
    data[sz++] = value;
}

template <typename T>
void Vector<T>::push_back(T&& value) {
    if (sz == cap) {
        cap *= capacity_factor;

        T* old = data;
        data = new T[cap];
        std::copy(old, old + sz, data);

        delete[] old;
    }        
    data[sz++] = value;
}

template <typename T>
void Vector<T>::pop_back() {
    if (sz > 0)
        --sz;
}

template <typename T>
T& Vector<T>::front() {
    if (!sz)
        throw std::logic_error("vector is empty");
    return data[0];
}

template <typename T>
T& Vector<T>::back() {
    if (!sz)
        throw std::logic_error("vector is empty");
    return data[sz - 1];
}

template <typename T>
T& Vector<T>::at(std::size_t index) {
    if (!sz)
        throw std::logic_error("vector is empty");
    else if (index >= sz)
        throw std::invalid_argument("invalid index");
    return data[index];
}

// operator[] overloads do not check that index is not >= sz.
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
Vector<T>& Vector<T>::operator=(const Vector<T>& rhs) {
    Vector<T> temp{rhs};    // Exceptions may occur at this state so we create a temp vector and then swap it with *this
    temp.swap(*this);    
    return *this;
}

template <typename T>
Vector<T>& Vector<T>::operator=(Vector<T>&& rhs) noexcept {
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
    delete[] data;
    sz = 0;
    cap = min_cap;
    data = new T[cap];
}

template <typename T>
void Vector<T>::swap(Vector<T>& vec) {
    std::swap(sz,   vec.sz);
    std::swap(cap,  vec.cap);
    std::swap(data, vec.data);
}

template <typename T>
void swap(Vector<T>& lhs, Vector<T>& rhs) {
    lhs.swap(rhs);
}

template <typename T>
typename Vector<T>::Iterator Vector<T>::find(const T& key) {
    for (auto iter = begin(); iter != end(); ++iter)
        if (*iter == key)
            return iter;
    return end();
}

template <typename T>
class Vector<T>::Iterator {
private:
    T* data_ptr;
    friend class Vector;
public:
    explicit Iterator() : data_ptr{} {}
    explicit Iterator(T* in_ptr) : data_ptr{in_ptr} {}

    Iterator& operator++() {
        ++data_ptr;
        return *this;
    }

    Iterator operator++(int) {
        Iterator temp{*this};
        ++data_ptr;
        return temp;
    }

    Iterator& operator--() {
        --data_ptr;
        return *this;
    }

    Iterator operator--(int) {
        Iterator temp{*this};
        --data_ptr;
        return temp;
    }

    bool operator==(const Iterator& rhs) const { return data_ptr == rhs.data_ptr; }
    bool operator!=(const Iterator& rhs) const { return data_ptr != rhs.data_ptr; }

    T& operator*() const { return *data_ptr; }
};

// TODO - add insert_at and remove