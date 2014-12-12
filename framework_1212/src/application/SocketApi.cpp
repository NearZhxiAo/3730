#include "SocketApi.h"


unsigned int TCP_CREATE(int *pfd)
{
	int sockefd;
	sockefd=socket(AF_INET, SOCK_STREAM, 0);
	if (sockefd > 0)
	{
		*pfd = sockefd;
		//int nNetTimeOut = 1000;
		//setsockopt(sockefd,SOL_SOCKET,SO_RCVTIMEO,(char *)&nNetTimeOut,sizeof(int));
		return 0;
	}
	else
	{
		return 1;
	}
}

unsigned int TCP_BIND(int socketfd, unsigned short port)
{
	struct sockaddr_in  myaddr;

	int so_reuseaddr = 1;
	setsockopt(socketfd, SOL_SOCKET, SO_REUSEADDR, (char*)&so_reuseaddr, sizeof(so_reuseaddr));
	memset(&myaddr, 0, sizeof(myaddr));
	myaddr.sin_family = AF_INET;
	myaddr.sin_port = htons(port);
	myaddr.sin_addr.s_addr = INADDR_ANY;
	if ( bind(socketfd, (struct sockaddr*)&myaddr, sizeof(myaddr)) == -1)
	{
		//ERR_PRINT("socket tcp bind failed");
		TCP_CLOSE(socketfd);
		return 1;
	}
    if (listen(socketfd, 5) == -1)
    {
		//ERR_PRINT("socket tcp listen failed");
		TCP_CLOSE(socketfd);
		return 1;
    }
	return 0;
}



unsigned int TCP_ACCEPT(int socketfd, int *pfd)
{
	int sockfd;
	socklen_t sin_size;
	struct sockaddr_in addr;
	memset((void*)&addr, 0, sizeof(addr));
	sin_size=sizeof(struct sockaddr_in);

	sockfd = accept(socketfd, (struct sockaddr*)&addr, &sin_size);
	if (sockfd > 0)
	{
		*pfd = sockfd;
		return 0;
	}
	return 1;

}

unsigned int TCP_CONNECT(int socketfd, unsigned short port, unsigned char *ip)
{
	struct sockaddr_in addr;
	int ret = 0;
	if( socketfd <= 0 )
	{
		//ERR_PRINT("tcp socket connect failed!");
		return 1;
	}

	memset(&addr, 0, sizeof(addr));
	addr.sin_family = AF_INET;
	addr.sin_port = htons(port);
	addr.sin_addr.s_addr = inet_addr((char *)ip);

	if( ( ret = connect( socketfd, (struct sockaddr*)&addr, sizeof(addr))) != 0)
	{
		//ERR_PRINT("tcp socket connectt failed!");
		return 1;
	}
	else
		return 0;
}

unsigned int TCP_SEND(int socketfd, unsigned char *psend, unsigned int send_len)
{
	unsigned int sent_num = 0;
	if( socketfd <= 0 )
	{
		return 1;
	}
	sent_num = send( socketfd, (char *)psend, send_len, 0 );
	if( sent_num <= 0 )
	{
		//ERR_PRINT("tcp send failed, send_buf = 0x%x, send_len = %d", psend, *send_len);
		return 1;
	}
	else
	{
		return 0;
	}
}

unsigned int TCP_RECV(int socketfd, unsigned char *precv, unsigned int *precv_len)
{
	int recv_num = 0;
	if( socketfd <= 0 )
	{
		return 1;
	}
	recv_num = recv( socketfd, (char *)precv, MTU, 0 );
	if( recv_num <= 0 )
	{
		//ERR_PRINT("tcp recv failed!");
		return 1;
	}
	else
	{
		*precv_len = recv_num;
		return 0;
	}
}

unsigned int TCP_CLOSE(int socketfd)
{
	if (close(socketfd) == -1)
	{
		//ERR_PRINT("socket close failed");
		return 1;
	}
	return 0;
}


static unsigned int udpsocketcreate(int *pfd)
{
	int sockefd;
	sockefd=socket(AF_INET, SOCK_DGRAM, 0);
	if (sockefd > 0)
	{
		*pfd = sockefd;
		return 0;
	}
	else
	{
		return 1;
	}
}

unsigned int UDP_CREATE(int *pfd)
{
	return udpsocketcreate(pfd);
}

unsigned int UDPBD_CREATE(int *pfd)
{
	int so_broadcast;
	if(0 != udpsocketcreate(pfd))
	{
		return 1;
	}
	else
	{
		so_broadcast = 1;
		setsockopt(*pfd, SOL_SOCKET, SO_BROADCAST, (char*)&so_broadcast, sizeof(so_broadcast));
		return 0;
	}
}

