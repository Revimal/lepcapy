#include "trans_ctrl.h"

struct proto_env env_pktm;
struct pktm_object_s *p_pktm = NULL;

#ifdef __LEPCAPY_ARCH_OPTS__
atomic64_t fastbuf_front;
atomic64_t fastbuf_rear;
atomic64_t fastbuf_size;
__volatile__ atomic64_t fastbuf_dummy;

struct queue_node_s fastbuf_arr[FAST_BUFFER_SIZE];
#endif
