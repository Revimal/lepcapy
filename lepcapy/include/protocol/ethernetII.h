#ifndef LEPCAPY_ETHERNETII_H
#define LEPCAPY_ETHERNETII_H

#include "macros.h"
#include "protocol/proto_struc.h"

#include <net/ethernet.h>

struct netaddr_ether{
    uint8_t eth_saddr[ETH_ALEN];
    uint8_t eth_daddr[ETH_ALEN];
};

#endif // LEPCAPY_ETHERNETII_H
