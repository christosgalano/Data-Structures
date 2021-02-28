//
// Red Black Tree
//

#ifndef RED_BLACK_TREE_RBTREE_H
#define RED_BLACK_TREE_RBTREE_H

#include <iostream>  // For std::cout
#include <algorithm> // For std::swap

enum Color {RED, BLACK, DOUBLE_BLACK};

template <typename T>
struct Node {
    T data;
    Color color;
    Node<T> *left, *right, *parent;
    Node(T data) {
        this->data = data;
        color = RED;
        left = right = parent = nullptr;
    }
};

template <typename T>
class RBTree {
public:
    RBTree() : root{}, sz{} {}
    ~RBTree();
    void insert(T val);
    void remove(T key);
    bool search(T key);
    void inorder();
    void preorder();
    int size()    const { return sz;      }
    bool empty()  const { return sz == 0; }

private:
    Node<T>* root;
    int sz;
    void rotateLeft(Node<T>*& node);
    void rotateRight(Node<T>*& node);
    void fixInsertRBTree(Node<T>*& node);
    void fixDeleteRBTree(Node<T>*& node);
    void inorderBST(Node<T>*& node);
    void preorderBST(Node<T>*& node);
    Color getColor(Node<T>*& node);
    void setColor(Node<T>*& node, Color c);
    Node<T>* insertBST(Node<T>*& root, Node<T>*& node);
    Node<T>* deleteBST(Node<T>*& node, T key);
    Node<T>* searchBST(Node<T>*, T key);
    Node<T>* minValueNode(Node<T>*& node);
    Node<T>* maxValueNode(Node<T>*& node);
    void destroy(Node<T>* node);
};

template <typename T>
RBTree<T>::~RBTree() {
    destroy(root);
}

template <typename T>
void RBTree<T>::destroy(Node<T>* node) {
    if (!node) 
        return;

    destroy(node->left);
    destroy(node->right);
    delete node;
}

template <typename T>
Color RBTree<T>::getColor(Node<T>*& node) {
    if (!node)    // In this way we don't need to check if uncle == nullptr because all nullptr are BLACK
        return BLACK;       

    return node->color;
}

template <typename T>
void RBTree<T>::setColor(Node<T>*& node, Color color) {
    if (!node)
        return;

    node->color = color;
}

template <typename T>
Node<T>* RBTree<T>::insertBST(Node<T>*& root, Node<T>*& ptr) {
    if (!root) {
        ++sz;
        return ptr;
    }

    if (ptr->data < root->data) {
        root->left = insertBST(root->left, ptr);
        root->left->parent = root;
    } else if (ptr->data > root->data) {
        root->right = insertBST(root->right, ptr);
        root->right->parent = root;
    }

    return root;
}

template <typename T>
void RBTree<T>::insert(T n) {
    // Use BST insertion and then amke sure the RBTree properties are preserved
    Node<T>* node = new Node<T>(n);
    root = insertBST(root, node);
    fixInsertRBTree(node);
}

template <typename T>
void RBTree<T>::rotateLeft(Node<T>*& node) {
    // Simple left rotation
    Node<T>* right_child = node->right;
    node->right = right_child->left;

    if (node->right)
        node->right->parent = node;

    right_child->parent = node->parent;

    if (!node->parent)
        root = right_child;
    else if (node == node->parent->left)
        node->parent->left = right_child;
    else
        node->parent->right = right_child;

    right_child->left = node;
    node->parent = right_child;
}

template <typename T>
void RBTree<T>::rotateRight(Node<T>*& node) {
    // Simple right rotation
    Node<T>* left_child = node->left;
    node->left = left_child->right;

    if (node->left)
        node->left->parent = node;

    left_child->parent = node->parent;

    if (!node->parent)
        root = left_child;
    else if (node == node->parent->left)
        node->parent->left = left_child;
    else
        node->parent->right = left_child;

    left_child->right = node;
    node->parent = left_child;
}

