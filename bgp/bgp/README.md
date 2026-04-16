# BMP (BGP Monitoring Protocol) Server Implementation

A production-ready BMP server implementation in C++ that handles BGP monitoring protocol connections from routers and monitoring systems.

## Overview

This implementation provides a complete BMP server that:

- **Accepts BMP client connections** on a configurable port (default: 11019)
- **Processes BMP messages** including route monitoring, statistics, peer state changes
- **Manages multiple concurrent connections** using multi-threaded architecture
- **Tracks peer information** and maintains statistics
- **Follows RFC 7854 BMP protocol** specification

## Features

### Protocol Support

- **BMP Version 3** (RFC 7854)
- **Message Types**:
  - Route Monitoring (Type 0)
  - Statistics Report (Type 1)
  - Peer Down (Type 2)
  - Peer Up (Type 3)
  - Initiation (Type 4)
  - Termination (Type 5)

- **Peer Types**:
  - Global Instance Peers
  - Local Instance Peers (RD-filtered)

- **Address Families**: IPv4 and IPv6 support

### Server Features

- Multi-threaded client handling
- Signal-based graceful shutdown (SIGINT/SIGTERM)
- Configurable buffer size and connection limits
- Real-time statistics tracking
- Comprehensive error handling
- Socket options optimization (SO_REUSEADDR)

## File Structure

```
.
├── bmp_server.h       # Header file with class definitions and structures
├── bmp_server.cpp     # Core implementation
├── main.cpp           # Example usage and entry point
├── Makefile           # Build configuration
└── README.md          # This file
```

## Classes and Components

### BMPMessage
Base class for BMP messages with serialization support.

```cpp
class BMPMessage {
public:
    BMPMessage(BMPMessageType type);
    std::vector<uint8_t> serialize() const;
    void appendPayload(const uint8_t* data, size_t len);
};
```

### BMPInitiationMessage
Handles BMP initiation messages with TLV (Type-Length-Value) support.

```cpp
class BMPInitiationMessage : public BMPMessage {
public:
    void addSystemDescription(const std::string& sysDescr);
    void addSystemName(const std::string& sysName);
};
```

### BMPPeer
Represents a BGP peer being monitored.

```cpp
class BMPPeer {
public:
    void addRoute(const BMPRoute& route);
    uint32_t getASN() const;
    std::string getAddress() const;
    uint32_t getRouterId() const;
};
```

### BMPServer
Main server class managing connections and statistics.

```cpp
class BMPServer {
public:
    bool start();
    void stop();
    std::shared_ptr<BMPPeer> addPeer(const std::string& address, 
                                      uint16_t port, uint32_t asn);
    size_t getPeerCount() const;
    uint64_t getMessagesReceived() const;
    uint64_t getMessagesSent() const;
};
```

## Building

### Requirements

- C++17 compatible compiler (g++, clang++)
- POSIX-compliant system (Linux, macOS, BSD)
- Standard development tools (make, etc.)

### Compilation

```bash
# Build the server
make

# Build with verbose output
make CXXFLAGS="-std=c++17 -Wall -Wextra -O2 -pthread -v"

# Clean build artifacts
make clean
```

## Running

### Start Server (Default Port 11019)

```bash
./bmp_server
```

Output:
```
=== BMP Server Implementation ===
Starting BMP server on port 11019
BMP Server started on port 11019

Added example peers:
  Peer 1: 192.168.1.1 AS65000
  Peer 2: 192.168.1.2 AS65001

Server is running. Waiting for BMP connections...
Press Ctrl+C to shutdown.

--- Statistics ---
Connected peers: 2
Messages received: 0
Messages sent: 0
```

### Start Server (Custom Port)

```bash
./bmp_server 12019
```

### Graceful Shutdown

Press `Ctrl+C` (sends SIGINT) or send SIGTERM:

```bash
kill -TERM <pid>
```

