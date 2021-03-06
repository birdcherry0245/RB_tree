#pragma once

#include <cstddef>
#include <memory>
#include <iostream>
#include <set>
#include <exception>
#include <vector>
#include <algorithm>


template<typename T>
class RB_tree {
private:
    enum class Color {
        RED,
        BLACK
    };
    struct Node {
        T key;
        Node* left = nullptr;
        Node* right = nullptr;
        Node* parent = nullptr;
        Color color = Color::BLACK;
    };

public:
    RB_tree() {
        size_ = 0;
        NULL_NODE = new Node{};
        root = NULL_NODE;
    }

    template<typename IteratorType>
    RB_tree(IteratorType first, IteratorType last) {
        std::vector<T> elems(first, last);
        bool is_sorted = true;
        for (size_t i = 1; i < elems.size(); ++i) {
            if (!(elems[i - 1] < elems[i])) {
                is_sorted = false;
                break;
            }
        }
        if (is_sorted) {
            BuildTreeFromSorted(elems);
        } else {
            size_ = 0;
            NULL_NODE = new Node{};
            root = NULL_NODE;
            for (const T& elem : elems) {
                insert(elem);
            }
        }
    }

    explicit RB_tree(std::initializer_list<T> init_list) {
        std::vector<T> elems(init_list);
        bool is_sorted = true;
        for (size_t i = 1; i < elems.size(); ++i) {
            if (!(elems[i - 1] < elems[i])) {
                is_sorted = false;
                break;
            }
        }
        if (is_sorted) {
            BuildTreeFromSorted(elems);
        } else {
            size_ = 0;
            NULL_NODE = new Node{};
            root = NULL_NODE;
            for (const T& elem : elems) {
                insert(elem);
            }
        }
    }

    RB_tree(const RB_tree& other): size_(other.size_) {
        NULL_NODE = new Node;
        root = CopySubtree(other.root, other.NULL_NODE, nullptr);
    }
    RB_tree(RB_tree&& other): size_(other.size_) {
        NULL_NODE = new Node;
        root = CopySubtree(other.root, other.NULL_NODE, nullptr);
    }

    RB_tree& operator = (const RB_tree& other) {
        if (root != other.root) {
            size_ = other.size_;
            DeleteSubtree(root);
            root = CopySubtree(other.root, other.NULL_NODE, nullptr);
        }
        return *this;
    }
    RB_tree& operator = (RB_tree&& other) {
        if (root != other.root) {
            size_ = other.size_;
            DeleteSubtree(root);
            root = CopySubtree(other.root, other.NULL_NODE, nullptr);
        }
        return *this;
    }

    ~RB_tree() {
        DeleteSubtree(root);
        delete NULL_NODE;
    }

    class Iterator {
        friend RB_tree;

    private:
        const RB_tree* rb_tree_ptr;
        Node* node;
    public:
        Iterator() = default;

        Iterator(const RB_tree* rb_tree_ptr, Node* node): rb_tree_ptr(rb_tree_ptr), node(node) {
        }

        bool operator == (const Iterator& other) const {
            return node == other.node;
        }
        bool operator != (const Iterator& other) const {
            return node != other.node;
        }

        Iterator& operator ++ () {
            node = rb_tree_ptr->next(node);
            return *this;
        }
        Iterator& operator -- () {
            node = rb_tree_ptr->prev(node);
            return *this;
        }

        Iterator operator ++ (int) {
            auto tmp(*this);
            node = rb_tree_ptr->next(node);
            return tmp;
        }
        Iterator operator -- (int) {
            auto tmp(*this);
            node = rb_tree_ptr->prev(node);
            return tmp;
        }

        const T& operator * () const {
            return node->key;
        }

        T* operator -> () const {
            return &(node->key);
        }
    };

private:
    Node* root;
    Node* NULL_NODE;
    size_t size_;

    void BuildTreeFromSorted(const std::vector<T>& sorted_elems) {
        size_ = sorted_elems.size();
        NULL_NODE = new Node{};
        root = Build(sorted_elems.begin(), sorted_elems.end(), nullptr);
        ColorBalancedTree(root, GetHeight(root), 1);
    }

    template<typename IteratorType>
    Node* Build(IteratorType first, IteratorType last, Node* parent) {
        if (first == last) {
            return NULL_NODE;
        }
        IteratorType middle = first + (last - first) / 2;
        Node* node = new Node{.key = *middle, .parent = parent};
        node->left = Build(first, middle, node);
        node->right = Build(middle + 1, last, node);
        return node;
    }

