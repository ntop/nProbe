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

#include "bgp.h"
#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include <unistd.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <fcntl.h>
#include <errno.h>

/* ===== Session Management ===== */

bgp_session_t *bgp_session_new(const char *peer_addr, uint16_t peer_port,
                                const char *local_addr, uint16_t local_asn,
                                uint32_t local_bgp_id) {
    bgp_session_t *session = malloc(sizeof(bgp_session_t));
    if (!session) return NULL;
    
    memset(session, 0, sizeof(bgp_session_t));
    
    session->socket = -1;
    session->peer_port = peer_port;
    session->local_asn = local_asn;
    session->local_bgp_id = local_bgp_id;
    session->state = BGP_STATE_IDLE;
    session->hold_time = 180;
    session->keepalive_interval = 60;
    
    /* Parse peer address */
    if (inet_pton(AF_INET, peer_addr, &session->peer_addr) != 1) {
        free(session);
        return NULL;
    }
    
    /* Parse local address */
    if (local_addr && inet_pton(AF_INET, local_addr, &session->local_addr) != 1) {
        free(session);
        return NULL;
    }
    
    /* Allocate receive buffer */
    session->recv_buf = malloc(BGP_MAX_MESSAGE_SIZE);
    if (!session->recv_buf) {
        free(session);
        return NULL;
    }
    session->recv_buf_len = 0;
    session->recv_buf_offset = 0;
    
    return session;
}

void bgp_session_free(bgp_session_t *session) {
    if (!session) return;
    
    if (session->socket >= 0) {
        close(session->socket);
    }
    if (session->recv_buf) {
        free(session->recv_buf);
    }
    free(session);
}

int bgp_session_connect(bgp_session_t *session) {
    struct sockaddr_in local_addr, peer_addr;
    int sock;
    
    if (!session) return -1;
    
    /* Create socket */
    sock = socket(AF_INET, SOCK_STREAM, 0);
    if (sock < 0) {
        perror("socket");
        return -1;
    }
    
    /* Set socket to non-blocking */
    int flags = fcntl(sock, F_GETFL, 0);
    if (fcntl(sock, F_SETFL, flags | O_NONBLOCK) < 0) {
        perror("fcntl");
        close(sock);
        return -1;
    }
    
    /* Bind to local address if specified */
    if (session->local_addr.s_addr != 0) {
        memset(&local_addr, 0, sizeof(local_addr));
        local_addr.sin_family = AF_INET;
        local_addr.sin_addr = session->local_addr;
        local_addr.sin_port = 0;  /* Let OS choose port */
        
        if (bind(sock, (struct sockaddr *)&local_addr, sizeof(local_addr)) < 0) {
            perror("bind");
            close(sock);
            return -1;
        }
    }
    
    /* Connect to peer */
    memset(&peer_addr, 0, sizeof(peer_addr));
    peer_addr.sin_family = AF_INET;
    peer_addr.sin_addr = session->peer_addr;
    peer_addr.sin_port = htons(session->peer_port);
    
    if (connect(sock, (struct sockaddr *)&peer_addr, sizeof(peer_addr)) < 0) {
        if (errno != EINPROGRESS) {
            perror("connect");
            close(sock);
            return -1;
        }
    }
    
    session->socket = sock;
    session->state = BGP_STATE_CONNECT;
    session->last_keepalive_sent = time(NULL);
    session->last_keepalive_received = time(NULL);
    
    return 0;
}

int bgp_session_disconnect(bgp_session_t *session) {
    if (!session || session->socket < 0) return -1;
    
    close(session->socket);
    session->socket = -1;
    session->state = BGP_STATE_IDLE;
    
    return 0;
}

/* ===== Helper Functions for Byte Processing ===== */

static uint16_t read_uint16_be(const uint8_t *data) {
    return ((uint16_t)data[0] << 8) | data[1];
}

static uint32_t read_uint32_be(const uint8_t *data) {
    return ((uint32_t)data[0] << 24) | ((uint32_t)data[1] << 16) |
           ((uint32_t)data[2] << 8) | data[3];
}

