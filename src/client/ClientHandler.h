#pragma once
class Database;
class CommandParser;

class ClientHandler
{
public:
    void handleClient(int client_fd, Database& db, CommandParser& parser);
};