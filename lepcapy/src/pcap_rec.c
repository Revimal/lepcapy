#include "macros.h"
#include "pcap_rec.h"

#include <stdio.h>
#include <stdlib.h>

int load_pcap_record(FILE* fp, struct pcaprec_hdr_s** p_pcap_rec, int cnt){
    if(!fp)
        return -EINVAL;

    if(*p_pcap_rec == NULL)
        if(!alloc_contig(*p_pcap_rec, struct pcaprec_hdr_s, cnt))
            return -ENULL;

    if(fread(*p_pcap_rec, sizeof(struct pcaprec_hdr_s), cnt, fp) != cnt){
        free_ptr(*p_pcap_rec);
        return -EFIO;
    }

    return SUCCESS;
}
