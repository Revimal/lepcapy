#include "macros.h"
#include "pcap_rec.h"

#include <stdio.h>
#include <stdlib.h>

int load_pcap_record(FILE **fp, struct pcaprec_hdr_s **p_pcap_rechdr, pcaprec_data **p_pcap_recdata){
    int err_code = SUCCESS;
    uint32_t inc_len;

    if(!fp){
        err_code = -EINVAL;
        goto out;
    }

    if(*p_pcap_rechdr == NULL)
        if(!alloc_type(*p_pcap_rechdr, struct pcaprec_hdr_s)){
            err_code = -ENULL;
            goto out;
        }

    if(fread(*p_pcap_rechdr, sizeof(struct pcaprec_hdr_s), 1, *fp) != 1){
        err_code = -EFIO;
        goto out;
    }

    inc_len = (*p_pcap_rechdr)->inc_len;

    if(*p_pcap_recdata == NULL)
        if(!alloc_contig(*p_pcap_recdata, pcaprec_data, inc_len)){
            err_code = -ENULL;
            goto out;
        }

    if(fread(*p_pcap_recdata, 1, inc_len, *fp) != inc_len){
        err_code = -EFIO;
        goto out;
    }

    goto success;

    out:
    free_ptr(*p_pcap_rechdr);
    free_ptr(*p_pcap_recdata);

    success:
    return err_code;
}

int load_pcap_rechdr(FILE **fp, struct pcaprec_hdr_s **p_pcap_rechdr){
    if(!fp)
        return -EINVAL;

    if(*p_pcap_rechdr == NULL)
        if(!alloc_type(*p_pcap_rechdr, struct pcaprec_hdr_s))
            return -ENULL;

    if(fread(*p_pcap_rechdr, sizeof(struct pcaprec_hdr_s), 1, *fp) != 1){
        free_ptr(*p_pcap_rechdr);
        return -EFIO;
    }

    fseek(*fp, (*p_pcap_rechdr)->inc_len, SEEK_CUR);
    return SUCCESS;
}

int load_pcap_recdata(FILE **fp, pcaprec_data **p_pcap_recdata, uint32_t cnt){
    if(!fp)
        return -EINVAL;

    if(*p_pcap_recdata == NULL)
        if(!alloc_contig(*p_pcap_recdata, pcaprec_data, cnt))
            return -ENULL;

    if(fread(*p_pcap_recdata, 1, cnt, *fp) != cnt){
        free_ptr(*p_pcap_recdata);
        return -EFIO;
    }

    fseek(*fp, sizeof(struct pcaprec_hdr_s), SEEK_CUR);
    return SUCCESS;
}
