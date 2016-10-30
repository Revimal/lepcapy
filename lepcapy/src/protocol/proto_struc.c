#include "protocol/proto_struc.h"

ssize_t pktm_dummy_send(struct pktm_object_s * const pktm, uint8_t * const buf, const ssize_t cnt, void *args){ return -EINVPF; }
int pktm_set_protbuf(struct pktm_object_s * const pktm, uint8_t * const buf, const ssize_t cnt, void *prot_addr){ return -EINVPF; }
int pktm_dummy_proto(struct pktm_object_s * const pktm, struct proto_chain_s *u_layer){ return -EINVPF; }
int pktm_dummy_get_naddr(struct pktm_object_s * const pktm, void * const addr){ return -EINVPF; }
int pktm_dummy_ctl(struct pktm_object_s * pktm, const int ctl_num, void* args){ return -EINVPF; }
