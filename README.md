# Redis Clone (C++)

A Redis-compatible in-memory key-value database built from scratch in C++, featuring custom storage, TTL expiration, persistence, concurrent request handling, LRU caching, and RESP protocol support.

This project implements many of the core systems that power high-performance databases and caches, including a custom hash table, key expiration engine, persistence layer, thread-pool based concurrency, LRU eviction, and performance benchmarking. The goal was to explore database internals, networking, concurrent programming, and systems design through a ground-up implementation.

---

### Highlights

- Custom hash table implementation (no std::unordered_map)
- Redis Serialization Protocol (RESP) support
- Compatible with redis-cli
- Snapshot persistence and recovery
- Thread-pool based concurrent request processing
- Automated unit testing with GoogleTest
- 9 automated unit tests covering database, persistence, and command parsing

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

## RESP Protocol Support

The server implements the Redis Serialization Protocol (RESP), allowing interoperability with standard Redis client tools.

Supported RESP commands:

* PING
* SET
* GET
* DEL
* EXISTS
* EXPIRE
* TTL

Example usage with redis-cli:

```bash
redis-cli -p 6379
```

```redis
127.0.0.1:6379> SET name frreyah
OK

127.0.0.1:6379> GET name
"frreyah"

127.0.0.1:6379> EXISTS name
(integer) 1

127.0.0.1:6379> TTL name
(integer) -1
```

The RESP implementation enables external Redis-compatible clients to communicate with the server without requiring a custom client application.
Verified using:

* redis-cli
* redis-benchmark


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
* The custom benchmark suite measured over 128k requests/second under synthetic concurrent workloads.
* RESP support enables interoperability with standard Redis tooling such as redis-cli.
* Benchmarking identified worker pool capacity as a major scalability bottleneck.
* Performance analysis was conducted using a custom benchmark suite.

Detailed results are available in `BENCHMARK.md`.

---

## Benchmark Comparison

Benchmarks were performed on the same machine using redis-benchmark.

| System | Operation | Throughput |
|----------|----------|----------|
| Redis Clone | PING_MBULK | ~12.3k req/s |
| Redis 8.0.5 | PING_MBULK | ~95.2k req/s |

### Observations

* The custom implementation achieves approximately 13% of Redis throughput.
* Redis benefits from highly optimized event-driven networking and memory management.
* The comparison highlights the performance impact of architectural choices such as thread pools versus event-driven I/O.

---

## Testing

The project includes automated unit tests using GoogleTest.

Current test coverage includes:

- SET / GET operations
- Key existence checks
- Key deletion semantics
- TTL expiration handling
- Snapshot persistence and recovery

Run tests:

```bash
cd build
ctest
```

Current test suite:

- DatabaseTest.SetAndGet
- DatabaseTest.Exists
- DatabaseTest.DeleteKey
- DatabaseTest.ExpireKey
- DatabaseTest.TTLExpiration
- PersistenceTest.SaveAndLoadSnapshot
- CommandParserTest.Ping
- CommandParserTest.SetCommand
- CommandParserTest.GetCommand

Framework: GoogleTest

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
│   ├── benchmark/
│   ├── client/
│   ├── database/
│   ├── hash/
│   ├── lru/
│   ├── parser/
│   ├── persistence/
│   ├── resp/
│   ├── server/
│   └── threadpool/
├── tests/
├── BENCHMARK.md
├── README.md
└── CMakeLists.txt
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

* Event-driven networking using epoll
* Incremental persistence and append-only logging
* Replication support
* Publish/Subscribe messaging
* Additional Redis data structures (Lists, Hashes, Sets)

---

## License

This project was developed for educational purposes to explore systems programming, database internals, and concurrent software design.
