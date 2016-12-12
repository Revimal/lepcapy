#include "exception_ctrl.h"
#include "record_queue.h"

int io_interact_flag = 0;

struct queue_list_s queue_list;

int queue_init(){
    int i = 0;

#if !defined(__LEPCAPY_ARCH_X86__)
    if(pthread_spin_init(get_queue_spinlock(), PTHREAD_PROCESS_PRIVATE)){
        raise_except(ERR_CALL_LIBC(pthread_spin_init), -ETHREAD);
        raise_except(ERR_LCK_ULCK(spinlock), -ETHREAD);
        return -ETHREAD;
    }
#endif
    for(i = 0; i < MAX_QUEUE_SIZE; ++i)
        queue_elem(i).pcaprec_buf = NULL;

    return SUCCESS;
}
