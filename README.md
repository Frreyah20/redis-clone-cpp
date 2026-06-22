# Redis Clone (C++)

A Redis-inspired in-memory key-value database built from scratch in modern C++.

This project implements many of the core systems that power high-performance databases and caches, including a custom hash table, key expiration engine, persistence layer, thread-pool based concurrency, LRU eviction, and performance benchmarking. The goal was to explore database internals, networking, concurrent programming, and systems design through a ground-up implementation.

---

## Features

### Networking & Command Processing

* TCP server built using BSD sockets
* Client-server architecture
* Command parsing and dispatching
* Persistent client connections
* Concurrent request processing using a thread pool

### Supported Commands

```text
PING
SET key value
GET key
DEL key
EXISTS key

SET key value EX seconds
EXPIRE key seconds
TTL key

SAVE
MAXMEMORY bytes
```

### Storage Engine

* In-memory key-value database
* Modular database architecture
* Efficient key lookup and updates

### Custom Hash Table

Implemented from scratch without using `std::unordered_map`.

Features:

* Bucket-based storage
* Separate chaining for collision resolution
* Dynamic resizing and rehashing
* Average-case O(1) insertion, lookup, and deletion

### TTL Expiration System

* Key expiration support
* Lazy expiration during reads
* Background cleanup thread
* Automatic removal of expired entries

### Persistence Engine

* Snapshot-based persistence
* Binary serialization format
* Database recovery on startup
* Durable storage across server restarts

### Concurrent Client Handling

* Producer-consumer architecture
* Thread pool based request execution
* Task queue implementation
* Synchronization using mutexes and condition variables
* Safe concurrent access to shared state

### LRU Eviction Engine

* Configurable memory limits
* Least Recently Used eviction policy
* O(1) access tracking
* O(1) eviction operations
* Hash map + doubly linked list design

### Benchmarking Framework

* Custom benchmark client
* Throughput measurement
* Latency analysis
* Multi-client scalability testing
* Performance comparison across thread pool configurations

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
                         Database
                  ┌────────┼────────┐
                  │        │        │
                  ▼        ▼        ▼
            Hash Table    TTL      LRU
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

### Basic Operations

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
1
```

### Expiration

```text
SET session abc EX 60
OK

TTL session
59
```

### Persistence

```text
SAVE
OK
```

### Memory Management

```text
MAXMEMORY 1048576
OK
```

---

## Performance

### Single Client Performance

| Operation | Throughput |
| --------- | ---------- |
| PING      | ~13k req/s |
| SET       | ~11k req/s |
| GET       | ~10k req/s |

### Concurrent Throughput

| Workers | Clients |  Throughput |
| ------- | ------: | ----------: |
| 4       |     100 |  ~33k req/s |
| 8       |     100 | ~128k req/s |

### Key Findings

* Throughput scaled significantly with increased worker pool size.
* The system achieved over **128k requests/second** under concurrent workloads.
* Benchmarking identified worker pool capacity as a major scalability bottleneck.
* Performance analysis was conducted using a custom benchmark suite.

Detailed results are available in `BENCHMARK.md`.

---

## Building

### Prerequisites

* C++17
* CMake
* Make

### Build

```bash
mkdir build
cd build
cmake ..
make
```

### Run Server

```bash
./redis_clone
```

### Run Benchmark

```bash
./benchmark
```

---

## Project Structure

```text
redis_clone/
├── src/
│   ├── client/
│   ├── database/
│   ├── hash/
│   ├── lru/
│   ├── parser/
│   ├── persistence/
│   ├── server/
│   ├── threadpool/
│   └── benchmark/
├── CMakeLists.txt
├── README.md
└── BENCHMARK.md
```

---

## Systems Concepts Explored

* TCP/IP Networking
* BSD Sockets
* Client-Server Architecture
* Custom Data Structures
* Hash Table Design
* Memory Management
* Database Internals
* Serialization & Recovery
* Thread Pools
* Producer-Consumer Pattern
* Synchronization Primitives
* Cache Eviction Policies
* Performance Engineering
* Scalability Analysis

---

## Future Improvements

* RESP protocol compatibility
* Incremental persistence
* Replication support
* Publish/Subscribe messaging
* Event-driven networking using epoll
* Clustered deployment

---

## License

This project was developed for educational purposes to explore systems programming, database internals, and concurrent software design.
