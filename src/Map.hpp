#include "Vector.hpp"
#include <cassert>
#include <stdexcept>
#include <functional>
#include <iterator>


namespace cds {

template <typename K, typename V, typename H> class Map_Iterator;
template <typename K, typename V, typename H> class Const_Map_Iterator;


template <typename K, typename V, typename H = std::hash<K>>
class Map {
private:
    using entry = std::pair<K,V>;
    
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
    Vector<Vector<entry>> array;

    void rehash() {
        Vector<Vector<entry>> old_array {std::move(array)};

        std::size_t old_cap = cap;
        int prime_no = sizeof(prime_sizes) / sizeof(int);
        for (int i = 0; i < prime_no; ++i) {
            if (prime_sizes[i] > old_cap) {
                cap = prime_sizes[i];
                break;
            }
        }

        if (cap == old_cap)
            cap *= 2;

        array.resize(cap);

        sz = 0;
        for (auto& vec : old_array) {
            if (vec.size()) {
                for (auto& p : vec)
                    insert(p.first, p.second);
            }
            vec.clear();
        }
        old_array.clear();
    }

    V& get_value(const K& key) {
        unsigned pos = hash_function(key) % cap;
        for (auto& p : array[pos])
            if (p.first == key)
                return p.second;
        throw std::runtime_error("");
    }

public:
    using iterator = Map_Iterator<K,V,H>;
    using const_iterator = Const_Map_Iterator<K,V,H>;

    Map();
    Map(H in_hash);
    Map(const std::initializer_list<entry>& list);

    void insert(const K& key, const V& value);
    void remove(const K& key);

    V& operator[](const K& key);

    std::size_t size() const { return sz;      }
    bool empty()       const { return sz == 0; }

    void clear();

    iterator find(K key);

    iterator begin();
    iterator end();

    const_iterator cbegin() const;
    const_iterator cend() const;
};


template <typename K, typename V, typename H>
Map<K,V,H>::Map()
    : hash_function{H()}
{
    array.reserve(cap);
}

template <typename K, typename V, typename H>
Map<K,V,H>::Map(H in_hash)
    : hash_function{in_hash()}
{
    array.reserve(cap);
}

template <typename K, typename V, typename H>
Map<K,V,H>::Map(const std::initializer_list<entry>& list)
    : hash_function{H()}
{
    array.reserve(cap);
    for (auto& e : list)
        insert(e.first, e.second);
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
    if (load_factor > max_load_factor)
        rehash();
} 

template <typename K, typename V, typename H>
void Map<K,V,H>::remove(const K& key) {
    unsigned pos = hash_function(key) % cap;
    if (array[pos].empty()) 
        return;
    
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
}

template <typename K, typename V, typename H>
V& Map<K,V,H>::operator[](const K& key) {
    // If the key exists in the map we return the value it maps to, otherwise 
    // we create a new entry that maps the given key to a default value V.
    try {
        return get_value(key);  
    }
    catch (std::runtime_error& e) {
        insert(key, V{});
        return get_value(key);
    }
}

template <typename K, typename V, typename H>
typename Map<K,V,H>::iterator Map<K,V,H>::find(K key) {
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
    auto last_pos = std::next(array.begin(), sz);
    return iterator{this, last_pos, last_pos->end()};    
}


template <typename K, typename V, typename H>
class Map_Iterator {
private:
    using node = std::pair<K,V>;
    using vec_iter = typename Vector<node>::iterator;

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
        else 
            in_iter  = map->array[out_pos].end();
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
    using node = std::pair<K,V>;
    using vec_iter = typename Vector<node>::const_iterator;

    const Map<K,V,H>* map;
    std::size_t out_pos;
    vec_iter in_iter;
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
        else 
            in_iter  = map->array[out_pos].cend();
    }

    Const_Map_Iterator(Map<K,V,H>* in_map, std::size_t in_out_pos, vec_iter in_in_iter)
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