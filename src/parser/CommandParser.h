#pragma once
#include <string>

class Database;

class CommandParser
{
public:
    std::string execute(const std::string& command,Database& db);
};