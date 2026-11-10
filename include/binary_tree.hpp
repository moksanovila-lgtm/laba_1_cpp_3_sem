#pragma once

#include <concepts>
#include <iostream>
#include <string>
#include <sstream>
#include <functional>
#include <stdexcept>
#include <utility>

template <typename T>
concept Comparable = requires(T a, T b) {
    { a < b } -> std::convertible_to<bool>;
    { b < a } -> std::convertible_to<bool>;
};

template <Comparable Key, typename Value>
class TreeNode {
public:
    Key key;
    Value value;
    TreeNode* left;
    TreeNode* right;

    TreeNode(const Key& k, const Value& v) : key(k), value(v), left(nullptr), right(nullptr) {}
};


template <Comparable Key, typename Value>
class BinaryTree{
private:
    TreeNode<Key, Value>* root;
    std::size_t size;

    void DestroyTree(TreeNode<Key, Value>* node);
    TreeNode<Key, Value>* CopyTree(const TreeNode<Key, Value>* node);
    TreeNode<Key, Value>* InsertNode(TreeNode<Key, Value>* node, const Key& key, const Value& value);
    TreeNode<Key, Value>* FindNode(TreeNode<Key, Value>* node, const Key& key) const;
    TreeNode<Key, Value>* EraseNode(TreeNode<Key, Value>* node, const Key& key);
    TreeNode<Key, Value>* MinNode(TreeNode<Key, Value>* node) const;
    bool AreIdentical(TreeNode<Key, Value>* a, TreeNode<Key, Value>* b) const;

public:
    BinaryTree() : root(nullptr), size(0) {}
    
    BinaryTree(const BinaryTree& other);
    
    BinaryTree& operator=(const BinaryTree& other);
    
    BinaryTree(BinaryTree&& other) noexcept;
    
    BinaryTree& operator=(BinaryTree&& other) noexcept;
    
    ~BinaryTree();

    void Insert(const Key& key, const Value& value);
    void Insert(const Key& key_and_value);
    bool Find(const Key& key) const;
    void Erase(const Key& key);
    void Clear() noexcept;

    BinaryTree<Key, Value> ExtractSubtree(const Key& key) const;
    bool ContainsSubtree(const BinaryTree<Key, Value>& other) const;

    std::size_t Size() const noexcept {return size;}
    bool IsEmpty() const noexcept {return size == 0;}
    
    void Traverse(
        std::function<void(const Key&, const Value&)> visit,
        std::function<void(
            TreeNode<Key, Value>*, 
            std::function<void()>, 
            std::function<void()>, 
            std::function<void()>
        )> strategy
    ) const;
    
    BinaryTree<Key, Value> Map(std::function<std::pair<Key, Value>(const Key&, const Value&)> f) const;
    BinaryTree<Key, Value> Where(std::function<bool(const Key&, const Value&)> pred) const;
    
    template <typename Accumulator>
    Accumulator Reduce(
        std::function<Accumulator(Accumulator, const Value&)> f, 
        Accumulator init
    ) const;

    std::string Serialize(bool only_values = false) const;
    void Deserialize(const std::string& str, bool only_values = false);

    TreeNode<Key, Value>* Root() const noexcept {return root;}

};

namespace TraversalStrategy {
    inline auto KLP = [](auto, auto process, auto left, auto right) { process(); left(); right(); };
    inline auto KPL = [](auto, auto process, auto left, auto right) { process(); right(); left(); };
    inline auto LKP = [](auto, auto process, auto left, auto right) { left(); process(); right(); };
    inline auto LPK = [](auto, auto process, auto left, auto right) { left(); right(); process(); };   
    inline auto PLK = [](auto, auto process, auto left, auto right) { right(); left(); process(); };
    inline auto PKL = [](auto, auto process, auto left, auto right) { right(); process(); left(); };
}


template <Comparable Key, typename Value>
BinaryTree<Key, Value>::BinaryTree(const BinaryTree& other) : root(nullptr), size(0) {
    root = CopyTree(other.root);
    size = other.size;
}

template <Comparable Key, typename Value>
BinaryTree<Key, Value>& BinaryTree<Key, Value>::operator=(const BinaryTree& other){
    if(this != &other){
        Clear();
        root = CopyTree(other.root);
        size = other.size;
    }
    return *this;
}

template <Comparable Key, typename Value>
BinaryTree<Key, Value>::~BinaryTree() {
    Clear();
}

template <Comparable Key, typename Value>
BinaryTree<Key, Value>::BinaryTree(BinaryTree&& other) noexcept : root(other.root), size(other.size){
    other.root = nullptr;
    other.size = 0;
}

