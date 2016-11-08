#include "exception_ctrl.h"
#include "file_io_ctrl.h"

static pthread_t t_thread;
static unsigned long file_io_cnt = 1;

int thread_file_io(FILE *fp){
    int err_code = SUCCESS;

    if(p_pktm == NULL){
        raise_except(ERR_NULL(p_pktm), -EINVAL);
        return -ENULL;
    }

    if((err_code = __file_io_init(fp))){
        raise_except(ERR_CALL_INTERNAL(__file_io_init), err_code);
        return err_code;
    }

    //TODO : Add Err Ctrl
    if(pthread_create(&t_thread, NULL, __thread_file_io, (void *)fp)){
        raise_except(ERR_CALL_LIBC(pthread_create), -ETHREAD);
        return -ETHREAD;
    }

    while(queue_current_size() < NETIO_QUEUING_SIZE){
        sched_yield();
    }

    io_interact_flag = 1;

    return err_code;
}

int thread_file_join(){
    unsigned long ret_thread = SUCCESS;

    pthread_join(t_thread, (void **)&ret_thread);
    return (int)ret_thread;
}

void *__thread_file_io(void *file_ptr){
    int err_code = SUCCESS;

    while(1){
        err_code = __thread_file_enqueue((FILE*)file_ptr);
        if(err_code){
            if(err_code == -EQUEUE){
                err_code = SUCCESS;
                sched_yield();
                continue;
            }
            else if(err_code == -__EEOF){
                err_code = SUCCESS;
                break;
            }
            raise_except(ERR_THREAD_INTERNAL_IWORK(__thread_file_io, __thread_file_enqueue), err_code);
            break;
        }
    }

    __debug__prtn_io_cnt(file_io_cnt);
    io_interact_flag = 0;
    pthread_exit((void *)(unsigned long)err_code);
}


int __thread_file_enqueue(FILE *fp){
    /*
     * TODO
     *  Change Spinloick model to CAS(Compare_And_Set) model
     */

    int err_code = SUCCESS;
    struct queue_node_s tmp_node;

    if(!fp){
        raise_except(ERR_NULL(fp), -EINVAL);
        return -EINVAL;
    }

    lock_queue_spinlock();
    if(queue_round_tail((queue_list.rear + 1)) == queue_list.front){
        unlock_queue_spinlock();
        return -EQUEUE;
    }
    unlock_queue_spinlock();

    if((err_code = __file_io_read(fp, &tmp_node))){
        if(err_code != -__EEOF)
            raise_except(ERR_CALL_INTERNAL(__file_io_read), err_code);
        return err_code;
    }

    __calc_relative_tv(&(tmp_node.pcaprec_info.tv_sec), &(tmp_node.pcaprec_info.tv_usec));

    err_code = __proto_parse_seq(&tmp_node);

    if(err_code){
        if(err_code == __EDROP){
            free_ptr(tmp_node.pcaprec_buf);
            err_code = SUCCESS;
            goto drop;
        }
        raise_except(ERR_CALL_INTERNAL(__proto_parse_seq), err_code);
        return err_code;
    }

    lock_queue_spinlock();
    memcpy(&queue_elem_rear(), &tmp_node, sizeof(struct queue_node_s));
    queue_list.rear = queue_round_tail(queue_list.rear + 1);
    file_io_cnt++;
    unlock_queue_spinlock();

    drop:
    return err_code;
}

int __file_io_init(FILE *fp){
    int err_code = SUCCESS;
    void *p_proto_obj = NULL;
    struct pcap_hdr_s tmp_pcaphdr;
    struct pcaprec_hdr_s tmp_rechdr;

    fseek(fp, 0, SEEK_SET);

    if((err_code = load_pcap_format(fp, &tmp_pcaphdr))){
        raise_except(ERR_CALL(load_pcap_format), err_code);
        return err_code;
    }

    queue_list.max_len = tmp_pcaphdr.snaplen;

    if((err_code = load_pcap_rechdr_pure(fp, &tmp_rechdr))){
        raise_except(ERR_CALL(load_pcap_rechdr_pure), err_code);
        return err_code;
    }

    fseek(fp, -sizeof(struct pcaprec_hdr_s), SEEK_CUR);

    __set_relative_tv(tmp_rechdr.tv_sec, tmp_rechdr.tv_usec);

    //TODO : Add Network Access Layer detection
    if((err_code = ether_operations.pkt_minit(p_pktm, env_pktm.if_name))){
        raise_except(ERR_CALL_PKTM(ether, pktm_minit), err_code);
        goto err;
    }
    if((err_code = ether_operations.pkt_get_naddr(p_pktm, env_pktm.eth_addr.eth_saddr))){
        raise_except(ERR_CALL_PKTM(ether, pkt_get_naddr), err_code);
        goto err;
    }
    if((err_code = ether_operations.pkt_get_iaddr(p_pktm, &(env_pktm.ipv4_addr.saddr)))){
        raise_except(ERR_CALL_PKTM(ether, pkt_get_iaddr), err_code);
        goto err;
    }

    if((err_code = ether_chain.proto_get_obj(&ether_chain, &p_proto_obj))){
        raise_except(ERR_CALL_PROTO(ether, proto_get_obj), err_code);
        goto err;
    }

    memcpy(ETH_PTR(p_proto_obj)->eth_saddr, env_pktm.eth_addr.eth_saddr, ETH_ALEN);
    memcpy(ETH_PTR(p_proto_obj)->eth_daddr, "\0\0\0\0\0\0", ETH_ALEN);

    if((err_code = ipv4_chain.proto_get_obj(&ipv4_chain, &p_proto_obj))){
        raise_except(ERR_CALL_PROTO(ipv4, proto_get_obj), err_code);
        goto err;
    }
    IPV4_PTR(p_proto_obj)->saddr = env_pktm.ipv4_addr.saddr;
    IPV4_PTR(p_proto_obj)->daddr = env_pktm.ipv4_addr.daddr;

    if((err_code = ether_chain.proto_set_ulayer(&ether_chain, &ipv4_chain))){
        raise_except(ERR_CALL_PROTO(ether, proto_set_ulayer), err_code);
        goto err;
    }

    err:
    return err_code;
}

int __file_io_read(FILE *fp, struct queue_node_s* tmp_node){
    int err_code = SUCCESS;
    unsigned char *temp_ptr = NULL;

    if((err_code = load_pcap_record(fp,
            &(tmp_node->pcaprec_info),
            &(tmp_node->pcaprec_buf), queue_list.max_len))){
        if(err_code != -__EEOF)
            raise_except(ERR_CALL(load_pcap_record), err_code);
        return err_code;
    }

    temp_ptr = tmp_node->pcaprec_buf;
    tmp_node->head = temp_ptr;
    temp_ptr += tmp_node->pcaprec_info.inc_len;
    tmp_node->tail = temp_ptr;

    return err_code;
}

int __proto_parse_seq(struct queue_node_s* tmp_node){
    int err_code = SUCCESS;
    uint16_t ether_type = 0;

    if((err_code = ether_get_uptype(tmp_node->pcaprec_buf, &ether_type))){
        raise_except(ERR_CALL(ether_get_uptype), err_code);
        goto err;
    }
    if(ether_type != ETH_P_IP)
        return __EDROP;
    if((err_code = ether_chain.proto_apply_chain(&ether_chain, tmp_node->pcaprec_buf))){
        raise_except(ERR_CALL_PROTO(ether, proto_apply_chain), err_code);
        goto err;
    }

    err:
    return err_code;
}
