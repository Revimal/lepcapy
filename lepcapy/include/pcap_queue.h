#ifndef LEPCAPY_PCAP_QUEUE_H
#define LEPCAPY_PCAP_QUEUE_H

#include "macros.h"
#include "pcap_rec.h"

#include <pthread.h>
#include <stdio.h>
#include <stdint.h>

#define queue_current_size()                \
    (queue_list.tail > queue_list.head ? queue_list.tail - queue_list.head : queue_list.head - queue_list.tail)

#define queue_round_tail(queue_cnt)         \
    ((queue_cnt) % MAX_QUEUE_SIZE)

#define queue_elem(queue_cnt)               \
    (queue_list.queue_buf[queue_cnt])

#define queue_elem_head()                   \
    (queue_elem(queue_list.head))

#define queue_elem_tail()                   \
    (queue_elem(queue_list.tail))

#define get_queue_spinlock() (&(queue_list.queue_spinlock))
#define lock_queue_spinlock() pthread_spin_lock(get_queue_spinlock())
#define unlock_queue_spinlock() pthread_spin_unlock(get_queue_spinlock())

//struct protocol_node_s{
//    unsigned char* begin;
//    unsigned char* end;

//    unsigned char* head;
//    unsigned char* payload;
//    unsigned char* tail;
//};

struct queue_node_s{
    struct pcaprec_hdr_s pcaprec_info;
    pcaprec_data* pcaprec_buf;

    unsigned char* head;    //Protocol header
    unsigned char* payload; //Protocol Payload
    unsigned char* tail;    //Protocol tail

    int is_record_init;
    int is_proto_init;
};

struct queue_list_s{
    struct queue_node_s queue_buf[MAX_QUEUE_SIZE];

    uint32_t head;
    uint32_t tail;

    uint32_t parse_cnt;
    pthread_spinlock_t queue_spinlock;
};

extern struct queue_list_s queue_list;
extern struct queue_node_s io_queue_node;

int queue_init();
int queue_enqueue_file_io(FILE** fp);
int queue_dequeue_net_io();

#endif // LEPCAPY_PCAP_QUEUE_H