static void write_uint16_be(uint8_t *data, uint16_t value) {
    data[0] = (value >> 8) & 0xFF;
    data[1] = value & 0xFF;
}

static void write_uint32_be(uint8_t *data, uint32_t value) {
    data[0] = (value >> 24) & 0xFF;
    data[1] = (value >> 16) & 0xFF;
    data[2] = (value >> 8) & 0xFF;
    data[3] = value & 0xFF;
}

/* ===== AS Path Parsing ===== */

int bgp_parse_as_path(const uint8_t *data, uint16_t len, bgp_as_path_t *as_path) {
    uint16_t offset = 0;
    uint8_t segment_count = 0;
    
    if (!data || !as_path || len < 2) return -1;
    
    memset(as_path, 0, sizeof(bgp_as_path_t));
    
    while (offset < len && segment_count < MAX_AS_PATH_SEGMENTS) {
        if (offset + 2 > len) break;
        
        uint8_t segment_type = data[offset];
        uint8_t segment_length = data[offset + 1];
        
        if (segment_type != 1 && segment_type != 2) break;  /* AS_SET=1, AS_SEQUENCE=2 */
        
        offset += 2;
        
        bgp_as_path_segment_t *segment = &as_path->segments[segment_count];
        segment->segment_type = segment_type;
        segment->as_count = 0;
        
        for (uint8_t i = 0; i < segment_length && segment->as_count < MAX_AS_PATH_LENGTH; i++) {
            if (offset + 2 > len) return -1;
            
            uint16_t asn = read_uint16_be(&data[offset]);
            segment->as_list[segment->as_count++] = asn;
            offset += 2;
        }
        
        segment_count++;
    }
    
    as_path->segment_count = segment_count;
    return 0;
}

/* ===== Communities Parsing ===== */

int bgp_parse_communities(const uint8_t *data, uint16_t len, bgp_communities_t *communities) {
    uint16_t offset = 0;
    
    if (!data || !communities) return -1;
    
    memset(communities, 0, sizeof(bgp_communities_t));
    
    /* Each community is 4 bytes */
    while (offset + 4 <= len && communities->count < MAX_COMMUNITIES) {
        communities->communities[communities->count].value = read_uint32_be(&data[offset]);
        communities->count++;
        offset += 4;
    }
    
    return 0;
}

int bgp_parse_large_communities(const uint8_t *data, uint16_t len,
                                bgp_large_communities_t *communities) {
    uint16_t offset = 0;
    
    if (!data || !communities) return -1;
    
    memset(communities, 0, sizeof(bgp_large_communities_t));
    
    /* Each large community is 12 bytes (3x 32-bit values) */
    while (offset + 12 <= len && communities->count < MAX_LARGE_COMMUNITIES) {
        communities->communities[communities->count].global_admin = read_uint32_be(&data[offset]);
        communities->communities[communities->count].local_data1 = read_uint32_be(&data[offset + 4]);
        communities->communities[communities->count].local_data2 = read_uint32_be(&data[offset + 8]);
        communities->count++;
        offset += 12;
    }
    
    return 0;
}

/* ===== NLRI Parsing ===== */

int bgp_parse_nlri(const uint8_t *data, uint16_t len, bgp_ipv4_nlri_t *nlri,
                   uint16_t *nlri_count) {
    uint16_t offset = 0;
    
    if (!data || !nlri || !nlri_count) return -1;
    
    *nlri_count = 0;
    
    while (offset < len && *nlri_count < MAX_NLRI) {
        if (offset >= len) break;
        
        uint8_t prefix_len = data[offset];
        offset++;
        
        if (prefix_len > 32) return -1;
        
        uint8_t bytes_needed = (prefix_len + 7) / 8;
        if (offset + bytes_needed > len) return -1;
        
        uint32_t prefix = 0;
        memcpy(&prefix, &data[offset], bytes_needed);
        
        /* Normalize prefix (zero out host bits) */
        if (prefix_len < 32) {
            uint32_t mask = 0xFFFFFFFFU << (32 - prefix_len);
            prefix = ntohl(prefix);
            prefix &= mask;
            prefix = htonl(prefix);
        }
        
        nlri[*nlri_count].prefix = prefix;
        nlri[*nlri_count].prefix_len = prefix_len;
        (*nlri_count)++;
        
        offset += bytes_needed;
    }
    
    return 0;
}

