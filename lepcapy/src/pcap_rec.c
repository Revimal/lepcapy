#include "macros.h"
#include "pcap_rec.h"

#include <stdio.h>
#include <stdlib.h>

int load_pcap_rechdr(const FILE** fp, struct pcaprec_hdr_s** p_pcap_rec){
    if(!fp)
        return -EINVAL;

    if(*p_pcap_rec == NULL)
        if(!alloc_type(*p_pcap_rec, struct pcaprec_hdr_s))
            return -ENULL;

    if(fread(*p_pcap_rec, sizeof(struct pcaprec_hdr_s), 1, *fp) != 1){
        free_ptr(*p_pcap_rec);
        return -EFIO;
    }

    fseek(*fp, (*p_pcap_rec)->incl_len, SEEK_CUR);
    return SUCCESS;
}
