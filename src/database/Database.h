#pragma once

#include <string>
#include "../hash/HashTable.h"
class Database
{
private:
    HashTable<std::string,std::string> data_;
public:
    void set(const std::string& key,const std::string& value);
    std::string get(const std::string& key) const;
    bool exists(const std::string& key) const;
    bool del(const std::string& key);
};