template <Comparable Key, typename Value>
BinaryTree<Key, Value>& BinaryTree<Key, Value>::operator=(BinaryTree&& other) noexcept {
    if(this != &other){
        Clear();
        root = other.root;
        size = other.size;
        other.root = nullptr;
        other.size = 0;
    }
    return *this;
}

template <Comparable Key, typename Value>
void BinaryTree<Key, Value>::DestroyTree(TreeNode<Key, Value>* node){
    if(!node) return;
    DestroyTree(node->left);
    DestroyTree(node->right);
    delete node;
}

template <Comparable Key, typename Value>
void BinaryTree<Key, Value>::Clear() noexcept {
    DestroyTree(root);
    root = nullptr;
    size = 0;
}

template <Comparable Key, typename Value>
TreeNode<Key, Value>* BinaryTree<Key, Value>::CopyTree(const TreeNode<Key, Value>* node) {
    if (!node) return nullptr;
    TreeNode<Key, Value>* newNode = new TreeNode<Key, Value>(node->key, node->value);
    newNode->left = CopyTree(node->left);
    newNode->right = CopyTree(node->right);
    return newNode;
}

template <Comparable Key, typename Value>
void BinaryTree<Key, Value>::Insert(const Key& key, const Value& value) {
    root = InsertNode(root, key, value);
}

template <Comparable Key, typename Value>
void BinaryTree<Key, Value>::Insert(const Key& key_and_value) {
    root = InsertNode(root, key_and_value, static_cast<Value>(key_and_value));
}

template <Comparable Key, typename Value>
TreeNode<Key, Value>* BinaryTree<Key, Value>::InsertNode(TreeNode<Key, Value>* node, const Key& key, const Value& value) {
    if (!node) {
        size++;
        return new TreeNode<Key, Value>(key, value);
    }
    if (key < node->key) {
        node->left = InsertNode(node->left, key, value);
    } else if (node->key < key) {
        node->right = InsertNode(node->right, key, value);
    }
    return node;
}

template <Comparable Key, typename Value>
bool BinaryTree<Key, Value>::Find(const Key& key) const {
    return FindNode(root, key) != nullptr;
}

template <Comparable Key, typename Value>
TreeNode<Key, Value>* BinaryTree<Key, Value>::FindNode(TreeNode<Key, Value>* node, const Key& key) const {
    if (!node || (!(key < node->key) && !(node->key < key))) return node;
    if (key < node->key) return FindNode(node->left, key);
    return FindNode(node->right, key);
}

template <Comparable Key, typename Value>
void BinaryTree<Key, Value>::Erase(const Key& key) {
    if (!Find(key)) {
        throw std::out_of_range("BinaryTree::Erase: key not found");
    }
    root = EraseNode(root, key);
}

template <Comparable Key, typename Value>
TreeNode<Key, Value>* BinaryTree<Key, Value>::EraseNode(TreeNode<Key, Value>* node, const Key& key) {
    if (!node) return nullptr;
    if (key < node->key) {
        node->left = EraseNode(node->left, key);
    } 
    else if (node->key < key) {
        node->right = EraseNode(node->right, key);
    } 
    else {
        if (!node->left) {
            TreeNode<Key, Value>* right = node->right;
            delete node; --size; return right;
        }
        if (!node->right) {
            TreeNode<Key, Value>* left = node->left;
            delete node; --size; return left;
        }
        TreeNode<Key, Value>* minRight = MinNode(node->right);
        node->key = minRight->key;
        node->value = minRight->value;
        node->right = EraseNode(node->right, minRight->key);
    }
    return node;
}

template <Comparable Key, typename Value>
TreeNode<Key, Value>* BinaryTree<Key, Value>::MinNode(TreeNode<Key, Value>* node) const {
    while(node && node->left) node = node->left;
    return node;
}

template <Comparable Key, typename Value>
void BinaryTree<Key, Value>::Traverse(
        std::function<void(const Key&, const Value&)> visit,
        std::function<void(
            TreeNode<Key, Value>*, 
            std::function<void()>, 
            std::function<void()>, 
            std::function<void()>
        )> strategy
    ) const {
    std::function<void(TreeNode<Key, Value>*)> rec = [&](TreeNode<Key, Value>* n) {
        if (!n) return;

        std::function<void()> process = [&]() { visit(n->key, n->value); };
        std::function<void()> left    = [&]() { rec(n->left); };
        std::function<void()> right   = [&]() { rec(n->right); };

        strategy(n, process, left, right);
    };
    rec(root);
}

