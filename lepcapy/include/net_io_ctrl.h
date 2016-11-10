#ifndef LEPCAPY_NET_IO_CTRL_H
#define LEPCAPY_NET_IO_CTRL_H

#include "trans_ctrl.h"


unsigned long thread_net_get_cnt();
/*
 *  Thread Functions
 */
int thread_net_io(pthread_t *th_file);
int thread_net_join();

void *__thread_net_io(void *th_file);
int __thread_net_dequeue();

#endif //LEPCAPY_NET_IO_CTRL_H