template <typename T>
void RBTree<T>::fixInsertRBTree(Node<T>*& node) {
    // Responsible to preserve the RBTree properties
    Node<T>* parent {};
    Node<T>* grandparent {};
    while (node != root && getColor(node) == RED && getColor(node->parent) == RED) {
        parent = node->parent;
        grandparent = parent->parent;

        /*  Case : A 
            Parent of node is left child  
            of Grand-parent of node */
        if (parent == grandparent->left) {
            Node<T>* uncle = grandparent->right;

            /* Case : 1 
               The uncle of node is also red 
               Only Recoloring required */
            if (getColor(uncle) == RED) {
                setColor(uncle, BLACK);
                setColor(parent, BLACK);
                setColor(grandparent, RED);
                node = grandparent;
            }
            else {
                /* Case : 2 
                   node is right child of its parent 
                   Left-rotation required */            
                if (node == parent->right) {
                    rotateLeft(parent);
                    node = parent;
                    parent = node->parent;
                }

                /* Case : 3 
                   node is left child of its parent 
                   Right-rotation required */
                rotateRight(grandparent);
                std::swap(parent->color, grandparent->color);
                node = parent;
            }
        }

        /* Case : B 
           Parent of node is right child  
           of Grand-parent of node */
        else {
            Node<T>* uncle = grandparent->left;

            /*  Case : 1 
                The uncle of node is also red 
                Only Recoloring required */
            if (getColor(uncle) == RED) {
                setColor(uncle, BLACK);
                setColor(parent, BLACK);
                setColor(grandparent, RED);
                node = grandparent;
            }
            else {
                /* Case : 2 
                   node is left child of its parent 
                   Right-rotation required */
                if (node == parent->left) {
                    rotateRight(parent);
                    node = parent;
                    parent = node->parent;
                }

                /* Case : 3 
                   node is right child of its parent 
                   Left-rotation required */
                rotateLeft(grandparent);
                std::swap(parent->color, grandparent->color);
                node = parent;
            }
        }
    }
    setColor(root, BLACK);
}

template <typename T>
void RBTree<T>::fixDeleteRBTree(Node<T>*& node) {
    // When we perform standard delete operation in BST, we always end up deleting a node which is either leaf or has only one child (For an internal node,
    // we copy the successor and then recursively call delete for successor, successor is always a leaf node or a node with one child).
    // So we only need to handle cases where a node is leaf or has one child.
    if (!node)
        return;

    if (node == root) {
        delete node;
        root = nullptr;
        --sz;
        return;
    }

    // Simple case : node or one of its children is red (they cannot both be red because that would violate RBTree properties)
    if (getColor(node) == RED || getColor(node->left) == RED || getColor(node->right) == RED) {
        Node<T>* child = node->left != nullptr ? node->left : node->right;

        // We replace the child and mark it as black - parent cannot be nullptr because then node would be root and root has black colour
        if (node == node->parent->left)
            node->parent->left = child;
        else
            node->parent->right = child;

        if (child)
            child->parent = node->parent;
        setColor(child, BLACK);
        delete node;
        --sz;
    } 
    else {  // If node and its children are black (Remember: NULL is black)
        Node<T>* sibling {};
        Node<T>* parent {};
        Node<T>* node = node;
        setColor(node, DOUBLE_BLACK);
        while (node != root && getColor(node) == DOUBLE_BLACK) {
            parent = node->parent;
            if (node == parent->left) {  // If node is left child
                sibling = parent->right;
                // If node is black and sibling is red
                if (getColor(sibling) == RED) {
                    setColor(sibling, BLACK);
                    setColor(parent, RED);
                    rotateLeft(parent);
                }
                else {  // If both node and sibling are black
                    if (getColor(sibling->left) == BLACK && getColor(sibling->right) == BLACK) { // If both sibling's children are black
                        setColor(sibling, RED);
                        if(getColor(parent) == RED)
                            setColor(parent, BLACK);
                        else
                            setColor(parent, DOUBLE_BLACK);
                        node = parent;
                    }
                    else {
                        if (getColor(sibling->right) == BLACK) { // If sibling's right child is black and left is red
                            setColor(sibling->left, BLACK);
                            setColor(sibling, RED);
                            rotateRight(sibling);
                            sibling = parent->right;
                        }

                        // If sibling's right child is red and left is black
                        setColor(sibling, parent->color);
                        setColor(parent, BLACK);
                        setColor(sibling->right, BLACK);
                        rotateLeft(parent);
                        break;
                    }
                }
            }
            else {  // Similar process as above only this time node is right child (symmetrical cases)
                sibling = parent->left;
                // If node is black and sibling is red
                if (getColor(sibling) == RED) {
                    setColor(sibling, BLACK);
                    setColor(parent, RED);
                    rotateRight(parent);
                }
                else {  // If both node and sibling are black
                    if (getColor(sibling->left) == BLACK && getColor(sibling->right) == BLACK) { // If both sibling's children are black
                        setColor(sibling, RED);
                        if (getColor(parent) == RED)
                            setColor(parent, BLACK);
                        else
                            setColor(parent, DOUBLE_BLACK);
                        node = parent;
                    }
                    else {
                        if (getColor(sibling->left) == BLACK) { // If sibling's left child is black and right is red
                            setColor(sibling->right, BLACK);
                            setColor(sibling, RED);
                            rotateLeft(sibling);
                            sibling = parent->left;
                        }

                        // If sibling's right child is black and left is red
                        setColor(sibling, parent->color);
                        setColor(parent, BLACK);
                        setColor(sibling->left, BLACK);
                        rotateRight(parent);
                        break;
                    }
                }
            }
        }
        if (node == node->parent->left)
            node->parent->left = nullptr;
        else
            node->parent->right = nullptr;

        delete node;
        --sz;
        setColor(root, BLACK);
    }
}

