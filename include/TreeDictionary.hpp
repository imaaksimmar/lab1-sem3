#pragma once
#include <stdexcept>
#include <cstddef>
#include <functional>

template <typename TKey, typename TElement>
class Node {
public:
    TKey key;
    TElement value;
    Node<TKey, TElement>* left;
    Node<TKey, TElement>* right;

    Node(TKey k, TElement val) : key(k), value(val), left(nullptr), right(nullptr) {} 
};

template <typename TKey, typename TElement>
class TreeDictionary {
private:
    Node<TKey, TElement>* root;

    void ClearRec(Node<TKey, TElement>* node) {
        if(node==nullptr) { return; }
        ClearRec(node->left);
        ClearRec(node->right);
        delete node;
    }

    Node<TKey, TElement>* AddRec(Node<TKey, TElement>* node, TKey key, TElement element) {
        if(node == nullptr) { return new Node<TKey, TElement>(key, element); }
        if(key < node->key) { node->left = AddRec(node->left, key, element); }
        else if(key > node->key) { node->right = AddRec(node->right, key, element); }
        else { node->value = element; }
        return node;
    }

    Node<TKey, TElement>* RemoveRec(Node<TKey, TElement>* node, TKey key) {
        if(node==nullptr) { return nullptr; }
        if(key<node->key) { node->left = RemoveRec(node->left, key); }
        else if(key>node->key) { node->right = RemoveRec(node->right, key); }
        else { 
            if(node->left==nullptr) {
                Node<TKey, TElement>* temp = node->right;
                delete node;
                return temp;
            }
            else if(node->right==nullptr) {
                Node<TKey, TElement>* temp = node->left;
                delete node;
                return temp;
            }
            Node<TKey, TElement>* replacement = GetMinNode(node->right);
            node->key = replacement->key; 
            node->value = replacement->value;

            node->right = RemoveRec(node->right, replacement->key);
        }
        return node;
    }

    Node<TKey, TElement>* GetMinNode(Node<TKey, TElement>* node) {
        if(node==nullptr) { return nullptr; }
        while(node->left!=nullptr) {
            node = node->left;
        }
        return node;
    }

    bool ContainsRec(Node<TKey, TElement>* node, TKey key) const {
        if(node==nullptr) { return false; }
        if(key<node->key) { return ContainsRec(node->left, key); }
        else if(key>node->key) { return ContainsRec(node->right, key); }
        else { return true; }
    }

    size_t GetCountRec(Node<TKey, TElement>* node) const {
        if(node==nullptr) { return 0; }
        size_t leftCount = GetCountRec(node->left);
        size_t rightCount = GetCountRec(node->right);
        return leftCount + rightCount + 1;
    }

    TElement GetRec(Node<TKey, TElement>* node, TKey key) const {
        if(node==nullptr) { throw std::out_of_range("Key is not found in the dictionary"); }
        if(key<node->key) { return GetRec(node->left, key); }
        else if(key>node->key) { return GetRec(node->right, key); }
        else { return node->value; }
    }

    Node<TKey, TElement>* FindNodeRec(Node<TKey, TElement>* node, TKey key) const {
        if(node==nullptr) { return nullptr; }
        if(key<node->key) { return FindNodeRec(node->left, key); }
        else if(key>node->key) { return FindNodeRec(node->right, key); }
        else { return node; }
    }

    // map-where-reduce
    template <typename UElement>
    void MapRec(Node<TKey, TElement>* node, const std::function<UElement(TElement)>& func, TreeDictionary<TKey, UElement>& newTree ) const {
        if(node == nullptr) { return; }
        UElement elem = func(node->value);
        newTree.Add(node->key, elem);

        MapRec(node->left, func, newTree);
        MapRec(node->right, func, newTree);
    }

