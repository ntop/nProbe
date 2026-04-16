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
#include <chrono>
#include <signal.h>

// Global server instance for signal handling
BMPServer* g_server = nullptr;

void signalHandler(int signal) {
    if (signal == SIGINT || signal == SIGTERM) {
        std::cout << "\nShutting down server..." << std::endl;
        if (g_server) {
            g_server->stop();
        }
        exit(0);
    }
}

int main(int argc, char* argv[]) {
    uint16_t port = BMP_DEFAULT_PORT;
    
    // Parse command line arguments
    if (argc > 1) {
        port = static_cast<uint16_t>(std::stoi(argv[1]));
    }
    
    std::cout << "=== BMP Server Implementation ===" << std::endl;
    std::cout << "Starting BMP server on port " << port << std::endl;
    
    // Create and start server
    BMPServer server(port);
    g_server = &server;
    
    // Set up signal handlers
    signal(SIGINT, signalHandler);
    signal(SIGTERM, signalHandler);
    
    if (!server.start()) {
        std::cerr << "Failed to start server" << std::endl;
        return 1;
    }
    
    // Add some example peers
    auto peer1 = server.addPeer("192.168.1.1", 179, 65000);
    auto peer2 = server.addPeer("192.168.1.2", 179, 65001);
    
    std::cout << "\nAdded example peers:" << std::endl;
    std::cout << "  Peer 1: " << peer1->getAddress() << " AS" << peer1->getASN() << std::endl;
    std::cout << "  Peer 2: " << peer2->getAddress() << " AS" << peer2->getASN() << std::endl;
    
    // Main loop - print statistics every 10 seconds
    std::cout << "\nServer is running. Waiting for BMP connections..." << std::endl;
    std::cout << "Press Ctrl+C to shutdown.\n" << std::endl;
    
    while (server.isRunning()) {
        std::this_thread::sleep_for(std::chrono::seconds(10));
        
        if (server.isRunning()) {
            std::cout << "--- Statistics ---" << std::endl;
            std::cout << "Connected peers: " << server.getPeerCount() << std::endl;
            std::cout << "Messages received: " << server.getMessagesReceived() << std::endl;
            std::cout << "Messages sent: " << server.getMessagesSent() << std::endl;
            std::cout << std::endl;
        }
    }
    
    std::cout << "Server shutdown complete" << std::endl;
    return 0;
}
