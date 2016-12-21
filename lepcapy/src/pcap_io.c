#include "exception_ctrl.h"
#include "pcap_io.h"

#if defined(__LEPCAPY_ARCH_X86_64__)
FILE *__org_fp = NULL;
unsigned char *__pcap_fbuf = NULL;
off_t __pcap_foff = 0;
size_t __pcap_fsz = 0;
#endif

static inline int fread_chk_eof(void *ptr, size_t size, size_t num, FILE *fp){
    if(arch_fread(ptr, size, num, fp) != num){
        if(arch_feof(fp))
            return -__EEOF;
        else
            return -EFIO;
    }
    return SUCCESS;
}

int load_pcap_format(FILE *fp, struct pcap_hdr_s *p_pcap_hdr){
    if(!(fp && p_pcap_hdr)){
        raise_except(ERR_NULL(fp|p_pcap_hdr), -EINVAL);
        return -EINVAL;
    }

    if(arch_fread(p_pcap_hdr, sizeof(struct pcap_hdr_s), 1, fp) != 1){
        raise_except(ERR_CALL_ARCH(arch_fread), -EFIO);
        return -EFIO;
    }

    if(p_pcap_hdr->magic_number != PCAP_MAGIC_NUM){
        raise_except(ERR_INVAL(magic_number), -EMAGIC);
        return -EMAGIC;
    }

    return SUCCESS;
}

int load_pcap_record(FILE *fp, struct pcaprec_hdr_s *p_pcap_rechdr, pcaprec_data **p_pcap_recdata, uint32_t max_len){
    int err_code = SUCCESS;

    if((err_code = load_pcap_rechdr_pure(fp, p_pcap_rechdr))){
        if(err_code != -__EEOF)
            raise_except(ERR_CALL(load_pcap_rechdr_pure), err_code);
        goto out;
    }

    if(p_pcap_rechdr->inc_len > max_len){
        raise_except(ERR_INVAL(inc_len), err_code);
        err_code = -EOVRFLW;
        goto out;
    }

    if(!alloc_contig(*p_pcap_recdata, pcaprec_data, p_pcap_rechdr->inc_len + 1)){
        raise_except(ERR_CALL_MACRO(alloc_contig), -ENULL);
        err_code = -ENULL;
        goto out;
    }

    if((err_code = load_pcap_recdata_pure(fp, p_pcap_recdata, p_pcap_rechdr->inc_len))){
            raise_except(ERR_CALL(load_pcap_recdata_pure), err_code);
            goto err_free;
    }

    goto out;

    err_free:
    free_ptr(*p_pcap_recdata);
    out:
    return err_code;
}

int load_pcap_rechdr(FILE *fp, struct pcaprec_hdr_s *p_pcap_rechdr, uint32_t max_len){
    int err_code = SUCCESS;

    if((err_code = load_pcap_rechdr_pure(fp, p_pcap_rechdr))){
        if(err_code != -__EEOF)
            raise_except(ERR_CALL(load_pcap_rechdr_pure), err_code);
        return err_code;
    }

    if(p_pcap_rechdr->inc_len > max_len){
        raise_except(ERR_INVAL(inc_len), err_code);
        return -EOVRFLW;
    }

    if(arch_fseek(fp, p_pcap_rechdr->inc_len, SEEK_CUR)){
        raise_except(ERR_CALL_ARCH(arch_fseek), -EFIO);
        return -EFIO;
    }

    return err_code;
}

int load_pcap_recdata(FILE *fp, pcaprec_data **p_pcap_recdata, uint32_t cnt){
    int err_code = SUCCESS;

    if(!fp){
        raise_except(ERR_NULL(fp), -EINVAL);
        return -EINVAL;
    }

    if((err_code = load_pcap_recdata_pure(fp, p_pcap_recdata, cnt))){
        raise_except(ERR_CALL(load_pcap_rechdr_pure), err_code);
        return err_code;
    }

    if(arch_fseek(fp, sizeof(struct pcaprec_hdr_s), SEEK_CUR)){
        raise_except(ERR_CALL_ARCH(arch_fseek), -EFIO);
        return -EFIO;
    }

    return err_code;
}

int load_pcap_rechdr_pure(FILE *fp, struct pcaprec_hdr_s *p_pcap_rechdr){
    int err_code = SUCCESS;

    if(!(fp && p_pcap_rechdr)){
        raise_except(ERR_NULL(fp|p_pcap_rechdr), -EINVAL);
        return -EINVAL;
    }

    if((err_code = fread_chk_eof(p_pcap_rechdr, sizeof(struct pcaprec_hdr_s), 1, fp))){
        if(err_code != -__EEOF)
            raise_except(ERR_CALL_LIBC(fread), -EFIO);
        return err_code;
    }

    return err_code;
}

int load_pcap_recdata_pure(FILE *fp, pcaprec_data **p_pcap_recdata, uint32_t cnt){
    if(!(fp && *p_pcap_recdata)){
        raise_except(ERR_NULL(fp|p_pcap_recdata), -EINVAL);
        return -EINVAL;
    }

    if(arch_fread(*p_pcap_recdata, 1, cnt, fp) != cnt){
        raise_except(ERR_CALL_ARCH(arch_fread), -EFIO);
        free_ptr(p_pcap_recdata);
        return -EFIO;
    }

    return SUCCESS;
}
