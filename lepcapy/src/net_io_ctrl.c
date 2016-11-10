#include "exception_ctrl.h"
#include "net_io_ctrl.h"

static pthread_t t_thread;
static unsigned long net_io_cnt = 0;

unsigned long thread_net_get_cnt(){
    return net_io_cnt;
}

int thread_net_io(pthread_t *th_file){
    int err_code = SUCCESS;
    pthread_attr_t t_attr;
    struct sched_param t_param;

    if(p_pktm == NULL){
        raise_except(ERR_NULL(p_pktm), -ENULL);
        return -ENULL;
    }

    if(!io_interact_flag){
        raise_except(ERR_INTERACT(io_interact_flag), -EINTRACT);
        return -EINTRACT;
    }

    if(pthread_attr_init(&t_attr)){
        raise_except(ERR_CALL_LIBC(pthread_attr_init), -ETHREAD);
        return -ETHREAD;
    }

    if(pthread_attr_setinheritsched(&t_attr, PTHREAD_EXPLICIT_SCHED)){
        raise_except(ERR_CALL_LIBC(pthread_attr_setinheritsched), -ETHREAD);
        err_code = -ETHREAD;
        goto thread_err;
    }
    if(pthread_attr_getschedparam(&t_attr, &t_param)){
        raise_except(ERR_CALL_LIBC(pthread_attr_getschedparam), -ETHREAD);
        err_code = -ETHREAD;
        goto thread_err;
    }
    t_param.__sched_priority = sched_get_priority_max(SCHED_FIFO);
    if(pthread_attr_setschedpolicy(&t_attr, SCHED_FIFO)){
        raise_except(ERR_CALL_LIBC(pthread_attr_setschedpolicy), -ETHREAD);
        err_code = -ETHREAD;
        goto thread_err;
    }
    if(pthread_attr_setschedparam(&t_attr, &t_param)){
        raise_except(ERR_CALL_LIBC(pthread_attr_setschedparam), -ETHREAD);
        err_code = -ETHREAD;
        goto thread_err;
    }

    if(pthread_create(&t_thread, &t_attr, __thread_net_io, (void *)th_file)){
        raise_except(ERR_CALL_LIBC(pthread_create), -ETHREAD);
        err_code = -ETHREAD;
    }

    thread_err:
    pthread_attr_destroy(&t_attr);

    return err_code;
}

int thread_net_join(){
    unsigned long ret_thread = SUCCESS;

    pthread_join(t_thread, (void **)&ret_thread);
    return (int)ret_thread;
}

void *__thread_net_io(void *th_file){
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
        else if(err_code){
            raise_except(ERR_THREAD_INTERNAL_IWORK(__thread_net_io, __thread_file_enqueue), err_code);
            pthread_cancel(*((pthread_t *)th_file));
            break;
        }
    }

    __debug__prtn_io_cnt(net_io_cnt);

    usleep(1);
    pthread_exit((void *)(unsigned long)err_code);
}

int __thread_net_dequeue(){
    int err_code = SUCCESS;
    struct queue_node_s tmp_node;

    lock_queue_spinlock();
    if(queue_list.front == queue_list.rear){
        unlock_queue_spinlock();
        return -EQUEUE;
    }

    tmp_node = queue_elem_front();

    __nwait_release_lock(tmp_node.pcaprec_info.tv_sec, tmp_node.pcaprec_info.tv_usec);

    if((err_code = ether_operations.pkt_send(p_pktm, tmp_node.pcaprec_buf,
             tmp_node.pcaprec_info.inc_len, NULL))){
        unlock_queue_spinlock();
        raise_except(ERR_CALL_PKTM(ether, pkt_send), err_code);
        return err_code;
    }

    free_ptr(queue_elem_front().pcaprec_buf);
    queue_list.front = queue_round_tail(queue_list.front + 1);
    net_io_cnt++;
    unlock_queue_spinlock();

    return SUCCESS;
}
