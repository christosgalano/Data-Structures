#pragma once

#include "Vector.hpp"
#include <functional>

namespace data_structures {

template <typename K, typename V, typename H> class Map_Iterator;
template <typename K, typename V, typename H> class Const_Map_Iterator;

template <typename K, typename V, typename H = std::hash<K>>
class Map {
public:
    using iterator = Map_Iterator<K,V,H>;
    using const_iterator = Const_Map_Iterator<K,V,H>;

    Map();
    explicit Map(H in_hash);
    explicit Map(const std::initializer_list<std::pair<K,V>>& list);
    Map(const Map& vec);
    Map(Map&& vec) noexcept;
    ~Map() = default;

    void insert(const K& key, const V& value);
    void remove(const K& key);

    std::size_t size() const { return sz;      }
    bool empty()       const { return sz == 0; }

    void clear();
    void swap(Map& rhs) noexcept;

    V& operator[](const K& key);
    const V& operator[](const K& key) const;
    Map&  operator=(const Map& rhs);
    Map&  operator=(Map&& rhs) noexcept;

    iterator find(const K& key);

    iterator begin();
    iterator end();

    const_iterator cbegin() const;
    const_iterator cend() const;

private: 
    friend class Map_Iterator<K,V,H>;
    friend class Const_Map_Iterator<K,V,H>;

    static constexpr int prime_sizes[] = { 
        53, 97, 193, 389, 769, 1543, 3079, 6151, 12289,
        24593, 49157, 98317, 196613, 393241,
        786433, 1572869, 3145739, 6291469, 12582917,
        25165843, 50331653, 100663319, 201326611,
        402653189, 805306457, 1610612741
    };

    static constexpr double max_load_factor {0.9};

    std::size_t sz {};
    std::size_t cap {prime_sizes[0]};

    H hash_function;
    Vector<Vector<std::pair<K,V>>> array;

