#pragma once

#include <string>
#include "../hash/HashTable.h"
#include <chrono>
#include <mutex>

class Database
{
private:
    std::mutex mutex_;
    HashTable<std::string,std::string> data_;
    HashTable<std::string,std::chrono::steady_clock::time_point> expirations_;
    bool isExpiredUnlocked(const std::string& key);
    void removeExpiredKeyUnlocked(const std::string& key);
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
};