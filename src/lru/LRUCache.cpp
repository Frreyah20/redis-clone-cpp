#include "LRUCache.h"

#include <unordered_map>
#include <iostream>

LRUCache::LRUCache(): head_(nullptr), tail_(nullptr)
{
}

LRUCache::~LRUCache()
{
    Node* current = head_;
    while(current)
    {
        Node* temp = current;
        current = current->next;
        delete temp;
    }
}

void LRUCache::addToFront(Node* node)
{
    node->prev = nullptr;
    node->next = head_;
    if(head_)
    {
        head_->prev = node;
    }
    head_ = node;
    if(!tail_)
    {
        tail_ = node;
    }
}

void LRUCache::removeNode(Node* node)
{
    if(node->prev)
    {
        node->prev->next = node->next;
    }
    else
    {
        head_ = node->next;
    }
    if(node->next)
    {
        node->next->prev = node->prev;
    }
    else
    {
        tail_ = node->prev;
    }
}

void LRUCache::touch(const std::string& key)
{
    auto it = map_.find(key);
    if(it != map_.end())
    {
        Node* node = it->second;
        removeNode(node);
        addToFront(node);
        return;
    }
    Node* node = new Node(key);
    addToFront(node);
    map_[key] = node;
}

size_t LRUCache::size() const
{
    return map_.size();
}

std::string LRUCache::evict()
{
    if(!tail_)
    {
        return "";
    }
    Node* victim = tail_;
    std::string key = victim->key;
    removeNode(victim);
    map_.erase(key);
    delete victim;
    return key;
}

void LRUCache::debugPrint() const
{
    Node* current = head_;
    while(current)
    {
        std::cout << current->key;
        if(current->next)
        {
            std::cout << " -> ";
        }

        current = current->next;
    }
    std::cout << std::endl;
}

void LRUCache::remove(const std::string& key)
{
    auto it = map_.find(key);
    if(it == map_.end())
    {
        return;
    }
    Node* node = it->second;
    removeNode(node);
    map_.erase(it);
    delete node;
}