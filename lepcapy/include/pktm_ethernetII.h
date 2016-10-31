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

#define ETH_OBJ(pktm_proto)                            \
    ((struct pktm_ether_s)pktm_proto)

#define ETH_PTR(pktm_proto)                            \
    ((struct pktm_ether_s *)pktm_proto)

extern struct pktm_operation_s ether_operations;

int pktm_ether_init(struct pktm_object_s * const pktm, char * const if_ifn);
void pktm_ether_exit(struct pktm_object_s * const pktm);
ssize_t pktm_ether_send(struct pktm_object_s * const pktm, uint8_t * const eth_buf, const ssize_t eth_len, void * dummy);
//int pktm_ether_init_etherbuf(struct pktm_object_s * const pktm, uint8_t * const buf, const ssize_t cnt, void *prot_addr);
int pktm_ether_get_naddr(struct pktm_object_s * const pktm, void * const hwa);
int pktm_ether_get_iaddr(struct pktm_object_s * const pktm, void * const ipa);
//int pktm_ether_get_header(struct pktm_object_s * const pktm, const void * p_ethbuf);
//int pktm_ether_get_payload(struct pktm_object_s * const pktm, const void * p_ethbuf);

#endif // LEPCAPY_PKTM_ETHERNETII_H
