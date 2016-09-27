#include <stdio.h>
#include <stdint.h>
#include <stdlib.h>

#define PCAP_MAGIC_NUM 0xa1b2c3d4

struct pcap_hdr_s{
    uint32_t magic_number;   /* magic number */
    uint16_t version_major;  /* major version number */
    uint16_t version_minor;  /* minor version number */
    int32_t  thiszone;       /* GMT to local correction */
    uint32_t sigfigs;        /* accuracy of timestamps */
    uint32_t snaplen;        /* max length of captured packets, in octets */
    uint32_t network;        /* data link type */
};

struct pcaprec_hdr_s{
    uint32_t tv_sec;
    uint32_t tv_usec;
    uint32_t incl_len;
    uint32_t orig_len;
};

int main(int argc, char *argv[])
{
    FILE *fp = NULL;
    size_t pkt_cnt = 0;
    struct pcaprec_hdr_s* recHdr = NULL;
    struct pcap_hdr_s* pcapHdr = NULL;

    if(argc < 2)
        goto out;

    fp = fopen(argv[1], "rb");

    if(fp == NULL)
        goto out;

    pcapHdr = (struct pcap_hdr_s*)malloc(sizeof(struct pcap_hdr_s));
    fread(pcapHdr, sizeof(struct pcap_hdr_s), 1, fp);
//    fseek(fp, 24, SEEK_SET);

    if(pcapHdr->magic_number != PCAP_MAGIC_NUM){
        printf("Invalid File Format!\n");
        goto out;
    }

    printf("PCAP Version : %d.%d\n", pcapHdr->version_major, pcapHdr->version_minor);
    printf("Packet Type : %d\n", pcapHdr->network);

    recHdr = (struct pcaprec_hdr_s*)malloc(sizeof(struct pcaprec_hdr_s));
    while(pkt_cnt < 300){
        fread(recHdr, sizeof(struct pcaprec_hdr_s), 1, fp);
        pkt_cnt++;
        printf("[No:%08lu : %08u.%06u] Length : %u / %u \n",
                       pkt_cnt, recHdr->tv_sec, recHdr->tv_usec, recHdr->incl_len, recHdr->orig_len);
        fseek(fp, recHdr->incl_len, SEEK_CUR);
    }

    out:
    if(recHdr != NULL){
        free(recHdr);
        recHdr = NULL;
    }
    if(pcapHdr != NULL){
        free(pcapHdr);
        pcapHdr = NULL;
    }
    return 0;
}