template <Comparable Key, typename Value>
BinaryTree<Key, Value> BinaryTree<Key, Value>::Map(std::function<std::pair<Key, Value>(const Key&, const Value&)> f) const {
    if (!f) throw std::invalid_argument("BinaryTree::Map: function is empty");
    BinaryTree<Key, Value> result;
    Traverse([&](const Key& k, const Value& v) {
        auto [newK, newV] = f(k, v);
        result.Insert(newK, newV);
    }, TraversalStrategy::KLP);
    return result;
}

template <Comparable Key, typename Value>
BinaryTree<Key, Value> BinaryTree<Key, Value>::Where(std::function<bool(const Key&, const Value&)> pred) const {
    if (!pred) throw std::invalid_argument("BinaryTree::Where: predicate is empty");
    BinaryTree<Key, Value> result;
    Traverse([&](const Key& k, const Value& v) {
        if (pred(k, v)) result.Insert(k, v);
    }, TraversalStrategy::KLP);
    return result;
}


template <Comparable Key, typename Value>
template <typename Accumulator>
Accumulator BinaryTree<Key, Value>::Reduce(
    std::function<Accumulator(Accumulator, const Value&)> f, 
    Accumulator init
) const {
    if (IsEmpty()) {
        throw std::logic_error("BinaryTree::Reduce: tree is empty");
    }
    
    Accumulator acc = init;
    Traverse(
        [&](const Key&, const Value& v) {
            acc = f(acc, v);
        },
        TraversalStrategy::KLP 
    );
    return acc;
}

template <Comparable Key, typename Value>
std::string BinaryTree<Key, Value>::Serialize(bool only_values) const {
    std::ostringstream oss;
    Traverse(
        [&](const Key& k, const Value& v) {
            if (only_values) {
                oss << v << " "; 
            } else {
                oss << k << ":" << v << " ";  
            }
        },
        TraversalStrategy::KLP
    );
    return oss.str();
}

template <Comparable Key, typename Value>
void BinaryTree<Key, Value>::Deserialize(const std::string& str, bool only_values) {
    if (str.empty()) throw std::invalid_argument("Пустая строка");
    Clear();
    std::istringstream iss(str);
    if (only_values) {
        Value val;
        int count = 0;
        while (iss >> val) {
            if constexpr (std::is_convertible_v<Value, Key>) {
                Insert(static_cast<Key>(val), val);
            } else {
                throw std::logic_error("Режим only_values не поддерживается для этого типа");
            }
            count++;
        }
        if (count == 0) throw std::runtime_error("Ничего не загружено");
    } else {
        std::string pair;
        int count = 0;
        while (iss >> pair) {
            auto colon = pair.find(':');
            if (colon == std::string::npos) throw std::runtime_error("Нет двоеточия");
            std::istringstream k_iss(pair.substr(0, colon));
            std::istringstream v_iss(pair.substr(colon + 1));
            Key k; Value v;
            if (!(k_iss >> k) || !(v_iss >> v)) throw std::runtime_error("Ошибка парсинга");
            Insert(k, v);
            ++count;
        }
        if (count == 0) throw std::runtime_error("Ничего не загружено");
    }
}

template <Comparable Key, typename Value>
BinaryTree<Key, Value> BinaryTree<Key, Value>::ExtractSubtree(const Key& key) const {
    TreeNode<Key, Value>* node = FindNode(root, key);
    if (!node) throw std::out_of_range("BinaryTree::ExtractSubtree: key not found");
    BinaryTree<Key, Value> result;
    result.root = result.CopyTree(node);
    result.size = 0;
    result.Traverse([&](const Key&, const Value&) { result.size++; }, TraversalStrategy::KLP);
    return result;
}

template <Comparable Key, typename Value>
bool BinaryTree<Key, Value>::AreIdentical(TreeNode<Key, Value>* a, TreeNode<Key, Value>* b) const {
    if (!a && !b) return true;
    if (!a || !b) return false;
    if (a->key < b->key || b->key < a->key) return false;
    return AreIdentical(a->left, b->left) && AreIdentical(a->right, b->right);
}

template <Comparable Key, typename Value>
bool BinaryTree<Key, Value>::ContainsSubtree(const BinaryTree<Key, Value>& other) const {
    if (other.IsEmpty()) return true;
    bool found = false;
    Traverse([&](const Key& k, const Value&) {
        if (found) return;
        TreeNode<Key, Value>* node = FindNode(root, k);
        if (node && AreIdentical(node, other.root)) found = true;
    }, TraversalStrategy::KLP);
    return found;
}