    size_t GetHeight(Node* node) const {
        if (node == NULL_NODE) {
            return 0;
        }
        return std::max(GetHeight(node->left), GetHeight(node->right)) + 1;
    }

    void ColorBalancedTree(Node* node, size_t tree_height, size_t current_depth) {
        if (node == NULL_NODE) {
            return;
        }
        if (current_depth == tree_height && node != root) {
            node->color = Color::RED;
        }
        ColorBalancedTree(node->left, tree_height, current_depth + 1);
        ColorBalancedTree(node->right, tree_height, current_depth + 1);
    }

    Node* CopySubtree(Node* other_node, Node* other_NULL_NODE, Node* parent) {
        if (other_node == other_NULL_NODE) {
            return NULL_NODE;
        }
        Node* node = new Node{.key = other_node->key, .parent = parent, .color = other_node->color};
        node->left = CopySubtree(other_node->left, other_NULL_NODE, node);
        node->right = CopySubtree(other_node->right, other_NULL_NODE, node);
        return node;
    }

    void DeleteSubtree(Node* v) {
        if (v == NULL_NODE) {
            return;
        }
        DeleteSubtree(v->left);
        DeleteSubtree(v->right);
        delete v;
    }

    void LeftRotation(Node* node) {
        if (node == NULL_NODE) {
            return;
        }
        Node* right = node->right;
        if (right == NULL_NODE) {
            return;
        }

        if (node->parent == nullptr) {
            root = right;
        } else if (IsLeftSon(node)) {
            node->parent->left = right;
        } else {
            node->parent->right = right;
        }

        right->parent = node->parent;
        node->right = right->left;
        if (node->right != NULL_NODE) {
            node->right->parent = node;
        }
        node->parent = right;
        right->left = node;
    }
    void RightRotation(Node* node) {
        if (node == NULL_NODE) {
            return;
        }
        Node* left = node->left;
        if (left == NULL_NODE) {
            return;
        }

        if (node->parent == nullptr) {
            root = left;
        } else if (IsLeftSon(node)) {
            node->parent->left = left;
        } else {
            node->parent->right = left;
        }
        left->parent = node->parent;
        node->left = left->right;
        if (node->left != NULL_NODE) {
            node->left->parent = node;
        }
        node->parent = left;
        left->right = node;
    }


    bool IsLeftSon(Node* node) const {
        return node->parent->left == node;
    }
    bool IsRightSon(Node* node) const {
        return node->parent->right == node;
    }
    Node* GetBrother(Node* node) const {
        if (IsLeftSon(node)) {
            return node->parent->right;
        }
        return node->parent->left;
    }
    Node* GetUncle(Node* node) const {
        return GetBrother(node->parent);
    }

    Node* Leftmost(Node* node) const {
        if (node == NULL_NODE) {
            return NULL_NODE;
        }
        while (node->left != NULL_NODE) {
            node = node->left;
        }
        return node;
    }
    Node* Rightmost(Node* node) const {
        if (node == NULL_NODE) {
            return NULL_NODE;
        }
        while (node->right != NULL_NODE) {
            node = node->right;
        }
        return node;
    }

    Node* next(Node* node) const {
        if (node == NULL_NODE) {
            return NULL_NODE;
        }

        if (node->right != NULL_NODE) {
            return Leftmost(node->right);
        }

        while (node != root && IsRightSon(node)) {
            node = node->parent;
        }
        return (node == root ? NULL_NODE : node->parent);
    }
    Node* prev(Node* node) const {
        if (node == NULL_NODE) {
            return Rightmost(root);
        }

        if (node->left != NULL_NODE) {
            return Rightmost(node->left);
        }

        while (node != root && IsLeftSon(node)) {
            node = node->parent;
        }
        return (node == root ? NULL_NODE : node->parent);
    }

    void FixInsert(Node* node) {
        while (node != root && node->parent->color == Color::RED) {
            Node* uncle = GetUncle(node);
            if (uncle->color == Color::RED) {
                node->parent->parent->color = Color::RED;
                node->parent->color = Color::BLACK;
                uncle->color = Color::BLACK;
                node = node->parent->parent;
            } else {
                if (IsRightSon(node->parent)) {
                    if (IsLeftSon(node)) {
                        node = node->parent;
                        RightRotation(node);
                    }
                    node->parent->color = Color::BLACK;
                    node->parent->parent->color = Color::RED;
                    LeftRotation(node->parent->parent);
                } else {
                    if (IsRightSon(node)) {
                        node = node->parent;
                        LeftRotation(node);
                    }
                    node->parent->color = Color::BLACK;
                    node->parent->parent->color = Color::RED;
                    RightRotation(node->parent->parent);
                }
                break;
            }
        }
        root->color = Color::BLACK;
    }

