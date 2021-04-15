#include "Vector.hpp"
#include <functional>


namespace cds {

template <typename T, typename C = std::less<T>>  // C is set to std::less<T> so the default version is a MaxPriorityQueue
class PriorityQueue {
private:
    Vector<T> data;
    C comparator;
    std::size_t sz;     // Current size of the PQ - not taking into consideration the dummy element of the Vector

    // heapify_up - heapify_down restore the heap property after an insertion - removal
    void heapify_up(std::size_t index) {
        if (index == 1)     // We reached the root
            return;

        int father = index / 2;
        if (comparator(data[father], data[index])) {
            std::swap(data[index], data[father]);
            heapify_up(father);
        }
    }

    void heapify_down(std::size_t index) {
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
    void efficient_heapify() {
        data.insert(data.begin(), T{});
        int node = sz / 2;
        while (node) {
            heapify_down(node);
            --node;
        }
    }

public:
    // Constructors - we always use a dummy element at position 0 of the vector so the indexes arithmetic is simpler
    PriorityQueue() 
        : data(1, T{}), comparator{C()}, sz{} {}

    PriorityQueue(C in_comparator)
        : data(1, T{}), comparator{in_comparator}, sz{} {}
    
    PriorityQueue(Vector<T>& vec)
        : data{vec}, comparator{C()}, sz{vec.size()}
    {   
        efficient_heapify();
    }

    PriorityQueue(Vector<T>& vec, C in_comparator)
        : data{vec}, comparator{in_comparator}, sz{data.size()}
    {
        efficient_heapify();
    }

    PriorityQueue(Vector<T>&& vec)
        : data{std::forward<Vector<T>>(vec)}, comparator{C()}, sz{data.size()}
    {   
        efficient_heapify();
    }

    PriorityQueue(Vector<T>&& vec, C in_comparator)
        : data{std::forward<Vector<T>>(vec)}, comparator{in_comparator}, sz{data.size()}
    {   
        efficient_heapify();
    }

    PriorityQueue(const std::initializer_list<T>& values)
        : data{values}, comparator{C()}, sz{data.size()}
    {
        efficient_heapify();
    }

    PriorityQueue(const std::initializer_list<T>& values, C in_comparator)
        : data{values}, comparator{in_comparator}, sz{data.size()}
    {
        efficient_heapify();
    }

    void insert(const T& value) {
        data.push_back(value);
        heapify_up(++sz);
    }

    void insert(T&& value) {
        data.push_back(std::forward<T>(value));
        heapify_up(++sz);
    }

    T top() {
        return data[1];
    }    

    void remove() {
        if (!sz) return;

        std::swap(data[1], data[sz--]);
        data.pop_back();

        heapify_down(1);
    }

    std::size_t size() const { return sz;      }
    bool empty()       const { return sz == 0; }
};

}