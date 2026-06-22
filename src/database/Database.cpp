#include "Database.h"

void Database::set(const std::string& key, const std::string& value)
{
    data_[key] = value;
}

std::string Database::get(const std::string& key) const
{
    auto it = data_.find(key);
    if (it == data_.end())
    {
        return "";
    }
    return it->second;
}

bool Database::exists(const std::string& key) const
{
    return data_.find(key) != data_.end();
}

bool Database::del(const std::string& key)
{
    return data_.erase(key) > 0;
}