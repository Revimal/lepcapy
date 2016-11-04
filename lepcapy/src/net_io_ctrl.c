#include "net_io_ctrl.h"

static pthread_t t_thread;
static unsigned long cnt = 0;

int thread_net_io(){
    pthread_attr_t t_attr;
    struct sched_param t_param;

    if(p_pktm == NULL)
        return -ENULL;
    \
    if(!io_interact_flag)
        return -EINTRACT;

    pthread_attr_init(&t_attr);
    pthread_attr_getschedparam(&t_attr, &t_param);
    t_param.__sched_priority = sched_get_priority_min(SCHED_FIFO);
    pthread_attr_setschedparam(&t_attr, &t_param);
    pthread_attr_setschedpolicy(&t_attr, SCHED_FIFO);
    if(pthread_create(&t_thread, NULL, __thread_net_io, NULL));
        return -ETHREAD;
}

int thread_net_join(){
    unsigned long ret_thread = SUCCESS;

    pthread_join(t_thread, (void **)&ret_thread);
    return (int)ret_thread;
}

void *__thread_net_io(){
    int err_code = SUCCESS;

    while(1){
        err_code = __thread_net_dequeue();
        if(err_code == -EQUEUE){
            if(LEPCAPY_EXPECT_T(io_interact_flag)){
                err_code = SUCCESS;
                sched_yield();
                continue;
            }
            else{
                err_code = SUCCESS;
                break;
            }
        }
        else if(err_code)
            break;
    }
    printf("ERR! - %d of %lu\n", err_code, cnt);
    pthread_exit((void *)(unsigned long)err_code);
}

int __thread_net_dequeue(){
    int tx_err = SUCCESS;
    struct queue_node_s tmp_node;

    lock_queue_spinlock();
    if(queue_list.front == queue_list.rear){
        unlock_queue_spinlock();
        return -EQUEUE;
    }

    tmp_node = queue_elem_front();

    __nwait_release_lock(tmp_node.pcaprec_info.tv_sec, tmp_node.pcaprec_info.tv_usec);

    if((tx_err = ether_operations.pkt_send(p_pktm, tmp_node.pcaprec_buf,
             tmp_node.pcaprec_info.inc_len, NULL)) < 0){
        unlock_queue_spinlock();
        return tx_err;
    }

    free_ptr(queue_elem_front().pcaprec_buf);
    queue_list.front = queue_round_tail(queue_list.front + 1);
    unlock_queue_spinlock();

    cnt++;
    return SUCCESS;
}
