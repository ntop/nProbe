/*
 *
 * (C) 2026 - ntop.org
 *
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation; either version 3 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program; if not, write to the Free Software Foundation,
 * Inc., 59 Temple Place - Suite 330, Boston, MA 02111-1307, USA.
 *
 */

#ifndef BMP_SERVER_H
#define BMP_SERVER_H

#include <cstdint>
#include <string>
#include <vector>
#include <memory>
#include <cstring>
#include <chrono>
#include <arpa/inet.h>

// BMP Protocol Constants
constexpr uint16_t BMP_DEFAULT_PORT = 11019;
constexpr uint32_t BMP_VERSION = 3;
constexpr uint8_t BMP_MARKER_SIZE = 16;
constexpr uint32_t BMP_MESSAGE_MAX_SIZE = 65535;

// BMP Message Types
enum class BMPMessageType : uint8_t {
    ROUTE_MONITORING = 0,
    STATS_REPORT = 1,
    PEER_DOWN = 2,
    PEER_UP = 3,
    INITIATION = 4,
    TERMINATION = 5
};

// BMP Peer Type
enum class BMPPeerType : uint8_t {
    GLOBAL_INSTANCE = 0,
    LOCAL_INSTANCE = 1
};

// BMP Peer Flags
constexpr uint8_t BMP_PEER_FLAG_V6 = 0x80;        // IPv6
constexpr uint8_t BMP_PEER_FLAG_L = 0x40;         // 4-byte AS Number
constexpr uint8_t BMP_PEER_FLAG_A = 0x20;         // Adjective-RIB-In used

// Structure definitions

#pragma pack(push, 1)

// BMP Common Header (fixed 19 bytes)
struct BMPCommonHeader {
    uint8_t marker[BMP_MARKER_SIZE];  // 16 bytes of 0xFF
    uint16_t length;                   // Total message length including header
    uint8_t msg_type;                  // Message type
};

// BMP Per-Peer Header (variable: 42 or 54 bytes depending on IPv4/IPv6)
struct BMPPerPeerHeader {
    uint8_t peer_type;                 // Peer type
    uint8_t peer_flags;                // Flags (V flag, L flag, A flag)
    uint32_t peer_router_id;           // Router ID
    uint32_t peer_as;                  // 2-byte AS or first 4 bytes of 4-byte AS
    uint32_t peer_bgp_id;              // BGP Identifier
    uint32_t timestamp_sec;            // Timestamp seconds
    uint32_t timestamp_usec;           // Timestamp microseconds
    // IPv4 address (4 bytes) or IPv6 address (16 bytes) follows
};

// BMP Initiation Message TLV Header
struct BMPInitiationTLV {
    uint16_t tlv_type;
    uint16_t tlv_length;
    // Value follows
};

// BMP Per-Peer Header with IPv4 address
struct BMPPerPeerHeaderIPv4 {
    BMPPerPeerHeader base_header;
    uint32_t peer_address;             // IPv4 address
};

// BMP Per-Peer Header with IPv6 address
struct BMPPerPeerHeaderIPv6 {
    BMPPerPeerHeader base_header;
    uint8_t peer_address[16];          // IPv6 address
};

#pragma pack(pop)

// C++ wrapper classes for easier manipulation

class BMPMessage {
public:
    BMPMessage(BMPMessageType type);
    
    // Getters
    BMPMessageType getType() const { return type_; }
    std::vector<uint8_t>& getPayload() { return payload_; }
    const std::vector<uint8_t>& getPayload() const { return payload_; }
    
    // Serialize to wire format
    std::vector<uint8_t> serialize() const;
    
    // Payload manipulation
    void appendPayload(const uint8_t* data, size_t len);
    void appendPayload(const std::vector<uint8_t>& data);
    
private:
    BMPMessageType type_;
    std::vector<uint8_t> payload_;
};

class BMPInitiationMessage : public BMPMessage {
public:
    BMPInitiationMessage();
    
    // Add system description
    void addSystemDescription(const std::string& sysDescr);
    
    // Add system name
    void addSystemName(const std::string& sysName);
    
    // Add other TLVs as needed
    
private:
    void addTLV(uint16_t type, const std::string& value);
};

class BMPRoute {
public:
    BMPRoute(const std::string& prefix, uint8_t prefixLen, uint32_t as);
    
    std::string getPrefix() const { return prefix_; }
    uint8_t getPrefixLength() const { return prefix_len_; }
    uint32_t getAS() const { return as_; }
    
private:
    std::string prefix_;
    uint8_t prefix_len_;
    uint32_t as_;
};

class BMPPeer {
public:
    BMPPeer(const std::string& address, uint16_t port, uint32_t asn);
    
    std::string getAddress() const { return address_; }
    uint16_t getPort() const { return port_; }
    uint32_t getASN() const { return asn_; }
    uint32_t getRouterId() const { return router_id_; }
    
    void setRouterId(uint32_t id) { router_id_ = id; }
    void setBGPId(uint32_t id) { bgp_id_ = id; }
    
    void addRoute(const BMPRoute& route);
    const std::vector<BMPRoute>& getRoutes() const { return routes_; }
    
    std::chrono::system_clock::time_point getLastUpdate() const { return last_update_; }
    void updateTimestamp() { last_update_ = std::chrono::system_clock::now(); }
    
private:
    std::string address_;
    uint16_t port_;
    uint32_t asn_;
    uint32_t router_id_;
    uint32_t bgp_id_;
    std::vector<BMPRoute> routes_;
    std::chrono::system_clock::time_point last_update_;
};

class BMPServer {
public:
    BMPServer(uint16_t port = BMP_DEFAULT_PORT);
    ~BMPServer();
    
    // Server control
    bool start();
    void stop();
    bool isRunning() const { return running_; }
    
    // Peer management
    std::shared_ptr<BMPPeer> addPeer(const std::string& address, uint16_t port, uint32_t asn);
    void removePeer(const std::string& address);
    std::shared_ptr<BMPPeer> getPeer(const std::string& address) const;
    
    // Statistics
    size_t getPeerCount() const { return peers_.size(); }
    uint64_t getMessagesReceived() const { return messages_received_; }
    uint64_t getMessagesSent() const { return messages_sent_; }
    
    // Configuration
    void setMaxConnections(size_t max) { max_connections_ = max; }
    void setBufferSize(size_t size) { buffer_size_ = size; }
    
private:
    uint16_t port_;
    int server_socket_;
    bool running_;
    size_t max_connections_;
    size_t buffer_size_;
    
    std::vector<std::shared_ptr<BMPPeer>> peers_;
    uint64_t messages_received_;
    uint64_t messages_sent_;
    
    // Internal methods
    void acceptConnections();
    void handleClient(int client_socket, const std::string& client_addr);
    std::vector<uint8_t> receiveMessage(int socket);
    bool sendMessage(int socket, const BMPMessage& msg);
};

#endif // BMP_SERVER_H
