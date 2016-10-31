#include "proto_ipv4.h"

struct proto_ipv4 obj_ipv4 = {0, };

struct proto_chain_s ipv4_chain = {
    &obj_ipv4,        //proto_obj
    NULL,               //u_layer

    ipv4_get_obj,   //proto_get_obj

    ipv4_set_ulayer,    //proto_set_ulayer
    ipv4_apply_chain,   //proto_apply_chain
};

int ipv4_get_obj(struct proto_chain_s * const protm, void *proto_obj){
    if(!(protm && proto_obj))
        return -ECHAIN;

    proto_obj = protm->proto_obj;

    return SUCCESS;
}

int ipv4_set_ulayer(struct proto_chain_s * const protm, struct proto_chain_s * const u_layer){
    if(!(protm && u_layer))
        return -ECHAIN;

    protm->u_layer = u_layer;

    return SUCCESS;
}

int ipv4_apply_chain(struct proto_chain_s * const protm, uint8_t * ipv4_buf){
    int err = SUCCESS;
    struct proto_ipv4 *p_ipv4_obj = NULL;
    struct ip *ipv4_hdr = NULL;
    uint8_t *ipv4_payload = NULL;

    if(!(protm && ipv4_buf))
        return -ECHAIN;

    p_ipv4_obj = IPV4_PTR(protm->proto_obj);
    ipv4_hdr = (struct ip*)ipv4_buf;
    ipv4_payload = ipv4_buf + ipv4_hdr->ip_hl;

    ipv4_hdr->ip_src = p_ipv4_obj->saddr;
    ipv4_hdr->ip_dst = p_ipv4_obj->daddr;

    if(protm->u_layer)
        err = protm->u_layer->proto_apply_chain(protm->u_layer, ipv4_payload);

    return err;
}
