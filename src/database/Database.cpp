#include "Database.h"

void Database::set(const std::string& key, const std::string& value)
{
    data_.insert(key, value);
}

std::string Database::get(const std::string& key) const
{
    auto value =const_cast<HashTable<std::string, std::string>&>(data_).find(key);
    if (!value)
    {
        return "";
    }
    return *value;
}

bool Database::exists(const std::string& key) const
{
    return data_.contains(key);
}

bool Database::del(const std::string& key)
{
    return data_.erase(key);
}