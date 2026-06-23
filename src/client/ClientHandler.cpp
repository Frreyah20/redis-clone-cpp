#include "ClientHandler.h"
#include "../database/Database.h"
#include "../parser/CommandParser.h"
#include "../persistence/PersistenceManager.h"
#include "../resp/RespSerializer.h"
#include "../resp/RespParser.h"
#include <iostream>
#include <string>
#include <sys/socket.h>
#include <unistd.h>
#include <sstream>


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
        std::string request = buffer;
        std::string response;
        try
        {
            if(!request.empty() && request[0] == '*')
            {
                auto tokens = RespParser::parse(request);
                std::string plainResponse =
                    parser.executeTokens(tokens, db, persistence);
                std::string cmd = tokens[0];
                if(cmd == "PING")
                {
                    response = RespSerializer::simpleString("PONG");
                }
                else if(cmd == "COMMAND")
                {
                    response = RespSerializer::simpleString("OK");
                }
                else if(cmd == "SET")
                {
                    response = RespSerializer::simpleString("OK");
                }
                else if(cmd == "GET")
                {
                    if(plainResponse == "(nil)\n")
                    {
                        response = RespSerializer::nullBulkString();
                    }
                    else
                    {
                        plainResponse.pop_back(); // remove \n
                        response = RespSerializer::bulkString(plainResponse);
                    }
                }
                else if(cmd == "EXISTS" ||
                cmd == "DEL" ||
                cmd == "EXPIRE" ||
                cmd == "TTL")
                {
                    if(!plainResponse.empty() && plainResponse.back() == '\n')
                    {
                        plainResponse.pop_back();
                    }
                    response = RespSerializer::integer(std::stol(plainResponse));
                }
                else
                {
                    response = plainResponse;
                }
            }
            else
            {
                std::string plainResponse =
                    parser.execute(request, db, persistence);

                    std::string command;

                    std::stringstream ss(request);
                    ss >> command;

                    if(command == "PING")
                    {
                        response =
                            RespSerializer::simpleString("PONG");
                    }
                    else
                    {
                        response = plainResponse;
                    }
            }
        }
        catch(const std::exception& e)
        {
            response = std::string("ERROR: ") + e.what() + "\n";
        }
        send(client_fd, response.c_str(), response.size(), 0);
    }
    close(client_fd);
}