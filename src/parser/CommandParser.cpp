#include "CommandParser.h"
#include "../database/Database.h"
#include "../persistence/PersistenceManager.h"

#include <sstream>

class PersistenceManager;

std::string CommandParser::execute(const std::string& command, Database& db, PersistenceManager& persistence)
{
    std::stringstream ss(command);
    std::string cmd;
    ss >> cmd;
    if (cmd == "PING")
    {
        return "PONG\n";
    }
    else if(cmd == "SET")
    {
        std::string key;
        std::string value;
        ss >> key >> value;
        db.set(key, value);
        std::string option;
        if(ss >> option)
        {
            if(option == "EX")
            {
                int seconds;
                ss >> seconds;
                db.expire(key, seconds);
            }
        }

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
    else if (cmd == "EXPIRE")
    {
        std::string key;
        int seconds;
        ss >> key >> seconds;
        return db.expire(key, seconds) ? "1\n" : "0\n";
    }
    else if(cmd == "TTL")
    {
        std::string key;
        ss >> key;
        return std::to_string(db.ttl(key)) + "\n";
    }
    else if(cmd == "SAVE")
    {
        bool success = persistence.saveSnapshot(db, "snapshot.rdb");
        return success ? "OK\n" : "ERROR: Save failed\n";
    }  
    return "ERROR: Unknown command\n";
} 