int bgp_parse_nlri_ipv6(const uint8_t *data, uint16_t len, bgp_ipv6_nlri_t *nlri,
                        uint16_t *nlri_count) {
    uint16_t offset = 0;
    
    if (!data || !nlri || !nlri_count) return -1;
    
    *nlri_count = 0;
    
    while (offset < len && *nlri_count < MAX_NLRI) {
        if (offset >= len) break;
        
        uint8_t prefix_len = data[offset];
        offset++;
        
        if (prefix_len > 128) return -1;
        
        uint8_t bytes_needed = (prefix_len + 7) / 8;
        if (offset + bytes_needed > len) return -1;
        
        memset(nlri[*nlri_count].prefix, 0, 16);
        memcpy(nlri[*nlri_count].prefix, &data[offset], bytes_needed);
        nlri[*nlri_count].prefix_len = prefix_len;
        
        /* Normalize prefix (zero out host bits) */
        if (prefix_len < 128) {
            uint8_t byte_index = prefix_len / 8;
            uint8_t bit_offset = prefix_len % 8;
            
            if (bit_offset > 0) {
                uint8_t mask = 0xFF << (8 - bit_offset);
                nlri[*nlri_count].prefix[byte_index] &= mask;
            }
            
            for (uint8_t i = byte_index + 1; i < 16; i++) {
                nlri[*nlri_count].prefix[i] = 0;
            }
        }
        
        (*nlri_count)++;
        offset += bytes_needed;
    }
    
    return 0;
}

/* ===== Attribute Parsing ===== */

int bgp_parse_attributes(const uint8_t *data, uint16_t len, bgp_attr_t *attr) {
    uint16_t offset = 0;
    
    if (!data || !attr) return -1;
    
    memset(attr, 0, sizeof(bgp_attr_t));
    attr->local_pref = 100;  /* Default local preference */
    
    while (offset < len) {
        if (offset + 2 > len) return -1;
        
        uint8_t flags = data[offset];
        uint8_t attr_type = data[offset + 1];
        offset += 2;
        
        attr->optional = (flags >> 7) & 1;
        attr->transitive = (flags >> 6) & 1;
        attr->partial = (flags >> 5) & 1;
        attr->extended_length = (flags >> 4) & 1;
        
        uint16_t attr_len;
        if (attr->extended_length) {
            if (offset + 2 > len) return -1;
            attr_len = read_uint16_be(&data[offset]);
            offset += 2;
        } else {
            if (offset + 1 > len) return -1;
            attr_len = data[offset];
            offset += 1;
        }
        
        if (offset + attr_len > len) return -1;
        
        const uint8_t *attr_data = &data[offset];
        
        switch (attr_type) {
            case BGP_ATTR_ORIGIN:
                if (attr_len >= 1) {
                    attr->origin = attr_data[0];
                }
                break;
                
            case BGP_ATTR_AS_PATH:
                bgp_parse_as_path(attr_data, attr_len, &attr->as_path);
                break;
                
            case BGP_ATTR_NEXT_HOP:
                if (attr_len >= 4) {
                    memcpy(&attr->ipv4_nexthop, attr_data, 4);
                }
                break;
                
            case BGP_ATTR_MULTI_EXIT_DISC:
                if (attr_len >= 4) {
                    attr->med = read_uint32_be(attr_data);
                }
                break;
                
            case BGP_ATTR_LOCAL_PREF:
                if (attr_len >= 4) {
                    attr->local_pref = read_uint32_be(attr_data);
                }
                break;
                
            case BGP_ATTR_ATOMIC_AGGREGATE:
                attr->atomic_aggregate = 1;
                break;
                
            case BGP_ATTR_AGGREGATOR:
                if (attr_len >= 6) {
                    attr->aggregator_asn = read_uint16_be(attr_data);
                    memcpy(&attr->aggregator_ip, &attr_data[2], 4);
                }
                break;
                
            case BGP_ATTR_COMMUNITY:
                bgp_parse_communities(attr_data, attr_len, &attr->communities);
                break;
                
            case BGP_ATTR_ORIGINATOR_ID:
                if (attr_len >= 4) {
                    attr->originator_id = read_uint32_be(attr_data);
                }
                break;
                
            case BGP_ATTR_CLUSTER_LIST:
                {
                    uint16_t pos = 0;
                    while (pos + 4 <= attr_len && attr->cluster_list_len < MAX_ROUTE_TARGETS) {
                        attr->cluster_list[attr->cluster_list_len] = read_uint32_be(&attr_data[pos]);
                        attr->cluster_list_len++;
                        pos += 4;
                    }
                }
                break;
                
            case BGP_ATTR_MP_REACH_NLRI:
                /* Handled separately in update parsing */
                break;
                
            case BGP_ATTR_MP_UNREACH_NLRI:
                /* Handled separately in update parsing */
                break;
                
            case BGP_ATTR_EXTENDED_COMMUNITY:
                {
                    uint16_t pos = 0;
                    while (pos + 8 <= attr_len && attr->extended_community_count < MAX_ROUTE_TARGETS) {
                        attr->extended_communities[attr->extended_community_count].type = attr_data[pos];
                        attr->extended_communities[attr->extended_community_count].subtype = attr_data[pos + 1];
                        memcpy(attr->extended_communities[attr->extended_community_count].value,
                               &attr_data[pos + 2], 6);
                        attr->extended_community_count++;
                        pos += 8;
                    }
                }
                break;
                
            case BGP_ATTR_AS4_PATH:
                bgp_parse_as_path(attr_data, attr_len, &attr->as4_path);
                break;
                
            case BGP_ATTR_AS4_AGGREGATOR:
                if (attr_len >= 8) {
                    attr->aggregator_asn4 = read_uint32_be(attr_data);
                    memcpy(&attr->aggregator_ip, &attr_data[4], 4);
                }
                break;
                
            case BGP_ATTR_LARGE_COMMUNITY:
                bgp_parse_large_communities(attr_data, attr_len, &attr->large_communities);
                break;
        }
        
        offset += attr_len;
    }
    
    return 0;
}

