#pragma once

#include <string>
#include <unordered_map>

class LRUCache
{
private:
    struct Node
    {
        std::string key;
        Node* prev;
        Node* next;
        Node(const std::string& k) : key(k),prev(nullptr),next(nullptr){}
    };
    Node* head_;
    Node* tail_;
    std::unordered_map<std::string, Node*> map_;
    void removeNode(Node* node);
    void addToFront(Node* node);

public:
    LRUCache();
    ~LRUCache();
    void touch(const std::string& key);
    std::string evict();
    size_t size() const;
    void debugPrint() const;
    void remove(const std::string& key);
};