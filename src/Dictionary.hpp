#include <unordered_map>
#include <vector>

// Dictionary implementation using std::map of key -> std::vector<value>.
// A Dictionary can have multiple entries with the same key. 

template <typename K, typename V>
class Dictionary {
private:
    using container = std::unordered_map<K, std::vector<V>>;
    using umap_iter = typename container::iterator;
    using vec_iter = typename std::vector<V>::iterator;

    container cnt;  // Container 
    int sz {};      // Number of entries

public:
    // MyIterator class deriving from iterator class
    class MyIterator : public std::iterator<std::input_iterator_tag, V> {
    private: 
        Dictionary* dic;    // Pointer to the object, whose elements we want to iterate
        umap_iter ump; 
        vec_iter vc;
        std::pair<K, V> p;  

        // Private helping function
        void shift() {
            // Container is an unordered_map<K, vector<V>> so we iterate through
            // the vector's elements and when we reach the end we increment the container's iterator
            // and repeat.
            while (true) {
                if (++vc != ump->second.end()) return;  // If we have not reached the end of the vector return
                
                if (++ump == dic->cnt.end()) break;

                vc = ump->second.begin();
                
                if (ump != dic->cnt.end()) return; // If we have not reached the end of the unordered_map we return
            }

            // If we have reached the end of the unordered_map, we find the unordered_map's iterator
            // that points to the last element. The vector's iterator is set to
            // the end of the last element's vector.
            for (umap_iter iter = dic->cnt.begin(); iter != dic->cnt.end(); ++iter)
                ump = iter;
            vc = ump->second.end();
        } 

        friend class Dictionary;

    public:
        // Constructors
        MyIterator() : dic{} {}

        MyIterator(Dictionary<K, V>* i) : dic{i} {
            // We initialize the iterators
            ump = dic->cnt.begin();
            vc = ump->second.begin();
        }

        MyIterator(Dictionary<K, V>* i, umap_iter it1, vec_iter it2)
            : dic{i}, ump{it1}, vc{it2} {}


        std::pair<K,V>& operator*() {
            p.first = ump->first;
            p.second = *vc;
            return p;
        }

        std::pair<K,V>* operator->() {
            p.first = ump->first;
            p.second = *vc;
            return &p;
        }
        
        // Prefix ++ calls shift and returns a reference to the iterator that is being incremented
        MyIterator& operator++() {
            shift();
            return *this;
        }
        
        // Post ++ calls prefix ++ and returns a copy of the iterator before he is incremented
        MyIterator operator++(int) {
            MyIterator temp = *this;
            operator++();
            return temp;            
        }

        // Equality and inequality conditions
        bool operator==(const MyIterator& r) {
            return dic == r.dic && ump == r.ump && vc == r.vc;
        }

        bool operator!=(const MyIterator& r) {
            return !operator==(r);
        }
    };

    void insert(K key, V val) {        
        // If the value that we want to insert (associated with that specific key) already exists in our Dictionary
        // we simply set the old_value = new_value and return.
        if (cnt[key].size() != 0) {
            for (V value : cnt[key])
                if (value == val) {
                    value = val;
                    return;
                }
        }
        cnt[key].push_back(val);
        ++sz;
    }

    int size()   const  { return sz;      }
    bool empty() const  { return sz == 0; }

    // Traverse the container until we find the key and return an iterator pointing to the first value with that key.
    // If the key is not found return end() iterator.
    MyIterator find(K key) {
        umap_iter ump;
        vec_iter vc;
        for (ump = cnt.begin(); ump != cnt.end(); ++ump)
            if (ump->first == key) 
                return MyIterator(this, ump, ump->second.begin());
        return end();
    }

    // Return a pair of iterators which represent the start and the end of the space that contains
    // all the values with the specific key.
    // If the key is not found both are end() iterators.
    std::pair<MyIterator, MyIterator> find_all(K key) {
        MyIterator start = find(key);
        MyIterator end;
        if (start == this->end())
            end = this->end();
        else {
            vec_iter last;
            for (vec_iter v = start.vc; v != start.ump->second.end(); ++v)
                last = v;
            end = MyIterator(this, start.ump, last);
        }
        return std::make_pair(start, end);
    }

    MyIterator begin() {
        MyIterator res(this);
        return res;
    }
    
    // The iterator should point to the end() of the vector that belongs to the last key
    MyIterator end() {
        umap_iter last_elem;
        for (umap_iter iter = cnt.begin(); iter != cnt.end(); ++iter)
            last_elem = iter;
        vec_iter last = last_elem->second.end();
        MyIterator res(this, last_elem, last);
        return res;
    }
};

