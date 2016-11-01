#ifndef LEPCAPY_PROTO_CTRL_H
#define LEPCAPY_PROTO_CTRL_H

#include "macros.h"
#include "pktm_ethernetII.h"
#include "proto_ethernetII.h"
#include "proto_ipv4.h"

#define alloc_pktm(pktm)                                                            \
    do{                                                                             \
        pktm = (struct pktm_object_s *)calloc(1, sizeof(struct pktm_object_s));     \
        pktm->init = 0;                                                             \
    }while(0)

#define free_pktm(pktm)                                 \
    do{                                                 \
        if(pktm->init)                                  \
            pktm->pkt_mexit(pktm);                      \
                                                        \
        if(pktm != NULL){                               \
                free(pktm);                             \
                pktm = NULL;                           \
        }                                               \
    }while(0)

struct proto_env{
    char if_name[IFNAMSIZ];
    struct proto_ether eth_addr;
    struct proto_ipv4 ipv4_addr;
};

extern struct proto_env env_pktm;
extern struct pktm_object_s *p_pktm;

int parse_ip_str(char *ip_str, struct in_addr *dst_ptr);
int parse_eth_str(char *eth_str, uint8_t *dst_ptr);

#endif //LEPCAPY_PROTO_CTRL_H
