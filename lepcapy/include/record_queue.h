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
    (queue_list.tail > queue_list.head  ?   \
    queue_list.tail - queue_list.head   :   \
    queue_list.head - queue_list.tail)

#define queue_round_tail(queue_cnt)         \
    ((queue_cnt) % MAX_QUEUE_SIZE)

#define queue_elem(queue_cnt)               \
    (queue_list.queue_buf[queue_cnt])


/*
 * Deprecated!!!
 */
#define queue_elem_head()                   \
    (queue_elem(queue_list.head))

#define queue_elem_tail()                   \
(queue_elem(queue_list.tail))
/*
 *
 */

#define get_queue_spinlock() (&(queue_list.queue_spinlock))
#define lock_queue_spinlock() pthread_spin_lock(get_queue_spinlock())
#define unlock_queue_spinlock() pthread_spin_unlock(get_queue_spinlock())

#define get_relative_tv(sec_val, usec_val)  \
    do{                                     \
        sec_val -= queue_list.base_sec;     \
        usec_val -= queue_list.base_usec;   \
        if(usec_val < 0){                   \
            (sec_val)--;                    \
            usec_val += 1000000;            \
        }                                   \
    }while(0)


struct queue_node_s{
    struct pcaprec_hdr_s pcaprec_info;
    pcaprec_data* pcaprec_buf;

    unsigned char* head;    //Protocol header
    unsigned char* payload; //Protocol Payload
    unsigned char* tail;    //Protocol tail
};

struct queue_list_s{
    struct queue_node_s queue_buf[MAX_QUEUE_SIZE];
    uint32_t max_len;

    uint32_t head;
    uint32_t tail;

    uint32_t base_sec;
    int32_t base_usec;

    pthread_spinlock_t queue_spinlock;
};

extern struct queue_list_s queue_list;
extern int io_interact_flag;

int queue_init();

#endif //LEPCAPY_RECORD_QUEUE_H
