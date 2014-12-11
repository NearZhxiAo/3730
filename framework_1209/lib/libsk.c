/************************************************************************
 * Copyright (C), 2007, islab@ZJU
 * FileName:   olym_libsk.h
 * Description:
 * 1:	socket create function
 * 2:	socket option set function
 * 3:	socket recv function
 * 4:	socket send function
 * Date:	2007-12-21
 * Version:	V1.0
 *  *************************************************************************/
#include "system_includes.h"
#include "system_debug.h"
#include "libsk.h"
/******************************************** 
 *   Function    : olym_libsk_setoption
 *   Author	 : 
 *   Date	 : 2007-12-20 
 *   Description : Set socket option
 *   Para	 :
 *   		  sk 	 --- socket
 *   		  option --- option choice
 *   		  ip	 --- ip addr
 *   		  ttl    --- ttl size
 *   Return	 : succeed_type_succeed -- suceed
 *   		   succeed_type_failed  -- failed	
 *********************************************/
static succeed_type libsk_setoption(int sk , int option, char *ip, unsigned long ttl)
{
	int opt = 1;
	socklen_t len = sizeof(opt);
	switch(option)
	{
		case libsk_opt_reuse_addr:	//Set reuse flag
			{
				 setsockopt(sk,SOL_SOCKET,SO_REUSEADDR,&opt,len);

			}
			break;
		case libsk_opt_set_ttl:		//Set ttl 
			{
				char t;
				t = (char )ttl;
				setsockopt(sk, IPPROTO_IP, IP_MULTICAST_TTL , (char *)&t, sizeof(t));
			}
			break;
		default:
			{
				SYS_ERROR("Unknow sk option : %d.\n", option);
				return succeed_type_failed;
			}
			break;
	}
	return succeed_type_succeed;
}


/******************************************** 
 *   Function    : dvs_libsk_create
 *   Author	 : 
 *   Date	 : 2007-12-20 
 *   Description : Create Socket
 *   Para	 :
 *   		  netmode --- UDP/TCP
 *   		  port 	 --- socket port
 *   		  ip	 --- ip addr
 *   		  ttl    --- ttl size
 *   		  pskfd	 --- socket fd 
 *   Return	 : succeed_type_succeed -- suceed
 *   		   succeed_type_failed  -- failed	
 *********************************************/
static succeed_type libsk_create(rt_net_mode netmode , short port, char *ip,   int *pskfd)
{
	int sockfd = 0;
	struct sockaddr_in myaddr;
	int size = 1<<19;		//send buf size value
	memset((void *)&myaddr, 0, sizeof(myaddr));
	*pskfd = 0;
	switch(netmode)
	{
		case LIBSK_NETMODE_TCP : //TCP
			if((sockfd = socket(AF_INET, SOCK_STREAM, 0)) == -1)
			{
				SYS_ERROR("TCP Socket Create Error.\n");
				return succeed_type_failed;
			}
			if ( libsk_setoption(sockfd , libsk_opt_reuse_addr, NULL, 0 ) == succeed_type_failed )
			{
				SYS_ERROR("Set Tcp option reuse failed.\n");
			}
			myaddr.sin_family = AF_INET;         	/* host byte order */
			myaddr.sin_port = htons(port);     	/* short, network byte order */
			myaddr.sin_addr.s_addr = INADDR_ANY;
			if (bind(sockfd, (struct sockaddr *)&myaddr, sizeof(myaddr))== -1)
			{
				SYS_ERROR("Bind to socket failed.\n");
				perror("Bind to socket");
				close(sockfd);
				return succeed_type_failed;;
			}
#if 0
			if(listen(sockfd,5)==-1)
			{
				SYS_ERROR("Listen failed\n");
			}
			//listen
#endif

			break;
		case LIBSK_NETMODE_UDP :
			if((sockfd = socket(AF_INET , SOCK_DGRAM,0)) == -1)
			{
				SYS_ERROR("UDP Socket Create Error.errorno = %d\r\n",errno);
				perror("udp socket error,\r\n");
				return succeed_type_failed;
			}
			if ( libsk_setoption(sockfd , libsk_opt_reuse_addr, NULL, 0) == succeed_type_failed )
			{
				SYS_ERROR("Set UDP option reuse failed.\n");
			}
			memset(&myaddr , 0 , sizeof(myaddr));
			myaddr.sin_family = AF_INET;         /* host byte order */
			myaddr.sin_port = htons(port);     /* short, network byte order */
			myaddr.sin_addr.s_addr = INADDR_ANY;
			setsockopt(sockfd , SOL_SOCKET , SO_SNDBUF , &size , sizeof(size));
			if (bind(sockfd, (struct sockaddr *)&myaddr, sizeof(myaddr))== -1)
			{
				SYS_ERROR("Bind to socket failed.\n");
				close(sockfd);
				return succeed_type_failed;;
			}

			break;
		default:
			SYS_ERROR("Unknow NetMode: %d.\n", netmode);
			return succeed_type_failed;
			break;
	}
	*pskfd = sockfd;
	return succeed_type_succeed;;
}

