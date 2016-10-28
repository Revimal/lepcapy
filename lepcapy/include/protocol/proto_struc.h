#ifndef LEPCAPY_PROTO_STRUC_H
#define LEPCAPY_PROTO_STRUC_H

#include "macros.h"

#include <arpa/inet.h>
#include <linux/if_packet.h>
#include <stdlib.h>
#include <sys/ioctl.h>
#include <sys/socket.h>
#include <net/if.h>
#include <unistd.h>

struct pktm_object_s{
    int rx_sd;
    int tx_sd;
    int offset;

    uint8_t* proto_buf;

    void *proto_hdr;
    uint8_t* proto_payload;

    struct sockaddr_ll tx_addr;
    struct sockaddr_ll rx_addr;

    struct pktm_object_s *u_layer;
    struct pktm_object_s *l_layer;
};

struct pktm_operation_s{
    int (*pkt_minit)(struct pktm_object_s * const pktm, char * const if_ifn);
    void (*pkt_mexit)(struct pktm_object_s * const pktm);

    ssize_t (*pkt_send)(struct pktm_object_s * const pktm, uint8_t * const buf, const ssize_t cnt, void *args);
    ssize_t (*pkt_reoff)(struct pktm_object_s * const pktm, const ssize_t cnt);

    int (*pkt_get_addr)(struct pktm_object_s * const pktm, void * const addr);

    int (*pkt_ctl)(struct pktm_object_s * pktm, const int ctl_num, void* args);
};

#define PKTM_OBJ(pktm_proto)                            \
    ((struct pktm_object_s)pktm_proto)

#define PKTM_PTR(pktm_proto)                            \
    ((struct pktm_object_s *)pktm_proto)

#endif // LEPCAPY_PROTO_STRUC_H
