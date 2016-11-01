#include "proto_ethernetII.h"

#include "proto_ethernetII.h"

struct proto_ether obj_ether;
struct proto_chain_s ether_chain = {
    &obj_ether,             //proto_obj
    NULL,                   //u_layer

    ether_get_uptype,       //proto_get_uptype
    ether_get_obj,          //proto_get_obj

    ether_set_ulayer,       //proto_set_ulayer
    ether_apply_chain,      //proto_apply_chain
};

int ether_get_uptype(uint8_t * const ether_buf, void * const ether_type){
    struct ether_header *ether_hdr = NULL;

    if(!(ether_buf && ether_type))
        return -EINVAL;

    ether_hdr = (struct ether_header *)ether_buf;

    *((uint16_t *)(ether_type)) = ether_hdr->ether_type;

    return SUCCESS;
}

int ether_get_obj(struct proto_chain_s * const protm, void *ether_obj){
    if(!protm)
        return -EINVAL;

    ether_obj = protm->proto_obj;

    return SUCCESS;
}

int ether_set_ulayer(struct proto_chain_s * const protm, struct proto_chain_s * const u_layer){
    if(!(protm && u_layer))
        return -EINVAL;

    protm->u_layer = u_layer;

    return SUCCESS;
}

int ether_apply_chain(struct proto_chain_s * const protm, uint8_t * const ether_buf){
    int err = SUCCESS, i = 0;
    struct proto_ether *p_ether_obj = NULL;
    struct ether_header *ether_hdr = NULL;
    uint8_t *ether_payload = NULL;

    if(!(protm && ether_buf))
        return -EINVAL;

    p_ether_obj = ETH_PTR(protm->proto_obj);
    ether_hdr = (struct ether_header *)ether_buf;
    ether_payload = ether_buf + sizeof(struct ether_header);

    for(i = 0; i < ETH_ALEN; ++i)
        ether_hdr->ether_shost[i] = p_ether_obj->eth_saddr[i];

    for(i = 0; i < ETH_ALEN; ++i)
        ether_hdr->ether_dhost[i] = p_ether_obj->eth_daddr[i];

    if(protm->u_layer)
        err = protm->u_layer->proto_apply_chain(protm->u_layer, ether_payload);

    return err;
}
