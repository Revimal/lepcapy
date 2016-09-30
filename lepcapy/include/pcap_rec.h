#ifndef LEPCAPY_PCAP_REC_H
#define LEPCAPY_PCAP_REC_H

#include <stdio.h>
#include <stdint.h>

struct pcaprec_hdr_s{
    uint32_t tv_sec;
    uint32_t tv_usec;
    uint32_t incl_len;
    uint32_t orig_len;
};

int load_pcap_rechdr(const FILE** fp, struct pcaprec_hdr_s** p_pcap_rec);

#endif // LEPCAPY_PCAP_REC_H
