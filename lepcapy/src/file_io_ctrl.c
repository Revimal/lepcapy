#include "file_io_ctrl.h"

static pthread_t p_thread;

int thread_file_io(FILE *fp){
    int err_code = SUCCESS;

    p_pktm = NULL;

    io_interact_flag = 1;
    __file_io_init(fp);
    err_code = pthread_create(&p_thread, NULL, __thread_file_io, (void *)fp);
//    while(queue_current_size() < NETIO_QUEUING_SIZE){
//        sleep(0);
//    }
    return err_code;
}

int thread_file_join(){
    unsigned long ret_thread = SUCCESS;

    pthread_join(p_thread, (void **)&ret_thread);
    return (int)ret_thread;
}

void *__thread_file_io(void *file_ptr){
    int err_code = SUCCESS;

    while(1){
        err_code = __thread_file_enqueue((FILE*)file_ptr);
/*        if(err_code == -EQUEUE)
            sleep(0);
        else */if(err_code)
            break;

        //TODO : Need Consumer
    }
    io_interact_flag = 0;
    pthread_exit((void *)(unsigned long)err_code);
}


int __thread_file_enqueue(FILE *fp){
    /*
     * TODO
     *  Change Spinloick model to CAS(Compare_And_Set) model
     */

    int err_code = SUCCESS;
    struct queue_node_s tmp_node = {0,};

    if(!fp)
        return -EINVAL;

    lock_queue_spinlock();
    if(queue_round_tail((queue_list.tail + 1)) == queue_list.head){
        unlock_queue_spinlock();
        return -EQUEUE;
    }
    unlock_queue_spinlock();

    if((err_code = __file_io_read(fp, &tmp_node)))
        return err_code;

//    if((err_code = proto_parse_seq()))
//        return err_code;

    lock_queue_spinlock();
    queue_elem_tail() = tmp_node;
    queue_list.tail = queue_round_tail(queue_list.tail + 1);
    unlock_queue_spinlock();
    return err_code;
}

int __file_io_init(FILE *fp){
    int err_code = SUCCESS;
    struct pcap_hdr_s tmp_pcaphdr;
    struct pcaprec_hdr_s tmp_rechdr;

    fseek(fp, 0, SEEK_SET);

    if((err_code = load_pcap_format(fp, &tmp_pcaphdr)))
        return err_code;

    queue_list.max_len = tmp_pcaphdr.snaplen;

    if((err_code = load_pcap_rechdr_pure(fp, &tmp_rechdr)))
        return err_code;

    fseek(fp, -sizeof(struct pcaprec_hdr_s), SEEK_CUR);

    queue_list.base_sec = tmp_rechdr.tv_sec;
    queue_list.base_usec = tmp_rechdr.tv_usec;

    //TODO : Add Network Access Layer detection
    p_pktm = alloc_pktm(p_pktm);
    ether_operations.pkt_minit(p_pktm, env_pktm.if_name);

    return err_code;
}

int __file_io_read(FILE *fp, struct queue_node_s* tmp_node){
    int err_code = SUCCESS;
    unsigned char *temp_ptr = NULL;

    if((err_code = load_pcap_record(fp,
            &(tmp_node->pcaprec_info),
            &(tmp_node->pcaprec_buf), queue_list.max_len)))
        return err_code;

    temp_ptr = tmp_node->pcaprec_buf;
    tmp_node->head = temp_ptr;
    temp_ptr += tmp_node->pcaprec_info.inc_len;
    tmp_node->tail = temp_ptr;

    return err_code;
}

//int proto_parse_seq(){
//    int err_code = SUCCESS;
//    struct ethernetII_layer_s ether_layer;

//    get_relative_tv(queue_elem_head().pcaprec_info.tv_sec,
//                    queue_elem_head().pcaprec_info.tv_usec);

//    err_code = ethernetII_operation.parse_queue_decap(&queue_elem_head(), PROTO_LAYER(&test));

//    //TODO : Add IPv4 & TCP Protocol Parser

//    return err_code;
//}