    void WhereRec(Node<TKey, TElement>* node, const std::function<bool(TElement)>& predicat, TreeDictionary<TKey, TElement>& newTree) const {
        if(node == nullptr) { return; }
        bool res = predicat(node->value);
        if(res == true) { newTree.Add(node->key, node->value); }

        WhereRec(node->left, predicat, newTree);
        WhereRec(node->right, predicat, newTree);
    }

    template <typename UElement>
    void ReduceRec(Node<TKey, TElement>* node, const std::function<UElement(TElement, UElement)>& func, UElement& accumulator) const {
        if(node == nullptr) { return; }

        ReduceRec(node->left, func, accumulator);       
        accumulator = func(node->value, accumulator);   
        ReduceRec(node->right, func, accumulator);    
    }

    // извлечение поддерева (по заданному корню)
    void CopyRec(Node<TKey, TElement>* node, TreeDictionary<TKey, TElement>& newTree) const {
        if(node == nullptr) { return; }

        newTree.Add(node->key, node->value);
        CopyRec(node->left, newTree);
        CopyRec(node->right, newTree);
    }

    // поиск на вхождение поддерева 
    bool SameTreeRec(const Node<TKey, TElement>* a, const Node<TKey, TElement>* b) const {
        if(a==nullptr && b==nullptr) { return true; }
        if(a==nullptr || b==nullptr) { return false; }

        if(a->key != b->key) { return false; }
        if(a->value != b->value) { return false; }

        return SameTreeRec(a->left, b->left) && SameTreeRec(a->right, b->right);
    }

public:
    TreeDictionary() : root(nullptr) {}
    ~TreeDictionary()  { ClearRec(root); }

    void Add(TKey key, TElement element)  {
        root = AddRec(root, key, element);
    }

    void Remove(TKey key)  {
        if(ContainsRec(root, key) == false) { throw std::out_of_range("Key is not found"); }
        root = RemoveRec(root, key);
    }

    bool ContainsKey(TKey key) const {
        return ContainsRec(root, key);
    }

    size_t GetCount() const {
        return GetCountRec(root);
    } 

    TElement Get(TKey key) const {
        return GetRec(root, key);
    }

    TElement* Find(TKey key) {
        Node<TKey, TElement>* node = FindNodeRec(root, key);
        if(node==nullptr) { return nullptr; }
        else { return &node->value; }
    }

    // map-where-reduce
    template <typename UElement>
    TreeDictionary<TKey, UElement> Map(const std::function<UElement(TElement)>& func) const {
        TreeDictionary<TKey, UElement> newTree;
        MapRec(root, func, newTree);
        return newTree;
    }

    TreeDictionary<TKey, TElement> Where(const std::function<bool(TElement)>& predicat) const {
        TreeDictionary<TKey, TElement> newTree;
        WhereRec(root, predicat, newTree);
        return newTree;
    }

    template <typename UElement>
    UElement Reduce(const std::function<UElement(TElement, UElement)>& func, UElement startValue) const {
        UElement acc = startValue;
        ReduceRec(root, func, acc);
        return acc;
    }

    // извлечение поддерева (по заданному корню)
    TreeDictionary<TKey, TElement> ExtractSubtree(TKey startKey) const {
        TreeDictionary<TKey, TElement> newTree;

        Node<TKey, TElement>* startNode = FindNodeRec(root, startKey);
        if(startNode == nullptr) { throw std::invalid_argument("Node is not found"); }

        CopyRec(startNode, newTree);
        return newTree;
    }

    // поиск на вхождение поддерева 
    bool ContainsSubtree(const TreeDictionary<TKey, TElement>& subTree) const {
        if(subTree.root == nullptr) { return true; }

        const Node<TKey, TElement>* node = root;
        while(node != nullptr) {
            if(subTree.root->key < node->key) { node = node->left; }
            else if(subTree.root->key > node->key) { node = node->right; }
            else { return SameTreeRec(node, subTree.root); }
        }
        return false;
    }
    
    const Node<TKey, TElement>* GetRoot() const { return root; }
};