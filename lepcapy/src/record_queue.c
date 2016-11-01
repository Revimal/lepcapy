#include "record_queue.h"

int io_interact_flag = 0;

struct queue_list_s queue_list;

int queue_init(){
    int i = 0;
    if(pthread_spin_init(get_queue_spinlock(), PTHREAD_PROCESS_PRIVATE))
        return -ETHREAD;

    for(i = 0; i < MAX_QUEUE_SIZE; ++i)
        queue_elem(i).pcaprec_buf = NULL;

    return SUCCESS;
}
