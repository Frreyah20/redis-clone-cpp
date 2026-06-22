#include "Server.h"
#include "../database/Database.h"
#include "../parser/CommandParser.h"
#include "../client/ClientHandler.h"
#include "../persistence/PersistenceManager.h"
#include <iostream>
#include <sys/socket.h>
#include <netinet/in.h>
#include <unistd.h>
#include <thread>
#include <chrono>


void Server::start()
{
    PersistenceManager persistence;
    Database db;
    CommandParser parser;
    ClientHandler handler;
    persistence.loadSnapshot(db, "snapshot.rdb");
    std::thread cleanup_thread([&db]()
    {
        while(true)
        {
            db.cleanupExpiredKeys();
            std::this_thread::sleep_for(std::chrono::seconds(1));
        }
    });
    cleanup_thread.detach();

    int server_fd = socket(AF_INET, SOCK_STREAM, 0);
    if (server_fd < 0)
    {
        std::cerr << "Socket creation failed\n";
        return;
    }
    int opt = 1;
    setsockopt(server_fd, SOL_SOCKET, SO_REUSEADDR, &opt, sizeof(opt));
    sockaddr_in address{};
    address.sin_family = AF_INET;
    address.sin_addr.s_addr = INADDR_ANY;
    address.sin_port = htons(6379);
    if (bind(server_fd, (sockaddr*)&address,sizeof(address)) < 0)
    {
        std::cerr << "Bind failed\n";
        return;
    }
    if (listen(server_fd, 10) < 0)
    {
        std::cerr << "Listen failed\n";
        return;
    }
    std::cout << "Server listening on port 6379\n";
    while (true)
    {
        sockaddr_in client_addr{};
        socklen_t client_len = sizeof(client_addr);

        int client_fd = accept(server_fd, (sockaddr*)&client_addr, &client_len);
        if (client_fd < 0)
        {
            continue;
        }
        std::cout << "Client connected\n";
        handler.handleClient(client_fd, db,parser,persistence);
    }
}