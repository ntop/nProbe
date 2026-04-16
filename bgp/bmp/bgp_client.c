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
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/select.h>
#include <signal.h>
#include <time.h>
#include <arpa/inet.h>

static volatile int running = 1;

void signal_handler(int sig) {
    running = 0;
}

int main(int argc, char *argv[]) {
    if (argc < 3) {
        fprintf(stderr, "Usage: %s <peer_ip> <local_asn> [local_ip]\n", argv[0]);
        fprintf(stderr, "Example: %s 10.0.0.1 65001 10.0.0.2\n", argv[0]);
        return 1;
    }
    
    const char *peer_ip = argv[1];
    uint16_t local_asn = atoi(argv[2]);
    const char *local_ip = argc > 3 ? argv[3] : NULL;
    
    printf("[*] BGP Passive Client\n");
    printf("[*] Peer: %s, Local ASN: %u\n", peer_ip, local_asn);
    
    /* Create BGP session */
    uint32_t local_bgp_id = 0x0A000001;  /* 10.0.0.1 - you may want to parametrize this */
    bgp_session_t *session = bgp_session_new(peer_ip, BGP_PORT, local_ip, local_asn, local_bgp_id);
    if (!session) {
        fprintf(stderr, "[!] Failed to create BGP session\n");
        return 1;
    }
    
    printf("[*] Connecting to %s:%u...\n", peer_ip, BGP_PORT);
    if (bgp_session_connect(session) < 0) {
        fprintf(stderr, "[!] Failed to connect\n");
        bgp_session_free(session);
        return 1;
    }
    
    /* Create RIB */
    bgp_rib_t *rib = bgp_rib_new(10000);
    if (!rib) {
        fprintf(stderr, "[!] Failed to create RIB\n");
        bgp_session_free(session);
        return 1;
    }
    
    printf("[*] RIB created with capacity for 10000 routes\n");
    
    signal(SIGINT, signal_handler);
    signal(SIGTERM, signal_handler);
    
    uint32_t ipv4_updates = 0;
    uint32_t ipv6_updates = 0;
    time_t start_time = time(NULL);
    
    /* Main loop */
    while (running) {
        fd_set readfds;
        struct timeval tv;
        
        FD_ZERO(&readfds);
        FD_SET(session->socket, &readfds);
        
        tv.tv_sec = 5;
        tv.tv_usec = 0;
        
        int ret = select(session->socket + 1, &readfds, NULL, NULL, &tv);
        
        if (ret < 0) {
            perror("select");
            break;
        }
        
        if (ret == 0) {
            /* Timeout - could send keepalive here if needed */
            continue;
        }
        
        if (FD_ISSET(session->socket, &readfds)) {
            /* Receive data */
            uint8_t buf[BGP_MAX_MESSAGE_SIZE];
            ssize_t n = recv(session->socket, buf, sizeof(buf), 0);
            
            if (n <= 0) {
                if (n < 0) {
                    perror("recv");
                }
                printf("[*] Connection closed\n");
                break;
            }
            
            /* Add to buffer */
            if (session->recv_buf_len + n > BGP_MAX_MESSAGE_SIZE) {
                fprintf(stderr, "[!] Receive buffer overflow\n");
                break;
            }
            
            memcpy(&session->recv_buf[session->recv_buf_len], buf, n);
            session->recv_buf_len += n;
            
            /* Process complete messages */
            while (session->recv_buf_len >= BGP_HEADER_SIZE) {
                /* Check for valid BGP marker */
                int valid_marker = 1;
                for (int i = 0; i < BGP_MARKER_SIZE; i++) {
                    if (session->recv_buf[i] != 0xFF) {
                        valid_marker = 0;
                        break;
                    }
                }
                
                if (!valid_marker) {
                    fprintf(stderr, "[!] Invalid BGP marker, seeking new one\n");
                    memmove(session->recv_buf, &session->recv_buf[1],
                            session->recv_buf_len - 1);
                    session->recv_buf_len--;
                    continue;
                }
                
                /* Get message length */
                uint16_t msg_len = (session->recv_buf[16] << 8) | session->recv_buf[17];
                
                if (msg_len < BGP_HEADER_SIZE || msg_len > BGP_MAX_MESSAGE_SIZE) {
                    fprintf(stderr, "[!] Invalid message length: %u\n", msg_len);
                    memmove(session->recv_buf, &session->recv_buf[1],
                            session->recv_buf_len - 1);
                    session->recv_buf_len--;
                    continue;
                }
                
                if (session->recv_buf_len < msg_len) {
                    /* Need more data */
                    break;
                }
                
                /* Parse message */
                uint8_t msg_type = session->recv_buf[18];
                const uint8_t *msg_data = &session->recv_buf[BGP_HEADER_SIZE];
                uint16_t msg_payload_len = msg_len - BGP_HEADER_SIZE;
                
                switch (msg_type) {
                    case BGP_MSG_OPEN: {
                        bgp_open_msg_t open;
                        if (bgp_parse_open(msg_data, msg_payload_len, &open) == 0) {
                            session->peer_asn = open.my_as;
                            session->peer_bgp_id = open.bgp_id;
                            session->hold_time = open.hold_time;
                            
                            char peer_id[16];
                            struct in_addr peer_id_addr;
                            peer_id_addr.s_addr = htonl(open.bgp_id);
                            
                            printf("[+] OPEN: peer_asn=%u, hold_time=%u, bgp_id=%s\n",
                                   open.my_as, open.hold_time, inet_ntoa(peer_id_addr));
                            
                            session->state = BGP_STATE_ESTABLISHED;
                            
                            if (open.opt_params) free(open.opt_params);
                        }
                        break;
                    }
                    
                    case BGP_MSG_UPDATE: {
                        bgp_update_msg_t update;
                        if (bgp_parse_update(msg_data, msg_payload_len, &update, session->peer_asn) == 0) {
                            
                            /* Process IPv4 updates */
                            if (update.has_ipv4) {
                                for (uint16_t i = 0; i < update.ipv4_update.nlri_count; i++) {
                                    bgp_rib_ipv4_entry_t entry;
                                    entry.prefix = update.ipv4_update.nlri[i];
                                    entry.attributes = update.ipv4_update.attributes;
                                    entry.peer_asn = session->peer_asn;
                                    entry.last_update = time(NULL);
                                    
                                    bgp_rib_add_ipv4(rib, &entry);
                                    
                                    printf("[+] IPv4 NLRI: ");
                                    bgp_print_ipv4_prefix(&update.ipv4_update.nlri[i]);
                                    printf(" from AS%u via ", session->peer_asn);
                                    
                                    struct in_addr nh;
                                    nh.s_addr = update.ipv4_update.attributes.ipv4_nexthop.s_addr;
                                    printf("%s\n", inet_ntoa(nh));
                                    
                                    ipv4_updates++;
                                }
                                
                                /* Process withdrawn routes */
                                for (uint16_t i = 0; i < update.ipv4_update.withdrawn_count; i++) {
                                    bgp_rib_remove_ipv4(rib, &update.ipv4_update.withdrawn[i]);
                                    
                                    printf("[-] IPv4 Withdrawn: ");
                                    bgp_print_ipv4_prefix(&update.ipv4_update.withdrawn[i]);
                                    printf("\n");
                                }
                            }
                            
                            /* Process IPv6 updates */
                            if (update.has_ipv6) {
                                for (uint16_t i = 0; i < update.ipv6_update.nlri_count; i++) {
                                    bgp_rib_ipv6_entry_t entry;
                                    entry.prefix = update.ipv6_update.nlri[i];
                                    entry.attributes = update.ipv6_update.attributes;
                                    entry.peer_asn = session->peer_asn;
                                    entry.last_update = time(NULL);
                                    
                                    bgp_rib_add_ipv6(rib, &entry);
                                    
                                    printf("[+] IPv6 NLRI: ");
                                    bgp_print_ipv6_prefix(&update.ipv6_update.nlri[i]);
                                    printf(" from AS%u via ", session->peer_asn);
                                    
                                    char buf[INET6_ADDRSTRLEN];
                                    inet_ntop(AF_INET6, &update.ipv6_update.attributes.ipv6_nexthop,
                                             buf, sizeof(buf));
                                    printf("%s\n", buf);
                                    
                                    ipv6_updates++;
                                }
                                
                                /* Process withdrawn IPv6 routes */
                                for (uint16_t i = 0; i < update.ipv6_update.withdrawn_count; i++) {
                                    bgp_rib_remove_ipv6(rib, &update.ipv6_update.withdrawn[i]);
                                    
                                    printf("[-] IPv6 Withdrawn: ");
                                    bgp_print_ipv6_prefix(&update.ipv6_update.withdrawn[i]);
                                    printf("\n");
                                }
                            }
                            
                            /* Print attributes if present */
                            if (update.ipv4_update.has_attributes) {
                                bgp_attr_t *attr = &update.ipv4_update.attributes;
                                
                                if (attr->as_path.segment_count > 0) {
                                    printf("    AS_PATH: ");
                                    bgp_print_as_path(&attr->as_path);
                                    printf("\n");
                                }
                                
                                printf("    Origin: %s\n", bgp_origin_str(attr->origin));
                                
                                if (attr->local_pref != 100) {
                                    printf("    Local_Pref: %u\n", attr->local_pref);
                                }
                                
                                if (attr->med != 0) {
                                    printf("    MED: %u\n", attr->med);
                                }
                                
                                if (attr->communities.count > 0) {
                                    printf("    Communities: ");
                                    bgp_print_communities(&attr->communities);
                                    printf("\n");
                                }
                                
                                if (attr->large_communities.count > 0) {
                                    printf("    Large Communities: ");
                                    for (uint16_t i = 0; i < attr->large_communities.count; i++) {
                                        if (i > 0) printf(" ");
                                        printf("%u:%u:%u",
                                               attr->large_communities.communities[i].global_admin,
                                               attr->large_communities.communities[i].local_data1,
                                               attr->large_communities.communities[i].local_data2);
                                    }
                                    printf("\n");
                                }
                            }
                        }
                        break;
                    }
                    
                    case BGP_MSG_KEEPALIVE: {
                        bgp_keepalive_msg_t ka;
                        bgp_parse_keepalive(msg_data, msg_payload_len, &ka);
                        printf("[*] KEEPALIVE\n");
                        session->last_keepalive_received = ka.timestamp;
                        break;
                    }
                    
                    case BGP_MSG_NOTIFICATION: {
                        bgp_notification_msg_t notif;
                        if (bgp_parse_notification(msg_data, msg_payload_len, &notif) == 0) {
                            printf("[!] NOTIFICATION: error_code=%u, error_subcode=%u\n",
                                   notif.error_code, notif.error_subcode);
                            if (notif.data) free(notif.data);
                        }
                        running = 0;
                        break;
                    }
                    
                    case BGP_MSG_ROUTE_REFRESH:
                        printf("[*] ROUTE_REFRESH\n");
                        break;
                        
                    default:
                        printf("[!] Unknown message type: %u\n", msg_type);
                }
                
                /* Remove processed message from buffer */
                memmove(session->recv_buf, &session->recv_buf[msg_len],
                        session->recv_buf_len - msg_len);
                session->recv_buf_len -= msg_len;
            }
        }
    }
    
    /* Print final statistics */
    time_t end_time = time(NULL);
    printf("\n[*] ========== Final Statistics ==========\n");
    printf("[*] IPv4 Routes in RIB: %u\n", rib->ipv4_route_count);
    printf("[*] IPv6 Routes in RIB: %u\n", rib->ipv6_route_count);
    printf("[*] IPv4 Updates received: %u\n", ipv4_updates);
    printf("[*] IPv6 Updates received: %u\n", ipv6_updates);
    printf("[*] Session Duration: %ld seconds\n", end_time - start_time);
    printf("[*] ======================================\n");
    
    /* Cleanup */
    bgp_rib_free(rib);
    bgp_session_free(session);
    
    printf("[*] Goodbye!\n");
    return 0;
}
