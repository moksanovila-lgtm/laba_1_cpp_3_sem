#pragma once

#include <iostream>
#include <string>
#include <sstream>
#include <functional>
#include <stdexcept>
#include <utility>

template <typename T>
class TreeNode {
public:
    T value;
    TreeNode* left;
    TreeNode* right;

    TreeNode(const T& val) : value(val), left(nullptr), right(nullptr) {}
};

template <typename T>
class BinaryTree{
private:
    TreeNode<T>* root;
    std::size_t size;

    void DestroyTree(TreeNode<T>* node);
    TreeNode<T>* CopyTree(const TreeNode<T>* node);
    TreeNode<T>* InsertNode(TreeNode<T>* node, const T& value);
    TreeNode<T>* FindNode(TreeNode<T>* node, const T& value) const;
    TreeNode<T>* EraseNode(TreeNode<T>* node, const T& value);
    TreeNode<T>* MinNode(TreeNode<T>* node) const;
    bool AreIdentical(TreeNode<T>* a, TreeNode<T>* b) const;

public:
    BinaryTree() : root(nullptr), size(0) {}
    
    BinaryTree(const BinaryTree& other);
    
    BinaryTree& operator=(const BinaryTree& other);
    
    BinaryTree(BinaryTree&& other) noexcept;
    
    BinaryTree& operator=(BinaryTree&& other) noexcept;
    
    ~BinaryTree();

    void Insert(const T& value);
    bool Find(const T& value) const;
    void Erase(const T& value);
    void Clear() noexcept;

    BinaryTree<T> ExtractSubtree(const T& value) const;
    bool ContainsSubtree(const BinaryTree<T>& other) const;

    std::size_t Size() const noexcept {return size;}
    bool IsEmpty() const noexcept {return size == 0;}
    
    template <typename Func> void TraverseKLP(Func visit) const;
    template <typename Func> void TraverseKPL(Func visit) const;
    template <typename Func> void TraverseLKP(Func visit) const;
    template <typename Func> void TraverseLPK(Func visit) const;
    template <typename Func> void TraversePLK(Func visit) const;
    template <typename Func> void TraversePKL(Func visit) const;

    BinaryTree<T> Map(std::function<T(const T&)> f) const;
    BinaryTree<T> Where(std::function<bool(const T&)> pred) const;
    T Reduce(std::function<T(const T&, const T&)> f, T init) const;

    std::string Serialize() const;
    void Deserialize(const std::string& str);

    TreeNode<T>* Root() const noexcept {return root;}

};



template <typename T>
BinaryTree<T>::BinaryTree(const BinaryTree& other) : root(nullptr), size(0) {
    root = CopyTree(other.root);
    size = other.size;
}

template <typename T>
BinaryTree<T>& BinaryTree<T>::operator=(const BinaryTree& other){
    if(this != &other){
        Clear();
        root = CopyTree(other.root);
        size = other.size;
    }
    return *this;
}

template <typename T>
BinaryTree<T>::~BinaryTree() {
    Clear();
}

template <typename T>
BinaryTree<T>::BinaryTree(BinaryTree&& other) noexcept : root(other.root), size(other.size){
    other.root = nullptr;
    other.size = 0;
}

template <typename T>
BinaryTree<T>& BinaryTree<T>::operator=(BinaryTree&& other) noexcept {
    if(this != &other){
        Clear();
        root = other.root;
        size = other.size;
        other.root = nullptr;
        other.size = 0;
    }
    return *this;
}

template <typename T>
void BinaryTree<T>::DestroyTree(TreeNode<T>* node){
    if(!node) return;
    DestroyTree(node->left);
    DestroyTree(node->right);
    delete node;
}

template <typename T>
void BinaryTree<T>::Clear() noexcept {
    DestroyTree(root);
    root = nullptr;
    size = 0;
}

template <typename T>
TreeNode<T>* BinaryTree<T>::CopyTree(const TreeNode<T>* node){
    if(!node) return nullptr;
    TreeNode<T>* newNode = new TreeNode<T>(node->value);
    newNode->left = CopyTree(node->left);
    newNode->right = CopyTree(node->right);
    return newNode;
}

template <typename T>
void BinaryTree<T>::Insert(const T& value){
    root = InsertNode(root, value);
}

template <typename T>
TreeNode<T>* BinaryTree<T>::InsertNode(TreeNode<T>* node, const T& value){
    if(!node){
        size++;
        return new TreeNode<T>(value);
    }
    if(value < node->value) {
        node->left = InsertNode(node->left, value);
    }
    else if(node->value < value){
        node->right = InsertNode(node->right, value);
    }
    return node;
}

template <typename T>
bool BinaryTree<T>::Find(const T& value) const {
    return FindNode(root, value) != nullptr;
}

template <typename T>
TreeNode<T>* BinaryTree<T>::FindNode(TreeNode<T>* node, const T& value) const {
    if(!node || (!(value < node->value) && !(node->value < value))) return node;
    if(value < node->value) return FindNode(node->left, value);
    return FindNode(node->right, value);
}

template <typename T>
void BinaryTree<T>::Erase(const T& value) {
    if (!Find(value)) {
        throw std::out_of_range("BinaryTree::Erase: value not found");
    }
    root = EraseNode(root, value);
}

