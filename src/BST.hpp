#include <cassert>
#include <cstddef>
#include <initializer_list>
#include <iterator>
#include <stdexcept>
#include <utility>


namespace cds {

template <typename T> class BST_Iterator;
template <typename T> class Const_BST_Iterator;


template <typename T>
struct TreeNode {
    T data;
    TreeNode *left, *right, *parent;
    TreeNode() : left{}, right{}, parent{} {}
    TreeNode(const T& in_data, TreeNode* in_left = nullptr, TreeNode* in_right = nullptr, TreeNode* in_parent = nullptr)
        : data{in_data}, left{in_left}, right{in_right}, parent{in_parent} {}

    bool is_leaf() { return left == nullptr && right == nullptr; }
};


template <typename T>
class BST {
public:
    using iterator = BST_Iterator<T>;
    using const_iterator = Const_BST_Iterator<T>;

    BST();
    BST(const std::initializer_list<T>& list);
    ~BST();
    
    void insert(const T& value);
    bool search(const T& value);
    void remove(const T& value);
    void clear();

    std::size_t size() const { return sz;      }
    bool empty()       const { return sz == 0; }

    iterator begin() { return iterator{this, p_find_min(root)}; }
    iterator end()   { return iterator{this};                   }

    const_iterator cbegin() const { return const_iterator{this, p_find_min(root)}; }
    const_iterator cend()   const { return const_iterator{this};                   }

private:
    TreeNode<T>* root;
    std::size_t sz;

    TreeNode<T>* p_insert(TreeNode<T>* node, const T& value);
    TreeNode<T>* p_search(TreeNode<T>* node, const T& value);
    TreeNode<T>* p_find_min(TreeNode<T>* node) const;
    TreeNode<T>* p_find_max(TreeNode<T>* node) const;
    TreeNode<T>* p_find_previous(TreeNode<T>* node, TreeNode<T>* target) const;
    TreeNode<T>* p_find_next(TreeNode<T>* node, TreeNode<T>* target) const;
    TreeNode<T>* p_remove(TreeNode<T>* node, const T& value, bool& removed);
    TreeNode<T>* p_remove_min(TreeNode<T>* node, TreeNode<T>** min_node);
    void p_destroy(TreeNode<T>* node);
    
    friend class BST_Iterator<T>;
    friend class Const_BST_Iterator<T>;
};


template <typename T>
BST<T>::BST() : root{}, sz{} {}


template <typename T>
BST<T>::BST(const std::initializer_list<T>& list) : root{}, sz{} {
    for (auto& value : list)
        insert(value);
}


template <typename T>
BST<T>::~BST() {
    p_destroy(root);
}

template <typename T>
void BST<T>::p_destroy(TreeNode<T>* node) {
    if (!node) return;
    p_destroy(node->left);
    p_destroy(node->right);
    delete node;
}


template <typename T>
void BST<T>::insert(const T& value) {
    root = p_insert(root, value);
    ++sz;
}

template <typename T>
TreeNode<T>* BST<T>::p_insert(TreeNode<T>* node, const T& value) {
    if (!node) 
        node = new TreeNode<T>{value};
    else if (node->data > value) {
        node->left = p_insert(node->left, value);
        node->left->parent = node;
    }
    else if (node->data < value) {
        node->right = p_insert(node->right, value);
        node->right->parent = node;
    }
    return node;
}


template<typename T>
bool BST<T>::search(const T& value) {
    return p_search(root, value) != nullptr ? true : false;
}

template<typename T>
TreeNode<T>* BST<T>::p_search(TreeNode<T>* node, const T& value) {
    if (!node)                      return nullptr;
    else if (node->data == value)   return node;
    else if (node->data < value)    return p_search(node->right, value);
    else                            return p_search(node->left, value);
}


template<typename T>
TreeNode<T>* BST<T>::p_find_min(TreeNode<T>* node) const {
    return node != nullptr && node->left != nullptr 
           ? p_find_min(node->left)    // If a left subtree exists the min value will be there,
	       : node;					   // otherwise the min value is in node
}

template<typename T>
TreeNode<T>* BST<T>::p_find_max(TreeNode<T>* node) const {
    return node != nullptr && node->rightt != nullptr 
           ? p_find_max(node->right)
		   : node;
}
    

// Returns target's previous in order TreeNode in the subtree with root node or nullptr if target is the subtree's min. 
template<typename T>
TreeNode<T>* BST<T>::p_find_previous(TreeNode<T>* node, TreeNode<T>* target) const {
    if (node == target)    // If target is root of the subtree then its previous will be the maximum value in the left subtree
        return p_find_max(node->left);
    else if (node->data > target->data)    // Target is in the left subtree so his previous will there too
        return p_find_previous(node->left, target);
    else {    // Target is in the right subtree so his previous wiil either be there or it will be node itself
	    TreeNode<T>* res = p_find_previous(node->right, target);
	    return res != nullptr ? res : node;
    }
}

// Returns target's next in order TreeNode in the subtree with root node or nullptr if target is the subtree's max. 
template<typename T>
TreeNode<T>* BST<T>::p_find_next(TreeNode<T>* node, TreeNode<T>* target) const {
        if (node == target)    // If target is root of the subtree then its next will be the minimum value in the right subtree
            return p_find_min(node->right);
        else if (node->data < target->data)    // Target is in the right subtree so his next will there too
            return p_find_next(node->right, target);
        else {    // Target is in the left subtree so his next wiil either be there or it will be node itself
		    TreeNode<T>* res = p_find_next(node->left, target);
		    return res != nullptr ? res : node;
        }
    }


template<typename T>
void BST<T>::remove(const T& value) {
    bool removed;
    root = p_remove(root, value, removed);
    if (!removed)
        throw std::runtime_error("no such value in the BST");
    --sz;
}

template<typename T>
TreeNode<T>* BST<T>::p_remove(TreeNode<T>* node, const T& value, bool& removed) {
    if (!node) {
        removed = false;
        return nullptr;
    }

    if (node->data == value) {
        removed = true;    // Found value

        if (node->is_leaf()) {
            delete node;
            node = nullptr;
            return nullptr;
        }

        if (!node->left) {    // No left subtree, so new root will be the right child
            TreeNode<T>* temp {node->right};
            temp->parent = node->parent;
            delete node;
            node = nullptr;
            return temp;
        }
        else if (!node->right) {    // No right subtree, so new root will be the left child
            TreeNode<T>* temp {node->left};
            temp->parent = node->parent;
            delete node;
            node = nullptr;
            return temp;
        }
        else {
            TreeNode<T>* min_right;
			node->right = p_remove_min(node->right, &min_right);

			min_right->left = node->left;
			min_right->right = node->right;
			min_right->parent = node->parent;
			
			delete node;
			return min_right;
        }
    }
    else if (node->data > value)
        node->left = p_remove(node->left, value, removed);
    else 
        node->right = p_remove(node->right, value, removed);
    
    return node;
}

template<typename T>
TreeNode<T>* BST<T>::p_remove_min(TreeNode<T>* node, TreeNode<T>** min_node) {
	if (!node->left) {    // No left subtree so node is the min_node
		*min_node = node;
        if (node->right)
            node->right->parent = node->parent;
		return node->right;		// new root is the right child
	}
    else {    // There is a left subtree so the min_node is there
		node->left = p_remove_min(node->left, min_node);
		return node;
	} 
}


template <typename T>
void BST<T>::clear() {
    p_destroy(root);
    root = nullptr;
    sz = 0;
}


template <typename T>
class BST_Iterator {
private:
    BST<T>* bst;
    TreeNode<T>* current;
    friend class BST<T>;
public:
    using iterator_category = std::forward_iterator_tag;
    using difference_type = std::ptrdiff_t;
    using value_type = T;
    using pointer = value_type*;
    using reference = value_type&;

