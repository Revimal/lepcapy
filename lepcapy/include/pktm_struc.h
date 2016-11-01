#ifndef LEPCAPY_PKTM_STRUC_H
#define LEPCAPY_PKTM_STRUC_H

#include <arpa/inet.h>
#include <linux/if_packet.h>
#include <stdlib.h>
#include <string.h>
#include <sys/ioctl.h>
#include <sys/socket.h>
#include <net/if.h>
#include <unistd.h>


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

struct pktm_object_s{
    void (*pkt_mexit)(struct pktm_object_s * const pktm);

    int init;
    char if_name[IFNAMSIZ];
    int sd;
//    int offset;

//    uint8_t* proto_buf;

//    void *proto_hdr;
//    uint8_t* proto_payload;

    struct sockaddr_ll tx_addr;
};

struct pktm_operation_s{
    int (*pkt_minit)(struct pktm_object_s * const pktm, char * const if_ifn);
    void (*pkt_mexit)(struct pktm_object_s * const pktm);

    ssize_t (*pkt_send)(struct pktm_object_s * const pktm, uint8_t * const prot_buf, const ssize_t prot_len, void *args);

//    int (*pkt_init_protbuf)(struct pktm_object_s * const pktm, uint8_t * const buf, const ssize_t cnt, void *prot_addr);
    int (*pkt_get_naddr)(struct pktm_object_s * const pktm, void * const addr);
    int (*pkt_get_iaddr)(struct pktm_object_s * const pktm, void * const addr);
//    int (*pkt_get_header)(struct pktm_object_s * const pktm, const void * p_protbuf);
//    int (*pkt_get_payload)(struct pktm_object_s * const pktm, const void * p_protbuf);

    int (*pkt_ctl)(struct pktm_object_s * pktm, const int ctl_num, void *args);
};

#endif //LEPCAPY_PKTM_STRUC_H
