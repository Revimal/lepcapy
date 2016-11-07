#ifndef LEPCAPY_PKTM_ETHERNETII_H
#define LEPCAPY_PKTM_ETHERNETII_H

#include "macros.h"
#include "pktm_struc.h"

#include <net/ethernet.h>

struct pktm_ether_s{
    void (*eth_mexit)(struct pktm_object_s * const pktm);

    int init;
    char eth_name[IFNAMSIZ];
    int sd;
//    int offset;

//    uint8_t* eth_buf;

//    struct ether_header *eth_hdr;
//    uint8_t *eth_payload;

    struct sockaddr_ll tx_addr;
};

#define PKTM_ETH_OBJ(pktm_proto)                            \
    ((struct pktm_ether_s)pktm_proto)

#define PKTM_ETH_PTR(pktm_proto)                            \
    ((struct pktm_ether_s *)pktm_proto)

extern struct pktm_operation_s ether_operations;

int pktm_ether_init(struct pktm_object_s * const pktm, char * const if_ifn);
void pktm_ether_exit(struct pktm_object_s * const pktm);

#endif // LEPCAPY_PKTM_ETHERNETII_H
