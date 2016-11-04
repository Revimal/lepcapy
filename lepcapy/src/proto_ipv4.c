#include "proto_ipv4.h"

static struct proto_ipv4 obj_ipv4;
struct proto_chain_s ipv4_chain = {
    &obj_ipv4,              //__proto_obj
    NULL,                   //__u_layer

    ipv4_get_obj,           //proto_get_obj

    ipv4_set_ulayer,        //proto_set_ulayer
    ipv4_apply_chain,       //proto_apply_chain
};

int ipv4_get_obj(struct proto_chain_s * const protm, void **ipv4_obj){
    if(!protm)
        return -EINVAL;

    *ipv4_obj = protm->__proto_obj;

    return SUCCESS;
}

int ipv4_set_ulayer(struct proto_chain_s * const protm, struct proto_chain_s * const u_layer){
    if(!(protm && u_layer))
        return -EINVAL;

    protm->__u_layer = u_layer;

    return SUCCESS;
}

int ipv4_apply_chain(struct proto_chain_s * const protm, uint8_t * const ipv4_buf){
    int err = SUCCESS;
    struct proto_ipv4 *p_ipv4_obj = NULL;
    struct ip *ipv4_hdr = NULL;
    uint8_t *ipv4_payload = NULL;

    if(!(protm && ipv4_buf))
        return -EINVAL;

    p_ipv4_obj = IPV4_PTR(protm->__proto_obj);
    ipv4_hdr = (struct ip *)ipv4_buf;
    ipv4_payload = ipv4_buf + ipv4_hdr->ip_hl;

    ipv4_hdr->ip_src = p_ipv4_obj->saddr;
    ipv4_hdr->ip_dst = p_ipv4_obj->daddr;

    if(protm->__u_layer)
        err = protm->__u_layer->proto_apply_chain(protm->__u_layer, ipv4_payload);

    return err;
}

int ipv4_parse_str(char *ip_str, struct in_addr *dst_ptr){
    if(!(ip_str && dst_ptr))
        return -EINVAL;

    if(!inet_pton(AF_INET, ip_str, &(dst_ptr->s_addr)))
        return -EINVAL;

    return SUCCESS;
}

int ipv4_get_uptype(uint8_t * const ipv4_buf, void * const ipv4_type){
    struct ip *ipv4_hdr = NULL;

    if(!(ipv4_buf && ipv4_type))
        return -EINVAL;

    ipv4_hdr = (struct ip *)ipv4_buf;

    *((uint8_t *)(ipv4_type)) = ntohs(ipv4_hdr->ip_p);

    return SUCCESS;
}
