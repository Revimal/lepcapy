#include "file_io_ctrl.h"

#include <stdio.h>
#include <stdint.h>
#include <stdlib.h>

int main(int argc, char *argv[])
{
    int err_code = SUCCESS;
    FILE *fp = NULL;
    struct pcap_hdr_s p_pcap_hdr;

    if(argc < 4){
        printf("Usage : lepcapy [Dump file] [Interface Name] [IP Address]\n");
        err_code = -EINVAL;
        return err_code;
    }

    strncpy(env_pktm.if_name, argv[2], IFNAMSIZ - 1);
    if((err_code = parse_ip_str(argv[3], &(env_pktm.ipv4_addr.daddr))))
        return err_code;

    fp = fopen(argv[1], "rb");
    if(fp == NULL)
        return -ENULL;

    //  TODO : Add .so ldr for custom toolkit

    if((err_code = load_pcap_format(fp, &p_pcap_hdr)))
        goto out;
    //  TODO : Need Refactoring (FILE I/O --> Source Control)

    printf("PCAP Version : %d.%d\n", p_pcap_hdr.version_major, p_pcap_hdr.version_minor);
    printf("Packet Type : %d\n", p_pcap_hdr.network);

    /*
     * Start of Routine
     */
    queue_init();
    alloc_pktm(p_pktm);
    thread_file_io(fp);

//    while(1){
//        err_code = queue_enqueue_file_io(&fp);
//        if(err_code && err_code != -EFIO)
//            goto out;
//        if(err_code == -EFIO){
//            err_code = SUCCESS;
//            break;
//        }

//        err_code = queue_dequeue_net_io();
//        if(err_code){
//            sleep(0);
//            if(LEPCAPY_EXPECT_F(io_interact_flag == 0))
//                break;
//        }

//        if(err_code)
//            goto out;
//    }
//    pthread_join(file_io_thread, (void **)ret_thread);
//    err_code = (int)ret_thread;

    err_code = thread_file_join();

    out:
    free_pktm(p_pktm);
    fclose(fp);
    return err_code;
}
