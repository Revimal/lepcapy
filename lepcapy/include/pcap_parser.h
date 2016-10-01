#ifndef LEPCAPY_PCAP_PARSER_H
#define LEPCAPY_PCAP_PARSER_H

#include "pcap_rec.h"

#include <stdio.h>
#include <stdint.h>

struct pcap_layer_s{
    struct pcap_parser_s* bottom;
    struct pcap_parser_s* top;
    int layer_cnt;
};

struct pcap_parser_s{
    struct pcap_parser_s* next;
    struct pcap_parser_s* prev;
    int layer;

    int (*pcap_parse_encap)(const struct pcaprec_data_s** src_data);
    int (*pcap_parse_decap)(const struct pcaprec_data_s** src_data);
    int (*pcap_parse_to_strc)(const struct pcaprec_data_s** src_data,void** proto_hdr);
    int (*pcap_parse_from_strc)(const struct pcaprec_data_s** src_data ,void** proto_hdr);
};

/*
 * Parser Macros
 */
#define P_LAYER(pcap_parser)    \
    ((struct pcap_parser_s)pcap_parser)

#define P_LAYER_P(pcap_parser)  \
    ((struct pcap_parser_s*)pcap_parser)

#define LAYER(pcap_proto)    \
    ((void**)pcap_proto)

#endif // LEPCAPY_PCAP_PARSER_H
