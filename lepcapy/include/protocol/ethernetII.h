#ifndef LEPCAPY_ETHERNETII_H
#define LEPCAPY_ETHERNETII_H

#include "macros.h"
#include "protocol/proto_struc.h"

#include <net/ethernet.h>

struct netaddr_ether{
    uint8_t eth_saddr[ETH_ALEN];
    uint8_t eth_daddr[ETH_ALEN];
};

struct pktm_ether_s{
    struct pktm_operations_s *eth_oper;

    int sd;
    int offset;

    uint8_t* eth_buf;

    struct ether_header *eth_hdr;
    uint8_t *eth_payload;

    struct sockaddr_ll tx_addr;

    struct proto_chain_s *u_layer;
};

#define ETH_OBJ(pktm_proto)                            \
    ((struct pktm_ether_s)pktm_proto)

#define ETH_PTR(pktm_proto)                            \
    ((struct pktm_ether_s *)pktm_proto)

extern struct pktm_operation_s ether_operations;

#endif // LEPCAPY_ETHERNETII_H
