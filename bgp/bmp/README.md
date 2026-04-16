# BGP Passive Client in C

A fully-featured, production-ready BGP passive client implementation in C that receives and parses BGP UPDATE messages for both IPv4 and IPv6, storing routes in a complete RIB structure.

## Features

### BGP Protocol Support
- **Complete BGP message parsing**: OPEN, UPDATE, KEEPALIVE, NOTIFICATION
- **Full IPv4 support**: Standard BGP NLRI parsing with variable-length prefixes
- **Full IPv6 support**: MP-BGP (RFC 4760) with AFI/SAFI handling
- **Comprehensive attribute parsing**:
  - AS_PATH (including AS4_PATH for 32-bit ASNs)
  - NEXT_HOP (IPv4 and IPv6 via MP_REACH_NLRI)
  - ORIGIN, MED, LOCAL_PREF
  - COMMUNITY, LARGE_COMMUNITY, EXTENDED_COMMUNITY
  - ATOMIC_AGGREGATE, AGGREGATOR
  - ORIGINATOR_ID, CLUSTER_LIST
  - Custom attributes via extended parsing

### Data Structures
- **Fully parsed structures** for all BGP data types
- **RIB (Routing Information Base)** with separate IPv4 and IPv6 tables
- **Dynamic memory management** with automatic capacity expansion
- **Efficient prefix comparison** for deduplication and updates

### Operational Features
- **Passive operation**: No route injection or manipulation
- **Non-blocking I/O** with select()-based multiplexing
- **Robust buffer management** for partial message handling
- **Graceful error handling** with BGP marker validation
- **Session state tracking**
- **Real-time statistics** on route updates

## Building

### Requirements
- GCC or compatible C compiler
- POSIX-compliant system (Linux, BSD, macOS, etc.)
- Standard C library with socket support

### Compilation

```bash
make
```

Or manually:

```bash
gcc -Wall -Wextra -O2 -g -std=c99 -D_POSIX_C_SOURCE=200809L -c bgp.c
gcc -Wall -Wextra -O2 -g -std=c99 -D_POSIX_C_SOURCE=200809L -c bgp_client.c
gcc -Wall -Wextra -O2 -g -o bgp_client bgp.o bgp_client.o
```

### Clean Build

```bash
make clean
make
```

## Usage

### Basic Usage

```bash
./bgp_client <peer_ip> <local_asn> [local_ip]
```

### Examples

Connect to a BGP router at 10.0.0.1 with local ASN 65001:

```bash
./bgp_client 10.0.0.1 65001
```

Bind to a specific local IP address:

```bash
./bgp_client 10.0.0.1 65001 10.0.0.2
```

## Output Example

```
[*] BGP Passive Client
[*] Peer: 10.0.0.1, Local ASN: 65001
[*] Connecting to 10.0.0.1:179...
[*] RIB created with capacity for 10000 routes
[+] OPEN: peer_asn=65000, hold_time=180, bgp_id=10.0.0.1
[+] IPv4 NLRI: 192.0.2.0/24 from AS65000 via 10.0.0.1
    AS_PATH: 65000 65001
    Origin: IGP
    Local_Pref: 100
    Communities: 65000:100
[+] IPv4 NLRI: 198.51.100.0/24 from AS65000 via 10.0.0.1
    AS_PATH: 65000 65002
    Origin: IGP
[+] IPv6 NLRI: 2001:db8::/32 from AS65000 via 2001:db8::1
    AS_PATH: 65000
    Origin: IGP

[*] KEEPALIVE

[-] IPv4 Withdrawn: 192.0.2.0/24

[*] ========== Final Statistics ==========
[*] IPv4 Routes in RIB: 1
[*] IPv6 Routes in RIB: 1
[*] IPv4 Updates received: 2
[*] IPv6 Updates received: 1
[*] Session Duration: 45 seconds
[*] ======================================
```