/* ===== Message Parsing ===== */

int bgp_parse_update(const uint8_t *data, uint16_t len, bgp_update_msg_t *update,
                     uint16_t peer_asn) {
    uint16_t offset = 0;
    
    if (!data || !update || len < 4) return -1;
    
    memset(update, 0, sizeof(bgp_update_msg_t));
    update->received_time = time(NULL);
    
    /* Withdrawn Routes Length */
    uint16_t withdrawn_len = read_uint16_be(&data[offset]);
    offset += 2;
    
    if (offset + withdrawn_len > len) return -1;
    
    /* Parse withdrawn routes (IPv4) */
    if (withdrawn_len > 0) {
        bgp_parse_nlri(&data[offset], withdrawn_len,
                      update->ipv4_update.withdrawn,
                      &update->ipv4_update.withdrawn_count);
    }
    offset += withdrawn_len;
    
    /* Total Path Attribute Length */
    if (offset + 2 > len) return -1;
    uint16_t attr_len = read_uint16_be(&data[offset]);
    offset += 2;
    
    if (offset + attr_len > len) return -1;
    
    /* Parse attributes */
    if (attr_len > 0) {
        bgp_parse_attributes(&data[offset], attr_len, &update->ipv4_update.attributes);
        update->ipv4_update.has_attributes = 1;
        update->ipv6_update.attributes = update->ipv4_update.attributes;
        update->ipv6_update.has_attributes = 1;
    }
    offset += attr_len;
    
    /* Parse NLRI (IPv4) from remaining data */
    uint16_t nlri_len = len - offset;
    if (nlri_len > 0) {
        bgp_parse_nlri(&data[offset], nlri_len,
                      update->ipv4_update.nlri,
                      &update->ipv4_update.nlri_count);
        if (update->ipv4_update.nlri_count > 0) {
            update->has_ipv4 = 1;
        }
    }
    
    /* Check for MP_REACH_NLRI and MP_UNREACH_NLRI in attributes */
    bgp_attr_t *attr = &update->ipv4_update.attributes;
    
    /* Re-parse attributes to extract MP-BGP */
    offset = attr_len ? offset - attr_len : 0;
    uint16_t mp_offset = 0;
    while (mp_offset < attr_len) {
        if (offset + mp_offset + 2 > len) break;
        
        uint8_t flags = data[offset + mp_offset];
        uint8_t attr_type = data[offset + mp_offset + 1];
        mp_offset += 2;
        
        uint8_t ext_len = (flags >> 4) & 1;
        uint16_t attr_len_mp;
        if (ext_len) {
            if (offset + mp_offset + 2 > len) break;
            attr_len_mp = read_uint16_be(&data[offset + mp_offset]);
            mp_offset += 2;
        } else {
            if (offset + mp_offset + 1 > len) break;
            attr_len_mp = data[offset + mp_offset];
            mp_offset += 1;
        }
        
        if (offset + mp_offset + attr_len_mp > len) break;
        
        if (attr_type == BGP_ATTR_MP_REACH_NLRI) {
            const uint8_t *mp_data = &data[offset + mp_offset];
            if (attr_len_mp >= 4) {
                uint16_t afi = read_uint16_be(mp_data);
                uint8_t safi = mp_data[2];
                
                if (afi == AFI_IPV6 && safi == SAFI_UNICAST) {
                    uint8_t nh_len = mp_data[3];
                    
                    if (nh_len == 16 || nh_len == 32) {
                        memcpy(&attr->ipv6_nexthop, &mp_data[4], 16);
                        attr->ipv6_nexthop_present = 1;
                        
                        uint16_t nlri_mp_len = attr_len_mp - 4 - nh_len;
                        if (nlri_mp_len > 0) {
                            bgp_parse_nlri_ipv6(&mp_data[4 + nh_len], nlri_mp_len,
                                              update->ipv6_update.nlri,
                                              &update->ipv6_update.nlri_count);
                            if (update->ipv6_update.nlri_count > 0) {
                                update->has_ipv6 = 1;
                            }
                        }
                    }
                }
            }
        } else if (attr_type == BGP_ATTR_MP_UNREACH_NLRI) {
            const uint8_t *mp_data = &data[offset + mp_offset];
            if (attr_len_mp >= 3) {
                uint16_t afi = read_uint16_be(mp_data);
                uint8_t safi = mp_data[2];
                
                if (afi == AFI_IPV6 && safi == SAFI_UNICAST) {
                    uint16_t nlri_mp_len = attr_len_mp - 3;
                    if (nlri_mp_len > 0) {
                        bgp_parse_nlri_ipv6(&mp_data[3], nlri_mp_len,
                                          update->ipv6_update.withdrawn,
                                          &update->ipv6_update.withdrawn_count);
                    }
                }
            }
        }
        
        mp_offset += attr_len_mp;
    }
    
    return 0;
}

