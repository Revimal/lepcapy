#ifndef LEPCAPY_NET_IO_CTRL_H
#define LEPCAPY_NET_IO_CTRL_H

#include "proto_ctrl.h"
#include "record_queue.h"

int start_net_io(FILE *fp);
int start_net_join();
void *__do_net_io(void *file_ptr);
int __do_net_dequeue(FILE *fp);

/*
 * File I/O Functions
 */
int __net_io_init(FILE *fp);
int __net_io_send(FILE *fp, struct queue_node_s*);

#endif //LEPCAPY_NET_IO_CTRL_H
