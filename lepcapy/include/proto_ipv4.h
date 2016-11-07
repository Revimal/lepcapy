#ifndef LEPCAPY_PROTO_IPV4_H
#define LEPCAPY_PROTO_IPV4_H

#include "macros.h"
#include "proto_struc.h"

#include <netinet/ip.h>

struct proto_ipv4{
    struct in_addr saddr;
    struct in_addr daddr;
};

#define IPV4_OBJ(pktm_proto)                            \
    ((struct proto_ipv4)pktm_proto)

#define IPV4_PTR(pktm_proto)                            \
    ((struct proto_ipv4 *)pktm_proto)

extern struct proto_chain_s ipv4_chain; //For protocol chaining call
int ipv4_parse_str(char *ip_str, struct in_addr *dst_ptr);
int ipv4_get_uptype(uint8_t * const ipv4_buf, void * const ipv4_type);

#endif // LEPCAPY_PROTO_IPV4_H
