#include "macros.h"
#include "pcap_rec.h"
#include "pcap_queue.h"
#include "ethernetII.h"

#include <stdio.h>
#include <stdlib.h>

struct queue_list_s queue_list = {
    head : 0,
    tail : 0,
    parse_cnt : 0,
};

int queue_init(){
    int i;

    if(pthread_spin_init(get_queue_spinlock(), PTHREAD_PROCESS_PRIVATE))
        return -ETHREAD;

    for(i = 0; i < MAX_QUEUE_SIZE; ++i)
        queue_elem(i).pcaprec_buf = NULL;

    return SUCCESS;
}

int queue_enqueue_file_io(FILE** fp){
    int err_code = SUCCESS;
    unsigned char *temp_ptr = NULL;


    if(!fp)
        return -EINVAL;

    lock_queue_spinlock();
    if(queue_round_tail((queue_list.tail + 1)) == queue_list.head){
        unlock_queue_spinlock();
        return -EQUEUE;
    }
    unlock_queue_spinlock();

    if((err_code = load_pcap_record(fp,
            &(queue_elem_tail().pcaprec_info),
            &(queue_elem_tail().pcaprec_buf))))
        return err_code;

    temp_ptr = queue_elem_tail().pcaprec_buf;
    queue_elem_tail().head = temp_ptr;
    temp_ptr += queue_elem_tail().pcaprec_info.inc_len;
    queue_elem_tail().tail = temp_ptr;


    lock_queue_spinlock();
    queue_list.tail = queue_round_tail(queue_list.tail + 1);
    unlock_queue_spinlock();
    return err_code;
}

int queue_dequeue_net_io(){
    struct ethernetII_layer_s test;
    int err_code = SUCCESS;

    lock_queue_spinlock();
    if(queue_list.head == queue_list.tail){
        unlock_queue_spinlock();
        return -EQUEUE;
    }
    unlock_queue_spinlock();

    printf("Current Queue Size : %u\n", queue_current_size());
    printf("[No:%08lu : %08u.%06u] Length : %u / %u \n",
        queue_list.head + 1, queue_elem_head().pcaprec_info.tv_sec,
        queue_elem_head().pcaprec_info.tv_usec,
        queue_elem_head().pcaprec_info.inc_len,
        queue_elem_head().pcaprec_info.orig_len);
    ethernetII_operation.parse_queue_decap(&queue_elem_head(), PROTO_LAYER(&test));
    printf("[Src]\t");
    ViewMac(test.eth_header->ether_shost);
    printf("\n[Dest]\t");
    ViewMac(test.eth_header->ether_dhost);
    printf("\n[Type]\t%#2x\n\n", ntohs(test.eth_header->ether_type));

    lock_queue_spinlock();
    free_ptr(queue_elem_head().pcaprec_buf);
    queue_list.head = queue_round_tail(queue_list.head + 1);
    unlock_queue_spinlock();

    return err_code;
}
