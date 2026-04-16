#!/usr/bin/env python3
"""

BMP (BGP Monitoring Protocol) Client Test Utility
(C) ntop 2026

This script provides a simple BMP client for testing the BMP server implementation.
It can send various BMP messages to the server for testing purposes.

Usage:
    python3 test_bmp_client.py [host] [port]
    
Example:
    python3 test_bmp_client.py localhost 11019
"""

import socket
import struct
import time
import sys
from typing import List, Tuple
from datetime import datetime

# BMP Constants
BMP_VERSION = 3
BMP_MARKER = b'\xff' * 16
BMP_MARKER_SIZE = 16

# Message Types
BMP_MSG_ROUTE_MONITORING = 0
BMP_MSG_STATS_REPORT = 1
BMP_MSG_PEER_DOWN = 2
BMP_MSG_PEER_UP = 3
BMP_MSG_INITIATION = 4
BMP_MSG_TERMINATION = 5

# Peer Types
BMP_PEER_GLOBAL = 0
BMP_PEER_LOCAL = 1

# Peer Flags
BMP_PEER_FLAG_V6 = 0x80  # IPv6
BMP_PEER_FLAG_L = 0x40   # 4-byte AS Number
BMP_PEER_FLAG_A = 0x20   # Adj-RIB-In used


class BMPMessage:
    """Base class for BMP messages"""
    
    def __init__(self, msg_type: int):
        self.msg_type = msg_type
        self.payload = b''
    
    def serialize(self) -> bytes:
        """Serialize the message to wire format"""
        # Calculate total length (header + payload)
        total_length = BMP_MARKER_SIZE + 3 + len(self.payload)  # 16 + 2 (length) + 1 (type) + payload
        
        # Build header
        header = BMP_MARKER
        header += struct.pack('!H', total_length)  # Length (network byte order)
        header += struct.pack('!B', self.msg_type)  # Message type
        
        return header + self.payload


class BMPInitiationMessage(BMPMessage):
    """BMP Initiation Message"""
    
    def __init__(self):
        super().__init__(BMP_MSG_INITIATION)
        self.add_tlv(0, b"Test BMP Client - Python Implementation")
        self.add_tlv(1, b"test-client-1.0")
    
    def add_tlv(self, tlv_type: int, value: bytes):
        """Add a TLV (Type-Length-Value) to the initiation message"""
        tlv_header = struct.pack('!HH', tlv_type, len(value))
        self.payload += tlv_header + value


class BMPTerminationMessage(BMPMessage):
    """BMP Termination Message"""
    
    def __init__(self, reason: int = 0):
        super().__init__(BMP_MSG_TERMINATION)
        # Add termination reason TLV
        tlv_header = struct.pack('!HH', 0, 2)  # Type 0, Length 2
        self.payload += tlv_header + struct.pack('!H', reason)


class BMPPeerUpMessage(BMPMessage):
    """BMP Peer Up Message"""
    
    def __init__(self, peer_type: int = BMP_PEER_GLOBAL, 
                 peer_flags: int = BMP_PEER_FLAG_L,
                 peer_router_id: str = "1.1.1.1",
                 peer_asn: int = 65000,
                 peer_bgp_id: str = "1.1.1.1",
                 peer_address: str = "192.168.1.1"):
        super().__init__(BMP_MSG_PEER_UP)
        
        # Build peer header
        self.payload += struct.pack('!B', peer_type)  # Peer type
        self.payload += struct.pack('!B', peer_flags)  # Peer flags
        
        # Convert IP addresses to binary
        router_id_bin = struct.unpack('!I', socket.inet_aton(peer_router_id))[0]
        bgp_id_bin = struct.unpack('!I', socket.inet_aton(peer_bgp_id))[0]
        
        self.payload += struct.pack('!I', router_id_bin)  # Router ID
        self.payload += struct.pack('!I', peer_asn)  # Peer AS
        self.payload += struct.pack('!I', bgp_id_bin)  # BGP ID
        
        # Timestamp
        now = time.time()
        timestamp_sec = int(now)
        timestamp_usec = int((now - timestamp_sec) * 1000000)
        self.payload += struct.pack('!I', timestamp_sec)  # Timestamp seconds
        self.payload += struct.pack('!I', timestamp_usec)  # Timestamp microseconds
        
        # Peer address (IPv4 or IPv6)
        if peer_flags & BMP_PEER_FLAG_V6:
            # IPv6 address
            self.payload += socket.inet_pton(socket.AF_INET6, peer_address)
        else:
            # IPv4 address
            self.payload += socket.inet_aton(peer_address)
        
        # Peer up message body (BGP Open messages - simplified)
        # This would normally contain the BGP OPEN messages exchanged
        # For testing, we'll add minimal placeholders
        self.payload += b'\x00\x00'  # Placeholder for BGP messages


