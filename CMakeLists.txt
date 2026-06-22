cmake_minimum_required(VERSION 3.10)

project(redis_clone)

set(CMAKE_CXX_STANDARD 17)

add_executable(redis_clone

    src/main.cpp

    src/server/Server.cpp
    src/database/Database.cpp
    src/parser/CommandParser.cpp
    src/client/ClientHandler.cpp
    src/persistence/PersistenceManager.cpp
    src/threadpool/ThreadPool.cpp
    src/lru/LRUCache.cpp
)
add_executable(
    benchmark
    src/benchmark/BenchMark.cpp
)