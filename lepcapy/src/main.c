#include "macros.h"
#include "pcap_glb.h"
#include "pcap_rec.h"

#include <stdio.h>
#include <stdint.h>
#include <stdlib.h>

int main(int argc, char *argv[])
{
    unsigned char err_code = 0;
    FILE *fp = NULL;
    size_t pkt_cnt = 0;
    struct pcaprec_hdr_s* arr_rec_hdr = NULL;
    struct pcap_hdr_s* p_pcap_hdr = NULL;

    if(argc < 2)
        return err_code = -EINVAL;

    fp = fopen(argv[1], "rb");
    if(fp == NULL)
        return -ENULL;

    if(err_code = load_pcap_format(fp, &p_pcap_hdr))
        goto out;

    printf("PCAP Version : %d.%d\n", p_pcap_hdr->version_major, p_pcap_hdr->version_minor);
    printf("Packet Type : %d\n", p_pcap_hdr->network);

    load_pcap_record(fp, &arr_rec_hdr, 300);
    while(pkt_cnt < 300){
        pkt_cnt++;
        printf("[No:%08lu : %08u.%06u] Length : %u / %u \n",
                       pkt_cnt, arr_rec_hdr[pkt_cnt].tv_sec, arr_rec_hdr[pkt_cnt].tv_usec, arr_rec_hdr[pkt_cnt].incl_len, arr_rec_hdr[pkt_cnt].orig_len);
        fseek(fp, arr_rec_hdr[pkt_cnt].incl_len, SEEK_CUR);
    }

    out:
    free_ptr(arr_rec_hdr);
    free_ptr(arr_rec_hdr);
    fclose(fp);
    return err_code;
}
