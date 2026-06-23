cmake_minimum_required(VERSION 3.10)

project(redis_clone)

set(CMAKE_CXX_STANDARD 17)

add_library(redis_core

    src/server/Server.cpp
    src/database/Database.cpp
    src/parser/CommandParser.cpp
    src/client/ClientHandler.cpp
    src/persistence/PersistenceManager.cpp
    src/threadpool/ThreadPool.cpp
    src/lru/LRUCache.cpp
    src/resp/RespParser.cpp
    src/resp/RespSerializer.cpp
)

add_executable(redis_clone
    src/main.cpp
)

target_link_libraries(redis_clone PRIVATE redis_core)

add_executable(
    benchmark
    src/benchmark/BenchMark.cpp
)

include(FetchContent)

FetchContent_Declare(
  googletest
  URL https://github.com/google/googletest/archive/refs/tags/v1.14.0.zip
)

FetchContent_MakeAvailable(googletest)

enable_testing()

add_executable(
    database_tests
    tests/database_tests.cpp
)

target_link_libraries(
    database_tests
    PRIVATE
    redis_core
    GTest::gtest_main
)

include(GoogleTest)

gtest_discover_tests(database_tests)