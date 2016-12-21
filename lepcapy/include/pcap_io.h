#ifndef LEPCAPY_PCAP_IO_H
#define LEPCAPY_PCAP_IO_H

#include "macros.h"
#include "pcap_struc.h"
#include "arch_deps.h"

#include <stdio.h>
#include <sys/mman.h>

#if defined(__LEPCAPY_ARCH_X86_64__)
extern FILE *__org_fp;
extern unsigned char *__pcap_fbuf;
extern off_t __pcap_foff;
extern size_t __pcap_fsz;
#endif

static inline FILE *arch_fopen(const char *path, const char *opts){
    int tmp_fd = 0;

    if(!(__org_fp = fopen(path, opts)))
        return NULL;

#if defined(__LEPCAPY_ARCH_X86_64__)
    __pcap_foff = 0;
    fseek(__org_fp, 0, SEEK_END);
    __pcap_fsz = ftell(__org_fp);
    fseek(__org_fp, 0, SEEK_SET);
    tmp_fd = fileno(__org_fp);
    if((__pcap_fbuf = mmap(0, __pcap_fsz, PROT_READ, MAP_PRIVATE, tmp_fd, 0)) == (void *)-1)
        return NULL;
#endif

    return __org_fp;
}

static inline int arch_fclose(FILE *fp){
    if(fp != __org_fp)
        return -EINVAL;

#if defined(__LEPCAPY_ARCH_X86_64__)
    if(munmap(__pcap_fbuf, __pcap_fsz))
        return -ESYSDEPS;
#endif

    return fclose(fp);
}

static inline int arch_fread(void *dest, size_t size, size_t num, FILE *fp){
#if defined(__LEPCAPY_ARCH_X86_64__)
    size_t tmp_size;

    if(fp != __org_fp)
        return -EINVAL;

    if(__pcap_foff + (size*num) >= __pcap_fsz)
        tmp_size = __pcap_fsz - __pcap_foff;
    else
        tmp_size = size*num;

    memcpy(dest, __pcap_fbuf + __pcap_foff, tmp_size);
    __pcap_foff += tmp_size;
    return (tmp_size / size);
#else
    return fread(dest, size, num, fp);
#endif
}

static inline int arch_fseek(FILE *fp, long offset, int opts){
#if defined(__LEPCAPY_ARCH_X86_64__)
    if(fp != __org_fp)
        return -EINVAL;
    switch(opts){
    case SEEK_SET:
        __pcap_foff = offset;
        break;
    case SEEK_CUR:
        __pcap_foff += offset;
        break;
    case SEEK_END:
        __pcap_foff = (off_t)__pcap_fsz;
        break;
    default:
        return -EFIO;
    }
    return SUCCESS;
#else
    return fseek(fp, offset, opts);
#endif
}

static inline long arch_ftell(FILE *fp){
#if defined(__LEPCAPY_ARCH_X86_64__)
    if(fp != __org_fp)
        return -EINVAL;
    return __pcap_foff;
#else
    return ftell(fp);
#endif
}

static inline long arch_feof(FILE *fp){
#if defined(__LEPCAPY_ARCH_X86_64__)
    if(fp != __org_fp)
        return -EINVAL;

    return (__pcap_foff >= __pcap_fsz) ? 1 : 0;
#else
    return feof(fp);
#endif
}

int load_pcap_format(FILE *fp, struct pcap_hdr_s *p_pcap_hdr);

int load_pcap_record(FILE *fp, struct pcaprec_hdr_s *p_pcap_rechdr, pcaprec_data **p_pcap_recdata, uint32_t max_len);

int load_pcap_rechdr(FILE *fp, struct pcaprec_hdr_s *p_pcap_rechdr, uint32_t max_len);
int load_pcap_recdata(FILE *fp, pcaprec_data **p_pcap_recdata, uint32_t cnt);

int load_pcap_rechdr_pure(FILE *fp, struct pcaprec_hdr_s *p_pcap_rechdr);
int load_pcap_recdata_pure(FILE *fp, pcaprec_data **p_pcap_recdata, uint32_t cnt);

#endif // LEPCAPY_PCAP_IO_H
