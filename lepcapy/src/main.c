#include "exception_ctrl.h"
#include "file_io_ctrl.h"
#include "net_io_ctrl.h"

#include <stdio.h>
#include <stdint.h>
#include <stdlib.h>

#include <sys/mman.h>

int main(int argc, char *argv[])
{
    int err_code = SUCCESS;
    FILE *fp = NULL;
    struct pcap_hdr_s p_pcap_hdr;

    if(argc < 4){
        printf("Usage : lepcapy [Dump file] [Interface Name] [IP Address]\n");
        return err_code;
    }

    mlockall(MCL_FUTURE);

    strncpy(env_pktm.if_name, argv[2], IFNAMSIZ - 1);
    if((err_code = ipv4_parse_str(argv[3], &(env_pktm.ipv4_addr.daddr)))){
        raise_except(ERR_CALL(ipv4_parse_str), err_code);
        return err_code;
    }

    fp = fopen(argv[1], "rb");
    if(fp == NULL){
        raise_except(ERR_CALL_LIBC(fopen), -ENULL);
        return -ENULL;
    }

    //  TODO : Add .so ldr for custom toolkit

    if((err_code = load_pcap_format(fp, &p_pcap_hdr))){
        raise_except(ERR_CALL(load_pcap_format), err_code);
        goto out;
    }
    //  TODO : Need Refactoring (FILE I/O --> Source Control)

    printf("PCAP Version : %d.%d\n", p_pcap_hdr.version_major, p_pcap_hdr.version_minor);
    printf("Packet Type : %d\n", p_pcap_hdr.network);

    /*
     * Start of Routine
     */
    __debug__chkpoint(routine_start);

    queue_init();
    alloc_pktm(p_pktm);
    if((err_code = thread_file_io(fp))){
        raise_except(ERR_CALL(thread_file_io), err_code);
        goto out;
    }
    if((err_code = thread_net_io())){
        raise_except(ERR_CALL(thread_net_io), err_code);
        goto err_net_th;
    };


    err_code = thread_file_join();
    if(err_code){
        __debug__chkpoint(err_thread_file);
        raise_except(ERR_CALL(thread_file_join), err_code);
    }
    err_code = thread_net_join();
    if(err_code){
        __debug__chkpoint(err_thread_net);
        raise_except(ERR_CALL(thread_net_join), err_code);
    }
    goto out;

    err_net_th:
    err_code = thread_file_join();

    out:
    __debug__chkpoint(clean);
    free_pktm(p_pktm);
    fclose(fp);
    munlockall();
    return err_code;
}
