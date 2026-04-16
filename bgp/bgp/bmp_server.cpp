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

#include "bmp_server.h"
#include <iostream>
#include <thread>
#include <mutex>
#include <condition_variable>
#include <algorithm>
#include <cstring>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <unistd.h>
#include <fcntl.h>
#include <signal.h>

// ============================================================================
// BMPMessage Implementation
// ============================================================================

BMPMessage::BMPMessage(BMPMessageType type)
    : type_(type) {
    // Initialize BMP marker (16 0xFF bytes)
}

std::vector<uint8_t> BMPMessage::serialize() const {
    std::vector<uint8_t> buffer;
    
    // Calculate total message length (header + payload)
    uint16_t total_length = sizeof(BMPCommonHeader) + payload_.size();
    
    // Create and fill common header
    BMPCommonHeader header;
    std::memset(header.marker, 0xFF, BMP_MARKER_SIZE);
    header.length = htons(total_length);
    header.msg_type = static_cast<uint8_t>(type_);
    
    // Append header to buffer
    const uint8_t* header_ptr = reinterpret_cast<const uint8_t*>(&header);
    buffer.insert(buffer.end(), header_ptr, header_ptr + sizeof(BMPCommonHeader));
    
    // Append payload
    buffer.insert(buffer.end(), payload_.begin(), payload_.end());
    
    return buffer;
}

void BMPMessage::appendPayload(const uint8_t* data, size_t len) {
    if (data && len > 0) {
        payload_.insert(payload_.end(), data, data + len);
    }
}

void BMPMessage::appendPayload(const std::vector<uint8_t>& data) {
    payload_.insert(payload_.end(), data.begin(), data.end());
}

// ============================================================================
// BMPInitiationMessage Implementation
// ============================================================================

BMPInitiationMessage::BMPInitiationMessage()
    : BMPMessage(BMPMessageType::INITIATION) {
}

void BMPInitiationMessage::addSystemDescription(const std::string& sysDescr) {
    addTLV(0, sysDescr);  // Type 0 = System Description
}

void BMPInitiationMessage::addSystemName(const std::string& sysName) {
    addTLV(1, sysName);   // Type 1 = System Name
}

void BMPInitiationMessage::addTLV(uint16_t type, const std::string& value) {
    BMPInitiationTLV tlv;
    tlv.tlv_type = htons(type);
    tlv.tlv_length = htons(value.length());
    
    // Append TLV header
    const uint8_t* tlv_ptr = reinterpret_cast<const uint8_t*>(&tlv);
    appendPayload(tlv_ptr, sizeof(BMPInitiationTLV));
    
    // Append TLV value
    appendPayload(reinterpret_cast<const uint8_t*>(value.c_str()), value.length());
}

// ============================================================================
// BMPRoute Implementation
// ============================================================================

BMPRoute::BMPRoute(const std::string& prefix, uint8_t prefixLen, uint32_t as)
    : prefix_(prefix), prefix_len_(prefixLen), as_(as) {
}

// ============================================================================
// BMPPeer Implementation
// ============================================================================

BMPPeer::BMPPeer(const std::string& address, uint16_t port, uint32_t asn)
    : address_(address), port_(port), asn_(asn), router_id_(0), bgp_id_(0) {
    updateTimestamp();
}

void BMPPeer::addRoute(const BMPRoute& route) {
    routes_.push_back(route);
    updateTimestamp();
}

// ============================================================================
// BMPServer Implementation
// ============================================================================

BMPServer::BMPServer(uint16_t port)
    : port_(port), server_socket_(-1), running_(false),
      max_connections_(100), buffer_size_(65536),
      messages_received_(0), messages_sent_(0) {
}

BMPServer::~BMPServer() {
    stop();
}

bool BMPServer::start() {
    if (running_) {
        std::cerr << "Server is already running" << std::endl;
        return false;
    }
    
    // Create socket
    server_socket_ = socket(AF_INET, SOCK_STREAM, 0);
    if (server_socket_ < 0) {
        std::cerr << "Failed to create socket: " << strerror(errno) << std::endl;
        return false;
    }
    
    // Allow socket reuse
    int opt = 1;
    if (setsockopt(server_socket_, SOL_SOCKET, SO_REUSEADDR, &opt, sizeof(opt)) < 0) {
        std::cerr << "Failed to set socket options: " << strerror(errno) << std::endl;
        close(server_socket_);
        return false;
    }
    
    // Bind socket
    struct sockaddr_in server_addr;
    std::memset(&server_addr, 0, sizeof(server_addr));
    server_addr.sin_family = AF_INET;
    server_addr.sin_addr.s_addr = htonl(INADDR_ANY);
    server_addr.sin_port = htons(port_);
    
    if (bind(server_socket_, (struct sockaddr*)&server_addr, sizeof(server_addr)) < 0) {
        std::cerr << "Failed to bind socket: " << strerror(errno) << std::endl;
        close(server_socket_);
        return false;
    }
    
    // Listen for connections
    if (listen(server_socket_, max_connections_) < 0) {
        std::cerr << "Failed to listen: " << strerror(errno) << std::endl;
        close(server_socket_);
        return false;
    }
    
    running_ = true;
    std::cout << "BMP Server started on port " << port_ << std::endl;
    
    // Start accepting connections in a separate thread
    std::thread accept_thread(&BMPServer::acceptConnections, this);
    accept_thread.detach();
    
    return true;
}

