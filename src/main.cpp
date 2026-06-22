#include <iostream>
#include <sys/socket.h>
#include <netinet/in.h>
#include <unistd.h>
#include <cstring>
#include <string>
#include <unordered_map>
#include <sstream> 

void send_response(int client_fd, const std::string& response)
{
    send(client_fd, response.c_str(), response.size(),0);
}


int main() 
{
    std::unordered_map<std::string, std::string> database;
    int server_fd = socket(AF_INET, SOCK_STREAM, 0);
    if (server_fd < 0) 
    {
        std::cerr << "Socket creation failed\n";
        return 1;
    }

    std::cout << "Socket created\n";
    sockaddr_in address{};
    address.sin_family = AF_INET;
    address.sin_addr.s_addr = INADDR_ANY;
    address.sin_port = htons(6379);
    int opt = 1;
    setsockopt(server_fd, SOL_SOCKET, SO_REUSEADDR, &opt, sizeof(opt));
    if (bind(server_fd, (sockaddr*)&address, sizeof(address)) < 0) 
    {
        std::cerr << "Bind failed\n";
        return 1;
    }
    std::cout << "Bind successful\n";
    if (listen(server_fd, 10) < 0) 
    {
        std::cerr << "Listen failed\n";
        return 1;
    }
    std::cout << "Server listening on port 6379\n";
    while (true) 
    {
        sockaddr_in client_addr{};
        socklen_t client_len = sizeof(client_addr);
        int client_fd = accept(server_fd, (sockaddr*)&client_addr, &client_len);
        if (client_fd < 0) 
        {
            std::cerr << "Accept failed\n";
            continue;
        }       
        std::cout << "Client connected\n";
        char buffer[1024];

        while (true) 
        {
            int bytes_received = recv(client_fd, buffer, sizeof(buffer) - 1, 0);
            if (bytes_received <= 0) 
            {
                std::cout << "Client disconnected\n";
                break;
            }
            buffer[bytes_received] = '\0';

            std::cout << "Received: " << buffer << std::endl;
            std::string command = buffer;

            std::stringstream ss(command);
            std::string cmd;
            ss >> cmd;

            if (cmd == "PING")
            {
                const char* response = "PONG\n";
                send_response(client_fd, response);
            }
            else if (cmd == "SET")
            {
                std::string key;
                std::string value;
                ss >> key >> value;
                database[key] = value;
                const char* response = "OK\n"; 
                send_response(client_fd, response);
            }
            else if (cmd == "GET")
            {
                std::string key;
                ss >> key;
                std::string response;
                if (database.find(key) != database.end())
                {
                    response = database[key] + "\n";
                }
                else
                {
                    response = "(nil)\n";
                }
                send_response(client_fd, response);
            }
            else if (cmd == "DEL")
            {
                std::string key;
                ss >> key;
                int deleted = database.erase(key);
                std::string response = std::to_string(deleted) + "\n";
                send_response(client_fd, response);
            }
            else if (cmd == "EXISTS")
            {
                std::string key;
                ss >> key;
                std::string response;
                if (database.find(key) != database.end())
                {
                    response = "1\n";
                }
                else
                {
                    response = "0\n";
                }
                send_response(client_fd, response);
            }
            else
            {
                const char* response = "ERROR: Unknown command\n";
                send_response(client_fd, response);
            }
        }

        close(client_fd);
    }

    close(server_fd);
    return 0;
}