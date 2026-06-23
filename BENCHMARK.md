# Redis Clone Benchmark Report

## Overview

This report evaluates the performance and scalability of the Redis Clone implementation developed in C++.

The system includes:

* Custom Hash Table implementation
* TCP Socket Server
* Command Parser
* TTL Expiration Engine
* Snapshot Persistence
* Thread Pool Based Concurrent Client Handling
* LRU Cache Eviction System

Benchmarks were executed on the local machine using a custom benchmark client that generated concurrent TCP connections and measured request throughput and latency.

---

# Single Client Performance

## Benchmark Methodology

Benchmarks were executed on Linux using:

- Custom benchmark client
- Persistent TCP connections
- Fixed request counts
- Multiple worker thread configurations
- Throughput and latency measurements

Metrics reported:

- Requests per second (Throughput)
- Average latency
- Scalability under concurrent client workloads

## PING Throughput

| Requests | Time (ms) | Throughput   |
| -------- | --------- | ------------ |
| 1000     | 76        | 13,157 req/s |

## SET Throughput

| Requests | Time (ms) | Throughput   |
| -------- | --------- | ------------ |
| 10,000   | 898       | 11,136 req/s |

## GET Throughput

| Requests | Time (ms) | Throughput   | Avg Latency |
| -------- | --------- | ------------ | ----------- |
| 10,000   | 925       | 10,811 req/s | 92.5 µs     |

---

# Concurrent Client Benchmark

Each client issued 5000 requests.

## Thread Pool Size: 4

| Clients | Requests | Time (ms) | Throughput (req/s) |
| ------- | -------: | --------: | -----------------: |
| 10      |   50,000 |     1,597 |             31,309 |
| 50      |  250,000 |     7,464 |             33,494 |
| 100     |  500,000 |    15,207 |             32,880 |

### Observation

Throughput saturated around 33k requests/sec.

Increasing the number of concurrent clients beyond 10 provided minimal performance gains, indicating worker-thread saturation.

---

## Thread Pool Size: 8

| Clients | Requests | Time (ms) | Throughput (req/s) |
| ------- | -------: | --------: | -----------------: |
| 10      |   50,000 |       918 |             54,466 |
| 50      |  250,000 |     2,278 |            109,745 |
| 100     |  500,000 |     3,887 |            128,634 |

### Observation

Increasing the worker pool from 4 threads to 8 threads significantly improved throughput.

The system scaled to over 128k requests/sec under 100 concurrent clients.

---

# Scalability Analysis

### Throughput Comparison

| Clients | 4 Workers | 8 Workers |
| ------- | --------: | --------: |
| 10      |    31,309 |    54,466 |
| 50      |    33,494 |   109,745 |
| 100     |    32,880 |   128,634 |

### Key Findings

* The worker pool size was a major performance bottleneck.
* Throughput improved by nearly 4x when increasing worker threads from 4 to 8 under heavy load.
* The producer-consumer thread pool architecture successfully handled concurrent client workloads.
* The system maintained stable throughput at high concurrency levels.
* Average single-client request latency remained below 100 microseconds.

---

# Comparison with Redis

To provide a reference point, the Redis Clone was also evaluated using redis-benchmark and compared against Redis 8.0.5 on the same machine.

| System | Operation | Throughput |
|----------|----------|----------|
| Redis Clone | PING_MBULK | ~12.3k req/s |
| Redis 8.0.5 | PING_MBULK | ~95.2k req/s |

### Observations

* The Redis Clone achieved approximately 13% of Redis throughput on this workload.
* Redis benefits from highly optimized event-driven networking and memory management.
* The comparison highlights the performance impact of architectural decisions such as thread pools versus event-driven I/O.

# Conclusion

The Redis Clone demonstrates the implementation of several core systems programming concepts:

* Network programming with BSD sockets
* Custom hash table design
* Background expiration processing
* Snapshot-based persistence
* Thread-pool based concurrency
* LRU cache eviction
* Protocol design using RESP
* Automated testing with GoogleTest
* Performance benchmarking and scalability analysis

Benchmarking showed that throughput scaled significantly with increased worker thread counts, reaching over 128k requests/sec under synthetic concurrent workloads.

Comparisons against Redis highlighted the performance trade-offs between a thread-pool architecture and highly optimized event-driven systems, providing practical insight into real-world database design.