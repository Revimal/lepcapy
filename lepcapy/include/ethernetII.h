#ifndef LEPCAPY_ETHERNETII_H
#define LEPCAPY_ETHERNETII_H

#include "pcap_parser.h"

#include <stdio.h>
#include <stdint.h>

#include <net/ethernet.h>

struct ethernetII_layer_s{
    struct ether_header* eth_header;
};

extern struct pcap_parser_s ethernetII_operation;

int ethernetII_queue_decap(struct queue_node_s* src_queue, struct ethernetII_layer_s* dest_layer);
int ethernetII_idx_decap(uint32_t src_idx, struct ethernetII_layer_s* dest_layer);
void ViewMac(unsigned char *mac);
unsigned short ntohs(unsigned short value);

#endif // LEPCAPY_ETHERNETII_H
