#ifndef LEPCAPY_PROTO_CTRL_H
#define LEPCAPY_PROTO_CTRL_H

#include "macros.h"
#include "pktm_ethernetII.h"
#include "proto_ethernetII.h"
#include "proto_ipv4.h"

struct proto_env{
    char if_name[IFNAMSIZ];
    struct proto_ether eth_addr;
    struct proto_ipv4 ipv4_addr;
};

extern struct proto_env env_pktm;
extern struct pktm_object_s *p_pktm;

#endif //LEPCAPY_PROTO_CTRL_H