    void FixErase(Node* node) {
        while (node != root && node->color == Color::BLACK) {
            if (IsLeftSon(node)) {
                Node* brother = node->parent->right;
                if (brother->color == Color::RED) {
                    node->parent->color = Color::RED;
                    brother->color = Color::BLACK;
                    LeftRotation(node->parent);
                    brother = node->parent->right;
                }
                if (brother->left->color == Color::BLACK && brother->right->color == Color::BLACK) { // ??????????????
                    brother->color = Color::RED;
                    node = node->parent;
                } else {
                    if (brother->right->color == Color::BLACK) {
                        brother->left->color = Color::RED;
                        brother->color = Color::RED;
                        RightRotation(brother);
                        brother = node->parent->right;
                    }
                    brother->color = node->parent->color;
                    node->parent->color = Color::BLACK;
                    brother->right->color = Color::BLACK;
                    LeftRotation(node->parent);
                    break;
                }
            } else {
                Node* brother = node->parent->left;
                if (brother->color == Color::RED) {
                    node->parent->color = Color::RED;
                    brother->color = Color::BLACK;
                    RightRotation(node->parent);
                    brother = node->parent->left;
                }
                if (brother->right->color == Color::BLACK && brother->left->color == Color::BLACK) { // ??????????????
                    brother->color = Color::RED;
                    node = node->parent;
                } else {
                    if (brother->left->color == Color::BLACK) {
                        brother->right->color = Color::RED;
                        brother->color = Color::RED;
                        LeftRotation(brother);
                        brother = node->parent->left;
                    }
                    brother->color = node->parent->color;
                    node->parent->color = Color::BLACK;
                    brother->left->color = Color::BLACK;
                    RightRotation(node->parent);
                    break;
                }
            }
        }
        node->color = Color::BLACK;
        root->color = Color::BLACK;
    }

    void erase(Node* node) {
        if (node == NULL_NODE) {
            return;
        }

        size_ -= 1;

        if (node->left != NULL_NODE && node->right != NULL_NODE) {
            Node* nxt = next(node);
            node->key = nxt->key;
            node = nxt;
        }

        Node* to_fix = (node->left == NULL_NODE ? node->right : node->left); // can be NULL_NODE

        if (node->parent == nullptr) {
            root = to_fix;
        } else {
            if (IsLeftSon(node)) {
                node->parent->left = to_fix;
            } else {
                node->parent->right = to_fix;
            }
        }
        to_fix->parent = node->parent;

        if (node->color == Color::BLACK) {
            FixErase(to_fix);
        }
        delete node;
    }

    Node* FindNode(const T& key) const {
        Node *node = root;
        while (node != NULL_NODE) {
            if (key < node->key) {
                node = node->left;
            } else if (node->key < key) {
                node = node->right;
            } else {
                return node;
            }
        }
        return NULL_NODE;
    }
    Node* GetLowerBoundNode(Node* node, const T& key) const {
        if (node == NULL_NODE) {
            return NULL_NODE;
        }
        if (key < node->key) {
            Node* left_result = GetLowerBoundNode(node->left, key);
            if (left_result == NULL_NODE) {
                return node;
            }
            return left_result;
        }
        if (node->key < key) {
            return GetLowerBoundNode(node->right, key);
        }
        return node;
    }

public:
    size_t size() const {
        return size_;
    }

    bool empty() const {
        return size_ == 0;
    }

    Iterator begin() const {
        return Iterator(this, Leftmost(root));
    }
    Iterator end() const {
        return Iterator(this, NULL_NODE);
    }

    Iterator find(const T& key) const {
        return Iterator(this, FindNode(key));
    }

    void insert(const T& key) {
        Node* parent = nullptr;
        Node* current = root;

        while (current != NULL_NODE) {
            parent = current;
            if (key < current->key) {
                current = current->left;
            } else if (current->key < key) {
                current = current->right;
            } else {
                return;
            }
        }

        size_ += 1;

        if (parent == nullptr) {
            root = new Node{key, NULL_NODE, NULL_NODE, nullptr, Color::BLACK};
        } else {
            Node* new_node = new Node{key, NULL_NODE, NULL_NODE, parent, Color::RED};
            if (key < parent->key) {
                parent->left = new_node;
            } else {
                parent->right = new_node;
            }
            FixInsert(new_node);
        }
    }

