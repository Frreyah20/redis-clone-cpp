#include "ThreadPool.h"
#include "../database/Database.h"
#include "../parser/CommandParser.h"
#include "../client/ClientHandler.h"
#include "../persistence/PersistenceManager.h"
#include <iostream>


ThreadPool::ThreadPool(size_t num_threads, Database& db, CommandParser& parser, ClientHandler& handler, PersistenceManager& persistence)
    :db_(db), parser_(parser), handler_(handler), persistence_(persistence), stop_(false)
{
    for(size_t i = 0; i < num_threads; i++)
    {
        workers_.emplace_back(&ThreadPool::workerThread, this);
    }
}

ThreadPool::~ThreadPool()
{
    {
        std::lock_guard<std::mutex> lock(mutex_);
        stop_ = true;
    }
    cv_.notify_all();
    for(auto& worker : workers_)
    {
        if(worker.joinable())
        {
            worker.join();
        }
    }
}

void ThreadPool::enqueueClient(int client_fd)
{
    {
        std::cout << "Client queued\n";
        std::lock_guard<std::mutex> lock(mutex_);
        client_queue_.push(client_fd);
    }
    cv_.notify_one();
}

void ThreadPool::workerThread()
{
    while(true)
    {
        int client_fd;
        {
            std::unique_lock<std::mutex> lock(mutex_);
            cv_.wait(lock,[this]{return stop_|| !client_queue_.empty();});
            if(stop_)
            {
                return;
            }
            std::cout << "Worker picked client\n";
            client_fd = client_queue_.front();
            client_queue_.pop();
        }
        handler_.handleClient(client_fd, db_, parser_, persistence_);
    }
}