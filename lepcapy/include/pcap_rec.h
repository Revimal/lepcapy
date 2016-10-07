#ifndef LEPCAPY_PCAP_REC_H
#define LEPCAPY_PCAP_REC_H

#include "macros.h"

#include <stdio.h>
#include <stdint.h>

#define pcaprec_data unsigned char

struct pcaprec_hdr_s{
    uint32_t tv_sec;
    uint32_t tv_usec;
    uint32_t inc_len;
    uint32_t orig_len;
};

int load_pcap_record(FILE **fp, struct pcaprec_hdr_s *p_pcap_rechdr, pcaprec_data **p_pcap_recdata);
int load_pcap_rechdr(FILE **fp, struct pcaprec_hdr_s *p_pcap_rechdr);
int load_pcap_recdata(FILE **fp, pcaprec_data **p_pcap_recdata, uint32_t cnt);

#endif // LEPCAPY_PCAP_REC_H
