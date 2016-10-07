#include "macros.h"
#include "pcap_glb.h"
#include "pcap_queue.h"

#include <stdio.h>
#include <stdlib.h>

int load_pcap_format(FILE **fp, struct pcap_hdr_s *p_pcap_hdr){
    if(!fp)
        return -EINVAL;

    if(p_pcap_hdr == NULL)
        if(!alloc_type(p_pcap_hdr, struct pcap_hdr_s))
            return -ENULL;

    if(fread(p_pcap_hdr, sizeof(struct pcap_hdr_s), 1, *fp) != 1){
        free_ptr(p_pcap_hdr);
        return -EFIO;
    }

    if(p_pcap_hdr->magic_number != PCAP_MAGIC_NUM){
        free_ptr(p_pcap_hdr);
        return -EMAGIC;
    }

    return SUCCESS;
}

//int thread_file_record_io(FILE **fp){

//}
