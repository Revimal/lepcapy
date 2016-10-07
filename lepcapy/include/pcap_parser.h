#ifndef LEPCAPY_PCAP_PARSER_H
#define LEPCAPY_PCAP_PARSER_H

#include "macros.h"
#include "pcap_rec.h"
#include "pcap_queue.h"

#include <stdio.h>
#include <stdint.h>

struct pcap_parser_s{
    int (*parse_queue_encap)(struct queue_node_s* dest_queue, void* src_layer);
    int (*parse_queue_decap)(struct queue_node_s* src_queue, void* dest_layer);

    int (*parse_idx_encap)(uint32_t dest_idx, void* src_layer);
    int (*parse_idx_decap)(uint32_t src_idx, void* dest_layer);

//    int(*parse_buf_encap)(pcaprec_data* dest_record, void* src_layer);
//    int(*parse_buf_decap)(pcaprec_data* src_record, void* dest_layer);
};

/*
 * Parser Macros
 */

#define PROTO_LAYER(pcap_proto)    \
    ((void*)pcap_proto)

#endif // LEPCAPY_PCAP_PARSER_H
