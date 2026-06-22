#include "PersistenceManager.h"
#include "../database/Database.h"
#include <fstream>
#include <vector>
#include <utility> 
#include <chrono>

bool PersistenceManager::saveSnapshot(Database& db, const std::string& filename)
{
    std::ofstream out(filename, std::ios::binary);
    if (!out)
    {
        return false;
    }
    auto entries = db.getAllData();
    size_t count = entries.size();
    out.write(reinterpret_cast<const char*>(&count), sizeof(count));
    for (const auto& [key, value] : entries)
    {
        size_t key_len = key.size();
        size_t value_len = value.size();
        out.write(reinterpret_cast<const char*>(&key_len), sizeof(key_len));
        out.write(key.data(), key_len);
        out.write(reinterpret_cast<const char*>(&value_len), sizeof(value_len));
        out.write(value.data(), value_len);
        std::chrono::system_clock::time_point expiry;
        bool has_expiry = db.getExpiration(key, expiry);
        out.write(reinterpret_cast<const char*>(&has_expiry), sizeof(has_expiry));
        if (has_expiry)
        {
            auto timestamp = std::chrono::duration_cast<std::chrono::seconds>(expiry.time_since_epoch()).count();
            out.write(reinterpret_cast<const char*>(&timestamp),sizeof(timestamp));
        }
    }
    return true;
}

bool PersistenceManager::loadSnapshot(Database& db, const std::string& filename)
{
    std::ifstream in(filename, std::ios::binary);
    if (!in)
    {
        return false;
    }
    size_t count;
    in.read(reinterpret_cast<char*>(&count), sizeof(count));
    for (size_t i = 0; i < count; i++)
    {
        size_t key_len;
        in.read(reinterpret_cast<char*>(&key_len), sizeof(key_len));
        std::string key(key_len, '\0');
        in.read(&key[0], key_len);
        size_t value_len;
        in.read(reinterpret_cast<char*>(&value_len), sizeof(value_len));
        std::string value(value_len, '\0');
        in.read(&value[0], value_len);
        bool has_expiry;
        in.read(reinterpret_cast<char*>(&has_expiry), sizeof(has_expiry));
        bool restore_key = true;
        std::chrono::system_clock::time_point expiry;
        if (has_expiry)
        {
            long long timestamp;
            in.read(reinterpret_cast<char*>(&timestamp), sizeof(timestamp));
            expiry = std::chrono::system_clock::time_point(std::chrono::seconds(timestamp));
            if (expiry <= std::chrono::system_clock::now())
            {
                restore_key = false;
            } 
        }
        if(restore_key)
        {
            db.restoreKey(key, value);
            if(has_expiry)
            {
                db.restoreExpiration(key, expiry);
            }
        }
    }
    return true;
}