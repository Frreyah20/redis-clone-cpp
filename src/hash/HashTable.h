#pragma once

#include <vector>
#include <functional>
#include <iostream>

template<typename K, typename V>
class HashTable
{
private:
    struct Node
    {
        K key;
        V value;
        Node* next;
        Node(const K& k,const V& v): key(k), value(v), next(nullptr){}
    };
    std::vector<Node*> buckets_;
    size_t size_;
    size_t bucket_count_;
    size_t hash(const K& key) const;
    void rehash(size_t new_bucket_count);
    

public:
    HashTable();
    ~HashTable();
    void insert(const K& key,const V& value);
    bool contains(const K& key) const;
    V* find(const K& key);
    bool erase(const K& key);
    void debugPrint() const;
    size_t size() const;
    double loadFactor() const;
    size_t bucketCount() const;
    std::vector<K> keys() const;
    std::vector<std::pair<K,V>> items() const;
    
};

#include "HashTable.tpp"

