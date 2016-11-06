#ifndef LEPCAPY_PROTO_CTRL_H
#define LEPCAPY_PROTO_CTRL_H

#include "macros.h"
#include "record_queue.h"

#include "pktm_ethernetII.h"
#include "proto_ethernetII.h"
#include "proto_ipv4.h"

#include <sys/select.h>
#include <sys/time.h>
//#include <time.h>

struct proto_env{
    char if_name[IFNAMSIZ];
    struct proto_ether eth_addr;
    struct proto_ipv4 ipv4_addr;
};

extern struct proto_env env_pktm;
extern struct pktm_object_s *p_pktm;

static inline void __nwait(uint32_t tv_sec, int32_t tv_usec){
    struct timespec timeout = {0, 0};

    timeout.tv_sec = tv_sec;
    timeout.tv_nsec = tv_usec * 1000;

    pselect(0, NULL, NULL, NULL, &timeout, NULL);
}

#endif //LEPCAPY_PROTO_CTRL_H
