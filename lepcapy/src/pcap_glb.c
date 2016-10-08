#include "macros.h"
#include "pcap_glb.h"
#include "pcap_queue.h"

#include <stdio.h>
#include <stdlib.h>

int load_pcap_format(FILE **fp, struct pcap_hdr_s *p_pcap_hdr){
    if(!fp || p_pcap_hdr == NULL)
        return -EINVAL;

    if(fread(p_pcap_hdr, sizeof(struct pcap_hdr_s), 1, *fp) != 1)
        return -EFIO;

    if(p_pcap_hdr->magic_number != PCAP_MAGIC_NUM)
        return -EMAGIC;

    return SUCCESS;
}

void *thread_file_record_io(void *file_ptr){
    int err_code = SUCCESS;
    while(1){
        err_code = queue_enqueue_file_io((FILE**)file_ptr);
        if(err_code == -EQUEUE)
            sleep(0);
        else if(err_code)
            break;
    }
    io_thread = 0;
    pthread_exit((void *)err_code);
}

//void* thread_network_record_io(LEPCAPY_DEBUG void *arg){
//    int err_code = SUCCESS;
//    while(queue_current_size() < NETIO_QUEUING_SIZE)
//        sleep(0);
//    while(1){
//        err_code = queue_dequeue_net_io();
//        if(err_code == -EQUEUE)
//            sleep(0);
//        }
//    }
//    pthread_exit((void*)err_code);
//}