    void rehash();
    V& get_value(const K& key);
};


template <typename K, typename V, typename H>
Map<K,V,H>::Map()
    : hash_function{H()}
{
    array.reserve(cap);
    array.resize(cap);  // value-initialize the inner vectors
}


template <typename K, typename V, typename H>
Map<K,V,H>::Map(H in_hash)
    : hash_function{in_hash()}
{
    array.reserve(cap);
    array.resize(cap);
}


template <typename K, typename V, typename H>
Map<K,V,H>::Map(const std::initializer_list<std::pair<K,V>>& list)
    : hash_function{H()}
{
    array.reserve(cap);
    array.resize(cap);
    for (auto& e : list)
        insert(e.first, e.second);
}


template <typename K, typename V, typename H>
Map<K,V,H>::Map(const Map& map) 
    : sz{map.sz}, cap{map.cap}, hash_function{H()}, array{map.array}
{    
    for (int i = 0; i < map.array.capacity(); ++i)
        array[i] = map.array[i];
}


template <typename K, typename V, typename H>
Map<K,V,H>::Map(Map&& map) noexcept
    : hash_function{H()}
{
    map.swap(*this);
} 


template <typename K, typename V, typename H>
void Map<K,V,H>::swap(Map& rhs) noexcept {
    std::swap(sz, rhs.sz);
    std::swap(cap, rhs.cap);

    // We need also to swap the contents of the inner vectors
    Map temp {*this};

    array = rhs.array;
    for (int i = 0; i < array.capacity(); ++i)
        array[i] = std::move(rhs.array[i]);

    rhs.array = temp.array;
    for (int i = 0; i < rhs.array.capacity(); ++i)
        rhs.array[i] = std::move(temp.array[i]);
}


template <typename K, typename V, typename H>
V& Map<K,V,H>::get_value(const K& key) {
    unsigned pos = hash_function(key) % cap;
    for (auto& p : array[pos])
        if (p.first == key)
            return p.second;
    throw std::runtime_error("");
}


template <typename K, typename V, typename H>
void Map<K,V,H>::rehash() {
    Vector<Vector<std::pair<K,V>>> old_array{array};

    std::size_t old_cap {cap};
    int prime_no = sizeof(prime_sizes) / sizeof(int);
    for (int i = 0; i < prime_no; ++i) {
        if (prime_sizes[i] > old_cap) {
            cap = prime_sizes[i];
            break;
        }
    }
 
    if (cap == old_cap)
        cap *= 2;
 
    array.clear();
    array.reserve(cap);
    array.resize(cap);

    sz = 0;
    for (std::size_t i = 0; i < old_array.size(); ++i) {
        for (auto& p : old_array[i])
            insert(p.first, p.second);
        old_array[i].clear();
    }
    old_array.clear();
}
    

template <typename K, typename V, typename H>
void Map<K,V,H>::insert(const K& key, const V& value) {
    bool already_in_map = false;
    unsigned pos = hash_function(key) % cap;

    for (auto& p : array[pos]) {
        if (p.first == key) {
            p.second = value;
            already_in_map = true;
        }
    }

    if (!already_in_map) {
        array[pos].push_back(std::make_pair(key, value));
        ++sz;
    }

    double load_factor = static_cast<double>(sz) / cap;
    if (load_factor >= max_load_factor)
        rehash();
} 


template <typename K, typename V, typename H>
void Map<K,V,H>::remove(const K& key) {
    unsigned pos = hash_function(key) % cap;
    if (array[pos].empty()) 
        throw std::runtime_error("no such key in the map");
    
    for (auto iter = array[pos].begin(); iter != array[pos].end(); ++iter) {
        if (iter->first == key) {
            array[pos].erase(iter);
            --sz;
            return;
        }
    }
}


template <typename K, typename V, typename H>
void Map<K,V,H>::clear() {
    array.clear();
    sz = 0;
}


// If the key exists in the map we return the value it maps to, otherwise 
// we create a new std::pair<K,V> that maps the given key to a default value V.
template <typename K, typename V, typename H>
V& Map<K,V,H>::operator[](const K& key) {
    try {
        return get_value(key);  
    }
    catch (std::runtime_error& e) {
        insert(key, V{});
        return get_value(key);
    }
}


template <typename K, typename V, typename H>
const V& Map<K,V,H>::operator[](const K& key) const
{
    try {
        return get_value(key);  
    }
    catch (std::runtime_error& e) {
        insert(key, V{});
        return get_value(key);
    }    
}


// Because self assignment happens so rarely we don't check that this != &rhs
template <typename K, typename V, typename H>
Map<K,V,H>& Map<K,V,H>::operator=(const Map& rhs) {
    Map temp{rhs};
    temp.swap(*this);
    return *this;
}


template <typename K, typename V, typename H>
Map<K,V,H>& Map<K,V,H>::operator=(Map&& rhs) noexcept {
    rhs.swap(*this);
    return *this;
}


template <typename K, typename V, typename H>
bool operator==(const Map<K,V,H>& lhs, const Map<K,V,H>& rhs) {
    if (lhs.size() != rhs.size())
        return false;

    auto lhs_iter {lhs.cbegin()};
    auto rhs_iter {rhs.cbegin()};

    while (lhs_iter != lhs.cend()) {
        if ( (lhs_iter->first != rhs_iter->first) || (lhs_iter->second != rhs_iter->second) )
            return false; 
        ++lhs_iter;
        ++rhs_iter;
    }   
    return true;    
}


template <typename K, typename V, typename H>
bool operator!=(const Map<K,V,H>& lhs, const Map<K,V,H>& rhs) {
    return !(lhs == rhs);
}


template <typename K, typename V, typename H>
typename Map<K,V,H>::iterator Map<K,V,H>::find(const K& key) {
    for (iterator iter = begin(); iter != end(); ++iter)
        if (iter->first == key)
            return iter;
    return end();
}


template <typename K, typename V, typename H>
inline typename Map<K,V,H>::iterator Map<K,V,H>::begin() {
    return iterator{this};
}
   
    
template <typename K, typename V, typename H>
inline typename Map<K,V,H>::iterator Map<K,V,H>::end() {
    return iterator{this, cap-1, array[cap-1].end()};
}


template <typename K, typename V, typename H>
inline typename Map<K,V,H>::const_iterator Map<K,V,H>::cbegin() const {
    return const_iterator{this};
}


template <typename K, typename V, typename H>
inline typename Map<K,V,H>::const_iterator Map<K,V,H>::cend() const {
    return const_iterator{this, cap-1, array[cap-1].cend()};
}


template <typename K, typename V, typename H>
class Map_Iterator {
private:
    using vec_iter = typename Vector<std::pair<K,V>>::iterator;

    Map<K,V,H>* map;
    std::size_t out_pos;    // used to iterate through the outer Vector
    vec_iter in_iter;       // used to iterate through the inner Vector 
    std::pair<K,V> p;

