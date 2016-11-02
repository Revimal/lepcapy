#include "net_io_ctrl.h"

static pthread_t p_thread;
static unsigned long cnt = 0;

int thread_net_io(){
    if(p_pktm == NULL)
        return -ENULL;
    \
    if(!io_interact_flag)
        return -EINTRACT;

    if(pthread_create(&p_thread, NULL, __thread_net_io, NULL));
        return -ETHREAD;
}

int thread_net_join(){
    unsigned long ret_thread = SUCCESS;

    pthread_join(p_thread, (void **)&ret_thread);
    return (int)ret_thread;
}

void *__thread_net_io(){
    int err_code = SUCCESS;

    while(1){
        err_code = __thread_net_dequeue();
        if(err_code == -EQUEUE){
            if(LEPCAPY_EXPECT_T(!io_interact_flag)){
                err_code = SUCCESS;
                break;
            }
            else{
                printf("Wait Enqueue\n");
                sleep(0);
                continue;
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
    free_ptr(queue_elem_front().pcaprec_buf);
    queue_list.front = queue_round_tail(queue_list.front + 1);
    unlock_queue_spinlock();

    if((tx_err = ether_operations.pkt_send(p_pktm, tmp_node.pcaprec_buf,
             tmp_node.pcaprec_info.inc_len, NULL)) < 0){
        return tx_err;
    }

    cnt++;
    return SUCCESS;
}
