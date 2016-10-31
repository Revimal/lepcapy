#ifndef LEPCAPY_ETHERNETII_H
#define LEPCAPY_ETHERNETII_H

#include "macros.h"
#include "proto_struc.h"

#include <net/ethernet.h>

struct netaddr_ether{
    uint8_t eth_saddr[ETH_ALEN];
    uint8_t eth_daddr[ETH_ALEN];
};

struct pktm_ether_s{
    void (*eth_mexit)(struct pktm_object_s * const pktm);

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

int pktm_ether_init(struct pktm_object_s * const pktm, char * const if_ifn);
void pktm_ether_exit(struct pktm_object_s * const pktm);
ssize_t pktm_ether_send(struct pktm_object_s * const pktm, void * dummy);
int pktm_ether_set_etherbuf(struct pktm_object_s * const pktm, uint8_t * const buf, const ssize_t cnt, void *prot_addr);
int pktm_ether_set_proto(struct pktm_object_s * const pktm, struct proto_chain_s *u_layer);
int pktm_ether_get_naddr(struct pktm_object_s * const pktm, void * const hwa);

#endif // LEPCAPY_ETHERNETII_H
