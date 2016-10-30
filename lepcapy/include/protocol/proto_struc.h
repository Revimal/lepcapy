#ifndef LEPCAPY_PROTO_STRUC_H
#define LEPCAPY_PROTO_STRUC_H

#include "macros.h"

#include <arpa/inet.h>
#include <linux/if_packet.h>
#include <stdlib.h>
#include <string.h>
#include <sys/ioctl.h>
#include <sys/socket.h>
#include <net/if.h>
#include <unistd.h>

struct pktm_opeartion_s;
struct proto_chain_s;
struct pktm_object_s;

struct pktm_object_s{
    struct pktm_operations_s *proto_oper;

    int sd;
    int offset;

    uint8_t* proto_buf;

    void *proto_hdr;
    uint8_t* proto_payload;

    struct sockaddr_ll tx_addr;

    struct proto_chain_s *u_layer;
};

struct proto_chain_s{
    void *proto_obj;
    struct proto_chain_s *u_layer;

    int (*proto_set_protbuf)(struct proto_chain_s * const protm, uint8_t * const prot_buf);
    int (*proto_set_ulayer)(struct proto_chain_s * const protm, struct proto_chain_s * const u_layer);
    int (*proto_apply_chain)(struct proto_chain_s * const protm);
    /*
     * TODO : Add other features
     */
};

struct pktm_operation_s{
    int (*pkt_minit)(struct pktm_object_s * const pktm, char * const if_ifn);
    void (*pkt_mexit)(struct pktm_object_s * const pktm);

    ssize_t (*pkt_send)(struct pktm_object_s * const pktm, uint8_t * const buf, const ssize_t cnt, void *args);

    int (*pkt_set_protbuf)(struct pktm_object_s * const pktm, uint8_t * const buf, const ssize_t cnt, void *prot_addr);
    int (*pkt_set_proto)(struct pktm_object_s * const pktm, struct proto_chain_s *u_layer);
    int (*pkt_get_naddr)(struct pktm_object_s * const pktm, void * const addr);

    int (*pkt_ctl)(struct pktm_object_s * pktm, const int ctl_num, void* args);
};

#define PKTM_CTL_GSOCK  0   /* Get socket */
#define PKTM_CTL_GOFFS  1   /* Get offset */
#define PKTM_CTL_GPBUF  2   /* Get protocol buffer */
#define PKTM_CTL_GPHDR  3   /* Get protocol header */
#define PKTM_CTL_GPPLD  4   /* Get protocol payload */
#define PKTM_CTL_GTXAD  5   /* Get transmix address */
#define PKTM_CTL_GPROT  6   /* Get protocol type */

#define PKTM_OBJ(pktm_proto)                            \
    ((struct pktm_object_s)pktm_proto)

#define PKTM_PTR(pktm_proto)                            \
    ((struct pktm_object_s *)pktm_proto)

ssize_t pktm_dummy_send(struct pktm_object_s * const pktm, uint8_t * const buf, const ssize_t cnt, void *args);
int pktm_set_protbuf(struct pktm_object_s * const pktm, uint8_t * const buf, const ssize_t cnt, void *prot_addr);
int pktm_dummy_proto(struct pktm_object_s * const pktm, struct proto_chain_s *u_layer);
int pktm_dummy_get_naddr(struct pktm_object_s * const pktm, void * const addr);
int pktm_dummy_ctl(struct pktm_object_s * pktm, const int ctl_num, void* args);

#endif // LEPCAPY_PROTO_STRUC_H
