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

#ifndef BGP_H
#define BGP_H

#include <stdint.h>
#include <netinet/in.h>
#include <time.h>

/* BGP Constants */
#define BGP_PORT 179
#define BGP_VERSION 4
#define BGP_MARKER_SIZE 16
#define BGP_HEADER_SIZE 19
#define BGP_MAX_MESSAGE_SIZE 65535

/* BGP Message Types */
#define BGP_MSG_OPEN 1
#define BGP_MSG_UPDATE 2
#define BGP_MSG_NOTIFICATION 3
#define BGP_MSG_KEEPALIVE 4
#define BGP_MSG_ROUTE_REFRESH 5

/* BGP Attribute Types */
#define BGP_ATTR_ORIGIN 1
#define BGP_ATTR_AS_PATH 2
#define BGP_ATTR_NEXT_HOP 3
#define BGP_ATTR_MULTI_EXIT_DISC 4
#define BGP_ATTR_LOCAL_PREF 5
#define BGP_ATTR_ATOMIC_AGGREGATE 6
#define BGP_ATTR_AGGREGATOR 7
#define BGP_ATTR_COMMUNITY 8
#define BGP_ATTR_ORIGINATOR_ID 9
#define BGP_ATTR_CLUSTER_LIST 10
#define BGP_ATTR_MP_REACH_NLRI 14
#define BGP_ATTR_MP_UNREACH_NLRI 15
#define BGP_ATTR_EXTENDED_COMMUNITY 16
#define BGP_ATTR_AS4_PATH 17
#define BGP_ATTR_AS4_AGGREGATOR 18
#define BGP_ATTR_LARGE_COMMUNITY 32

/* Origin types */
#define ORIGIN_IGP 0
#define ORIGIN_EGP 1
#define ORIGIN_INCOMPLETE 2

/* AFI/SAFI values */
#define AFI_IPV4 1
#define AFI_IPV6 2
#define SAFI_UNICAST 1
#define SAFI_MULTICAST 2

/* Maximum number of elements in structures */
#define MAX_NLRI 1000
#define MAX_WITHDRAWN 1000
#define MAX_AS_PATH_SEGMENTS 50
#define MAX_AS_PATH_LENGTH 200
#define MAX_COMMUNITIES 100
#define MAX_ATTRIBUTES 256
#define MAX_ROUTE_TARGETS 10
#define MAX_LARGE_COMMUNITIES 100

/* ===== Data Structures ===== */

/* IPv4 NLRI */
typedef struct {
    uint32_t prefix;
    uint8_t prefix_len;
} bgp_ipv4_nlri_t;

/* IPv6 NLRI */
typedef struct {
    uint8_t prefix[16];
    uint8_t prefix_len;
} bgp_ipv6_nlri_t;

/* AS Path Segment */
typedef struct {
    uint8_t segment_type;  /* AS_SET (1) or AS_SEQUENCE (2) */
    uint16_t as_list[MAX_AS_PATH_LENGTH];
    uint16_t as_count;
} bgp_as_path_segment_t;

/* AS Path */
typedef struct {
    bgp_as_path_segment_t segments[MAX_AS_PATH_SEGMENTS];
    uint8_t segment_count;
} bgp_as_path_t;

/* Standard Community */
typedef struct {
    uint32_t value;  /* 32-bit community value (asn:value) */
} bgp_community_t;

/* Communities */
typedef struct {
    bgp_community_t communities[MAX_COMMUNITIES];
    uint16_t count;
} bgp_communities_t;

/* Large Community */
typedef struct {
    uint32_t global_admin;  /* Global Administrator (usually ASN) */
    uint32_t local_data1;   /* Local Data 1 */
    uint32_t local_data2;   /* Local Data 2 */
} bgp_large_community_t;

/* Large Communities */
typedef struct {
    bgp_large_community_t communities[MAX_LARGE_COMMUNITIES];
    uint16_t count;
} bgp_large_communities_t;

