#pragma once

#include <string>
#include "../hash/HashTable.h"
#include <chrono>
#include <mutex>
#include <vector>
#include <utility>
#include "../lru/LRUCache.h"

class Database
{
private:
    std::mutex mutex_;
    HashTable<std::string,std::string> data_;
    HashTable<std::string,std::chrono::system_clock::time_point> expirations_;
    bool isExpiredUnlocked(const std::string& key);
    void removeExpiredKeyUnlocked(const std::string& key);
    LRUCache lru_;
    size_t memory_used_ = 0;
    size_t max_memory_ = 1024 * 1024; // 1 MB default
    void evictIfNeeded();
    size_t getMaxMemory() const;

public:
    void set(const std::string& key,const std::string& value);
    std::string get(const std::string& key);
    bool exists(const std::string& key);
    bool del(const std::string& key);
    bool expire(const std::string& key, int seconds);
    long ttl(const std::string& key);
    bool isExpired(const std::string& key);
    void removeExpiredKey(const std::string& key);
    void cleanupExpiredKeys();
    std::vector<std::pair<std::string,std::string>> getAllData();
    std::vector<std::pair<std::string, std::chrono::system_clock::time_point >> getAllExpirations();
    void restoreKey(const std::string& key, const std::string& value);
    void restoreExpiration(const std::string& key, const std::chrono::system_clock::time_point& expiry);
    bool getExpiration(const std::string& key, std::chrono::system_clock::time_point& expiry);
    void setMaxMemory(size_t bytes);

};