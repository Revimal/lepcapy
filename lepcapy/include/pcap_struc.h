#ifndef LEPCAPY_PCAP_STRUC_H
#define LEPCAPY_PCAP_STRUC_H

#include "macros.h"

#define pcaprec_data unsigned char
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

struct pcaprec_hdr_s{
    uint32_t tv_sec;
    int32_t tv_usec;
    uint32_t inc_len;
    uint32_t orig_len;
};

#endif //LEPCAPY_PCAP_STRUC_H
