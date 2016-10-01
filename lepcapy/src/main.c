#include "macros.h"
#include "pcap_glb.h"
#include "pcap_rec.h"

#include <stdio.h>
#include <stdint.h>
#include <stdlib.h>

int main(int argc, char *argv[])
{
    int err_code = SUCCESS;
    FILE *fp = NULL;
    size_t pkt_cnt = 0;
    struct pcaprec_hdr_s *p_rec_hdr = NULL;
    pcaprec_data *p_rec_data = NULL;
    struct pcap_hdr_s *p_pcap_hdr = NULL;

    if(argc < 2)
        return err_code = -EINVAL;

    fp = fopen(argv[1], "rb");
    if(fp == NULL)
        return -ENULL;

    if(err_code = load_pcap_format(&fp, &p_pcap_hdr))
        goto out;

    printf("PCAP Version : %d.%d\n", p_pcap_hdr->version_major, p_pcap_hdr->version_minor);
    printf("Packet Type : %d\n", p_pcap_hdr->network);

    while(1){
        err_code = load_pcap_record(&fp, &p_rec_hdr, &p_rec_data);
        if(err_code && err_code != -EFIO)
            goto out;
        if(err_code == -EFIO){
            err_code = SUCCESS;
            break;
        }

        pkt_cnt++;
        printf("[No:%08lu : %08u.%06u] Length : %u / %u \n",
            pkt_cnt, p_rec_hdr->tv_sec, p_rec_hdr->tv_usec,
            p_rec_hdr->inc_len, p_rec_hdr->orig_len);
    }

    out:
    free_ptr(p_rec_hdr);
    free_ptr(p_pcap_hdr);
    fclose(fp);
    return err_code;
}
