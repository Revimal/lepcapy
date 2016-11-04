#include "proto_ethernetII.h"

static int ether_get_obj(struct proto_chain_s * const protm, void **ether_obj);
static int ether_set_ulayer(struct proto_chain_s * const protm, struct proto_chain_s * const u_layer);
static int ether_apply_chain(struct proto_chain_s * const protm, uint8_t * const ether_buf);

static struct proto_ether obj_ether;
struct proto_chain_s ether_chain = {
    &obj_ether,             //__proto_obj
    NULL,                   //__u_layer

    ether_get_obj,          //proto_get_obj

    ether_set_ulayer,       //proto_set_ulayer
    ether_apply_chain,      //proto_apply_chain
};

static int ether_get_obj(struct proto_chain_s * const protm, void **ether_obj){
    if(!protm)
        return -EINVAL;

    *ether_obj = protm->__proto_obj;

    return SUCCESS;
}

static int ether_set_ulayer(struct proto_chain_s * const protm, struct proto_chain_s * const u_layer){
    if(!(protm && u_layer))
        return -EINVAL;

    protm->__u_layer = u_layer;

    return SUCCESS;
}

static int ether_apply_chain(struct proto_chain_s * const protm, uint8_t * const ether_buf){
    int err = SUCCESS, i = 0;
    struct proto_ether *p_ether_obj = NULL;
    struct ether_header *ether_hdr = NULL;
    uint8_t *ether_payload = NULL;

    if(!(protm && ether_buf))
        return -EINVAL;

    p_ether_obj = ETH_PTR(protm->__proto_obj);
    ether_hdr = (struct ether_header *)ether_buf;
    ether_payload = ether_buf + sizeof(struct ether_header);

    for(i = 0; i < ETH_ALEN; ++i)
        ether_hdr->ether_shost[i] = p_ether_obj->eth_saddr[i];

    for(i = 0; i < ETH_ALEN; ++i)
        ether_hdr->ether_dhost[i] = p_ether_obj->eth_daddr[i];

    if(protm->__u_layer)
        err = protm->__u_layer->proto_apply_chain(protm->__u_layer, ether_payload);

    return err;
}

int ether_parse_str(char *eth_str, uint8_t *dst_ptr){
    int i = 0;

    if(!(eth_str && dst_ptr) || *eth_str == '\0')
        return -EINVAL;

    for(i = 0; i < ETH_ALEN; ++i){
        if(eth_str[i] == '\0'){
            memset(dst_ptr, '\0', ETH_ALEN);
            return -EINVAL;
        }

        dst_ptr[i] = strtoul(eth_str, NULL, 16);
        eth_str = strchr(eth_str, ':') + 1;
    }

    return SUCCESS;
}

int ether_get_uptype(uint8_t * const ether_buf, void * const ether_type){
    struct ether_header *ether_hdr = NULL;

    if(!(ether_buf && ether_type))
        return -EINVAL;

    ether_hdr = (struct ether_header *)ether_buf;

    *((uint16_t *)(ether_type)) = ntohs(ether_hdr->ether_type);

    return SUCCESS;
}

