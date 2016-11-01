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

extern struct proto_ether obj_ether; //For change EthernetII header
extern struct proto_chain_s ether_chain; //For protocol chaining call

int ether_get_uptype(uint8_t * const ether_buf, void * const ether_type);
int ether_get_obj(struct proto_chain_s * const protm, void *proto_obj);
int ether_set_ulayer(struct proto_chain_s * const protm, struct proto_chain_s * const u_layer);
int ether_apply_chain(struct proto_chain_s * const protm, uint8_t * ipv4_buf);


#endif //LEPCAPY_PROTO_ETHERNETII_H