/* Extended Community */
typedef struct {
    uint8_t type;
    uint8_t subtype;
    uint8_t value[6];
} bgp_extended_community_t;

/* BGP Attributes */
typedef struct {
    /* Flags and types */
    int origin;
    uint32_t med;
    uint32_t local_pref;
    uint32_t originator_id;
    
    /* AS Path */
    bgp_as_path_t as_path;
    bgp_as_path_t as4_path;
    
    /* Next Hop */
    struct in_addr ipv4_nexthop;
    struct in6_addr ipv6_nexthop;
    uint8_t ipv6_nexthop_present;
    
    /* Communities */
    bgp_communities_t communities;
    bgp_large_communities_t large_communities;
    bgp_extended_community_t extended_communities[MAX_ROUTE_TARGETS];
    uint8_t extended_community_count;
    
    /* Cluster info */
    uint32_t cluster_list[MAX_ROUTE_TARGETS];
    uint16_t cluster_list_len;
    
    /* Aggregator info */
    uint16_t aggregator_asn;
    struct in_addr aggregator_ip;
    uint32_t aggregator_asn4;
    
    /* Flags */
    uint8_t atomic_aggregate;
    uint8_t optional;
    uint8_t transitive;
    uint8_t partial;
    uint8_t extended_length;
} bgp_attr_t;

/* IPv4 Update */
typedef struct {
    bgp_ipv4_nlri_t withdrawn[MAX_WITHDRAWN];
    uint16_t withdrawn_count;
    
    bgp_ipv4_nlri_t nlri[MAX_NLRI];
    uint16_t nlri_count;
    
    bgp_attr_t attributes;
    uint8_t has_attributes;
} bgp_ipv4_update_t;

/* IPv6 Update */
typedef struct {
    bgp_ipv6_nlri_t withdrawn[MAX_WITHDRAWN];
    uint16_t withdrawn_count;
    
    bgp_ipv6_nlri_t nlri[MAX_NLRI];
    uint16_t nlri_count;
    
    bgp_attr_t attributes;
    uint8_t has_attributes;
} bgp_ipv6_update_t;

/* BGP Message Header */
typedef struct {
    uint8_t marker[BGP_MARKER_SIZE];
    uint16_t length;
    uint8_t type;
} bgp_msg_header_t;

/* BGP OPEN Message */
typedef struct {
    uint8_t version;
    uint16_t my_as;
    uint16_t hold_time;
    uint32_t bgp_id;
    uint8_t opt_param_len;
    uint8_t *opt_params;
} bgp_open_msg_t;

/* BGP Keepalive Message */
typedef struct {
    /* Empty - just the header */
    uint32_t timestamp;
} bgp_keepalive_msg_t;

/* BGP Update Message (generic holder) */
typedef struct {
    bgp_ipv4_update_t ipv4_update;
    bgp_ipv6_update_t ipv6_update;
    uint8_t has_ipv4;
    uint8_t has_ipv6;
    time_t received_time;
} bgp_update_msg_t;

/* BGP Notification Message */
typedef struct {
    uint8_t error_code;
    uint8_t error_subcode;
    uint8_t *data;
    uint16_t data_len;
} bgp_notification_msg_t;

/* BGP Session State */
typedef enum {
    BGP_STATE_IDLE = 1,
    BGP_STATE_CONNECT = 2,
    BGP_STATE_ACTIVE = 3,
    BGP_STATE_OPENSENT = 4,
    BGP_STATE_OPENCONFIRM = 5,
    BGP_STATE_ESTABLISHED = 6
} bgp_state_t;

/* BGP Session */
typedef struct {
    int socket;
    struct in_addr local_addr;
    struct in_addr peer_addr;
    uint16_t peer_port;
    
    uint16_t local_asn;
    uint16_t peer_asn;
    uint32_t local_bgp_id;
    uint32_t peer_bgp_id;
    
    uint16_t hold_time;
    uint16_t keepalive_interval;
    time_t last_keepalive_sent;
    time_t last_keepalive_received;
    
    bgp_state_t state;
    
    /* Buffers */
    uint8_t *recv_buf;
    uint16_t recv_buf_len;
    uint16_t recv_buf_offset;
} bgp_session_t;

