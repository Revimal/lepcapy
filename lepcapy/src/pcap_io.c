#include "pcap_io.h"

/*
 *
 * TODO : Buffer Overflow Check
 * - Complete (2016/10/25)
 *
 */

int load_pcap_format(FILE *fp, struct pcap_hdr_s *p_pcap_hdr){
    if(!fp || p_pcap_hdr == NULL)
        return -EINVAL;

    if(fread(p_pcap_hdr, sizeof(struct pcap_hdr_s), 1, fp) != 1)
        return -EFIO;

    if(p_pcap_hdr->magic_number != PCAP_MAGIC_NUM)
        return -EMAGIC;

    return SUCCESS;
}

int load_pcap_record(FILE *fp, struct pcaprec_hdr_s *p_pcap_rechdr, pcaprec_data **p_pcap_recdata, uint32_t max_len){
    int err_code = SUCCESS;

    if((err_code = load_pcap_rechdr_pure(fp, p_pcap_rechdr)))
        return err_code;

    if(p_pcap_rechdr->inc_len > max_len * 8){
        err_code = -EOVRFLW;
        goto out;
    }

    if((err_code = load_pcap_recdata_pure(fp, p_pcap_recdata,  p_pcap_rechdr->inc_len)))
        goto out;

    goto success;

    out:
    free_ptr(*p_pcap_recdata);
    success:
    return err_code;
}

int load_pcap_rechdr(FILE *fp, struct pcaprec_hdr_s *p_pcap_rechdr, uint32_t max_len){
    int err_code = SUCCESS;

    if((err_code = load_pcap_rechdr_pure(fp, p_pcap_rechdr)))
        return err_code;

    if(p_pcap_rechdr->inc_len > max_len * 8)
        return -EOVRFLW;

    fseek(fp, p_pcap_rechdr->inc_len, SEEK_CUR);
    return SUCCESS;
}

int load_pcap_rechdr_pure(FILE *fp, struct pcaprec_hdr_s *p_pcap_rechdr){
    if(!fp || p_pcap_rechdr == NULL)
        return -EINVAL;

    if(fread(p_pcap_rechdr, sizeof(struct pcaprec_hdr_s), 1, fp) != 1)
        return -EFIO;

    return SUCCESS;
}

int load_pcap_recdata(FILE *fp, pcaprec_data **p_pcap_recdata, uint32_t cnt){
    int err_code = SUCCESS;

    if(!fp)
        return -EINVAL;

    if((err_code = load_pcap_recdata_pure(fp, p_pcap_recdata, cnt)))
        return err_code;

    fseek(fp, sizeof(struct pcaprec_hdr_s), SEEK_CUR);
    return err_code;
}

int load_pcap_recdata_pure(FILE *fp, pcaprec_data **p_pcap_recdata, uint32_t cnt){
    if(*p_pcap_recdata == NULL)
        if(!alloc_contig(*p_pcap_recdata, pcaprec_data, cnt))
            return -ENULL;

    if(fread(*p_pcap_recdata, 1, cnt, fp) != cnt){
        free_ptr(p_pcap_recdata);
        return -EFIO;
    }

    return SUCCESS;
}
