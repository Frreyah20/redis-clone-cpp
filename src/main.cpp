#include <iostream>
#include <sys/socket.h>
#include <netinet/in.h>
#include <unistd.h>
#include <cstring>
#include <string>
#include <unordered_map>
#include <sstream> 
#include "database/Database.h"
#include "parser/CommandParser.h"
#include "server/Server.h"
#include "client/ClientHandler.h"


void send_response(int client_fd, const std::string& response)
{
    send(client_fd, response.c_str(), response.size(),0);
}

int main()
{
    Server server;
    server.start();
    return 0;
}