static unsigned int udpsocket_bind(int socketfd, unsigned short port)
{
	int size = 1<<19;
	struct sockaddr_in  myaddr;
	int so_reuseaddr = 1;
	setsockopt(socketfd, SOL_SOCKET, SO_REUSEADDR, (char*)&so_reuseaddr, sizeof(so_reuseaddr));
	memset(&myaddr, 0, sizeof(myaddr));
	myaddr.sin_family = AF_INET;
	myaddr.sin_port = htons(port);
	myaddr.sin_addr.s_addr = htonl(INADDR_ANY);
	setsockopt(socketfd , SOL_SOCKET , SO_SNDBUF , (const char*)&size , sizeof(size));
	if ( bind(socketfd, (struct sockaddr*)&myaddr, sizeof(myaddr)) == -1)
	{
		//ERR_PRINT("socket tcp bind failed");
		TCP_CLOSE(socketfd);
		return 1;
	}
	return 0;
}

unsigned int UDP_BIND(int socketfd, unsigned short port)
{
	return udpsocket_bind(socketfd, port);
}

unsigned int UDPBD_BIND(int socketfd, unsigned short port)
{
	return udpsocket_bind(socketfd, port);
}



static unsigned int udpsocket_send(int socketfd, unsigned char *psend, unsigned int psend_len,
								   unsigned char *pdest, unsigned short destport)
{
	int ret = 0;
	struct sockaddr_in  dstaddr;
	struct sockaddr_in *pdstaddr = &dstaddr;
	int addrlen = 0;

	addrlen = sizeof(struct sockaddr);
	memset(pdstaddr, 0, sizeof(dstaddr));
	if( socketfd <= 0 )
	{
		return 1;
	}
	dstaddr.sin_family = AF_INET;
	dstaddr.sin_port = htons(destport);
	dstaddr.sin_addr.s_addr = inet_addr((char *)pdest);
	if( ( ret = sendto( socketfd, (char *)psend, psend_len,0, (struct sockaddr *)pdstaddr, addrlen)) < 0 )
	{
		//ERR_PRINT("\n\rsocketfd = %d. sendbuf = %s, send_len = %d. addr.sin_addr=%s,addr.sin_port=%d.\r\n", \
			socketfd, psend, *psend_len,inet_ntoa(pdstaddr->sin_addr),ntohs(pdstaddr->sin_port));
		return 1;
	}
	return 0;
}

unsigned int UDP_SEND(int socketfd, unsigned char *psend, unsigned int psend_len,
					      unsigned char *pdest, unsigned short destport)
{
	return udpsocket_send(socketfd, psend, psend_len, pdest, destport);
}

unsigned int UDPBD_SEND(int socketfd, unsigned char *psend, unsigned int psend_len,
					   unsigned short destport)
{
	char boadcast[] = "255.255.255.255";
	return udpsocket_send(socketfd, psend, psend_len, (unsigned char *)boadcast, destport);
}


static unsigned int udpsocket_recv(int socketfd, unsigned char *precv, unsigned int *precv_len,
								 unsigned char *precvaddr, unsigned short *precv_port)
{
	int ret = 0;
	socklen_t addrlen = 0;
	unsigned char *precvIP = NULL;
	struct sockaddr_in addr;
	memset(&addr, 0, sizeof(addr));
	addrlen = sizeof( struct sockaddr);

	if( socketfd <= 0 )
		return 1;

	if( ( ret = recvfrom( socketfd, (char *)precv, MTU,0, (struct sockaddr*)&addr, &addrlen )  ) < 0 )
	{
		//ERR_PRINT( "receive from failed");
		return 1;
	}
	else
	{
		*precv_len=ret;
		precvIP = (unsigned char*)inet_ntoa(addr.sin_addr);
		if(precvaddr)
		{
		strcpy((char *)precvaddr,(char *)precvIP);
		}
		if (precv_port)
		{	
			*precv_port = ntohs(addr.sin_port);
		}
		//memcpy((void *)precvaddr, precvIP, strlen(precvIP));
		return 0;
	}
}

unsigned int UDP_RECV(int socketfd, unsigned char *precv, unsigned int *precv_len,
					  unsigned char *precvaddr, unsigned short *precv_port)
{
	return udpsocket_recv(socketfd, precv, precv_len, precvaddr, precv_port);
}

unsigned int UDPBD_RECV(int socketfd, unsigned char *precv, unsigned int *precv_len,
					  unsigned char *precvaddr, unsigned short *precv_port)
{
	return udpsocket_recv(socketfd, precv, precv_len, precvaddr, precv_port);
}

static unsigned int udpsocket_close(int socketfd)
{
	if (close(socketfd) == -1)
	{
		//ERR_PRINT("socket udp close failed");
		return 1;
	}
	return 0;
}

unsigned int UDP_CLOSE(int socketfd)
{
	return udpsocket_close(socketfd);
}

unsigned int UDPBD_CLOSE(int socketfd)
{
	return udpsocket_close(socketfd);
}
