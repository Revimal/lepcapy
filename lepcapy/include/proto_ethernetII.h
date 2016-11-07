#ifndef LEPCAPY_PROTO_ETHERNETII_H
#define LEPCAPY_PROTO_ETHERNETII_H

#include "macros.h"
#include "proto_struc.h"

#include <net/ethernet.h>

struct proto_ether{
    uint8_t eth_saddr[ETH_ALEN];
    uint8_t eth_daddr[ETH_ALEN];
};

#define ETH_OBJ(pktm_proto)                            \
    ((struct proto_ether)pktm_proto)

#define ETH_PTR(pktm_proto)                            \
    ((struct proto_ether *)pktm_proto)

extern struct proto_chain_s ether_chain; //For protocol chaining call
int ether_parse_str(char *eth_str, uint8_t *dst_ptr);
int ether_get_uptype(uint8_t * const ether_buf, void * const ether_type);

#endif //LEPCAPY_PROTO_ETHERNETII_H
