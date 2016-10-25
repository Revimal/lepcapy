#ifndef LEPCAPY_PROTO_BASE_H
#define LEPCAPY_PROTO_BASE_H

#include "macros.h"
#include "record_queue.h"

struct pcap_parser_s{
    int (*parse_queue_encap)(struct queue_node_s* dest_queue, void* src_layer);
    int (*parse_queue_decap)(struct queue_node_s* src_queue, void* dest_layer);

    int (*parse_idx_encap)(uint32_t dest_idx, void* src_layer);
    int (*parse_idx_decap)(uint32_t src_idx, void* dest_layer);
};

/*
 * Parser Macros
 */

#define PROTO_LAYER(pcap_proto)    \
    ((void*)pcap_proto)

#endif // LEPCAPY_PROTO_BASE_H
