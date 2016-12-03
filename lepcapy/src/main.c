#include "exception_ctrl.h"
#include "file_io_ctrl.h"
#include "net_io_ctrl.h"

#include <stdio.h>
#include <stdint.h>
#include <stdlib.h>

#include <sys/mman.h>

static struct{
    unsigned long phys_core;
    unsigned long virt_core;
    unsigned long *virt_phys;
}core_info;

static inline void print_result(){
    printf("\n[*] Replay Result\n");
    printf("Total    : %10lupkts\n", thread_file_get_cnt() + thread_file_get_dropped());
    printf("Parsed   : %10lupkts\n", thread_file_get_cnt());
    printf("Filtered : %10lupkts\n", thread_file_get_dropped());
    printf("Replayed : %10lupkts\n\n", thread_net_get_cnt());
}

int main(int argc, char *argv[])
{
    int err_code = SUCCESS;
    FILE *fp = NULL;
    struct pcap_hdr_s p_pcap_hdr;

    if(argc < 4){
        printf("Usage : lepcapy [Dump file] [Interface Name] [IP Address]\n");
        return err_code;
    }

    if(mlockall(MCL_FUTURE))
        raise_except(ERR_CALL_LIBC(mlockall), -EMEM);

    strncpy(env_pktm.if_name, argv[2], IFNAMSIZ - 1);
    if((err_code = ipv4_parse_str(argv[3], &(env_pktm.ipv4_addr.daddr)))){
        raise_except(ERR_CALL(ipv4_parse_str), err_code);
        return err_code;
    }

    if(!(fp = fopen(argv[1], "rb"))){
        raise_except(ERR_CALL_LIBC(fopen), -ENULL);
        return -ENULL;
    }

    //  TODO : Add .so ldr for custom toolkit

    if((err_code = load_pcap_format(fp, &p_pcap_hdr))){
        raise_except(ERR_CALL(load_pcap_format), err_code);
        goto out;
    }

    printf("PCAP Version : %d.%d\n", p_pcap_hdr.version_major, p_pcap_hdr.version_minor);
    printf("Packet Type : %d\n", p_pcap_hdr.network);

    __debug__chkpoint(routine_start);

    queue_init();
    alloc_pktm(p_pktm);

    printf("\n[*] Start Replay...\n\n");

    if((err_code = thread_file_io(fp))){
        raise_except(ERR_CALL(thread_file_io), err_code);
        goto out;
    }

    if((err_code = thread_net_io(thread_file_getthp()))){
        raise_except(ERR_CALL(thread_net_io), err_code);
        pthread_cancel(*thread_file_getthp());
        goto out;
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

    print_result();

    out:
    __debug__chkpoint(clean);
    free_pktm(p_pktm);
    fclose(fp);

    munlockall();
    return err_code;
}
