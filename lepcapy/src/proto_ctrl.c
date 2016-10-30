#include "proto_ctrl.h"

struct proto_env env_pktm = {0, };
struct pktm_object_s *p_pktm = NULL;

int parse_ip_str(char *ip_str, struct in_addr *dst_ptr){
    if(!inet_pton(AF_INET, ip_str, &(dst_ptr->s_addr)))
        return -EINVAL;

    return SUCCESS;
}

int parse_eth_str(char *eth_str, uint8_t *dst_ptr){
    int i = 0;

    if(!eth_str || *eth_str == '\0')
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
