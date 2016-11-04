#include "pktm_ethernetII.h"

static ssize_t pktm_ether_send(struct pktm_object_s * const pktm, uint8_t * const prot_buf, const ssize_t prot_len, void *dummy);
static int pktm_ether_get_naddr(struct pktm_object_s * const pktm, void * const hwa);
static int pktm_ether_get_iaddr(struct pktm_object_s * const pktm, void * const ipa);
static int pktm_ether_ctl(struct pktm_object_s * pktm, const int ctl_num, void *dummy);

struct pktm_operation_s ether_operations = {
    pktm_ether_init,            //pkt_minit
    pktm_ether_exit,            //pkt_mexit

    pktm_ether_send,            //pkt_send

//    pktm_ether_init_etherbuf,    //pkt_apply_protbuf
    pktm_ether_get_naddr,       //pkt_get_addr
    pktm_ether_get_iaddr,       //pkt_get_iaddr
//    pktm_ether_get_header,      //pkt_get_header
//    pktm_ether_get_payload,     //pkt_get_payload

    pktm_ether_ctl,            //pkt_ctl
};

int pktm_ether_init(struct pktm_object_s * const pktm, char * const if_ifn){
    int err = SUCCESS, i = 0;
    struct ifreq ifobj;
    struct pktm_ether_s *eth_pktm = NULL;

    if(!pktm)
        return -EINVAL;

    if(pktm->__init)
        return -EPMINIT;

    pktm->__init = 0;
    eth_pktm = PKTM_ETH_PTR(pktm);
    eth_pktm->eth_mexit = pktm_ether_exit;

    if((eth_pktm->sd = socket(AF_PACKET, SOCK_RAW, htons(ETH_P_ALL))) == -1){
        err = -ESOCK;
        goto sock_err;
    }

    strncpy(eth_pktm->eth_name, if_ifn, IFNAMSIZ -1);

    memset(&ifobj, 0, sizeof(struct ifreq));
    strncpy(ifobj.ifr_ifrn.ifrn_name, eth_pktm->eth_name, IFNAMSIZ - 1);
    if(ioctl(eth_pktm->sd, SIOCGIFINDEX, &ifobj)){
        err = -EIOCTL;
        goto err;
    }
    eth_pktm->tx_addr.sll_ifindex = ifobj.ifr_ifru.ifru_ivalue;

    memset(&ifobj, 0, sizeof(struct ifreq));
    strncpy(ifobj.ifr_ifrn.ifrn_name, eth_pktm->eth_name, IFNAMSIZ - 1);
    if(ioctl(eth_pktm->sd, SIOCGIFHWADDR, &ifobj)){
        err = -EIOCTL;
        goto err;
    }

    for(i = 0; i < ETH_ALEN; ++i)
        eth_pktm->tx_addr.sll_addr[i] = ifobj.ifr_ifru.ifru_addr.sa_data[i];
    eth_pktm->tx_addr.sll_halen = ETH_ALEN;

    pktm->__init = 1;
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

    if(!(pktm->__init))
        return;

    pktm->__init = 0;

    eth_pktm = PKTM_ETH_PTR(pktm);

    if(eth_pktm->sd){
        close(eth_pktm->sd);
        eth_pktm->sd = 0;
    }
}

static ssize_t pktm_ether_send(struct pktm_object_s * const pktm, uint8_t * const prot_buf, const ssize_t prot_len, void *dummy){
    ssize_t sz_tx;
    struct pktm_ether_s *eth_pktm = NULL;

    if(!(pktm && prot_buf && prot_len))
        return -EINVAL;

    if(!(pktm->__init))
        return -EPMINIT;

    eth_pktm = PKTM_ETH_PTR(pktm);

    if(prot_len > ETH_FRAME_LEN)
        return -EJFRAME;

    if((sz_tx = sendto(eth_pktm->sd, prot_buf, prot_len, 0, (struct sockaddr *)&eth_pktm->tx_addr, sizeof(struct sockaddr_ll))) < 0){
        return -ETRANS;
    }

    return sz_tx;
}

static int pktm_ether_get_naddr(struct pktm_object_s * const pktm, void * const hwa){
    int i = 0;

    if(!(pktm && hwa))
        return -EINVAL;

    if(!(pktm->__init))
        return -EPMINIT;

    for(i = 0; i < ETH_ALEN; ++i)
        ((uint8_t*)hwa)[i] = PKTM_ETH_PTR(pktm)->tx_addr.sll_addr[i];

    return SUCCESS;
}

static int pktm_ether_get_iaddr(struct pktm_object_s * const pktm, void * const ipa){
    int err = SUCCESS;
    struct pktm_ether_s *eth_pktm = NULL;
    struct ifreq ifobj;

    if(!(pktm && ipa))
        return -EINVAL;

    if(!(pktm->__init))
        return -EPMINIT;

    eth_pktm = PKTM_ETH_PTR(pktm);

    memset(&ifobj, 0, sizeof(struct ifreq));
    strncpy(ifobj.ifr_ifrn.ifrn_name, eth_pktm->eth_name, IFNAMSIZ - 1);
    if(ioctl(eth_pktm->sd, SIOCGIFADDR, &ifobj)){
        err = -EIOCTL;
        goto err;
    }

    *((in_addr_t *)ipa) =
            ((struct sockaddr_in *)&(ifobj.ifr_ifru.ifru_addr))->sin_addr.s_addr;

    err:
    return err;
}

static int pktm_ether_ctl(struct pktm_object_s * pktm, const int ctl_num, void *dummy){
    return -EINVPF;
}
