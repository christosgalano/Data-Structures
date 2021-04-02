#include <vector>
#include <cassert>
#include <stdexcept>
#include <functional>
#include <iostream>

template <typename K, typename V, typename H = std::hash<K>>
class Map {
private:
    static constexpr int prime_sizes[] = { 
        53, 97, 193, 389, 769, 1543, 3079, 6151, 12289,
        24593, 49157, 98317, 196613, 393241,
        786433, 1572869, 3145739, 6291469, 12582917,
        25165843, 50331653, 100663319, 201326611,
        402653189, 805306457, 1610612741
    };
    
    struct Node {
        K key;
        V value;
        Node() : key{}, value{} {}
        Node(const K& in_key, const V& in_value)
            : key{in_key}, value{in_value} {}
        
        bool operator<(const Node& rhs) {
            return key < rhs.key;
        }

        bool operator==(const Node& rhs) {
            return key == rhs.key;
        }
    };

    static constexpr double max_load_factor {0.9};

    std::size_t sz {};
    std::size_t cap {prime_sizes[0]};

    H hash_function;
    std::vector<std::vector<Node>> array;

    void rehash() {
        std::vector<std::vector<Node>> old_array {std::move(array)};

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
                for (auto& node : vec)
                    insert(node.key, node.value);
            }
            vec.clear();
        }
        old_array.clear();
    }

    V& get_value(const K& key) {
        unsigned pos = hash_function(key) % cap;
        for (auto& node : array[pos])
            if (node.key == key)
                return node.value;
        throw std::runtime_error("");
    }

public:
    Map() : hash_function{H()}, array{cap} {}

    void insert(const K& key, const V& value) {
        bool already_in_map = false;
        unsigned pos = hash_function(key) % cap;

        for (auto& node : array[pos]) {
            if (node.key == key) {
                node.key = key;
                node.value = value;
                already_in_map = true;
            }
        }

        if (!already_in_map) {
            array[pos].push_back(Node{key, value});
            ++sz;
        }

        double load_factor = static_cast<double>(sz) / cap;
        if (load_factor > max_load_factor)
            rehash();
    } 

    void remove(const K& key) {
        unsigned pos = hash_function(key) % cap;
        if (array[pos].size()) {
            for (auto iter = array[pos].begin(); iter != array[pos].end(); ++iter) {
                if (iter->key == key) {
                    array[pos].erase(iter);
                    --sz;
                    return;
                }
            }
        }
    }

    V& operator[](const K& key) {
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

    std::size_t size() const { return sz;      }
    bool empty()       const { return sz == 0; }
};

// TODO : iterator