/******************************************** 
 *   Function    : dvs_libsk_accept
 *   Author	 : 
 *   Date	 : 2007-12-20 
 *   Description : Socket accept func
 *   Para	 :
 *   		  sk --- socket to listen
 *   		  client_addr --- connect addr
 *   		  pskfd	 --- new accept socket fd 
 *   Return	 : succeed_type_succeed -- suceed
 *   		   succeed_type_failed  -- failed	
 *********************************************/
static succeed_type libsk_accept(int sk, struct sockaddr_in *client_addr, int *pskfd)
{
	int sockfd = 0 ;
	struct sockaddr_in addr;
	int sin_size=sizeof(struct sockaddr_in);
	memset((void *)&addr, 0, sizeof(addr));
	*pskfd = 0;
	if(client_addr)
	{
		if ((sockfd = accept(sk , (struct sockaddr*) client_addr , ( socklen_t* )&sin_size)) == -1) 
		{
			SYS_ERROR("Accept failed.\n");
			return succeed_type_failed;
		}
	}
	else
	{
		if ((sockfd = accept(sk , (struct sockaddr*)&addr , ( socklen_t* )&sin_size)) == -1)
		{
			SYS_ERROR("Accept failed.\n");
			return succeed_type_failed;
		}
	}
	*pskfd = sockfd;	
	return succeed_type_succeed;
}

/******************************************** 
 *   Function    : dvs_libsk_close
 *   Author	 : 
 *   Date	 : 2007-12-20 
 *   Description : Socket close func
 *   Para	 :
 *   		  sk --- socket to connect
 *   		  port --- remote server port
 *   		  ip --- remote server ip
 *   Return	 : succeed_type_succeed -- suceed
 *   		   succeed_type_failed  -- failed	
 *********************************************/
static succeed_type libsk_connect(int sk, short port, char *ip)
{
	struct sockaddr_in addr;
	int ret = 0;

	if( sk <= 0 )
	{
		SYS_ERROR("libsk_connect sk <= 0\r\n");
		return succeed_type_failed;
	}

	memset( &addr, 0, sizeof( addr ) );

	addr.sin_family = AF_INET;
	addr.sin_port = htons( port );

	if(inet_aton( ip, (struct in_addr*)&addr.sin_addr ) == 0) 
		//if(inet_pton( AF_INET, ip, &addr.sin_addr ) <= 0) 
	{
		SYS_ERROR( "failed to convert ip string...ip=%s", ip );
		return succeed_type_failed;
	}

	if( ( ret = connect( sk, (struct sockaddr *)&addr, sizeof( addr ) ) ) < 0 )
	{
		SYS_ERROR( "connect failed...errno=%d", errno );
		return succeed_type_failed;
	}
	else
		return succeed_type_succeed;
}

/******************************************** 
 *   Function    : dvs_libsk_close
 *   Author	 : 
 *   Date	 : 2007-12-20 
 *   Description : Socket close func
 *   Para	 :
 *   		  sk --- socket to close
 *   Return	 : succeed_type_succeed -- suceed
 *   		   succeed_type_failed  -- failed	
 *********************************************/
static succeed_type libsk_close(int sk)
{
	if((sk != 0) && (sk != -1))
	{
		close(sk);
	}
	return succeed_type_succeed;
}


/******************************************** 
 *   Function    : dvs_libsk_send
 *   Author	 : 
 *   Date	 : 2007-12-20 
 *   Description : Socket send function
 *   Para	 :
 *   Return	 : succeed_type_succeed -- suceed
 *   		   succeed_type_failed  -- failed	
 *********************************************/
