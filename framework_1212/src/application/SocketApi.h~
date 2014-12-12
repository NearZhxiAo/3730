#ifndef _NET_INTERFACE_H_
#define _NET_INTERFACE_H_

#ifdef _cplusplus
extern "C" {
#endif

#include "system_includes.h"

#define MTU        1500
#define DEBUG_FLAG 1


/*tcp*/
extern unsigned int TCP_CREATE(int *pfd);

extern unsigned int TCP_BIND(int socketfd, unsigned short port);

extern unsigned int TCP_ACCEPT(int socketfd, int *pfd);

extern unsigned int TCP_CONNECT(int socketfd, unsigned short port, unsigned char *ip);

extern unsigned int TCP_SEND(int socketfd, unsigned char *psend, unsigned int send_len);

extern unsigned int TCP_RECV(int socketfd, unsigned char *precv, unsigned int *precv_len);

extern unsigned int TCP_CLOSE(int socketfd);

/*udp*/
extern unsigned int UDP_CREATE(int *pfd);

extern unsigned int UDPBD_CREATE(int *pfd);

extern unsigned int UDP_BIND(int socketfd, unsigned short port);

extern unsigned int UDPBD_BIND(int socketfd, unsigned short port);

extern unsigned int UDP_SEND(int socketfd, unsigned char *psend, unsigned int psend_len, unsigned char *pdest, unsigned short destport);

extern unsigned int UDPBD_SEND(int socketfd, unsigned char *psend, unsigned int psend_len, unsigned short destport);

extern unsigned int UDP_RECV(int socketfd, unsigned char *precv, unsigned int *precv_len, unsigned char *precvaddr, unsigned short *precv_port);

extern unsigned int UDPBD_RECV(int socketfd, unsigned char *precv,unsigned int *precv_len, unsigned char *precvaddr, unsigned short *precv_port);

extern unsigned int UDP_CLOSE(int socketfd);

extern unsigned int UDPBD_CLOSE(int socketfd);
 
#ifdef _cplusplus
}
#endif

#endif /*#ifndef _NET_INTERFACE_H_*/
