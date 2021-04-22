#include <cassert>
#include <cstddef>
#include <initializer_list>
#include <stdexcept>
#include <utility>


namespace cds {

template <typename T> class List;
template <typename T> class List_Iterator;
template <typename T> class Const_List_Iterator;


template <typename T>
class ListNode {
    private:
        T data;
        ListNode<T>* next;
        friend class List<T>;
        friend class List_Iterator<T>;
        friend class Const_List_Iterator<T>;
    public:
        ListNode() : next{} {}
        ListNode(T in_data, ListNode<T>* in_next = nullptr)
            : data{in_data}, next{in_next} {}
};


template <typename T>
class List {
public:
    using iterator = List_Iterator<T>;
    using const_iterator = Const_List_Iterator<T>;

    List();
    explicit List(const std::initializer_list<T>& values);
    List(const List& list);
    List(List&& list) noexcept;
    ~List();

    // Insert node at the front
    void push_front(const T& data);

    // Insert node at the back
    void push_back(const T& data);

    // Insert an item at a given index. The first argument is the index of the value before which to insert (indexes start at 0)
    void insert(std::size_t index, const T& data);

    // Removes the first value of the list
    void pop_front();

    // Removes the last value of the list
    void pop_back();
    
    // Remove the item at a given index. If the index is greater than the list's size the function throws an exception.
    void remove(std::size_t index);

    // Returns an iterator to the key's first occurrence in the list. If the key is not found the function returns iterator end().
    iterator find(T key);

    T& front();
    T& back();
    T& at(std::size_t index);

    void clear();
    void swap(List<T>& rhs) noexcept;

    List&  operator=(const List& rhs);
    List&  operator=(List&& rhs) noexcept;

    std::size_t size()  const { return sz;      }
    bool        empty() const { return sz == 0; }

    iterator begin() { return iterator{dummy->next}; }
    iterator end()   { return iterator{};            }

    const_iterator cbegin() const { return const_iterator{dummy->next}; }
    const_iterator cend()   const { return const_iterator{};            }

private:
    ListNode<T>* dummy;
    ListNode<T>* last;
    std::size_t sz;
};


// Constructor: create a dummy node so that even an empty list has one node.
// In an empty list last_node == dummy.
template <typename T>
List<T>::List() 
    : dummy{new ListNode<T>}, last{dummy}, sz{} {}


template<typename T>
List<T>::List(const std::initializer_list<T>& values)
    : dummy{new ListNode<T>}, last{dummy}, sz{}
{
    for (auto p : values)
        push_back(p);
} 


template <typename T>
List<T>::List(const List<T>& list)
    : dummy{new ListNode<T>}, last{dummy}, sz{}
{
    ListNode<T>* node = list.dummy->next;
    while (node) {
        push_back(node->data);
        node = node->next;
    }
}


template <typename T>
List<T>::List(List<T>&& list) noexcept
    : dummy{new ListNode<T>}, last{dummy}, sz{}
{
    list.swap(*this);
}

// Destructor: delete all the nodes including the dummy node
template <typename T>
List<T>::~List() {
    clear();
    delete dummy;
}


template<typename T>
void List<T>::push_front(const T& data) {
    ListNode<T>* new_node = new ListNode<T>{data, dummy->next};
    dummy->next = new_node;

    // Update size and last
    if (dummy == last)
        last = new_node;
    ++sz;     
}


template <typename T>
void List<T>::push_back(const T& data) {
    ListNode<T>* new_node = new ListNode<T>{data};
    last->next = new_node;
    
    // Update size and last
    last = new_node;
    ++sz;   
}


template<typename T>
void List<T>::insert(std::size_t index, const T& data) {
    if (index >= sz)
        index = sz;
    
    ListNode<T>* node = dummy;
    std::size_t t_index {};

    while (t_index++ != index)
        node = node->next;
    
    ListNode<T>* new_node = new ListNode<T>(data, node->next);
    node->next = new_node;
    
    if (node == last)
        last = new_node;
    ++sz;    
}


template<typename T>
void List<T>::pop_front() {
    if (!sz)  return;

    ListNode<T>* first = dummy->next;
    dummy->next = first->next;
    
    if (first == last)
        last = dummy;       
    delete first;
    --sz;    
}


template<typename T>
void List<T>::pop_back() {
    if (!sz)  return;

    ListNode<T>* node = dummy;
    while (node->next != last)
        node = node->next;

    delete last;
    last = node;
    node->next = nullptr;
    --sz;    
}


template<typename T>
void List<T>::remove(std::size_t index) {
    if (index >= sz)
        throw std::invalid_argument("invalid index");       

    ListNode<T>* node = dummy;
    std::size_t t_index {};
    while (t_index++ != index)
        node = node->next;

    ListNode<T>* to_remove = node->next;
    node->next = to_remove->next;

    if (to_remove == last)
        last = node;
    delete to_remove;
    --sz;    
}


template <typename T>
typename List<T>::iterator List<T>::find(T key) {
    for (auto iter = begin(); iter != end(); ++iter)
        if (*iter == key)
            return iter;
    return end();
}


template <typename T>
T& List<T>::front() {
    if (!sz)
        throw std::runtime_error("list is empty");
    return dummy->next->data;
}


template <typename T>
T& List<T>::back() {
    if (!sz)
        throw std::runtime_error("list is empty");
    return last->data;
}


template <typename T>
T& List<T>::at(std::size_t index) {
    if (!sz)
        throw std::runtime_error("list is empty");
    else if (index >= sz)
        throw std::invalid_argument("invalid index");

    ListNode<T>* node = dummy->next;
    std::size_t t_index {};
    while (t_index++ != index)
        node = node->next;
    return node->data;    
}


template <typename T>
void List<T>::clear() {
    ListNode<T>* node = dummy->next;
    while (node) {
        ListNode<T>* next = node->next;
        delete node;
        node = next;
    }
    dummy->next = nullptr;
    last = dummy;
    sz = 0;
}


template <typename T>
void List<T>::swap(List<T>& rhs) noexcept {
    std::swap(sz,    rhs.sz);
    std::swap(last,  rhs.last);
    std::swap(dummy, rhs.dummy);
}


template <typename T>
void swap(List<T>& lhs, List<T>& rhs) noexcept {
    lhs.swap(rhs);
}


template <typename T>
List<T>&  List<T>::operator=(const List<T>& rhs) {
    // Because self assignment happens so rarely we don't check that this != &rhs
    List<T> temp{rhs};  // Exceptions may occur at this state so we create a temp list and then swap it with *this
    temp.swap(*this);
    return *this;
}


template <typename T>
List<T>&  List<T>::operator=(List<T>&& rhs) noexcept {
    rhs.swap(*this);
    return *this;
}


template <typename T>
bool operator==(const List<T>& lhs, const List<T>& rhs) {
    if (lhs.size() != rhs.size())
        return false;

    auto lhs_iter {lhs.cbegin()};
    auto rhs_iter {rhs.cbegin()};

    while (lhs_iter != lhs.cend())
        if (*lhs_iter++ != *rhs_iter++)
            return false; 
    
    return true;   
}


template <typename T>
bool operator!=(const List<T>& lhs, const List<T>& rhs) {
    return !(lhs == rhs);
}


template <typename T>
class List_Iterator {
private:
    ListNode<T>* current;
    friend class List<T>;
public:
    using iterator_category = std::forward_iterator_tag;
    using difference_type = std::ptrdiff_t;
    using value_type = T;
    using pointer = value_type*;
    using reference = value_type&;

