/************************************************************************
 * Copyright (C), 2007, islab@ZJU
 * FileName:   dvs_libunix.h
 * Description:
 * 1:	unix socket create function
 * 2:	unix socket recv function
 * 3:	unix socket send function
 * Date:	2007-12-21
 * Version:	V1.0
 *  *************************************************************************/
#include "system_includes.h"
#include "system_debug.h"
#include "libunix.h"

/******************************************** 
 *   Function    : libunix_create
 *   Author      : 
 *   Date        : 2007-12-20 
 *   Description : Socket unix create
 *   Para        : localname -- bind name
 *   Ret	 : succeed_type_succeed -- succeed
 *   		   succeed_type_failed  -- failed
 **********************************************/
static succeed_type libunix_create( char *localname , int *sockfd, BLOCK_BOOL block)
{
		int local_sockfd = 0;
		struct sockaddr_un	localaddr;
		int oflags = 0;
		memset((void *)&localaddr, 0, sizeof(localaddr));

		*sockfd = 0;
		local_sockfd = socket( AF_LOCAL, SOCK_DGRAM, 0 );
		if ( local_sockfd < 0 )
		{
				SYS_ERROR("Local Unix Socket Create failed.\n");
				return succeed_type_failed;
		}

		unlink( localname );

		localaddr.sun_family = AF_LOCAL;
		strcpy( localaddr.sun_path, localname );
		if ( bind( local_sockfd, (struct sockaddr *)( &localaddr ), sizeof( localaddr ) ) < 0 )
		{
				SYS_ERROR("Bind Unix name : %s failed.\n", localname);
				close(local_sockfd);
				return succeed_type_failed;
		}

		oflags = fcntl(local_sockfd, F_GETFL);	
		if ( block == TRUE )
		{
				oflags &= ~O_NONBLOCK;
		}
		else
		{
				oflags |= O_NONBLOCK;
		}
		{
				int size = 10*1024;
				setsockopt(local_sockfd , SOL_SOCKET , SO_RCVBUF , &size , sizeof(size));
				setsockopt(local_sockfd , SOL_SOCKET , SO_SNDBUF , &size , sizeof(size));
		}
		fcntl(local_sockfd, F_SETFL , oflags);
		*sockfd = local_sockfd;
		return succeed_type_succeed;
}
/******************************************** 
 *    Function    : dvs_libunix_recv
 *    Author      : 
 *    Date        : 2007-12-20 
 *    Description : Socket unix recv
 *    Para        : sockfd  -- socket fd
 *    		    remotename -- data source
 *    		    data -- data pointer
 *    		    len  -- data len
 *    Ret         : succeed_type_succeed -- succeed
 *                 succeed_type_failed  -- failed
 * **********************************************/
succeed_type libunix_recv( int sockfd, char *remotename, char *data, int len , int *pretlen)
{
		struct sockaddr_un remoteaddr;
		socklen_t addrlen = 0;
		int ret = 0;
		memset( &remoteaddr, 0, sizeof( remoteaddr ) );
		addrlen = sizeof( remoteaddr );
		*pretlen = 0;

		ret = recvfrom( sockfd, data, len, 0, ( struct sockaddr* )&remoteaddr, &addrlen );
		if( ret < 0 )
		{
				SYS_ERROR("recvfrom failed.\n");
				return succeed_type_failed;
		}

		strcpy( remotename, remoteaddr.sun_path );

		*pretlen = ret;
		return succeed_type_succeed;
}


/******************************************** 
 *    Function    : libunix_send
 *    Author      : 
 *    Date        : 2007-12-20 
 *    Description : Socket unix send
 *    Para        : sockfd  -- socket fd
 *    		    remotename -- destination
 *    		    data -- data pointer
 *    		    len  -- data len
 *    Ret         : succeed_type_succeed -- succeed
 *                 succeed_type_failed  -- failed
 * **********************************************/
static succeed_type libunix_send(int sockfd, char *remotename, char *data, int len , int *pretlen)
{
		struct sockaddr_un	remoteaddr;
		int ret_value = 0;
		socklen_t addrlen = 0;
		memset( &remoteaddr, 0x00, sizeof( remoteaddr ) );
		remoteaddr.sun_family = AF_LOCAL;
		strcpy( remoteaddr.sun_path,  remotename );
		addrlen = sizeof( remoteaddr );
		if ( sockfd <= 0 )
		{
				SYS_ERROR("dvs_libunix send socket error.\n");
				return succeed_type_failed;
		}
		ret_value = sendto(sockfd, data, len, 0, ( struct sockaddr * )&remoteaddr, addrlen );
		if ( ret_value <= 0 )
		{
				SYS_ERROR("dvs_libunix_sendto failed... remotename = %s errno=%d.\n", remotename, errno );
				perror( "sendto error:" );
				return succeed_type_failed;
		}
		*pretlen = ret_value;	
		return succeed_type_succeed;
}
/******************************************** 
 *    Function    : libunix_destroy
 *    Author      : 
 *    Date        : 2007-12-20 
 *    Description : Socket unix send
 *    Para        : sockfd  -- socket fd
 *    		    remotename -- destination
 *    Ret         : succeed_type_succeed -- succeed
 *                 succeed_type_failed  -- failed
 * **********************************************/
