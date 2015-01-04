#include "system_includes.h"
#include "system_debug.h"
#include "libsk.h"

//#define SERVER_PORT 8888 
//#define SERVER_IP "113.138.248.241"
#define SERVER_PORT 5000
#define SERVER_IP "114.255.55.150"
#define LOCAL_PORT 6666
#define LOCAL_IP "172.17.118.170"

int main(int argc, char **argv)
{
	int sockfd = 0;
	int sk_ret = 0;
	int test_cnt = 10;
	succeed_type iret = succeed_type_failed;
	struct sockaddr_in local_addr;
	struct sockaddr_in server_addr;

	local_addr.sin_family = AF_INET;         	/* host byte order */
	local_addr.sin_port = htons(LOCAL_PORT);     	/* short, network byte order */
	//local_addr.sin_addr.s_addr = inet_addr(LOCAL_IP);
	local_addr.sin_addr.s_addr = INADDR_ANY;//inet_addr(LOCAL_IP);
	bzero(&(local_addr.sin_zero),0);

	server_addr.sin_family = AF_INET;
	server_addr.sin_port = htons(SERVER_PORT);
	server_addr.sin_addr.s_addr = inet_addr(SERVER_IP);
	bzero(&(server_addr.sin_zero),0);

#if 1
	iret = SOCKET_CREATE(LIBSK_NETMODE_TCP,LOCAL_PORT, NULL, &sockfd);
	if(succeed_type_succeed != iret)
	{
		SYS_ERROR("\r\nSOCKET_CREATE failed\r\n");
		SOCKET_CLOSE(sockfd);
		return -1;
	}
#endif
#if 0
	sockfd = socket(AF_INET,SOCK_STREAM,0);
	int opt = 1;
	int len = sizeof(opt);
	if (0 != setsockopt(sockfd , SOL_SOCKET, SO_REUSEADDR, &opt, len))
	{
		SYS_ERROR("Set Tcp option reuse failed.\n");
		SOCKET_CLOSE(sockfd);
		return -1;
	}
	if (bind(sockfd, (struct sockaddr *)&local_addr, sizeof(local_addr))== -1)
	{
		SYS_ERROR("Bind to socket failed.\n");
		perror("Bind to socket");
		SOCKET_CLOSE(sockfd);
		return -1;;
	}
#endif
	iret = SOCKET_CONNECT(sockfd, SERVER_PORT, SERVER_IP);
	if(succeed_type_succeed != iret)
	{
		SYS_ERROR("\r\nSOCKET_CONNECT failed\r\n");
		SOCKET_CLOSE(sockfd);
		return -1;
	}
	unsigned char send_buf[100] = "Hello World!!";
	unsigned char rcv_buf[100] = "Hello World!!";
	int send_len = sizeof(send_buf);
	int rcv_len = sizeof(rcv_buf);
	while(test_cnt--)
	{
#if 0
		if( ( sk_ret = sendto( sockfd, (void *)send_buf, 20,0, (struct sockaddr *)&server_addr, sizeof(server_addr)) ) < 0 )
		{
			perror("sendto error ");
			return -1;
		}
#endif
#if 1 //send
		if(succeed_type_succeed != SOCKET_SEND(sockfd, (void*)send_buf, &send_len,&server_addr))
		{
			perror("SOCKET_SEND error");
			SOCKET_CLOSE(sockfd);
			return -1;
		}
#endif
		printf("\r\n send buf=%s sk_ret=%d\r\n",send_buf,sk_ret);
#if 0 //rcv
//succeed_type SOCKET_RECV( int sk, unsigned char *ptr, unsigned int *len,struct sockaddr_in* addr)
		if(succeed_type_succeed != SOCKET_RECV(sockfd, (void*)rcv_buf, &rcv_len,&server_addr))
		{
			perror("SOCKET_SEND error");
			SOCKET_CLOSE(sockfd);
			return -1;
		}
		printf("\r\n recv buf=%s \r\n",rcv_buf);
#endif
	}
	SOCKET_CLOSE(sockfd);

	return 0;

}