    explicit List_Iterator() : current{} {}
    explicit List_Iterator(ListNode<T>* in_node) : current{in_node} {}

    List_Iterator& operator++() {
        assert(current != nullptr && "out-of-bounds iterator increment!");
        current = current->next;
        return *this;
    }
    
    List_Iterator operator++(int) {
        assert(current != nullptr && "out-of-bounds iterator increment!");
        List_Iterator temp{*this};
        current = current->next;
        return temp;
    }

    bool operator == (const List_Iterator& rhs) const { return current == rhs.current; }
    bool operator != (const List_Iterator& rhs) const { return current != rhs.current; }

    reference operator* () const {
        assert(current != nullptr && "invalid iterator dereference!");
        return current->data;
    }

    pointer operator->() const {
        assert(current != nullptr && "invalid iterator dereference!");
        return &current->data;
    }

    void swap(List_Iterator& other) { std::swap(current, other.current); }
};


template <typename T>
class Const_List_Iterator {
private:
    ListNode<T>* current;
    friend class List<T>;
public:
    using iterator_category = std::forward_iterator_tag;
    using difference_type = std::ptrdiff_t;
    using value_type = const T;
    using pointer = value_type*;
    using reference = value_type&;

    explicit Const_List_Iterator() : current{} {}
    explicit Const_List_Iterator(ListNode<T>* in_node) : current{in_node} {}

    Const_List_Iterator& operator++() {
        assert(current != nullptr && "out-of-bounds iterator increment!");
        current = current->next;
        return *this;
    }
    
    Const_List_Iterator operator++(int) {
        assert(current != nullptr && "out-of-bounds iterator increment!");
        Const_List_Iterator temp{*this};
        current = current->next;
        return temp;
    }

    bool operator == (const Const_List_Iterator& rhs) const { return current == rhs.current; }
    bool operator != (const Const_List_Iterator& rhs) const { return current != rhs.current; }

    reference operator*() const {
        assert(current != nullptr && "invalid iterator dereference!");
        return current->data;
    }

    pointer operator->() const {
        assert(current != nullptr && "invalid iterator dereference!");
        return &current->data;
    }
};

}