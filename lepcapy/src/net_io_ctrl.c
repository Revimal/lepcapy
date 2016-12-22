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
                raise_except(ERR_INTERACT(queue_depletion), -EQUEUE);
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

inline int __thread_net_dequeue(){
    int err_code = SUCCESS;
    struct queue_node_s tmp_node;
#if defined(__LEPCAPY_ARCH_X86__)
    uint32_t tmp_front;

    if(LEPCAPY_EXPECT_F(!queue_current_size()))
        return -EQUEUE;

    tmp_front = queue_idx_front();
    __fastcpy_aligned32(tmp_node, queue_elem(tmp_front));
#else
    lock_queue_spinlock();
    if(LEPCAPY_EXPECT_F(queue_list.front == queue_list.rear){
        unlock_queue_spinlock();
        return -EQUEUE;
    }
    unlock_queue_spinlock();

    __fastcpy_aligned32(tmp_node, queue_elem_front());
#endif
    __nwait(tmp_node.pcaprec_info.tv_sec, tmp_node.pcaprec_info.tv_usec);

    if(LEPCAPY_EXPECT_F((err_code = ether_operations.pkt_send(p_pktm, tmp_node.pcaprec_buf,
             tmp_node.pcaprec_info.inc_len, NULL)))){
        raise_except(ERR_CALL_PKTM(ether, pkt_send), err_code);
        return err_code;
    }

#if defined(__LEPCAPY_ARCH_X86__)
    free_ptr(tmp_node.pcaprec_buf);
    if(!atomic32_cmpxchg(&queue_list.front, tmp_front, queue_round_tail(tmp_front + 1)))
        return -ENQUEUE;
    atomic32_dec(&queue_elem_cnt());
#else
    lock_queue_spinlock();
    free_ptr(tmp_node.pcaprec_buf);
    queue_list.front = queue_round_tail(queue_list.front + 1);
    unlock_queue_spinlock();
#endif
    net_io_cnt++;
    return SUCCESS;
}