template <typename T>
TreeNode<T>* BinaryTree<T>::EraseNode(TreeNode<T>* node, const T& value){
    if(!node) return nullptr;
    if(value < node->value){
        node->left = EraseNode(node->left, value);
    }
    else if(node->value < value){
        node->right = EraseNode(node->right, value);
    }
    else{
        if(!node->left){
            TreeNode<T>* right = node->right;
            delete node;
            size--;
            return right;
        }
        if(!node->right){
            TreeNode<T>* left = node->left;
            delete node;
            size--;
            return left;
        }
        TreeNode<T>* minRight = MinNode(node->right);
        node->value = minRight->value;
        node->right = EraseNode(node->right, minRight->value);
    }
    return node;
}

template <typename T>
TreeNode<T>* BinaryTree<T>::MinNode(TreeNode<T>* node) const {
    while(node && node->left) node = node->left;
    return node;
}

template <typename T>
template <typename Func>
void BinaryTree<T>::TraverseKLP(Func visit) const {
    std::function<void(TreeNode<T>*)> rec = [&](TreeNode<T>* n){
        if(!n) return;
        visit(n->value);
        rec(n->left);
        rec(n->right);
    };
    rec(root);
}

template <typename T>
template <typename Func>
void BinaryTree<T>::TraverseKPL(Func visit) const {
    std::function<void(TreeNode<T>*)> rec = [&](TreeNode<T>* n) {
        if (!n) return;
        visit(n->value);
        rec(n->right);
        rec(n->left);
    };
    rec(root);
}

template <typename T>
template <typename Func>
void BinaryTree<T>::TraverseLKP(Func visit) const {
    std::function<void(TreeNode<T>*)> rec = [&](TreeNode<T>* n) {
        if (!n) return;
        rec(n->left);
        visit(n->value);
        rec(n->right);
    };
    rec(root);
}

template <typename T>
template <typename Func>
void BinaryTree<T>::TraverseLPK(Func visit) const {
    std::function<void(TreeNode<T>*)> rec = [&](TreeNode<T>* n) {
        if (!n) return;
        rec(n->left);
        rec(n->right);
        visit(n->value);
    };
    rec(root);
}

template <typename T>
template <typename Func>
void BinaryTree<T>::TraversePLK(Func visit) const {
    std::function<void(TreeNode<T>*)> rec = [&](TreeNode<T>* n) {
        if (!n) return;
        rec(n->right);
        rec(n->left);
        visit(n->value);
    };
    rec(root);
}

template <typename T>
template <typename Func>
void BinaryTree<T>::TraversePKL(Func visit) const {
    std::function<void(TreeNode<T>*)> rec = [&](TreeNode<T>* n) {
        if (!n) return;
        rec(n->right);
        visit(n->value);
        rec(n->left);
    };
    rec(root);
}

template <typename T>
BinaryTree<T> BinaryTree<T>::Map(std::function<T(const T&)> f) const {
    if (!f) {
        throw std::invalid_argument("BinaryTree::Map: function is empty");
    }
    BinaryTree<T> result;
    TraverseKLP([&](const T& val) { result.Insert(f(val)); });
    return result;
}

template <typename T>
BinaryTree<T> BinaryTree<T>::Where(std::function<bool(const T&)> pred) const {
    if (!pred) {
        throw std::invalid_argument("BinaryTree::Where: predicate is empty");
    }
    BinaryTree<T> result;
    TraverseKLP([&](const T& val) { if (pred(val)) result.Insert(val); });
    return result;
}

template <typename T>
T BinaryTree<T>::Reduce(std::function<T(const T&, const T&)> f, T init) const {
    if (!f) {
        throw std::invalid_argument("BinaryTree::Reduce: function is empty");
    }
    if (IsEmpty()) {
        throw std::logic_error("BinaryTree::Reduce: tree is empty");
    }
    T acc = init;
    TraverseKLP([&](const T& val) { acc = f(acc, val); });
    return acc;
}


template <typename T>
std::string BinaryTree<T>::Serialize() const {
    std::ostringstream oss;
    TraverseKLP([&](const T& val) { oss << val << " "; });
    return oss.str();
}

template <typename T>
void BinaryTree<T>::Deserialize(const std::string& str) {
    if (str.empty()) {
        throw std::invalid_argument("BinaryTree::Deserialize: empty string");
    }
    Clear();
    std::istringstream iss(str);
    T val;
    int count = 0;
    while (iss >> val) {
        Insert(val);
        count++;
    }
    if (count == 0) {
        throw std::runtime_error("BinaryTree::Deserialize: no values parsed");
    }
}

template <typename T>
BinaryTree<T> BinaryTree<T>::ExtractSubtree(const T& value) const {
    TreeNode<T>* node = FindNode(root, value);
    if (!node) {
        throw std::out_of_range("BinaryTree::ExtractSubtree: value not found");
    }
    BinaryTree<T> result;
    result.root = result.CopyTree(node);
    result.size = 0;
    result.TraverseKLP([&](const T&) {result.size++;});
    return result;   
}

template <typename T>
bool BinaryTree<T>::AreIdentical(TreeNode<T>* a, TreeNode<T>* b) const{
    if(!a && !b) return true;
    if(!a || !b) return false;
    if(a->value < b->value || b->value < a->value) return false;
    return AreIdentical(a->left, b->left) && AreIdentical(a->right, b->right);
}

template <typename T>
bool BinaryTree<T>::ContainsSubtree(const BinaryTree<T>& other) const {
    if(other.IsEmpty()) return true;
    bool found = false;
    TraverseKLP([&](const T& val){
        if(found) return;
        TreeNode<T>* node = FindNode(root, val);
        if(node && AreIdentical(node, other.root)){
            found = true;
        }
    });
    return found;
}