    BST_Iterator() : bst{}, current{} {}

    BST_Iterator(BST<T>* in_bst, TreeNode<T>* in_current = nullptr)
        : bst{in_bst}, current{in_current} {}

    BST_Iterator& operator++() {
        assert(current != nullptr && "out-of-bounds iterator increment!");
        current = bst->p_find_next(bst->root, current);
        return *this;
    }
    
    BST_Iterator operator++(int) {
        assert(current != nullptr && "out-of-bounds iterator increment!");
        BST_Iterator temp{*this};
        current = bst->p_find_next(bst->root, current);
        return temp;
    }

    bool operator == (const BST_Iterator& rhs) const { return bst == rhs.bst && current == rhs.current; }
    bool operator != (const BST_Iterator& rhs) const { return bst != rhs.bst || current != rhs.current; }

    reference operator* () const {
        assert(current != nullptr && "invalid iterator dereference!");
        return current->data;
    }

    pointer operator->() const {
        assert(current != nullptr && "invalid iterator dereference!");
        return &current->data;
    }

    void swap(BST_Iterator& other) { 
        std::swap(bst, other.bst); 
        std::swap(current, other.current);
    }
};


template <typename T>
class Const_BST_Iterator {
private:
    const BST<T>* bst;
    TreeNode<T>* current;
    friend class BST<T>;
public:
    using iterator_category = std::forward_iterator_tag;
    using difference_type = std::ptrdiff_t;
    using value_type = const T;
    using pointer = value_type*;
    using reference = value_type&;

    Const_BST_Iterator() : bst{}, current{} {}

    Const_BST_Iterator(const BST<T>* in_bst, TreeNode<T>* in_current = nullptr)
        : bst{in_bst}, current{in_current} {}

    Const_BST_Iterator& operator++() {
        assert(current != nullptr && "out-of-bounds iterator increment!");
        current = bst->p_find_next(bst->root, current);
        return *this;
    }
    
    Const_BST_Iterator operator++(int) {
        assert(current != nullptr && "out-of-bounds iterator increment!");
        Const_BST_Iterator temp{*this};
        current = bst->p_find_next(bst->root, current);
        return temp;
    }

    bool operator == (const Const_BST_Iterator& rhs) const { return bst == rhs.bst && current == rhs.current; }
    bool operator != (const Const_BST_Iterator& rhs) const { return bst != rhs.bst || current != rhs.current; }

    reference operator* () const {
        assert(current != nullptr && "invalid iterator dereference!");
        return current->data;
    }

    pointer operator->() const {
        assert(current != nullptr && "invalid iterator dereference!");
        return &current->data;
    }
};

}