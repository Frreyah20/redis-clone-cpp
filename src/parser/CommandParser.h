#pragma once
#include <string>
#include <vector>

class Database;
class PersistenceManager;

class CommandParser
{
public:
    std::string execute(const std::string& command,Database& db, PersistenceManager& persistence);
    std::string executeTokens(const std::vector<std::string>& tokens,Database& db,PersistenceManager& persistence);
};