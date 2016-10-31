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

struct proto_ipv4 obj_ipv4; //For change IPv4 header
struct proto_chain_s ipv4_chain; //For pktm chaining call

int ipv4_get_obj(struct proto_chain_s * const protm, void *proto_obj);
int ipv4_set_ulayer(struct proto_chain_s * const protm, struct proto_chain_s * const u_layer);
int ipv4_apply_chain(struct proto_chain_s * const protm, uint8_t * ipv4_buf);

#endif // LEPCAPY_PROTO_IPV4_H