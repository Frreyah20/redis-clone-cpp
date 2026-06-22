#include "Database.h"
#include <iostream>

void Database::set(const std::string& key, const std::string& value)
{
    std::lock_guard<std::mutex> lock(mutex_);
    auto existing = data_.find(key);
    if(existing)
    {
        memory_used_ -= key.size();
        memory_used_ -= existing->size();
    }
    data_.insert(key, value);
    memory_used_ += key.size();
    memory_used_ += value.size();
    lru_.touch(key);
    evictIfNeeded();
}

std::string Database::get(const std::string& key)
{
    std::lock_guard<std::mutex> lock(mutex_);
    if(isExpiredUnlocked(key))
    {
        removeExpiredKeyUnlocked(key);
        return "";
    }
    auto value =const_cast<HashTable<std::string, std::string>&>(data_).find(key);
    if (!value)
    {
        return "";
    }
    lru_.touch(key);
    return *value;
}

bool Database::exists(const std::string& key)
{
    std::lock_guard<std::mutex> lock(mutex_);
    if(isExpiredUnlocked(key))
    {
        removeExpiredKeyUnlocked(key);
        return false;
    }
    return data_.contains(key);
}

bool Database::del(const std::string& key)
{
    std::lock_guard<std::mutex> lock(mutex_);
    auto value = data_.find(key);
    if(value)
    {
        memory_used_ -= key.size();
        memory_used_ -= value->size();
    }
    bool erased = data_.erase(key);
    if(erased)
    {
        lru_.remove(key);
    }
    return erased;
}

bool Database::expire(const std::string& key,int seconds)
{
    std::lock_guard<std::mutex> lock(mutex_);
    if (!data_.contains(key))
    {
        return false;
    }
    auto expiry_time = std::chrono::system_clock::now() + std::chrono::seconds(seconds);
    expirations_.insert(key,expiry_time);

    return true;
}

long Database::ttl(const std::string& key)
{
    std::lock_guard<std::mutex> lock(mutex_);
    if(!data_.contains(key))
    {
        return -2;
    }
    if(isExpiredUnlocked(key))
    {
        removeExpiredKeyUnlocked(key);
        return -2;
    }
    auto expiry = expirations_.find(key);
    if(!expiry)
    {
        return -1;
    }
    auto remaining = std::chrono::duration_cast<std::chrono::seconds>(
            *expiry - std::chrono::system_clock::now());
    return remaining.count();
}

bool Database::isExpired(const std::string& key)
{
    std::lock_guard<std::mutex> lock(mutex_);
    return isExpiredUnlocked(key);
}

void Database::removeExpiredKey(const std::string& key)
{
    std::lock_guard<std::mutex> lock(mutex_);
    data_.erase(key);
    expirations_.erase(key);
}

void Database::cleanupExpiredKeys()
{
    std::lock_guard<std::mutex> lock(mutex_);
    auto keys = expirations_.keys();
    for(const auto& key : keys)
    {
        if(isExpiredUnlocked(key))
        {
            removeExpiredKeyUnlocked(key);
        }
    }
} 

bool Database::isExpiredUnlocked(const std::string& key)
{
    auto expiry = expirations_.find(key);
    if (!expiry)
    {
        return false;
    }
    return std::chrono::system_clock::now() >= *expiry;
}

void Database::removeExpiredKeyUnlocked(const std::string& key)
{
    auto value = data_.find(key);
    if(value)
    {
        memory_used_ -= key.size();
        memory_used_ -= value->size();
    }
    data_.erase(key);
    expirations_.erase(key);
    lru_.remove(key);
}

std::vector<std::pair<std::string,std::string>>
Database::getAllData()
{
    std::lock_guard<std::mutex> lock(mutex_);
    return data_.items();
}

std::vector<
    std::pair<
        std::string,
        std::chrono::system_clock::time_point
    >
>
Database::getAllExpirations()
{
    std::lock_guard<std::mutex> lock(mutex_);
    return expirations_.items();
}

void Database::restoreKey(const std::string& key, const std::string& value)
{
    std::lock_guard<std::mutex> lock(mutex_);
    data_.insert(key, value);
}

void Database::restoreExpiration(const std::string& key, const std::chrono::system_clock::time_point& expiry)
{
    std::lock_guard<std::mutex> lock(mutex_);
    expirations_.insert(key, expiry);
}

bool Database::getExpiration(const std::string& key, std::chrono::system_clock::time_point& expiry)
{
    std::lock_guard<std::mutex> lock(mutex_);
    auto value = expirations_.find(key);
    if (!value)
    {
        return false;
    }
    expiry = *value;
    return true;
}

void Database::setMaxMemory(size_t bytes)
{
    std::lock_guard<std::mutex> lock(mutex_);
    max_memory_ = bytes;
}

void Database::evictIfNeeded()
{
    while(memory_used_ > max_memory_)
    {
        std::string victim = lru_.evict();
        if(victim.empty())
        {
            return;
        }
        auto value = data_.find(victim);
        if(value)
        {
            memory_used_ -= victim.size();
            memory_used_ -= value->size();
        }
        data_.erase(victim);
        expirations_.erase(victim);
        std::cout
            << "Evicted key: "
            << victim
            << std::endl;
    }
} 

size_t Database::getMaxMemory() const
{
    return max_memory_;
}