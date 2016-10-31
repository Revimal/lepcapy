#ifndef LEPCAPY_PROTO_CTRL_H
#define LEPCAPY_PROTO_CTRL_H

#include "macros.h"
#include "ethernetII.h"
#include "ipv4.h"

#define alloc_pktm(pktm)                       \
    (pktm = (struct pktm_object_s *)calloc(1, sizeof(struct pktm_object_s)))

#define free_pktm(pktm)                                 \
    do{                                                 \
        if(pktm->pkt_mexit)                             \
            pktm->pkt_mexit(pktm);                      \
                                                        \
        if(pktm != NULL){                               \
                free(pktm);                             \
                pktm = NULL;                           \
        }                                               \
    }while(0)

struct proto_env{
    char if_name[IFNAMSIZ];
    struct netaddr_ether eth_addr;
    struct in_addr src_ip;
    struct in_addr dst_ip;
};

extern struct proto_env env_pktm;
extern struct pktm_object_s *p_pktm;

int parse_ip_str(char *ip_str, struct in_addr *dst_ptr);
int parse_eth_str(char *eth_str, uint8_t *dst_ptr);

#endif //LEPCAPY_PROTO_CTRL_H
