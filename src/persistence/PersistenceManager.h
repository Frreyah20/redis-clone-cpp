#pragma once

#include <string>

class Database;

class PersistenceManager
{
public:
    bool saveSnapshot(Database& db, const std::string& filename);
    bool loadSnapshot(Database& db, const std::string& filename);
};