int bgp_parse_open(const uint8_t *data, uint16_t len, bgp_open_msg_t *open) {
    if (!data || !open || len < 10) return -1;
    
    memset(open, 0, sizeof(bgp_open_msg_t));
    
    open->version = data[0];
    open->my_as = read_uint16_be(&data[1]);
    open->hold_time = read_uint16_be(&data[3]);
    open->bgp_id = read_uint32_be(&data[5]);
    open->opt_param_len = data[9];
    
    if (open->opt_param_len > 0) {
        open->opt_params = malloc(open->opt_param_len);
        if (!open->opt_params) return -1;
        memcpy(open->opt_params, &data[10], open->opt_param_len);
    }
    
    return 0;
}

int bgp_parse_keepalive(const uint8_t *data, uint16_t len, bgp_keepalive_msg_t *ka) {
    if (!ka) return -1;
    ka->timestamp = time(NULL);
    return 0;
}

int bgp_parse_notification(const uint8_t *data, uint16_t len,
                           bgp_notification_msg_t *notif) {
    if (!data || !notif || len < 2) return -1;
    
    memset(notif, 0, sizeof(bgp_notification_msg_t));
    
    notif->error_code = data[0];
    notif->error_subcode = data[1];
    notif->data_len = len - 2;
    
    if (notif->data_len > 0) {
        notif->data = malloc(notif->data_len);
        if (!notif->data) return -1;
        memcpy(notif->data, &data[2], notif->data_len);
    }
    
    return 0;
}

