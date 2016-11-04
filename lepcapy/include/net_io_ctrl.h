#ifndef LEPCAPY_NET_IO_CTRL_H
#define LEPCAPY_NET_IO_CTRL_H

#include "trans_ctrl.h"
#include "record_queue.h"

/*
 *  Thread Functions
 */
int thread_net_io();
int thread_net_join();
void *__thread_net_io();
int __thread_net_dequeue();

#endif //LEPCAPY_NET_IO_CTRL_H
