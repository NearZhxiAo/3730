#ifndef _LIBSK_H_E87D4805_2871_4881_AE3F_0ECD2F42EB70_
#define _LIBSK_H_E87D4805_2871_4881_AE3F_0ECD2F42EB70_

#include "system_debug.h"
#include <netinet/in.h>

#ifdef __cplusplus
extern "C"{
#endif

typedef enum rt_net_mode_ 
{
	/*! TCP socket */
	LIBSK_NETMODE_TCP,

	/*! UDP socket */
	LIBSK_NETMODE_UDP,
	
	LIBSK_NETMODE_END	
}rt_net_mode;

enum libsk_option_type
{
	libsk_opt_reuse_addr = 1,
	libsk_opt_set_ttl
};
/* netmode: 表示网络连接形式，取值范围为上面的枚举定义
   port：表示绑定的端口号
   ip:   表示绑定的IP,不用填写，调用时置位NULL 
   pskfd：socket创建返回的fd，指针*/
succeed_type SOCKET_CREATE(rt_net_mode netmode , short port, char *ip,   int *pskfd);

/* sk: socket创建返回的fd
   client_addr：接收到的连接信息指针
   *pskfd：接收到的socket创建的fd
 */
succeed_type SOCKET_ACCEPT(int sk, struct sockaddr_in *client_addr, int *pskfd);

/* sk：socket创建返回的fd
   port：需要连接的端口号
   ip: 连接的目标IP
 */
succeed_type SOCKET_CONNECT(int sk, short port, char *ip);

/* sk：socket创建返回的fd
ptrwritebuf: 待发送的内容缓冲指针
   len: 发送的缓冲里面有效数据的长度
   addr:发送给谁的表示 
 */
succeed_type SOCKET_SEND( int sk, unsigned char *ptrwritebuf, unsigned int *len,struct sockaddr_in* addr);

succeed_type SOCKET_RECV( int sk, unsigned char *ptr, unsigned int *len,struct sockaddr_in* addr);

succeed_type SOCKET_CLOSE( int sk);

#ifdef __cplusplus
}
#endif

#endif // _LIBSK_H_E87D4805_2871_4881_AE3F_0ECD2F42EB70_

