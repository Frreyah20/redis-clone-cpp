#pragma once

#include <queue>
#include <thread>
#include <vector>
#include <mutex>
#include <condition_variable>
class Database;
class CommandParser;
class ClientHandler;
class PersistenceManager;
class ThreadPool

{
public:
    ThreadPool(size_t num_threads, Database& db, CommandParser& parser, ClientHandler& handler, PersistenceManager& persistence);
    ~ThreadPool();
    void enqueueClient(int client_fd);

private:
    std::vector<std::thread> workers_;
    std::queue<int> client_queue_;
    std::mutex mutex_;
    std::condition_variable cv_;
    bool stop_;
    Database& db_;
    CommandParser& parser_;
    ClientHandler& handler_;
    PersistenceManager& persistence_;
    void workerThread();
    
};