//void printsignal(int signo)
//{
//	printf("signal=%n",signo);
//	signal(signo,printsignal);
//}
static succeed_type libsk_send( int sk, unsigned char *ptrwritebuf, unsigned int *len)
{
	unsigned int sent = 0;

	if( sk <= 0 )
		return succeed_type_failed;
	if( ( sent = send( sk, ptrwritebuf, *len, 0 ) ) <= 0 )
	{

		SYS_ERROR( "sendto failed...errno=%s",strerror(errno) );

		return succeed_type_failed;
	}
	else
	{
		*len = sent;
		return succeed_type_succeed;
	}

	return succeed_type_succeed;
}
/******************************************** 
 *   Function    : dvs_libsk_accept
 *   Author	 : 
 *   Date	 : 2007-12-20 
 *   Description : Socket accept func
 *   Para	 :
 *   		  sk --- socket to listen
 *   		  client_addr --- connect addr
 *   		  pskfd	 --- new accept socket fd 
 *   Return	 : succeed_type_succeed -- suceed
 *   		   succeed_type_failed  -- failed	
 *********************************************/
static succeed_type libsk_sendto(int sk, unsigned char *ptrwritebuf, unsigned int *len, struct sockaddr_in* addr)
{
	int ret = 0;
	socklen_t addrlen = 0;
	addrlen = sizeof( struct sockaddr );


	if( sk <= 0 )
	{
		return succeed_type_failed;
	}


	//SYS_INFO("fd=%d  pc_packet=%d  len =%d, src addr = 0x%x. src port = %d\n",fd,pc_packet->cmd, addr.sin_addr, addr.sin_port);

	if( ( ret = sendto( sk, ptrwritebuf, *len,0, (struct sockaddr *)addr, addrlen ) ) < 0 )
	{
		SYS_ERROR(" sk = %d. buf = %s, len = %d. addr.sin_addr=%s,addr.sin_port=%d.\r\n",sk, ptrwritebuf, *len,inet_ntoa(addr->sin_addr),addr->sin_port);
		SYS_ERROR( "sendto failed...errno=%s",strerror(errno) );
		return succeed_type_failed;
	}
	return succeed_type_succeed;
}

/******************************************** 
 *   Function    : dvs_libsk_recv
 *   Author	 : 
 *   Date	 : 2007-12-20 
 *   Description : Socket send function
 *   Para	 :
 *   Return	 : succeed_type_succeed -- suceed
 *   		   succeed_type_failed  -- failed	
 *********************************************/
static succeed_type libsk_recv( int sk, unsigned char *ptr, unsigned int *len )
{
	unsigned int read = 0;

	if( sk <= 0 )
		return succeed_type_failed;


	if( ( read = recv( sk, ptr, *len, 0 ) ) <= 0 )
	{
		return succeed_type_failed;
	}
	else
	{
		*len = read;
		return succeed_type_succeed;
	}
}

/******************************************** 
 *   Function    : dvs_libsk_recv
 *   Author	 : 
 *   Date	 : 2007-12-20 
 *   Description : Socket send function
 *   Para	 :
 *   Return	 : succeed_type_succeed -- suceed
 *   		   succeed_type_failed  -- failed	
 *********************************************/
static succeed_type libsk_recvfrom( int sk,unsigned char *ptr,unsigned int *len,struct sockaddr_in *addr)
{
	int ret = 0;
	socklen_t addrlen = 0;
	addrlen = sizeof( struct sockaddr );

	if( sk <= 0 )
		return succeed_type_failed;

	if( ( ret = recvfrom( sk, ptr, *len,0, (struct sockaddr*)addr, &addrlen )  ) < 0 )
	{
		SYS_ERROR( "receive from failed...errno=%s", strerror(errno) );
		return succeed_type_failed;
	}
	else
	{
		*len=ret;
		return succeed_type_succeed;
	}
}

succeed_type SOCKET_CREATE(rt_net_mode netmode , short port, char *ip,   int *pskfd)
{
	return libsk_create(netmode,port,ip,pskfd);
}

succeed_type SOCKET_ACCEPT(int sk, struct sockaddr_in *client_addr, int *pskfd)
{
	return libsk_accept(sk, client_addr, pskfd);
}

succeed_type SOCKET_CONNECT(int sk, short port, char *ip)
{
	return libsk_connect(sk, port, ip);
}

succeed_type SOCKET_SEND( int sk, unsigned char *ptrwritebuf, unsigned int *len,struct sockaddr_in* addr)
{
	if ( addr == NULL )
	{
		return libsk_send( sk, ptrwritebuf, len);
	}
	else
	{
		return libsk_sendto(sk, ptrwritebuf, len, addr);
	}
}
succeed_type SOCKET_RECV( int sk, unsigned char *ptr, unsigned int *len,struct sockaddr_in* addr)
{
	if ( addr == NULL )
	{
		return libsk_recv( sk, ptr, len);
	}
	else
	{
		return libsk_recvfrom( sk,ptr,len,addr);
	}
}
succeed_type SOCKET_CLOSE( int sk)
{
	return libsk_close(sk);
}
