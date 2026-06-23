#include "CommandParser.h"
#include "../database/Database.h"
#include "../persistence/PersistenceManager.h"

#include <sstream>

class PersistenceManager;

std::string CommandParser::execute(
    const std::string& command,
    Database& db,
    PersistenceManager& persistence)
{
    std::stringstream ss(command);
    std::vector<std::string> tokens;
    std::string token;
    while (ss >> token)
    {
        tokens.push_back(token);
    }
    return executeTokens(tokens, db, persistence);
}
std::string CommandParser::executeTokens(
    const std::vector<std::string>& tokens,
    Database& db,
    PersistenceManager& persistence)
{
    if(tokens.empty())
    {
        return "ERROR: Empty command\n";
    }
    std::string cmd = tokens[0];
    if (cmd == "PING")
    {
        return "PONG\n";
    }
    else if(cmd == "COMMAND")
    {
        return "OK\n";
    }
    else if(cmd == "SET")
    {
        if(tokens.size() < 3)
        {
            return "ERROR: Missing arguments\n";
        }

        std::string key = tokens[1];
        std::string value = tokens[2];
        db.set(key, value);
        if(tokens.size() >= 5 && tokens[3] == "EX")
        {
            int seconds = std::stoi(tokens[4]);
            db.expire(key, seconds);
        }
        return "OK\n";
    }
    else if (cmd == "GET")
    {
        if(tokens.size() < 2)
        {
            return "ERROR: Missing key\n";
        }
        std::string key = tokens[1];
        if (db.exists(key))
        {
            return db.get(key) + "\n";
        }
        return "(nil)\n";
    }
    else if (cmd == "EXISTS")
    {
        if(tokens.size() < 2)
        {
            return "ERROR: Missing key\n";
        }
        std::string key = tokens[1];
        return db.exists(key) ? "1\n" : "0\n";
    }
    else if (cmd == "DEL")
    {
        if(tokens.size() < 2)
        {
            return "ERROR: Missing key\n";
        }
        std::string key = tokens[1];
        return db.del(key) ? "1\n" : "0\n";
    }
    else if (cmd == "EXPIRE")
    {
        if(tokens.size() < 3)
        {
            return "ERROR: Missing arguments\n";
        }

        std::string key = tokens[1];
        int seconds = std::stoi(tokens[2]);
        return db.expire(key, seconds) ? "1\n" : "0\n";
    }
    else if(cmd == "TTL")
    {
        if(tokens.size() < 2)
        {
            return "ERROR: Missing key\n";
        }
        std::string key = tokens[1];
        return std::to_string(db.ttl(key)) + "\n";
    }
    else if(cmd == "SAVE")
    {
        bool success = persistence.saveSnapshot(db, "snapshot.rdb");
        return success ? "OK\n" : "ERROR: Save failed\n";
    }  
    else if(cmd == "MAXMEMORY")
    {
        if(tokens.size() < 2)
        {
            return "ERROR: Missing value\n";
        }
        size_t bytes = std::stoull(tokens[1]);
        db.setMaxMemory(bytes);
        return "OK\n";
    }
    
    return "ERROR: Unknown command\n";
} 
    