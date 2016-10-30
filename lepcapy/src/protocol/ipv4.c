#include "protocol/ipv4.h"

struct proto_ipv4_obj proto_ipv4 = {0, };

struct proto_chain_s ipv4_chain = {
    proto_obj : &proto_ipv4,
    u_layer : NULL,

    proto_set_protbuf : ipv4_set_protbuf,
    proto_set_ulayer : ipv4_set_ulayer,
    proto_set_addr : ipv4_apply_chain,
};

int ipv4_set_protbuf(struct proto_chain_s * const protm, uint8_t * const prot_buf){
    struct proto_ipv4_obj *ipv4_protm = NULL;

    if(!(protm && prot_buf))
        return -ECHAIN;

    ipv4_protm = IPV4_PTR(protm->proto_obj);

    ipv4_protm->ipv4_buf = prot_buf;

    return SUCCESS;
}

int ipv4_set_ulayer(struct proto_chain_s * const protm, struct proto_chain_s * const u_layer){
    if(!(protm && u_layer))
        return -ECHAIN;

    protm->u_layer = u_layer;

    return SUCCESS;
}

int ipv4_apply_chain(struct proto_chain_s * const protm){
    struct proto_ipv4_obj *ipv4_protm = NULL;
    struct ip *ipv4_hdr = NULL;

    if(!protm)
        return -ECHAIN;

    ipv4_protm = IPV4_PTR(protm->proto_obj);
    ipv4_hdr = ipv4_protm->ipv4_buf;

    ipv4_hdr->ip_src = ipv4_protm->saddr;
    ipv4_hdr->ip_dst = ipv4_protm->daddr;

    return SUCCESS;
}
