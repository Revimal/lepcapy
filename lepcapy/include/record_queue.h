#ifndef LEPCAPY_RECORD_QUEUE_H
#define LEPCAPY_RECORD_QUEUE_H

#include "macros.h"
#include "pcap_struc.h"
#include "arch_deps.h"

#include <pthread.h>
#include <stdint.h>
#include <unistd.h>

/*
 * Queue Control Macro Functions
 */
#ifdef __LEPCAPY_ARCH_X86__
    #define queue_elem_cnt() queue_list.elem_cnt
    #define queue_current_size() queue_elem_cnt().cnt

    #define queue_round_tail(queue_cnt)         \
        ((queue_cnt) % MAX_QUEUE_SIZE)

    #define queue_elem(queue_cnt)               \
        (queue_list.queue_buf[queue_cnt])

    #define queue_idx_front()                   \
        (queue_list.front.cnt)

    #define queue_idx_rear()                    \
        (queue_list.rear.cnt)

    #define queue_elem_front()                   \
        (queue_elem(queue_idx_front())

    #define queue_elem_rear()                   \
        (queue_elem(queue_idx_rear())
#else
    #define queue_current_size()                \
        (queue_list.rear > queue_list.front  ?   \
        queue_list.rear - queue_list.front   :   \
        queue_list.front - queue_list.rear)

    #define queue_round_tail(queue_cnt)         \
        ((queue_cnt) % MAX_QUEUE_SIZE)

    #define queue_elem(queue_cnt)               \
        (queue_list.queue_buf[queue_cnt])

    #define queue_elem_front()                   \
        (queue_elem(queue_list.front))

    #define queue_elem_rear()                   \
        (queue_elem(queue_list.rear))
#endif

#define get_queue_spinlock() (&(queue_list.queue_spinlock))
#define lock_queue_spinlock() pthread_spin_lock(get_queue_spinlock())
#define unlock_queue_spinlock() pthread_spin_unlock(get_queue_spinlock())

#define __set_relative_tv(cur_sec, cur_usec)  \
    do{                                     \
        queue_list.rel_sec = (cur_sec);       \
        queue_list.rel_usec = (cur_usec);     \
    }while(0)

struct queue_node_s{
    struct pcaprec_hdr_s pcaprec_info;
    pcaprec_data* pcaprec_buf;
} __attribute__((aligned(32)));

struct queue_list_s{
    struct queue_node_s queue_buf[MAX_QUEUE_SIZE];
    uint32_t max_len;

#ifdef __LEPCAPY_ARCH_X86__
    atomic32_t elem_cnt;

    atomic32_t front;
    atomic32_t rear;
#else
    uint32_t front;
    uint32_t rear;
#endif

    uint32_t rel_sec;
    int32_t rel_usec;

    pthread_spinlock_t queue_spinlock;
} __attribute__((aligned(128)));

extern struct queue_list_s queue_list;
extern int io_interact_flag;

static inline void __calc_relative_tv(uint32_t * const tv_sec, int32_t * const tv_usec){
    uint32_t tmp_sec;
    int32_t tmp_usec;

    tmp_sec = *tv_sec;
    tmp_usec = *tv_usec;

    tmp_sec -= queue_list.rel_sec;
    tmp_usec -= queue_list.rel_usec;
    if(tmp_usec < 0){
        (tmp_sec)--;
        tmp_usec += 1000000;
    }

    __set_relative_tv(*tv_sec, *tv_usec);
    *tv_sec = tmp_sec;
    *tv_usec = tmp_usec;
}

int queue_init();

#endif //LEPCAPY_RECORD_QUEUE_H
