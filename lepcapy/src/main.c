#include "macros.h"
#include "pcap_glb.h"
#include "pcap_queue.h"
#include "ethernetII.h"

#include <stdio.h>
#include <stdint.h>
#include <stdlib.h>

//unsigned long diff(struct timespec start, struct timespec end)
//{
//        if ((end.tv_nsec - start.tv_nsec) < 0)
//                return 1000000000 + end.tv_nsec - start.tv_nsec;
//        else
//                return end.tv_nsec - start.tv_nsec;
//}

int main(int argc, char *argv[])
{
//    struct timespec tspec1, tspec2;
    unsigned long ret_thread = SUCCESS;
    int err_code = SUCCESS;
    FILE *fp = NULL;
    struct pcap_hdr_s p_pcap_hdr;
    pthread_t file_io_thread;

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
//    clock_gettime(CLOCK_MONOTONIC, &tspec1);
    io_thread = 1;
    pthread_create(&file_io_thread, NULL, &thread_file_record_io, (void *)&fp);

    while(1){
//        err_code = queue_enqueue_file_io(&fp);
//        if(err_code && err_code != -EFIO)
//            goto out;
//        if(err_code == -EFIO){
//            err_code = SUCCESS;
//            break;
//        }
        err_code = queue_dequeue_net_io();
        if(err_code){
            sleep(0);
            if(LEPCAPY_EXPECT_F(io_thread == 0))
                break;
        }
//        if(err_code)
//            goto out;
    }
    pthread_join(file_io_thread, (void **)ret_thread);
    err_code = (int)ret_thread;


    out:
    fclose(fp);

//    clock_gettime(CLOCK_MONOTONIC, &tspec2);
//    printf("Elapsed Time : %luns\n", diff(tspec1, tspec2));
    return err_code;
}
