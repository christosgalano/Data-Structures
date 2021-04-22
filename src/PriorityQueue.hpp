#include "Vector.hpp"
#include <functional>


namespace cds {

template <typename T, typename C = std::less<T>>  // Default version is a MaxPriorityQueue
class PriorityQueue {
private:
    Vector<T> data;
    C comparator;
    std::size_t sz;

    void heapify_up(std::size_t index);
    void heapify_down(std::size_t index);
    void efficient_heapify();

public:
    PriorityQueue();
    explicit PriorityQueue(C in_comparator);
    explicit PriorityQueue(Vector<T>& vec);
    PriorityQueue(Vector<T>& vec, C in_comparator);
    PriorityQueue(Vector<T>&& vec);
    PriorityQueue(Vector<T>&& vec, C in_comparator);
    explicit PriorityQueue(const std::initializer_list<T>& values);
    PriorityQueue(const std::initializer_list<T>& values, C in_comparator);
    PriorityQueue(const PriorityQueue& pq);
    PriorityQueue(PriorityQueue&& pq) noexcept;

    void insert(const T& value);
    void insert(T&& value);
    void pop();

    T top();

    void clear();
    void swap(PriorityQueue& pq) noexcept;

    PriorityQueue&  operator=(const PriorityQueue& rhs);
    PriorityQueue&  operator=(PriorityQueue&& rhs) noexcept;

    template <typename K, typename V>
    friend bool operator==(const PriorityQueue<K,V>& lhs, const PriorityQueue<K,V>& rhs);

    std::size_t size() const { return sz;      }
    bool empty()       const { return sz == 0; }
};


// heapify_up - heapify_down restore the heap property after an insertion - removal
template <typename T, typename C>
void PriorityQueue<T,C>::heapify_up(std::size_t index) {
    if (index == 1)     // We reached the root
        return;
    int father = index / 2;
    if (comparator(data[father], data[index])) {
        std::swap(data[index], data[father]);
        heapify_up(father);
    }
}

template <typename T, typename C>
void PriorityQueue<T,C>::heapify_down(std::size_t index) {
    int left_child = 2 * index;
    int right_child = left_child + 1;
    if (left_child > sz)  // left_child is a leaf
        return;
    int max = left_child;
    if (right_child <= sz && comparator(data[max], data[right_child]))
        max = right_child;
    if (comparator(data[index], data[max])) {
        std::swap(data[max], data[index]);
        heapify_down(max);
    }
}


// Restores the heap property in O(n)
template <typename T, typename C>
void PriorityQueue<T,C>::efficient_heapify() {
    data.insert(data.begin(), T{});
    int node = sz / 2;
    while (node) {
        heapify_down(node);
        --node;
    }
}

// We always use a dummy element at position 0 of the vector so the indexes arithmetic is simpler.
// The dummy element does not affect the size of the pq.
template <typename T, typename C>
PriorityQueue<T,C>::PriorityQueue()
    : data(1, T{}), comparator{C()}, sz{} {}


template <typename T, typename C>
PriorityQueue<T,C>::PriorityQueue(C in_comparator)
    : data(1, T{}), comparator{in_comparator}, sz{} {}    


template <typename T, typename C>
PriorityQueue<T,C>::PriorityQueue(Vector<T>& vec)
    : data{vec}, comparator{C()}, sz{vec.size()}
{   
    efficient_heapify();
}


template <typename T, typename C>
PriorityQueue<T,C>::PriorityQueue(Vector<T>& vec, C in_comparator)
    : data{vec}, comparator{in_comparator}, sz{data.size()}
{
    efficient_heapify();
}


template <typename T, typename C>
PriorityQueue<T,C>::PriorityQueue(Vector<T>&& vec)
    : data{std::forward<Vector<T>>(vec)}, comparator{C()}, sz{data.size()}
{   
    efficient_heapify();
}


template <typename T, typename C>
PriorityQueue<T,C>::PriorityQueue(Vector<T>&& vec, C in_comparator)
    : data{std::forward<Vector<T>>(vec)}, comparator{in_comparator}, sz{data.size()}
{   
    efficient_heapify();
}


template <typename T, typename C>
PriorityQueue<T,C>::PriorityQueue(const std::initializer_list<T>& values)
    : data{values}, comparator{C()}, sz{data.size()}
{
    efficient_heapify();
}


template <typename T, typename C>
PriorityQueue<T,C>::PriorityQueue(const std::initializer_list<T>& values, C in_comparator)
    : data{values}, comparator{in_comparator}, sz{data.size()}
{
    efficient_heapify();
}


template <typename T, typename C>
PriorityQueue<T,C>::PriorityQueue(const PriorityQueue<T,C>& pq) 
    : data{pq.data}, comparator{pq.comparator}, sz{pq.sz} {}


template <typename T, typename C>
PriorityQueue<T,C>::PriorityQueue(PriorityQueue<T,C>&& pq) noexcept 
    : data(1, T{}), comparator{C()}, sz{}
{
    pq.swap(*this);
}


template <typename T, typename C>
void PriorityQueue<T,C>::insert(const T& value) {
    data.push_back(value);
    heapify_up(++sz);
}


template <typename T, typename C>
void PriorityQueue<T,C>::insert(T&& value) {
    data.push_back(std::forward<T>(value));
    heapify_up(++sz);
}


template <typename T, typename C>
T PriorityQueue<T,C>::top() {
    if (!sz)
        throw std::runtime_error("pq is empty");
    return data[1];
}    


template <typename T, typename C>
void PriorityQueue<T,C>::pop() {
    if (!sz)
        throw std::runtime_error("pq is empty");
    std::swap(data[1], data[sz--]);
    data.pop_back();
    heapify_down(1);
}


template <typename T, typename C>
void PriorityQueue<T,C>::clear() {
    data.clear();
    sz = 0;
}


template <typename T, typename C>
void PriorityQueue<T,C>::swap(PriorityQueue<T,C>& pq) noexcept {
    data.swap(pq.data);
    std::swap(comparator, pq.comparator);
    std::swap(sz, pq.sz);
}


template <typename T, typename C>
void swap(PriorityQueue<T,C>& lhs, PriorityQueue<T,C>& rhs) {
    lhs.swap(rhs);
}


// Because self assignment happens so rarely we don't check that this != &rhs
template <typename T, typename C>
PriorityQueue<T,C>& PriorityQueue<T,C>::operator=(const PriorityQueue<T,C>& rhs)  {
    PriorityQueue<T,C> temp{rhs};    // Exceptions may occur at this state so we create a temp pq and then swap it with *this
    temp.swap(*this);    
    return *this;    
}


template <typename T, typename C>
PriorityQueue<T,C>& PriorityQueue<T,C>::operator=(PriorityQueue<T,C>&& rhs) noexcept {
    rhs.swap(*this);
    return *this;    
}


template <typename T, typename C>
bool operator==(const PriorityQueue<T,C>& lhs, const PriorityQueue<T,C>& rhs) {
    if (lhs.sz != rhs.sz)
        return false;

    for (std::size_t i = 0; i < lhs.sz; ++i)
        if (lhs.data[i] != rhs.data[i])
            return false;
        
    return true;   
}


template <typename T, typename C>
bool operator!=(const PriorityQueue<T,C>& lhs, const PriorityQueue<T,C>& rhs) {
    return !(lhs == rhs);
}

}