/* RIB Entry for IPv4 */
typedef struct {
    bgp_ipv4_nlri_t prefix;
    bgp_attr_t attributes;
    uint32_t peer_asn;
    time_t last_update;
} bgp_rib_ipv4_entry_t;

/* RIB Entry for IPv6 */
typedef struct {
    bgp_ipv6_nlri_t prefix;
    bgp_attr_t attributes;
    uint32_t peer_asn;
    time_t last_update;
} bgp_rib_ipv6_entry_t;

/* RIB */
typedef struct {
    bgp_rib_ipv4_entry_t *ipv4_routes;
    uint32_t ipv4_route_count;
    uint32_t ipv4_route_capacity;
    
    bgp_rib_ipv6_entry_t *ipv6_routes;
    uint32_t ipv6_route_count;
    uint32_t ipv6_route_capacity;
} bgp_rib_t;

/* ===== Function Prototypes ===== */

/* Session Management */
bgp_session_t *bgp_session_new(const char *peer_addr, uint16_t peer_port,
                                const char *local_addr, uint16_t local_asn,
                                uint32_t local_bgp_id);
void bgp_session_free(bgp_session_t *session);
int bgp_session_connect(bgp_session_t *session);
int bgp_session_disconnect(bgp_session_t *session);

/* Message Parsing */
int bgp_parse_message(bgp_session_t *session, uint8_t **msg_data, uint16_t *msg_len);
int bgp_parse_update(const uint8_t *data, uint16_t len, bgp_update_msg_t *update,
                     uint16_t peer_asn);
int bgp_parse_open(const uint8_t *data, uint16_t len, bgp_open_msg_t *open);
int bgp_parse_keepalive(const uint8_t *data, uint16_t len, bgp_keepalive_msg_t *ka);
int bgp_parse_notification(const uint8_t *data, uint16_t len,
                           bgp_notification_msg_t *notif);

/* Attribute Parsing */
int bgp_parse_attributes(const uint8_t *data, uint16_t len, bgp_attr_t *attr);
int bgp_parse_as_path(const uint8_t *data, uint16_t len, bgp_as_path_t *as_path);
int bgp_parse_communities(const uint8_t *data, uint16_t len, bgp_communities_t *communities);
int bgp_parse_large_communities(const uint8_t *data, uint16_t len,
                                bgp_large_communities_t *communities);

/* NLRI Parsing */
int bgp_parse_nlri(const uint8_t *data, uint16_t len, bgp_ipv4_nlri_t *nlri,
                   uint16_t *nlri_count);
int bgp_parse_nlri_ipv6(const uint8_t *data, uint16_t len, bgp_ipv6_nlri_t *nlri,
                        uint16_t *nlri_count);

/* RIB Management */
bgp_rib_t *bgp_rib_new(uint32_t initial_capacity);
void bgp_rib_free(bgp_rib_t *rib);
int bgp_rib_add_ipv4(bgp_rib_t *rib, const bgp_rib_ipv4_entry_t *entry);
int bgp_rib_add_ipv6(bgp_rib_t *rib, const bgp_rib_ipv6_entry_t *entry);
int bgp_rib_remove_ipv4(bgp_rib_t *rib, const bgp_ipv4_nlri_t *prefix);
int bgp_rib_remove_ipv6(bgp_rib_t *rib, const bgp_ipv6_nlri_t *prefix);

/* Utility Functions */
const char *bgp_state_str(bgp_state_t state);
const char *bgp_origin_str(int origin);
void bgp_print_ipv4_prefix(const bgp_ipv4_nlri_t *nlri);
void bgp_print_ipv6_prefix(const bgp_ipv6_nlri_t *nlri);
void bgp_print_as_path(const bgp_as_path_t *as_path);
void bgp_print_communities(const bgp_communities_t *communities);

#endif /* BGP_H */
