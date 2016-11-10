#ifndef LEPCAPY_PCAP_IO_H
#define LEPCAPY_PCAP_IO_H

#include "macros.h"
#include "pcap_struc.h"

//TODO pure_rechdr

int load_pcap_format(FILE *fp, struct pcap_hdr_s *p_pcap_hdr);

int load_pcap_record(FILE *fp, struct pcaprec_hdr_s *p_pcap_rechdr, pcaprec_data **p_pcap_recdata, uint32_t max_len);

int load_pcap_rechdr(FILE *fp, struct pcaprec_hdr_s *p_pcap_rechdr, uint32_t max_len);
int load_pcap_rechdr_pure(FILE *fp, struct pcaprec_hdr_s *p_pcap_rechdr);

int load_pcap_recdata(FILE *fp, pcaprec_data **p_pcap_recdata, uint32_t cnt);
int load_pcap_recdata_pure(FILE *fp, pcaprec_data **p_pcap_recdata, uint32_t cnt);

#endif // LEPCAPY_PCAP_IO_H
