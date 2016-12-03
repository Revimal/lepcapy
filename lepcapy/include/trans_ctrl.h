#ifndef LEPCAPY_PROTO_CTRL_H
#define LEPCAPY_PROTO_CTRL_H

#include "macros.h"
#include "record_queue.h"

#include "pktm_ethernetII.h"
#include "proto_ethernetII.h"
#include "proto_ipv4.h"

#include <sys/time.h>
//#include <time.h>

struct proto_env{
    char if_name[IFNAMSIZ];
    struct proto_ether eth_addr;
    struct proto_ipv4 ipv4_addr;
};

extern struct proto_env env_pktm;
extern struct pktm_object_s *p_pktm;

#ifdef __LEPCAPY_ARCH_OPTS__

extern atomic64_t fastbuf_front;
extern atomic64_t fastbuf_rear;
extern atomic64_t fastbuf_size;
extern __volatile__ atomic64_t fastbuf_dummy;

extern struct queue_node_s fastbuf_arr[FAST_BUFFER_SIZE];

static inline void __enqueue_fastbuf(){
    if(fastbuf_size.cnt >= FAST_BUFFER_SIZE)
        return;

    lock_queue_spinlock();
    __fastcpy_aligned32_wcmem(&fastbuf_arr[fastbuf_rear.cnt], &queue_elem_front());
    queue_elem_front().pcaprec_buf = NULL;
    queue_list.front = queue_round_tail(queue_list.front + 1);
    unlock_queue_spinlock();
    atomic64_mov(&fastbuf_rear, (fastbuf_rear.cnt + 1) % FAST_BUFFER_SIZE);
    atomic64_inc(&fastbuf_size);
}

static inline int __chk_fastbuf(){
    return fastbuf_size.cnt;
}

static inline struct queue_node_s *__access_fastbuf(){
    return fastbuf_size.cnt ? &fastbuf_arr[fastbuf_front.cnt] : NULL;
}

static inline void __dequeue_fastbuf(){
    if(!fastbuf_size.cnt)
        return;

    free_ptr(fastbuf_arr[fastbuf_front.cnt].pcaprec_buf);
    atomic64_mov(&fastbuf_front, (fastbuf_front.cnt + 1) % FAST_BUFFER_SIZE);
    atomic64_dec(&fastbuf_size);
}
#endif

static inline void __nwait(uint32_t tv_sec, int32_t tv_usec){
    struct timespec timeout = {0, 0};

    timeout.tv_sec = tv_sec;
    timeout.tv_nsec = tv_usec * 1000;

    pselect(0, NULL, NULL, NULL, &timeout, NULL);
}

static inline void __nwait_release_lock(uint32_t tv_sec, int32_t tv_usec){
    struct timespec timeout = {0, 0};

    timeout.tv_sec = tv_sec;
    timeout.tv_nsec = tv_usec * 1000;

    unlock_queue_spinlock();
    pselect(0, NULL, NULL, NULL, &timeout, NULL);
    lock_queue_spinlock();
}

#endif //LEPCAPY_PROTO_CTRL_H
