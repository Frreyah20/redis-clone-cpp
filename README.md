# Redis Clone (C++)

A Redis-inspired in-memory key-value database built from scratch in C++.

This project is designed to explore the core systems concepts behind modern in-memory databases, including networking, storage engines, custom data structures, persistence, concurrency, caching, and performance engineering.

The goal is not to reproduce Redis feature-for-feature, but to understand and implement the fundamental components that make high-performance key-value stores possible.

---

## Project Goals

This project focuses on:

* Systems Programming
* Network Programming
* Database Internals
* Memory Management
* Concurrent Programming
* Performance Engineering
* Data Structure Design

---

## Current Features

### Networking

* TCP server implementation
* Client-server communication
* Command parsing and dispatching

### Key-Value Operations

* `PING`
* `SET`
* `GET`
* `DEL`
* `EXISTS`

---

## Example Usage

```text
SET name frreyah
OK

GET name
frreyah

EXISTS name
1

DEL name
OK
```

---

## Project Architecture

```text
Client
   │
   ▼
TCP Server
   │
   ▼
Command Parser
   │
   ▼
Storage Engine
   │
   ▼
Key-Value Database
```

---

## Planned System Components

The project is being developed incrementally to explore the major building blocks of modern in-memory databases:

* Storage Engine
* Custom Hash Table
* TTL Expiration Engine
* Persistence Layer
* Concurrent Client Handling
* LRU Eviction Engine
* Benchmarking Framework

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

## Technologies

* C++
* CMake
* BSD Sockets
* STL

---

## Learning Outcomes

This project is intended to provide practical experience with:

* Socket Programming
* Client-Server Architectures
* Database Storage Engines
* Custom Data Structures
* Serialization and Persistence
* Concurrent Programming
* Cache Design
* Systems Performance Analysis

---

## Repository Structure

```text
redis-clone-cpp/
├── src/
├── build/
├── CMakeLists.txt
└── README.md
```

---

## Future Architecture

```text
Client
   │
   ▼
TCP Server
   │
   ▼
Command Parser
   │
   ▼
Storage Engine
   ├── Custom Hash Table
   ├── TTL Manager
   ├── Persistence Layer
   └── LRU Cache
           │
           ▼
      Thread Pool
           │
           ▼
      Benchmark Suite
```

---

## License

This project is developed for educational purposes and systems programming practice.
