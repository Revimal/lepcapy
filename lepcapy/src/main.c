#include "macros.h"
#include "pcap_glb.h"
#include "pcap_queue.h"
#include "ethernetII.h"

#include <stdio.h>
#include <stdint.h>
#include <stdlib.h>

int main(int argc, char *argv[])
{
    int err_code = SUCCESS;
    FILE *fp = NULL;
    size_t pkt_cnt = 0;
//    struct pcaprec_hdr_s p_rec_hdr;
//    pcaprec_data *p_rec_data = NULL;
    struct pcap_hdr_s p_pcap_hdr;
    struct ethernetII_layer_s test;

    if(argc < 2)
        return err_code = -EINVAL;

    fp = fopen(argv[1], "rb");
    if(fp == NULL)
        return -ENULL;

    if((err_code = load_pcap_format(&fp, &p_pcap_hdr)))
        goto out;

    printf("PCAP Version : %d.%d\n", p_pcap_hdr.version_major, p_pcap_hdr.version_minor);
    printf("Packet Type : %d\n", p_pcap_hdr.network);

    queue_init();

    while(1){        
        err_code = queue_enqueue_file_io(&fp);
        if(err_code && err_code != -EFIO && err_code != -EQUEUE)
            goto out;
        if(err_code == -EQUEUE || err_code == -EFIO){
            err_code = SUCCESS;
            break;
        }

        err_code = queue_dequeue_net_io();
        if(err_code)
            goto out;
//        printf("Current Queue Size : %u\n", queue_current_size());
//        printf("[No:%08lu : %08u.%06u] Length : %u / %u \n",
//            pkt_cnt + 1, queue_elem(pkt_cnt).pcaprec_info.tv_sec,
//            queue_elem(pkt_cnt).pcaprec_info.tv_usec,
//            queue_elem(pkt_cnt).pcaprec_info.inc_len,
//            queue_elem(pkt_cnt).pcaprec_info.orig_len);
//        ethernetII_operation.parse_queue_decap(&queue_elem(pkt_cnt), PROTO_LAYER(&test));
//        printf("[Src]\t");
//        ViewMac(test.eth_header->ether_shost);
//        printf("\n[Dest]\t");
//        ViewMac(test.eth_header->ether_dhost);
//        printf("\n[Type]\t%#2x\n\n", ntohs(test.eth_header->ether_type));
////        ethernetII_operation.parse_buf_decap_ptr(queue_elem(pkt_cnt).pcaprec_buf);
//        pkt_cnt++;
    }

    out:
//    free_ptr(p_rec_data);
    fclose(fp);
    return err_code;
}
