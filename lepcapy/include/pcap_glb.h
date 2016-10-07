#ifndef LEPCAPY_PCAP_GLB_H
#define LEPCAPY_PCAP_GLB_H

#include "macros.h"

#include <stdio.h>
#include <stdint.h>

#define PCAP_MAGIC_NUM 0xa1b2c3d4

struct pcap_hdr_s{
    uint32_t magic_number;   /* magic number */
    uint16_t version_major;  /* major version number */
    uint16_t version_minor;  /* minor version number */
    int32_t  thiszone;       /* GMT to local correction */
    uint32_t sigfigs;        /* accuracy of timestamps */
    uint32_t snaplen;        /* max length of captured packets, in octets */
    uint32_t network;        /* data link type */
};

int load_pcap_format(FILE **fp, struct pcap_hdr_s *p_pcap_hdr);

#endif // LEPCAPY_PCAP_GLB_H
