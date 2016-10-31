#ifndef LEPCAPY_NET_IO_CTRL_H
#define LEPCAPY_NET_IO_CTRL_H

int thread_net_io(FILE *fp);
int thread_net_join();
void *__thread_net_io(void *file_ptr);
int __thread_net_dequeue(FILE *fp);

/*
 * File I/O Functions
 */
int __net_io_init(FILE *fp);
int __net_io_read(FILE *fp, struct queue_node_s*);

#endif //LEPCAPY_NET_IO_CTRL_H
