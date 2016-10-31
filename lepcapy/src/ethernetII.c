#include "ethernetII.h"

struct pktm_operation_s ether_operations = {
    pktm_ether_init,            //pkt_minit
    pktm_ether_exit,            //pkt_mexit

    pktm_ether_send,            //pkt_send

    pktm_ether_set_etherbuf,    //pkt_set_protbuf
    pktm_ether_set_proto,       //pkt_set_proto
    pktm_ether_get_naddr,       //pkt_get_addr

    NULL,            //pkt_ctl
};

int pktm_ether_init(struct pktm_object_s * const pktm, char * const if_ifn){
    int err = SUCCESS, i = 0;
    struct ifreq ifobj;
    struct pktm_ether_s *eth_pktm = NULL;

    if(!pktm)
        return -EINVAL;

    eth_pktm = ETH_PTR(pktm);

    memset(eth_pktm, 0, sizeof(struct pktm_ether_s));
    eth_pktm->eth_mexit = pktm_ether_exit;

    if((eth_pktm->sd = socket(AF_PACKET, SOCK_RAW, htons(ETH_P_ALL))) == -1){
        err = -ESOCK;
        goto sock_err;
    }

    memset(&ifobj, 0, sizeof(struct ifreq));
    strncpy(ifobj.ifr_ifrn.ifrn_name, if_ifn, IFNAMSIZ - 1);
    if(ioctl(eth_pktm->sd, SIOCGIFINDEX, &ifobj)){
        err = -EIOCTL;
        goto err;
    }
    eth_pktm->tx_addr.sll_ifindex = ifobj.ifr_ifru.ifru_ivalue;

    memset(&ifobj, 0, sizeof(struct ifreq));
    strncpy(ifobj.ifr_ifrn.ifrn_name, if_ifn, IFNAMSIZ - 1);
    if(ioctl(eth_pktm->sd, SIOCGIFHWADDR, &ifobj)){
        err = -EIOCTL;
        goto err;
    }

    for(i = 0; i < ETH_ALEN; ++i)
        eth_pktm->tx_addr.sll_addr[i] = ifobj.ifr_ifru.ifru_addr.sa_data[i];
    eth_pktm->tx_addr.sll_halen = ETH_ALEN;

    goto success;

err:
    if(eth_pktm->sd){
        close(eth_pktm->sd);
        eth_pktm->sd = 0;
    }

sock_err: success:
    return err;
}

void pktm_ether_exit(struct pktm_object_s * const pktm){
    struct pktm_ether_s * eth_pktm = NULL;

    if(!pktm)
        return;

    eth_pktm = ETH_PTR(pktm);

    if(eth_pktm->sd){
        close(eth_pktm->sd);
        eth_pktm->sd = 0;
    }
}

ssize_t pktm_ether_send(struct pktm_object_s * const pktm, void * dummy){
    struct pktm_ether_s *eth_pktm = NULL;

    if(!pktm)
        return -EINVAL;

    eth_pktm = ETH_PTR(pktm);

    if(!(eth_pktm->offset && eth_pktm->eth_buf))
        return -ETRANS;

    if((sendto(eth_pktm->sd, eth_pktm->eth_buf, eth_pktm->offset, 0, (struct sockaddr *)&eth_pktm->tx_addr, sizeof(struct sockaddr_ll))) < 0){
        return -ETRANS;
    }

    eth_pktm->u_layer = NULL;
    eth_pktm->eth_buf = NULL;
    eth_pktm->eth_hdr = NULL;
    eth_pktm->eth_payload = NULL;
    eth_pktm->offset = 0;

    return SUCCESS;
}

int pktm_ether_set_etherbuf(struct pktm_object_s * const pktm, uint8_t * const buf, const ssize_t cnt, void *prot_addr){
    int err = SUCCESS, i = 0;
    struct pktm_ether_s *eth_pktm = NULL;
    struct netaddr_ether *eth_addr = NULL;

    if(!(pktm && prot_addr))
        return -EINVAL;

    eth_pktm = ETH_PTR(pktm);
    eth_addr = (struct netaddr_ether *)prot_addr;


    if(eth_pktm->eth_buf)
        return -EADDR;

    if(cnt > (int)(ETH_DATA_LEN))
        return -EOVRFLW;

    eth_pktm->offset = 0;
    eth_pktm->eth_buf = buf;
    eth_pktm->eth_hdr = (struct ether_header *)eth_pktm->eth_buf;
    eth_pktm->eth_payload = eth_pktm->eth_buf + sizeof(struct ether_header);
    eth_pktm->offset += sizeof(struct ether_header);

    for(i = 0; i < ETH_ALEN; ++i)
        eth_pktm->eth_hdr->ether_shost[i] = eth_addr->eth_saddr[i];

    for(i = 0; i < ETH_ALEN; ++i)
        eth_pktm->eth_hdr->ether_dhost[i] = eth_addr->eth_daddr[i];

    if(eth_pktm->u_layer)
        if((err = eth_pktm->u_layer->proto_apply_chain(eth_pktm->u_layer))){
            eth_pktm->eth_buf = NULL;
            eth_pktm->eth_hdr = NULL;
            eth_pktm->eth_payload = NULL;
            eth_pktm->offset = 0;
            return err;
        }

    eth_pktm->offset += (cnt - sizeof(struct ether_header));

    return err;
}

int pktm_ether_set_proto(struct pktm_object_s * const pktm, struct proto_chain_s *u_layer){
    struct pktm_ether_s *eth_pktm = NULL;

    if(!(pktm && u_layer))
        return -EINVAL;

    eth_pktm = ETH_PTR(pktm);
    eth_pktm->u_layer = u_layer;

    return SUCCESS;
}

int pktm_ether_get_naddr(struct pktm_object_s * const pktm, void * const hwa){
    int i = 0;

    for(i = 0; i < ETH_ALEN; ++i)
        ((uint8_t*)hwa)[i] = ETH_PTR(pktm)->tx_addr.sll_addr[i];

    return SUCCESS;
}
