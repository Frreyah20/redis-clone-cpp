# Redis Clone (C++)

A Redis-inspired in-memory key-value database built from scratch in C++.

This project implements the core components of a modern in-memory database, including a custom storage engine, hash table implementation, key expiration, persistence, concurrent client handling, memory eviction policies, and performance benchmarking.

The objective of the project is to understand and implement the fundamental systems concepts that power high-performance key-value stores such as Redis.

---

## Features

### Networking

* TCP server implementation using BSD sockets
* Client-server communication
* Command parsing and dispatching
* Concurrent client handling using a thread pool

### Key-Value Operations

* `PING`
* `SET`
* `GET`
* `DEL`
* `EXISTS`

### Storage Engine

* In-memory key-value database
* Modular storage engine design
* Efficient lookup and update operations

### Custom Hash Table

* Bucket-based hash table implementation
* Collision handling through chaining
* Dynamic resizing and rehashing
* Average-case O(1) insert, lookup, and delete operations

### TTL Expiration Engine

* Key expiration support
* Lazy expiration during reads
* Background cleanup thread
* TTL management and automatic key removal

### Persistence

* Snapshot-based persistence
* Binary serialization and deserialization
* Database recovery on startup
* Durable storage across server restarts

### LRU Eviction Engine

* Configurable memory limits
* Least Recently Used (LRU) eviction policy
* O(1) access and eviction operations
* Doubly linked list and hash table based design

### Concurrency

* Thread pool architecture
* Task queue based request processing
* Synchronization using mutexes and condition variables
* Safe concurrent access to shared data structures

### Benchmarking

* Throughput measurement
* Latency analysis
* Multi-client performance evaluation
* Scalability testing under concurrent workloads

---

## Architecture

```text
                     Client Connections
                             │
                             ▼
                      TCP Server
                             │
                             ▼
                      Command Parser
                             │
                             ▼
                      Storage Engine
                   ┌─────────┼─────────┐
                   │         │         │
                   ▼         ▼         ▼
             Hash Table   TTL Engine  LRU Cache
                   │
                   ▼
              Persistence
                   │
                   ▼
              Snapshot File

          Concurrent Request Processing
                   │
                   ▼
               Thread Pool
```

---

## Example Usage

```text
PING
PONG

SET name frreyah
OK

GET name
frreyah

EXISTS name
1

DEL name
OK
```

### TTL Operations

```text
SET session abc EX 60
OK

TTL session
59
```

---

## Build Instructions

### Prerequisites

* C++17 or newer
* CMake
* Make

### Build

```bash
mkdir build
cd build
cmake ..
make
```

### Run

```bash
./redis_clone
```

---

## Technologies Used

* C++
* CMake
* BSD Sockets
* Multithreading
* STL
* File I/O
* Binary Serialization

---

## Concepts Explored

This project provides hands-on experience with:

* Systems Programming
* Network Programming
* Database Internals
* Storage Engine Design
* Custom Data Structures
* Memory Management
* Concurrent Programming
* Thread Pools
* Synchronization Primitives
* Persistence and Recovery
* Cache Design
* Performance Engineering

---

## Repository Structure

```text
redis-clone-cpp/
├── src/
├── include/
├── build/
├── benchmarks/
├── CMakeLists.txt
└── README.md
```

---

## Performance

The project includes a benchmarking framework for evaluating:

* Request throughput
* Latency under concurrent load
* Scalability with increasing client counts
* Storage engine performance
* Hash table performance

Benchmark results and performance analysis are available in the repository documentation.

---

## Motivation

Redis is widely used as an in-memory database, cache, and message broker. Rebuilding its core components from scratch provides practical insight into how high-performance backend systems manage memory, persistence, networking, concurrency, and data structures at scale.

---

## License

This project is developed for educational purposes and systems programming practice.