/* ===== RIB Management ===== */

bgp_rib_t *bgp_rib_new(uint32_t initial_capacity) {
    bgp_rib_t *rib = malloc(sizeof(bgp_rib_t));
    if (!rib) return NULL;
    
    memset(rib, 0, sizeof(bgp_rib_t));
    
    rib->ipv4_route_capacity = initial_capacity ? initial_capacity : 1000;
    rib->ipv4_routes = malloc(sizeof(bgp_rib_ipv4_entry_t) * rib->ipv4_route_capacity);
    if (!rib->ipv4_routes) {
        free(rib);
        return NULL;
    }
    
    rib->ipv6_route_capacity = initial_capacity ? initial_capacity : 1000;
    rib->ipv6_routes = malloc(sizeof(bgp_rib_ipv6_entry_t) * rib->ipv6_route_capacity);
    if (!rib->ipv6_routes) {
        free(rib->ipv4_routes);
        free(rib);
        return NULL;
    }
    
    return rib;
}

void bgp_rib_free(bgp_rib_t *rib) {
    if (!rib) return;
    
    if (rib->ipv4_routes) free(rib->ipv4_routes);
    if (rib->ipv6_routes) free(rib->ipv6_routes);
    free(rib);
}

static int ipv4_nlri_cmp(const bgp_ipv4_nlri_t *a, const bgp_ipv4_nlri_t *b) {
    if (a->prefix_len != b->prefix_len) {
        return a->prefix_len - b->prefix_len;
    }
    return memcmp(&a->prefix, &b->prefix, sizeof(uint32_t));
}

static int ipv6_nlri_cmp(const bgp_ipv6_nlri_t *a, const bgp_ipv6_nlri_t *b) {
    if (a->prefix_len != b->prefix_len) {
        return a->prefix_len - b->prefix_len;
    }
    return memcmp(a->prefix, b->prefix, 16);
}

int bgp_rib_add_ipv4(bgp_rib_t *rib, const bgp_rib_ipv4_entry_t *entry) {
    if (!rib || !entry) return -1;
    
    /* Check if route exists and update it */
    for (uint32_t i = 0; i < rib->ipv4_route_count; i++) {
        if (ipv4_nlri_cmp(&rib->ipv4_routes[i].prefix, &entry->prefix) == 0) {
            rib->ipv4_routes[i] = *entry;
            return 0;
        }
    }
    
    /* Need to expand? */
    if (rib->ipv4_route_count >= rib->ipv4_route_capacity) {
        rib->ipv4_route_capacity *= 2;
        bgp_rib_ipv4_entry_t *new_routes = realloc(rib->ipv4_routes,
                                                     sizeof(bgp_rib_ipv4_entry_t) * rib->ipv4_route_capacity);
        if (!new_routes) return -1;
        rib->ipv4_routes = new_routes;
    }
    
    rib->ipv4_routes[rib->ipv4_route_count] = *entry;
    rib->ipv4_route_count++;
    
    return 0;
}

int bgp_rib_add_ipv6(bgp_rib_t *rib, const bgp_rib_ipv6_entry_t *entry) {
    if (!rib || !entry) return -1;
    
    /* Check if route exists and update it */
    for (uint32_t i = 0; i < rib->ipv6_route_count; i++) {
        if (ipv6_nlri_cmp(&rib->ipv6_routes[i].prefix, &entry->prefix) == 0) {
            rib->ipv6_routes[i] = *entry;
            return 0;
        }
    }
    
    /* Need to expand? */
    if (rib->ipv6_route_count >= rib->ipv6_route_capacity) {
        rib->ipv6_route_capacity *= 2;
        bgp_rib_ipv6_entry_t *new_routes = realloc(rib->ipv6_routes,
                                                     sizeof(bgp_rib_ipv6_entry_t) * rib->ipv6_route_capacity);
        if (!new_routes) return -1;
        rib->ipv6_routes = new_routes;
    }
    
    rib->ipv6_routes[rib->ipv6_route_count] = *entry;
    rib->ipv6_route_count++;
    
    return 0;
}

