#ifndef LEPCAPY_FILE_IO_CTRL_H
#define LEPCAPY_FILE_IO_CTRL_H

#include "trans_ctrl.h"
#include "pcap_io.h"

unsigned long thread_file_get_cnt();
unsigned long thread_file_get_dropped();
/*
 *  Thread Functions
 */
int thread_file_io(FILE *fp);
int thread_file_join();

pthread_t *thread_file_getthp();

void *__thread_file_io(void *file_ptr);
int __thread_file_enqueue(FILE *fp);
void __thread_file_destructor();

/*
 * File I/O Functions
 */
int __file_io_init(FILE *fp);
int __file_io_read(FILE *fp, struct queue_node_s*);

/*
 * Protocol Parsing Functions
 */
int __proto_parse_seq(struct queue_node_s* tmp_node);

#endif //LEPCAPY_FILE_IO_CTRL_H