template <typename T>
Node<T>* RBTree<T>::deleteBST(Node<T>*& root, T data) {
    // Simple BST deletion
    if (!root)
        return root;

    if (data < root->data)
        return deleteBST(root->left, data);

    if (data > root->data)
        return deleteBST(root->right, data);

    if (!root->left || !root->right)
        return root;

    Node<T>* temp = minValueNode(root->right);
    root->data = temp->data;
    return deleteBST(root->right, temp->data);
}

template <typename T>
void RBTree<T>::remove(T key) {
    // Calling helping function and restore RBTree properties
    Node<T>* node = deleteBST(root, key);
    fixDeleteRBTree(node);
}

template <typename T>
bool RBTree<T>::search(T key) {
    Node<T>* flag = searchBST(root, key);
    return flag != nullptr ? true : false;
}

template <typename T>
Node<T>* RBTree<T>::searchBST(Node<T>* node, T key) {
    // Simple BST search
    if (!node)
        return nullptr;

    if (key == node->data) 
        return node;
    else if (key > node->data)
        return searchBST(node->right, key);
    else 
        return searchBST(node->left, key);
}

template <typename T>
void RBTree<T>::inorderBST(Node<T>*& ptr) {
    // Simple inorder traversal
    if (!ptr) return;
    inorderBST(ptr->left);
    std::cout << ptr->data << ' ';
    inorderBST(ptr->right);
}

template <typename T>
void RBTree<T>::inorder() {
    inorderBST(root);
}

template <typename T>
void RBTree<T>::preorderBST(Node<T>*& ptr) {
    // Simple preorder traversal
    if (!ptr) return;
    std::cout << ptr->data << ' ';
    preorderBST(ptr->left);
    preorderBST(ptr->right);
}

template <typename T>
void RBTree<T>::preorder() {
    preorderBST(root);
}

template <typename T>
Node<T>* RBTree<T>::minValueNode(Node<T>*& node) {
    // Most left node
    Node<T>* ptr = node;
    while (ptr->left)
        ptr = ptr->left;
    return ptr;
}

template <typename T>
Node<T>* RBTree<T>::maxValueNode(Node<T>*& node) {
    // Most right node
    Node<T>* ptr = node;
    while (ptr->right)
        ptr = ptr->right;
    return ptr;
}

#endif //RED_BLACK_TREE_RBTREE_H