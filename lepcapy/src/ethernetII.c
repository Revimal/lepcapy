#include "macros.h"
#include "pcap_parser.h"
#include "pcap_queue.h"
#include "ethernetII.h"

//int ethernetII_parse_decap()

unsigned short ether_ntohs(unsigned short value)
{
        return(value << 8) | (value >> 8);
}

void ViewMac(unsigned char *mac)
{
        int i;
        for (i = 0;i < 6; ++i)
        {
            if(i<5)
               printf("%02x:", mac[i]);
            else
               printf("%02x", mac[i]);
        }
}

struct pcap_parser_s ethernetII_operation = {
    parse_queue_decap   :   ethernetII_queue_decap,
    parse_idx_decap     :   ethernetII_idx_decap,
};

LEPCAPY_EXPLICIT int ethernetII_queue_decap(struct queue_node_s* src_queue, struct ethernetII_layer_s* dest_layer){
    if(src_queue->tail - src_queue->head < sizeof(struct ether_header))
        return -EINVAL;

    dest_layer->eth_header = (struct ether_header*)src_queue->head;
    src_queue->head -= sizeof(struct ether_header);
    return SUCCESS;
}

LEPCAPY_EXPLICIT int ethernetII_idx_decap(uint32_t src_idx, struct ethernetII_layer_s* dest_layer){
    struct queue_node_s* ptr_queue = &queue_elem(src_idx);
    if(ptr_queue->tail - ptr_queue->head < sizeof(struct ether_header))
        return -EINVAL;

    dest_layer->eth_header = (struct ether_header*)ptr_queue->head;
    ptr_queue->head -= sizeof(struct ether_header);

    return SUCCESS;
}

//int ethernetII_buf_decap_ptr(pcaprec_data* src_record){
//    struct ethernetII_hdr_s* etherheader = (struct ethernetII_hdr_s*)src_record;
//    printf("Src : 0x");
//    ViewMac(etherheader->ether_src);
//    printf("\nDest : 0x");
//    ViewMac(etherheader->ether_dest);
//    printf("\nProtocol : %#x\n", ether_ntohs(etherheader->ether_type));
//    return 0;
//}
