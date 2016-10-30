#ifndef LEPCAPY_IPV4_H
#define LEPCAPY_IPV4_H

#include "macros.h"
#include "protocol/proto_struc.h"

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

extern struct proto_chain_s ipv4_chain;

#endif // LEPCAPY_IPV4_H