int bgp_rib_remove_ipv4(bgp_rib_t *rib, const bgp_ipv4_nlri_t *prefix) {
    if (!rib || !prefix) return -1;
    
    for (uint32_t i = 0; i < rib->ipv4_route_count; i++) {
        if (ipv4_nlri_cmp(&rib->ipv4_routes[i].prefix, prefix) == 0) {
            /* Swap with last and decrement count */
            if (i < rib->ipv4_route_count - 1) {
                rib->ipv4_routes[i] = rib->ipv4_routes[rib->ipv4_route_count - 1];
            }
            rib->ipv4_route_count--;
            return 0;
        }
    }
    
    return -1;
}

int bgp_rib_remove_ipv6(bgp_rib_t *rib, const bgp_ipv6_nlri_t *prefix) {
    if (!rib || !prefix) return -1;
    
    for (uint32_t i = 0; i < rib->ipv6_route_count; i++) {
        if (ipv6_nlri_cmp(&rib->ipv6_routes[i].prefix, prefix) == 0) {
            /* Swap with last and decrement count */
            if (i < rib->ipv6_route_count - 1) {
                rib->ipv6_routes[i] = rib->ipv6_routes[rib->ipv6_route_count - 1];
            }
            rib->ipv6_route_count--;
            return 0;
        }
    }
    
    return -1;
}

/* ===== Utility Functions ===== */

const char *bgp_state_str(bgp_state_t state) {
    switch (state) {
        case BGP_STATE_IDLE:       return "IDLE";
        case BGP_STATE_CONNECT:    return "CONNECT";
        case BGP_STATE_ACTIVE:     return "ACTIVE";
        case BGP_STATE_OPENSENT:   return "OPENSENT";
        case BGP_STATE_OPENCONFIRM: return "OPENCONFIRM";
        case BGP_STATE_ESTABLISHED: return "ESTABLISHED";
        default:                    return "UNKNOWN";
    }
}

const char *bgp_origin_str(int origin) {
    switch (origin) {
        case ORIGIN_IGP:        return "IGP";
        case ORIGIN_EGP:        return "EGP";
        case ORIGIN_INCOMPLETE: return "INCOMPLETE";
        default:                return "UNKNOWN";
    }
}

void bgp_print_ipv4_prefix(const bgp_ipv4_nlri_t *nlri) {
    if (!nlri) return;
    
    struct in_addr addr;
    addr.s_addr = nlri->prefix;
    printf("%s/%u", inet_ntoa(addr), nlri->prefix_len);
}

void bgp_print_ipv6_prefix(const bgp_ipv6_nlri_t *nlri) {
    if (!nlri) return;
    
    char buf[INET6_ADDRSTRLEN];
    inet_ntop(AF_INET6, nlri->prefix, buf, sizeof(buf));
    printf("%s/%u", buf, nlri->prefix_len);
}

void bgp_print_as_path(const bgp_as_path_t *as_path) {
    if (!as_path) return;
    
    for (uint8_t i = 0; i < as_path->segment_count; i++) {
        bgp_as_path_segment_t *seg = &as_path->segments[i];
        
        if (seg->segment_type == 1) printf("{");
        else if (i > 0) printf(" ");
        
        for (uint16_t j = 0; j < seg->as_count; j++) {
            if (j > 0) printf(" ");
            printf("%u", seg->as_list[j]);
        }
        
        if (seg->segment_type == 1) printf("}");
    }
}

void bgp_print_communities(const bgp_communities_t *communities) {
    if (!communities || communities->count == 0) return;
    
    for (uint16_t i = 0; i < communities->count; i++) {
        if (i > 0) printf(" ");
        
        uint32_t value = communities->communities[i].value;
        uint16_t asn = (value >> 16) & 0xFFFF;
        uint16_t val = value & 0xFFFF;
        
        printf("%u:%u", asn, val);
    }
}
