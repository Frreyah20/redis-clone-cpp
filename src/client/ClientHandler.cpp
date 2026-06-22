#include "ClientHandler.h"
#include "../database/Database.h"
#include "../parser/CommandParser.h"
#include "../persistence/PersistenceManager.h"
#include <iostream>
#include <string>
#include <sys/socket.h>
#include <unistd.h>

void ClientHandler::handleClient(int client_fd,Database& db,CommandParser& parser,PersistenceManager& persistence)
{
    char buffer[1024];
    while (true)
    {
        int bytes_received =recv(client_fd, buffer, sizeof(buffer) - 1, 0);
        if (bytes_received <= 0)
        {
            std::cout<< "Client disconnected\n";
            break;
        }

        buffer[bytes_received] = '\0';
        std::string command = buffer;
        std::string response = parser.execute(command, db,persistence);
        send(client_fd, response.c_str(), response.size(), 0);
    }
    close(client_fd);
}