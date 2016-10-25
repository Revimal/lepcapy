#include "record_queue.h"

int io_interact_flag = 0;

struct queue_list_s queue_list = {
    max_len : 0,
    head : 0,
    tail : 0,
    base_sec : 0,
    base_usec : 0,
};

int queue_init(){
    int i = 0;
    if(pthread_spin_init(get_queue_spinlock(), PTHREAD_PROCESS_PRIVATE))
        return -ETHREAD;

    for(i = 0; i < MAX_QUEUE_SIZE; ++i)
        queue_elem(i).pcaprec_buf = NULL;

    return SUCCESS;
}
