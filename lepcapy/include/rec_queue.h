#ifndef LEPCAPY_REC_QUEUE_H
#define LEPCAPY_REC_QUEUE_H

#include "pcap_rec.h"
#include "pcap_parser.h"

#include <stdio.h>
#include <stdint.h>

struct queue_node{
    struct pcaprec_hdr_s* pcaprec_info;
    pcaprec_data* pcaprec_buf;

    unsigned char* begin;
    unsigned char* end;

    unsigned char* head;    //Protocol header
    unsigned char* payload; //Protocol Payload
    unsigned char* tail;    //Protocol tail

    bool in_use = 0;
};

#endif // LEPCAPY_REC_QUEUE_H