class BMPClient:
    """BMP Client for testing"""
    
    def __init__(self, host: str = "localhost", port: int = 11019):
        self.host = host
        self.port = port
        self.socket = None
        self.connected = False
    
    def connect(self) -> bool:
        """Connect to BMP server"""
        try:
            self.socket = socket.socket(socket.AF_INET, socket.SOCK_STREAM)
            self.socket.connect((self.host, self.port))
            self.connected = True
            print(f"[+] Connected to {self.host}:{self.port}")
            return True
        except Exception as e:
            print(f"[-] Connection failed: {e}")
            return False
    
    def send_message(self, message: BMPMessage) -> bool:
        """Send a BMP message to the server"""
        if not self.connected:
            print("[-] Not connected to server")
            return False
        
        try:
            data = message.serialize()
            self.socket.sendall(data)
            print(f"[+] Sent {message.msg_type} message ({len(data)} bytes)")
            return True
        except Exception as e:
            print(f"[-] Failed to send message: {e}")
            return False
    
    def receive_message(self, timeout: float = 5.0) -> bytes:
        """Receive a BMP message from the server"""
        if not self.connected:
            print("[-] Not connected to server")
            return b''
        
        try:
            self.socket.settimeout(timeout)
            data = self.socket.recv(4096)
            if data:
                print(f"[+] Received {len(data)} bytes")
                return data
            else:
                print("[-] Connection closed by server")
                self.connected = False
                return b''
        except socket.timeout:
            print("[!] Receive timeout")
            return b''
        except Exception as e:
            print(f"[-] Receive failed: {e}")
            return False
    
    def disconnect(self):
        """Disconnect from server"""
        if self.connected:
            try:
                # Send termination message
                term_msg = BMPTerminationMessage(reason=1)
                self.send_message(term_msg)
                
                # Close socket
                self.socket.close()
                self.connected = False
                print("[+] Disconnected from server")
            except Exception as e:
                print(f"[-] Disconnect error: {e}")
    
    def run_test_sequence(self):
        """Run a sequence of test messages"""
        if not self.connected:
            print("[-] Not connected")
            return
        
        print("\n[*] Running test sequence...\n")
        
        # Step 1: Send initiation message
        print("[1] Sending Initiation Message")
        init_msg = BMPInitiationMessage()
        self.send_message(init_msg)
        
        # Receive server's initiation message
        print("[*] Waiting for server initiation message...")
        self.receive_message(timeout=2.0)
        time.sleep(0.5)
        
        # Step 2: Send Peer Up message
        print("\n[2] Sending Peer Up Message")
        peer_up = BMPPeerUpMessage(
            peer_type=BMP_PEER_GLOBAL,
            peer_flags=BMP_PEER_FLAG_L,
            peer_router_id="10.0.0.1",
            peer_asn=65000,
            peer_bgp_id="10.0.0.1",
            peer_address="192.168.100.1"
        )
        self.send_message(peer_up)
        time.sleep(0.5)
        
        # Step 3: Send another Peer Up message (different peer)
        print("\n[3] Sending another Peer Up Message")
        peer_up2 = BMPPeerUpMessage(
            peer_type=BMP_PEER_GLOBAL,
            peer_flags=BMP_PEER_FLAG_L,
            peer_router_id="10.0.0.2",
            peer_asn=65001,
            peer_bgp_id="10.0.0.2",
            peer_address="192.168.100.2"
        )
        self.send_message(peer_up2)
        time.sleep(0.5)
        
        # Step 4: Send Termination message
        print("\n[4] Sending Termination Message")
        term_msg = BMPTerminationMessage(reason=1)  # Reason: Administratively Down
        self.send_message(term_msg)
        
        print("\n[*] Test sequence complete")


def main():
    """Main function"""
    # Parse arguments
    host = sys.argv[1] if len(sys.argv) > 1 else "localhost"
    port = int(sys.argv[2]) if len(sys.argv) > 2 else 11019
    
    print("=== BMP Client Test Utility ===")
    print(f"Target: {host}:{port}\n")
    
    # Create and run client
    client = BMPClient(host, port)
    
    if client.connect():
        try:
            client.run_test_sequence()
        except KeyboardInterrupt:
            print("\n[!] Interrupted by user")
        finally:
            client.disconnect()
    else:
        print("[-] Failed to connect to server")
        sys.exit(1)


if __name__ == "__main__":
    main()