    // Helping function
    void shift() {
        while (true) {
            if (++in_iter != map->array[out_pos].end())
                return;

            bool flag = false;
            while (++out_pos < map->cap) {
                if (map->array[out_pos].size()) {
                    in_iter = map->array[out_pos].begin();
                    flag = true;
                    return;
                }
            }

            if (!flag) break;
        }

        out_pos = map->cap - 1;
        in_iter = map->array[out_pos].end();
    }

public:
    using iterator_category = std::bidirectional_iterator_tag;
    using difference_type = std::ptrdiff_t;
    using value_type = std::pair<K, V>;
    using pointer = value_type*;
    using reference = value_type&;

    Map_Iterator() : map{} {}

    Map_Iterator(Map<K,V,H>* in_map) 
        : map{in_map}
    {
        bool flag = false;
        for (unsigned i = 0; i < map->cap; ++i) {
            if (map->array[i].size()) {
                out_pos = i;
                flag = true;
                break;
            }
        }

        if (flag)
            in_iter = map->array[out_pos].begin();
        else {
            out_pos = map->cap - 1;
            in_iter  = map->array[out_pos].end();
        }
    }

    Map_Iterator(Map<K,V,H>* in_map, std::size_t in_out_pos, vec_iter in_in_iter)
        : map{in_map}, out_pos{in_out_pos}, in_iter{in_in_iter} {}
    
    reference operator*() {
        p.first  = in_iter->first;
        p.second = in_iter->second;
        return p;
    }

    pointer operator->() {
        p.first  = in_iter->first;
        p.second = in_iter->second;
        return &p;
    }        

    Map_Iterator& operator++() {
        shift();
        return *this;
    }        
    
    Map_Iterator operator++(int) {
        Map_Iterator temp = *this;
        operator++();
        return temp;            
    }

    bool operator==(const Map_Iterator& rhs) const {
        return map == rhs.map && out_pos == rhs.out_pos && in_iter == rhs.in_iter;
    }
    
    bool operator!=(const Map_Iterator& rhs) const {
        return !operator==(rhs);
    }
};


template <typename K, typename V, typename H>
class Const_Map_Iterator {
private:
    using vec_iter = typename Vector<std::pair<K,V>>::const_iterator;

    const Map<K,V,H>* map;
    std::size_t out_pos;
    vec_iter in_iter;
    std::pair<K,V> p;

    // Helping function
    void shift() {
        while (true) {
            if (++in_iter != map->array[out_pos].cend())
                return;

            bool flag = false;
            while (++out_pos < map->cap) {
                if (map->array[out_pos].size()) {
                    in_iter = map->array[out_pos].cbegin();
                    flag = true;
                    return;
                }
            }

            if (!flag) break;
        }

        out_pos = map->cap - 1;
        in_iter = map->array[out_pos].cend();
    }

public:
    using iterator_category = std::bidirectional_iterator_tag;
    using difference_type = std::ptrdiff_t;
    using value_type = const std::pair<K, V>;
    using pointer = value_type*;
    using reference = value_type&;

    Const_Map_Iterator() : map{} {}

    Const_Map_Iterator(const Map<K,V,H>* in_map) 
        : map{in_map}
    {
        bool flag = false;
        for (unsigned i = 0; i < map->cap; ++i) {
            if (map->array[i].size()) {
                out_pos = i;
                flag = true;
                break;
            }
        }

        if (flag)
            in_iter = map->array[out_pos].cbegin();
        else {
            out_pos = map->cap - 1;
            in_iter  = map->array[out_pos].cend();
        }
    }

    Const_Map_Iterator(const Map<K,V,H>* in_map, std::size_t in_out_pos, vec_iter in_in_iter)
        : map{in_map}, out_pos{in_out_pos}, in_iter{in_in_iter} {}
    
    reference operator*() {
        p.first  = in_iter->first;
        p.second = in_iter->second;
        return p;
    }

    pointer operator->() {
        p.first  = in_iter->first;
        p.second = in_iter->second;
        return &p;
    }        

    Const_Map_Iterator& operator++() {
        shift();
        return *this;
    }        
    
    Const_Map_Iterator operator++(int) {
        Const_Map_Iterator temp = *this;
        operator++();
        return temp;            
    }

    bool operator==(const Const_Map_Iterator& rhs) const {
        return map == rhs.map && out_pos == rhs.out_pos && in_iter == rhs.in_iter;
    }
    
    bool operator!=(const Const_Map_Iterator& rhs) const {
        return !operator==(rhs);
    }
};

}