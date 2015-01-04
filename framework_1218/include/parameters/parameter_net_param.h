#ifndef __PARAMETER_NET_PARAM_H__
#define __PARAMETER_NET_PARAM_H__

#ifdef __cplusplus
extern "C"{
#endif

#define IP_LENGTH	256
#define LOGINID_LEN 19
typedef enum _IP_MODE
{
    IP_MODE_START,
    IP_MODE_STATIC,
    IP_MODE_DHCP,
    IP_MODE_END
}IP_MODE;

typedef struct net_param
{
    IP_MODE ipmode;
    unsigned char ntpaddr[IP_LENGTH];
    unsigned char ipaddr[IP_LENGTH];
    unsigned char ipmask[IP_LENGTH];
    unsigned char ipgateway[IP_LENGTH];
    unsigned char wireless_ipaddr[IP_LENGTH];
    unsigned char center_ipaddr[IP_LENGTH];
    unsigned char center_loginid[LOGINID_LEN];
    unsigned int center_port;
    unsigned int port;
    unsigned int httpport;
}net_param;

#ifdef __cplusplus
}
#endif



#endif
