#include "trans_ctrl.h"

struct proto_env env_pktm;
struct pktm_object_s *p_pktm = NULL;

#ifdef __LEPCAPY_ARCH_OPTS__
    struct queue_node_s netio_buf[NETIO_BUFFER_SIZE];
#endif
