#include "stdio.h"
#include "stdlib.h"
#include "sys/socket.h"
#include "sys/types.h"
#include "netinet/in.h"

#define PORT 8000

int main(int argc, char **argv)
{
	int sockfd;
	struct sockaddr_in myaddr;
	int opt = 1; 
	int len = sizeof(opt);

	memset(&myaddr, 0x00, sizeof(myaddr));
	if((sockfd = socket(AF_INET, SOCK_STREAM, 0)) == -1)
	{
		printf("TCP Socket Create Error.\n");
		return -1;
	}
	setsockopt(sockfd,SOL_SOCKET,SO_REUSEADDR,&opt,len);
	myaddr.sin_family = AF_INET;                /* host byte order */
	myaddr.sin_port = htons(PORT);      /* short, network byte order */
	myaddr.sin_addr.s_addr = INADDR_ANY;
	if (bind(sockfd, (struct sockaddr *)&myaddr, sizeof(myaddr))== -1)
	{
		printf("Bind to socket failed.\n");
		close(sockfd);
	}
	printf("Tcp create success!!!!");
/*
	succeed_type iret = succeed_type_failed;
	iret = SOCKET_CREATE(LIBSK_NETMODE_TCP, PORT, NULL, &sockfd);
	SYS_INFO("\n\r!!!!!!!!!!!!!!!!!!!!!!!!!!!iret = %d!!!!!!!!!!!!!!!!!!!!!!\r\n",iret);
*/	return 0;

}
