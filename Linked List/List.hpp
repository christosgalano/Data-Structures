#include <utility>
#include <cassert>
#include <stdexcept>
#include <initializer_list>
template <typename T>
class List {
private:
    class ListNode; // Forward declaration

    ListNode* dummy;
    ListNode* last;
    std::size_t sz;
    
public:
    class Iterator; // Forward declaration

    // Constructor: create a dummy node so that even an empty list has one node.
    // In an empty list last_node == dummy.
    List() : dummy{new ListNode}, last{dummy}, sz{} {}

    // Constructor with initializer_list
    List(const std::initializer_list<T>& values) : dummy{new ListNode}, last{dummy}, sz{} {
        for (auto p : values)
            push_back(p);
    }

    List(const List<T>& list) 
        : dummy{new ListNode}, last{dummy}, sz{}
    {   
        ListNode* node = list.dummy->next;
        while (node) {
            push_back(node->data);
            node = node->next;
        }
    }

    List(List<T>&& list)
        : dummy{new ListNode}, last{dummy}, sz{}
    {
        list.swap(*this);
    }

    // Destructor: delete all the nodes including the dummy node
    ~List() {
        clear();
        delete dummy;
    }

    // Insert node at the front
    void push_front(const T& data) { 
        ListNode* new_node = new ListNode(data, dummy->next);
        dummy->next = new_node;
        
        // Update size and last
        if (dummy == last)
            last = new_node;
        ++sz; 
    }

    // Insert node at the back
    void push_back(const T& data) {
        ListNode* new_node = new ListNode(data);
        last->next = new_node;
        
        // Update size and last
        last = new_node;
        ++sz;   
    }

    // Insert an item at a given index. The first argument is the index of the value before which to insert (indexes start at 0)
    void insert(std::size_t index, const T& data) {
        if (index >= sz)
            index = sz;

        ListNode* node = dummy;
        std::size_t t_index {};
        while (t_index++ != index)
            node = node->next;

        ListNode* new_node = new ListNode(data, node->next);
        node->next = new_node;

        if (node == last)
            last = new_node;
        ++sz;
    }

    // Removes the first value of the list
    void pop_front() {
        if (!sz)
            throw std::logic_error("list is empty");

        ListNode* first = dummy->next;
        dummy->next = first->next;

        if (first == last)
            last = dummy;
       
        delete first;
        --sz;
    }

    // Removes the last value of the list
    void pop_back() {
        if (!sz) 
            throw std::logic_error("list is empty");

        ListNode* node = dummy;
        while (node->next != last)
            node = node->next;

        delete last;

        last = node;
        node->next = nullptr;
        --sz;
    }

    // Remove the item at a given index. If the index is greater than the list's size the function throws an exception.
    void remove(std::size_t index) {
        if (index >= sz)
            throw std::invalid_argument("invalid index");
        
        ListNode* node = dummy;
        std::size_t t_index {};
        while (t_index++ != index)
            node = node->next;

        ListNode* to_remove = node->next;
        node->next = to_remove->next;
        if (to_remove == last)
            last = node;

        delete to_remove;
        --sz;
    }

    // Returns an iterator to the key's first occurrence in the list. If the key is not found the function returns iterator end().
    Iterator find(T key) {
        for (auto iter = begin(); iter != end(); ++iter)
            if (*iter == key)
                return iter;
        return end();
    }

    T& front() {
        if (!sz) throw std::logic_error("list is empty");
        return dummy->next->data;
    }

    T& back() {
        if (!sz) throw std::logic_error("list is empty");
        return last->data;
    }

    T& at(std::size_t index) {
        if (index >= sz)
            throw std::invalid_argument("invalid index");
        
        ListNode* node = dummy->next;
        std::size_t t_index {};
        while (t_index++ != index)
            node = node->next;
        return node->data;    
    }

    std::size_t size()  const { return sz;      }
    bool        empty() const { return sz == 0; }

    void clear() {
        ListNode* node = dummy->next;
        while (node) {
            ListNode* next = node->next;
            delete node;
            node = next;
        }

        dummy->next = nullptr;
        last = dummy;
        sz = 0;
    }

    void swap(List<T>& rhs) {
        std::swap(sz,    rhs.sz);
        std::swap(last,  rhs.last);
        std::swap(dummy, rhs.dummy);
    }

    friend void swap(List<T>& lhs, List<T>& rhs) {
        lhs.swap(rhs);
    }

    // Because self assignment happens so rarely we don't check that this != &rhs
    List<T>&  operator=(const List<T>& rhs) {
        List<T> temp{rhs};  // Exceptions may occur at this state so we create a temp list and then swap it with *this
        temp.swap(*this);
        return *this;
    }

    List<T>&  operator=(List<T>&& rhs) noexcept {
        rhs.swap(*this);
        return *this;
    }

    friend bool operator==(const List<T>& lhs, const List<T>& rhs) {
        if (lhs.sz != rhs.sz)
            return false;

        ListNode* l_node = lhs.dummy->next;
        ListNode* r_node = rhs.dummy->next;

        while (l_node) {

            if (l_node->data != r_node->data)
                return false;

            l_node = l_node->next;
            r_node = r_node->next;
        }
            
        return true;   
    }

    friend bool operator!=(const List<T>& lhs, const List<T>& rhs) {
        return !(lhs == rhs);
    }

    Iterator begin() { return Iterator(dummy->next); }
    Iterator end()   { return Iterator(nullptr);     }
};

template <typename T>
class List<T>::ListNode {
    private:
        T data;
        ListNode* next;
        friend class List;
    public:
        ListNode() : next{} {}
        ListNode(T in_data, ListNode* in_next = nullptr)
            : data{in_data}, next{in_next} {}
};


template <typename T>
class List<T>::Iterator {
private:
    ListNode* current;
    explicit Iterator(ListNode* in_node) : current{in_node} {}
    friend class List;

public:
    explicit Iterator() : current{} {}
    Iterator& operator++() {
        assert(current);
        current = current->next;
        return *this;
    }
    
    Iterator operator++(int) {
        assert(current);
        Iterator temp(*this);
        current = current->next;
        return temp;
    }

    bool operator == (const Iterator& rhs) const { return current == rhs.current; }
    bool operator != (const Iterator& rhs) const { return current != rhs.current; }

    T& operator* () const {
        assert(current);
        return current->data;
    }

    void swap(Iterator& other) { std::swap(current, other.current); }
};