void BMPServer::stop() {
    if (!running_) {
        return;
    }
    
    running_ = false;
    
    if (server_socket_ >= 0) {
        close(server_socket_);
        server_socket_ = -1;
    }
    
    std::cout << "BMP Server stopped" << std::endl;
}

void BMPServer::acceptConnections() {
    struct sockaddr_in client_addr;
    socklen_t client_addr_len = sizeof(client_addr);
    
    while (running_) {
        int client_socket = accept(server_socket_, (struct sockaddr*)&client_addr,
                                   &client_addr_len);
        
        if (client_socket < 0) {
            if (running_) {
                std::cerr << "Accept failed: " << strerror(errno) << std::endl;
            }
            continue;
        }
        
        std::string client_addr_str = inet_ntoa(client_addr.sin_addr);
        std::cout << "New client connection from " << client_addr_str << std::endl;
        
        // Handle client in a separate thread
        std::thread client_thread(&BMPServer::handleClient, this, client_socket, 
                                 client_addr_str);
        client_thread.detach();
    }
}

void BMPServer::handleClient(int client_socket, const std::string& client_addr) {
    try {
        // Send initiation message
        BMPInitiationMessage init_msg;
        init_msg.addSystemDescription("BMP Server Implementation");
        init_msg.addSystemName("bmp-server-1.0");
        
        auto init_data = init_msg.serialize();
        if (send(client_socket, init_data.data(), init_data.size(), 0) < 0) {
            std::cerr << "Failed to send initiation message: " << strerror(errno) << std::endl;
            close(client_socket);
            return;
        }
        messages_sent_++;
        
        std::cout << "Sent initiation message to " << client_addr << std::endl;
        
        // Main message loop
        while (running_) {
            std::vector<uint8_t> msg = receiveMessage(client_socket);
            
            if (msg.empty()) {
                break;  // Connection closed
            }
            
            messages_received_++;
            
            // Parse and handle the message
            if (msg.size() >= sizeof(BMPCommonHeader)) {
                const BMPCommonHeader* header = 
                    reinterpret_cast<const BMPCommonHeader*>(msg.data());
                
                BMPMessageType msg_type = static_cast<BMPMessageType>(header->msg_type);
                uint16_t msg_length = ntohs(header->length);
                
                std::cout << "Received message type " << static_cast<int>(msg_type)
                         << " (length: " << msg_length << ") from " << client_addr << std::endl;
                
                // Handle different message types
                switch (msg_type) {
                    case BMPMessageType::ROUTE_MONITORING:
                        std::cout << "  Route Monitoring message" << std::endl;
                        break;
                    case BMPMessageType::STATS_REPORT:
                        std::cout << "  Statistics Report message" << std::endl;
                        break;
                    case BMPMessageType::PEER_DOWN:
                        std::cout << "  Peer Down message" << std::endl;
                        break;
                    case BMPMessageType::PEER_UP:
                        std::cout << "  Peer Up message" << std::endl;
                        break;
                    case BMPMessageType::TERMINATION:
                        std::cout << "  Termination message from " << client_addr << std::endl;
                        close(client_socket);
                        return;
                    default:
                        std::cerr << "  Unknown message type: " << static_cast<int>(msg_type) << std::endl;
                }
            }
        }
    } catch (const std::exception& e) {
        std::cerr << "Error handling client " << client_addr << ": " << e.what() << std::endl;
    }
    
    close(client_socket);
    std::cout << "Client connection closed: " << client_addr << std::endl;
}

std::vector<uint8_t> BMPServer::receiveMessage(int socket) {
    std::vector<uint8_t> buffer(buffer_size_);
    
    // Receive at least the header
    ssize_t bytes = recv(socket, buffer.data(), buffer_size_, 0);
    
    if (bytes <= 0) {
        return std::vector<uint8_t>();  // Connection closed or error
    }
    
    buffer.resize(bytes);
    return buffer;
}

bool BMPServer::sendMessage(int socket, const BMPMessage& msg) {
    auto data = msg.serialize();
    ssize_t bytes = send(socket, data.data(), data.size(), 0);
    
    if (bytes < 0) {
        std::cerr << "Failed to send message: " << strerror(errno) << std::endl;
        return false;
    }
    
    messages_sent_++;
    return true;
}

std::shared_ptr<BMPPeer> BMPServer::addPeer(const std::string& address, 
                                            uint16_t port, uint32_t asn) {
    auto peer = std::make_shared<BMPPeer>(address, port, asn);
    peers_.push_back(peer);
    return peer;
}

void BMPServer::removePeer(const std::string& address) {
    peers_.erase(
        std::remove_if(peers_.begin(), peers_.end(),
                      [&address](const std::shared_ptr<BMPPeer>& p) {
                          return p->getAddress() == address;
                      }),
        peers_.end()
    );
}

std::shared_ptr<BMPPeer> BMPServer::getPeer(const std::string& address) const {
    for (const auto& peer : peers_) {
        if (peer->getAddress() == address) {
            return peer;
        }
    }
    return nullptr;
}
