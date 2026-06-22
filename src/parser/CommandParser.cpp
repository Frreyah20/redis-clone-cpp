#include "CommandParser.h"
#include "../database/Database.h"

#include <sstream>

std::string CommandParser::execute(const std::string& command, Database& db)
{
    std::stringstream ss(command);
    std::string cmd;
    ss >> cmd;
    if (cmd == "PING")
    {
        return "PONG\n";
    }
    else if (cmd == "SET")
    {
        std::string key;
        std::string value;
        ss >> key >> value;
        db.set(key, value);
        return "OK\n";
    }
    else if (cmd == "GET")
    {
        std::string key;
        ss >> key;
        if (db.exists(key))
        {
            return db.get(key) + "\n";
        }
        return "(nil)\n";
    }
    else if (cmd == "EXISTS")
    {
        std::string key;
        ss >> key;
        return db.exists(key) ? "1\n" : "0\n";
    }

    else if (cmd == "DEL")
    {
        std::string key;
        ss >> key;
        return db.del(key) ? "1\n" : "0\n";
    }

    return "ERROR: Unknown command\n";
}