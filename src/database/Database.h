#pragma once

#include <string>
#include "../hash/HashTable.h"
#include <chrono>
#include <mutex>
#include <vector>
#include <utility>

class Database
{
private:
    std::mutex mutex_;
    HashTable<std::string,std::string> data_;
    HashTable<std::string,std::chrono::system_clock::time_point> expirations_;
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
    std::vector<std::pair<std::string,std::string>> getAllData();
    std::vector<std::pair<std::string, std::chrono::system_clock::time_point >> getAllExpirations();
    void restoreKey(const std::string& key, const std::string& value);
    void restoreExpiration(const std::string& key, const std::chrono::system_clock::time_point& expiry);
    bool getExpiration(const std::string& key, std::chrono::system_clock::time_point& expiry);
};