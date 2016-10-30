#ifndef LEPCAPY_IPV4_H
#define LEPCAPY_IPV4_H

#include "macros.h"
#include "proto_struc.h"

#include <netinet/ip.h>

struct proto_ipv4_obj{
    uint8_t* ipv4_buf;

    struct in_addr saddr;
    struct in_addr daddr;
};

#define IPV4_OBJ(pktm_proto)                            \
    ((struct proto_ipv4_obj)pktm_proto)

#define IPV4_PTR(pktm_proto)                            \
    ((struct proto_ipv4_obj *)pktm_proto)

struct proto_ipv4_obj proto_ipv4; //For change IPv4 header
struct proto_chain_s ipv4_chain; //For pktm chaining call

int ipv4_set_protbuf(struct proto_chain_s * const protm, uint8_t * const prot_buf);
int ipv4_set_ulayer(struct proto_chain_s * const protm, struct proto_chain_s * const u_layer);
int ipv4_apply_chain(struct proto_chain_s * const protm);

#endif // LEPCAPY_IPV4_H