static succeed_type libunix_destroy( char *localname, int sockfd)
{
		close(sockfd);
		unlink(localname);
		return succeed_type_succeed;
}
/******************************************** 
 *    Function    : CHECK_CMD
 *    Author      : 
 *    Date        : 2007-12-20 
 *    Description : Socket unix recv
 *    Para        : pinfo  --
 *    Ret         : succeed_type_succeed -- succeed
 *                 succeed_type_failed  -- failed
 * **********************************************/
static succeed_type CHECK_CMD(UNIX_CMD_STRUCT *pinfo)
{
		if(pinfo->cmd > PARACMD_END )
		{
				return succeed_type_failed;
		}
		return succeed_type_succeed;	
}
/******************************************** 
 *    Function    : UNIX_CREATE
 *    Author      : 
 *    Date        : 2007-12-20 
 *    Description : Socket unix recv
 *    Para        : pinfo  --
 *    Ret         : succeed_type_succeed -- succeed
 *                 succeed_type_failed  -- failed
 * **********************************************/
succeed_type UNIX_CREATE(char *localname,int *sockfd,BLOCK_BOOL block)
{
		succeed_type	ret = succeed_type_succeed;
		ret = libunix_create(localname,sockfd,block);
		if(ret == succeed_type_failed)
		{
				SYS_ERROR("libunix create error.\r\n");
				return succeed_type_failed;
		}
		return succeed_type_succeed;
}

/******************************************** 
 *    Function    : UNIX_DESTROY
 *    Author      : 
 *    Date        : 2007-12-20 
 *    Description : Socket unix recv
 *    Para        : pinfo  --
 *    Ret         : succeed_type_succeed -- succeed
 *                 succeed_type_failed  -- failed
 * **********************************************/
succeed_type UNIX_DESTROY(char *localname,int sockfd)
{
		succeed_type	ret = succeed_type_succeed;
		ret = libunix_destroy(localname,sockfd);
		if(ret == succeed_type_failed)
		{
				SYS_ERROR("libunix destroy error.\r\n");
				return succeed_type_failed;
		}
		return succeed_type_succeed;
}

succeed_type UNIX_RECV(int sockfd, char *remotename, UNIX_CMD_STRUCT *pinfo)
{
		succeed_type ret = succeed_type_succeed;
		int retlen = 0;
		if ( (sockfd < 0) || ( remotename == NULL) || (pinfo == NULL))
		{
				SYS_ERROR("parameter error.\r\n");
				return succeed_type_failed;
		}
		ret = libunix_recv(sockfd, remotename, (char*)pinfo, sizeof(UNIX_CMD_STRUCT), &retlen);

		if (ret == succeed_type_failed)
		{
				SYS_ERROR("libunix_recv failed.\r\n");
				return succeed_type_failed;
		}
		if(retlen!=sizeof(UNIX_CMD_STRUCT))
		{
				SYS_ERROR("libunxi_recv length failed.\r\n");
				return succeed_type_failed;
		}
		ret = CHECK_CMD(pinfo);
		return ret;
}
succeed_type UNIX_POLL(int sockfd, int timeout)
{
		struct pollfd polltimeout;

		memset(&polltimeout, 0x00, sizeof(polltimeout));
		polltimeout.fd = sockfd;
		polltimeout.events = POLLIN;

		if (poll(&polltimeout, 1, timeout) > 0 )
		{
				return succeed_type_succeed;
		}
		SYS_INFO("UNIX_POLL timeout.\r\n");
		return succeed_type_failed;
}
succeed_type UNIX_SEND(int sockfd,  UNIX_CMD_STRUCT *pinfo)
{
		succeed_type ret = succeed_type_failed;
		UNIX_CMD_STRUCT recvcmd;
		unsigned char remotename[128];
		int retlen = 0;

		memset(&recvcmd, 0x00, sizeof(recvcmd));
		memset(remotename, 0x00, 128);

		if (CHECK_CMD(pinfo) == succeed_type_failed)
		{
				SYS_ERROR("UNIX_SEND　cmd value is error.\r\n");
				return succeed_type_failed;
		}
		{
				unsigned int senddest =  find_sendto_dest_throw_cmd(pinfo->cmd);
				unsigned int sendtoflag = find_sendflag_ofcmd(pinfo->cmd);
				unsigned int i = 0;
				if (sendtoflag == 1)
				{
						pinfo->ret_ack = ACK_REQUIRE;
				}
				else
				{
						pinfo->ret_ack = ACK_NO_REQUIRE;
				}
				for(i = 0; i < UNIX_END; i++)
				{
						if ( (1<<i) & senddest)
						{
								ret = libunix_send( sockfd, findcmd_unix_name[i], pinfo, sizeof(UNIX_CMD_STRUCT),&retlen);
								if (ret == succeed_type_failed)
								{
										SYS_ERROR("libunix_send to dest %s failed.\r\n", findcmd_unix_name[i]);
								}
								if (sendtoflag == 1)
								{
										if (UNIX_RECV(sockfd, remotename, &recvcmd) == succeed_type_failed)
										{
												SYS_ERROR("UNIX_RECV from %s failed.\r\n", findcmd_unix_name[i]);
										}
								}
						}
				}
		}
		return succeed_type_succeed;	 
}

