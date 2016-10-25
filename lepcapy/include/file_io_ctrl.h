#ifndef LEPCAPY_FILE_IO_CTRL_H
#define LEPCAPY_FILE_IO_CTRL_H

#include "macros.h"
#include "pcap_io.h"
#include "record_queue.h"
#include "protocol/ethernetII.h"

//  TODO : Split as File I/O & Source Ctrl

/*
 *  Source Ctrl : dump read & pkt capture + protocol parsing
 *      - Multi threading
 *
 *  File I/O : dump parsing
 *
 *  #Function Pointer Structure
 */

/*
 *  Thread Functions
 */
int thread_file_io(FILE *fp);
int thread_file_join();
void *__thread_file_io(void *file_ptr);
int __thread_file_enqueue(FILE *fp);

/*
 * File I/O Functions
 */
int __file_io_init(FILE *fp);
int __file_io_read(FILE *fp);

/*
 * Protocol Parsing Functions
 */
int proto_parse_seq();

#endif //LEPCAPY_FILE_IO_CTRL_H