/*
 * OpenOS - Networking Stack
 * 
 * Provides basic TCP/IP networking capabilities
 */

#ifndef OPENOS_NETWORK_H
#define OPENOS_NETWORK_H

#include <stdint.h>
#include <stddef.h>

/* MAC address length */
#define MAC_ADDR_LEN 6

/* IP address length */
#define IP_ADDR_LEN 4

/* Maximum packet size */
#define MAX_PACKET_SIZE 1518

/* Protocol numbers */
#define PROTO_ICMP 1
#define PROTO_TCP  6
#define PROTO_UDP  17

/* MAC address structure */
typedef struct mac_addr {
    uint8_t addr[MAC_ADDR_LEN];
} mac_addr_t;

/* IP address structure */
typedef struct ip_addr {
    uint8_t addr[IP_ADDR_LEN];
} ip_addr_t;

/* Ethernet header */
typedef struct eth_header {
    mac_addr_t dest;
    mac_addr_t src;
    uint16_t type;
} __attribute__((packed)) eth_header_t;

/* IP header */
typedef struct ip_header {
    uint8_t version_ihl;
    uint8_t tos;
    uint16_t total_length;
    uint16_t id;
    uint16_t flags_offset;
    uint8_t ttl;
    uint8_t protocol;
    uint16_t checksum;
    ip_addr_t src_ip;
    ip_addr_t dst_ip;
} __attribute__((packed)) ip_header_t;

/* TCP header */
typedef struct tcp_header {
    uint16_t src_port;
    uint16_t dst_port;
    uint32_t seq_num;
    uint32_t ack_num;
    uint16_t flags;
    uint16_t window;
    uint16_t checksum;
    uint16_t urgent;
} __attribute__((packed)) tcp_header_t;

/* UDP header */
typedef struct udp_header {
    uint16_t src_port;
    uint16_t dst_port;
    uint16_t length;
    uint16_t checksum;
} __attribute__((packed)) udp_header_t;

/* Network packet structure */
typedef struct packet {
    uint8_t data[MAX_PACKET_SIZE];
    size_t length;
} packet_t;

/* Socket structure */
typedef struct socket {
    uint32_t id;
    uint16_t local_port;
    uint16_t remote_port;
    ip_addr_t remote_ip;
    uint8_t protocol;
    int is_open;
} socket_t;

/* Network device structure */
typedef struct net_device {
    char name[16];
    mac_addr_t mac;
    ip_addr_t ip;
    int is_up;
} net_device_t;

/* Initialize networking subsystem */
void net_init(void);

/* Network device management */
void net_set_ip(ip_addr_t* ip);
void net_set_mac(mac_addr_t* mac);
net_device_t* net_get_device(void);

/* Packet operations */
int net_send_packet(packet_t* packet);
int net_receive_packet(packet_t* packet);

/* Socket operations */
socket_t* net_socket_create(uint8_t protocol);
int net_socket_bind(socket_t* socket, uint16_t port);
int net_socket_connect(socket_t* socket, ip_addr_t* ip, uint16_t port);
int net_socket_send(socket_t* socket, const void* data, size_t size);
int net_socket_recv(socket_t* socket, void* buffer, size_t size);
void net_socket_close(socket_t* socket);

/* Utility functions */
uint16_t net_checksum(const void* data, size_t length);

#endif /* OPENOS_NETWORK_H */
