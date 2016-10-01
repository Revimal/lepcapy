#ifndef LEPCAPY_ETHERNETII_H
#define LEPCAPY_ETHERNETII_H

#include "pcap_parser.h"

#include <stdio.h>
#include <stdint.h>

struct ethernetII_struct{
    unsigned char ether_dest[6];
    unsigned char ether_src[6];
    uint16_t ether_type;
    uint32_t ether_chksum;
};

extern struct pcap_parser_s ethernetII_struct;

#endif // LEPCAPY_ETHERNETII_H
