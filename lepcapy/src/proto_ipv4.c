#include "exception_ctrl.h"
#include "proto_ipv4.h"

static int ipv4_get_obj(struct proto_chain_s * const protm, void **ipv4_obj);
static int ipv4_set_ulayer(struct proto_chain_s * const protm, struct proto_chain_s * const u_layer);
static int ipv4_apply_chain(struct proto_chain_s * const protm, uint8_t * const ipv4_buf);

static struct proto_ipv4 obj_ipv4;
struct proto_chain_s ipv4_chain = {
    &obj_ipv4,              //__proto_obj
    NULL,                   //__u_layer

    ipv4_get_obj,           //proto_get_obj

    ipv4_set_ulayer,        //proto_set_ulayer
    ipv4_apply_chain,       //proto_apply_chain
};

static int ipv4_get_obj(struct proto_chain_s * const protm, void **ipv4_obj){
    if(!protm){
        raise_except(ERR_NULL(protm), -EINVAL);
        return -EINVAL;
    }

    *ipv4_obj = protm->__proto_obj;

    return SUCCESS;
}

static int ipv4_set_ulayer(struct proto_chain_s * const protm, struct proto_chain_s * const u_layer){
    if(!(protm && u_layer)){
        raise_except(ERR_NULL(protm|u_layer), -EINVAL);
        return -EINVAL;
    }

    protm->__u_layer = u_layer;

    return SUCCESS;
}

static int ipv4_apply_chain(struct proto_chain_s * const protm, uint8_t * const ipv4_buf){
    int err_code = SUCCESS;
    struct proto_ipv4 *p_ipv4_obj = NULL;
    struct ip *ipv4_hdr = NULL;
    uint8_t *ipv4_payload = NULL;

    if(!(protm && ipv4_buf)){
        raise_except(ERR_NULL(protm|ipv4_buf), -EINVAL);
        return -EINVAL;
    }

    p_ipv4_obj = IPV4_PTR(protm->__proto_obj);
    ipv4_hdr = (struct ip *)ipv4_buf;
    ipv4_payload = ipv4_buf + ipv4_hdr->ip_hl;

    ipv4_hdr->ip_src = p_ipv4_obj->saddr;
    ipv4_hdr->ip_dst = p_ipv4_obj->daddr;

    if(protm->__u_layer){
        err_code = protm->__u_layer->proto_apply_chain(protm->__u_layer, ipv4_payload);
        if(err_code)
            raise_except(ERR_CALL_PROTO(ether, proto_apply_chain), err_code);
    }

    return err_code;
}

int ipv4_parse_str(char *ip_str, struct in_addr *dst_ptr){
    if(!(ip_str && dst_ptr)){
        raise_except(ERR_NULL(ip_str|dst_ptr), -EINVAL);
        return -EINVAL;
    }

    if(!inet_pton(AF_INET, ip_str, &(dst_ptr->s_addr)))
        return -EINVAL;

    return SUCCESS;
}

int ipv4_get_uptype(uint8_t * const ipv4_buf, void * const ipv4_type){
    struct ip *ipv4_hdr = NULL;

    if(!(ipv4_buf && ipv4_type)){
        raise_except(ERR_NULL(ipv4_buf|ipv4_type), -EINVAL);
        return -EINVAL;
    }

    ipv4_hdr = (struct ip *)ipv4_buf;

    *((uint8_t *)(ipv4_type)) = ntohs(ipv4_hdr->ip_p);

    return SUCCESS;
}