    void erase(const T& key) {
        erase(FindNode(key));
    }

    Iterator lower_bound(const T& key) const {
        return Iterator(this, GetLowerBoundNode(root, key));
    }

    void InOrder(Node* node, std::vector<T>& out) const {
        if (node == NULL_NODE) {
            return;
        }
        InOrder(node->left, out);
        out.push_back(node->key);
        InOrder(node->right, out);
    }
    std::vector<T> InOrder() const {
        std::vector<T> out;
        InOrder(root, out);
        return out;
    }

    void InOrderPrint(Node* node) const {
        if (node == NULL_NODE) {
            return;
        }
        InOrderPrint(node->left);
        std::cout << node->key << ' ';
        InOrderPrint(node->right);
    }
    void InOrderPrint() const {
        std::cout << "InOrder: \t";
        InOrderPrint(root);
        std::cout << '\n';
    }

    void PreOrderPrint(Node* node) const {
        if (node == NULL_NODE) {
            return;
        }
        std::cout << node->key << ' ';
        PreOrderPrint(node->left);
        PreOrderPrint(node->right);
    }
    void PreOrderPrint() const {
        std::cout << "PreOrder: \t";
        PreOrderPrint(root);
        std::cout << '\n';
    }

    void CheckHeight(Node* node, std::set<int>& hbs, int hb = 0) const {
        if (node == NULL_NODE) {
            hbs.insert(hb);
            return;
        }
        hb += int(node->color == Color::BLACK);
        CheckHeight(node->left, hbs, hb);
        CheckHeight(node->right, hbs, hb);
    }
    void CheckHeight(const std::string& message) const {
        std::set<int> hbs;
        CheckHeight(root, hbs);
        if (hbs.size() != 1) {
            throw std::runtime_error(message);
        }
    }

    bool CheckColors(Node* node) const {
        if (node == NULL_NODE) {
            return true;
        }
        if (node->parent != nullptr) {
            if (node->color == Color::RED && node->parent->color == Color::RED) {
                return false;
            }
        }
        return CheckColors(node->left) && CheckColors(node->right);
    }
    void CheckColors(const std::string& message) const {
        if (!CheckColors(root)) {
            throw std::runtime_error(message);
        }
    }

    void CheckRoot(const std::string& message) const {
        if (root->color == Color::RED) {
            throw std::runtime_error(message);
        }
    }

    void Check(const std::string& message = "") const {
        CheckColors(message + "CheckColors()\n");
        CheckHeight(message + "CheckHeight()\n");
        CheckRoot(message + "CheckRoot()\n");
    }

    void fck() const {
        if (root->left != NULL_NODE) {
            if (root->left->color == Color::BLACK) {
                root->left->color = Color::RED;
            }
        }
    }

    std::vector<T> NextOrder() const {
        std::vector<T> out;
        for (Node* x = Leftmost(root); x != NULL_NODE; x = next(x)) {
            out.push_back(x->key);
        }
        return out;
    }
    void NextOrderPrint() const {
        std::cout << "print_next: \t";
        for (Node* x = Leftmost(root); x != NULL_NODE; x = next(x)) {
            std::cout << x->key << ' ';
        }
        std::cout << '\n';
    }

    std::vector<T> PrevOrder() const {
        std::vector<T> out;
        for (Node* x = Rightmost(root); x != NULL_NODE; x = prev(x)) {
            out.push_back(x->key);
        }
        return out;
    }
    void PrevOrderPrint() const {
        std::cout << "print_prev: \t";
        for (Node* x = Rightmost(root); x != NULL_NODE; x = prev(x)) {
            std::cout << x->key << ' ';
        }
        std::cout << '\n';
    }
};


template<class T>
class Set {
public:
    Set() {}

    template<typename Iterator>
    Set(Iterator first, Iterator last) : data_(first, last) {}

    Set(std::initializer_list<T> elems) : data_(elems) {}

    size_t size() const {
        return data_.size();
    }

    bool empty() const {
        return data_.empty();
    }

    void insert(const T& elem) {
        data_.insert(elem);
    }

    void erase(const T& elem) {
        data_.erase(elem);
    }

    typedef typename RB_tree<T>::Iterator iterator;

    iterator begin() const {
        return data_.begin();
    }

    iterator end() const {
        return data_.end();
    }

    iterator find(const T& elem) const {
        return data_.find(elem);
    }

    iterator lower_bound(const T& elem) const {
        return data_.lower_bound(elem);
    }
private:
    RB_tree<T> data_;
};
