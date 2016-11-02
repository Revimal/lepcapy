#ifndef LEPCAPY_RECORD_QUEUE_H
#define LEPCAPY_RECORD_QUEUE_H

#include "macros.h"
#include "pcap_struc.h"

#include <pthread.h>
#include <stdint.h>
#include <unistd.h>

/*
 * Support OSX
 */
#if (defined(__APPLE__) && defined(__MACH__))
    #include <libkern/OSAtomic.h>
    #define pthread_spinlock_t OSSpinLock
    #define pthread_spin_lock(spinlock)         \
        OSSpinLockLock(get_queue_spinlock())
    #define pthread_spin_unlock(spinlock)       \
        OSSpinLockUnlock(get_queue_spinlock())
    #define pthread_spin_init(spinlock, mode)   0
#endif


/*
 * Queue Control Macro Functions
 */
#define queue_current_size()                \
    (queue_list.rear > queue_list.front  ?   \
    queue_list.rear - queue_list.front   :   \
    queue_list.front - queue_list.rear)

#define queue_round_tail(queue_cnt)         \
    ((queue_cnt) % MAX_QUEUE_SIZE)

#define queue_elem(queue_cnt)               \
    (queue_list.queue_buf[queue_cnt])

/*
 * Deprecated!!!
 */
#define queue_elem_front()                   \
    (queue_elem(queue_list.front))

#define queue_elem_rear()                   \
    (queue_elem(queue_list.rear))
//

#define get_queue_spinlock() (&(queue_list.queue_spinlock))
#define lock_queue_spinlock() pthread_spin_lock(get_queue_spinlock())
#define unlock_queue_spinlock() pthread_spin_unlock(get_queue_spinlock())

#define set_relative_tv(cur_sec, cur_usec)  \
    do{                                     \
        queue_list.rel_sec = cur_sec;       \
        queue_list.rel_usec = cur_usec;     \
    }while(0)

#define calc_relative_tv(cur_sec, cur_usec) \
    do{                                     \
        cur_sec -= queue_list.rel_sec;      \
        cur_usec -= queue_list.rel_usec;    \
        if(cur_usec < 0){                   \
            (cur_sec)--;                    \
            cur_usec += 1000000;            \
        }                                   \
    }while(0)

struct queue_node_s{
    struct pcaprec_hdr_s pcaprec_info;
    pcaprec_data* pcaprec_buf;

    unsigned char* head;    //Pcap record head
    unsigned char* tail;    //Pcap record tail
};

struct queue_list_s{
    struct queue_node_s queue_buf[MAX_QUEUE_SIZE];
    uint32_t max_len;

    uint32_t front;
    uint32_t rear;

    uint32_t rel_sec;
    int32_t rel_usec;

    pthread_spinlock_t queue_spinlock;
};

extern struct queue_list_s queue_list;
extern int io_interact_flag;

int queue_init();

#endif //LEPCAPY_RECORD_QUEUE_H
