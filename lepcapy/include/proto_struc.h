#ifndef LEPCAPY_PROTO_STRUC_H
#define LEPCAPY_PROTO_STRUC_H

#include "macros.h"
#include <string.h>
#include <arpa/inet.h>

struct proto_chain_s{
    void *__proto_obj;
    struct proto_chain_s *__u_layer;

    int (*proto_get_obj)(struct proto_chain_s * const protm, void **proto_obj);
    int (*proto_set_ulayer)(struct proto_chain_s * const protm, struct proto_chain_s * const u_layer);
    int (*proto_apply_chain)(struct proto_chain_s * const protm, uint8_t * const proto_buf);
    /*
     * TODO : Add other features
     */
};

#endif // LEPCAPY_PROTO_STRUC_H
