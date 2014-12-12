#ifndef	_CLIENT_PC_PRIVATE_H__ 
#define _CLIENT_PC_PRIVATE_H__
#ifdef __cplusplus
extern "C"{
#endif

#include "system_includes.h"
#include "system_debug.h"
#include "libgbshm.h"
#include "libunix.h"
#include "libsignal.h"
#include <openssl/md5.h>
#include <signal.h>
//#include "intercmd_ctl_ptz_req.h"

#define LOGINID "010000000100011801"
#define PORT 5000
#define SERVER_IP "192.168.0.38"
#define  LOCAL_IP "192.168.0.177"
#define  LOCAL_PORT 1038
#define CLIENT_PC_POLL_COUNT_MAX	2


typedef struct _CLIENT_PC_PRIVATE_HANDLE
{
		LIBGBSHM_OBJ *pgbshm_obj;
		struct pollfd polltimeout[CLIENT_PC_POLL_COUNT_MAX];
		int unix_fd;
		int socket_fd;
		int active_fdcount;
		int seq;
		unsigned char socketbuf[1024];	//
		unsigned char socketbuf2[1024];	//
		unsigned char heartbeatbuf[150];
		unsigned char pebuf[30];
		int lenr;
		int lens;	
		int heart_time;
		net_param net_param;
}CLIENT_PC_PRIVATE_HANDLE;
//client_pc_main.c

//client_pc_socket.c
//void request_step_one(CLIENT_PC_PRIVATE_HANDLE*);
//int request_step_two(CLIENT_PC_PRIVATE_HANDLE*,int);
//void heartbeat(CLIENT_PC_PRIVATE_HANDLE*);
//void GetParam(unsigned short int,int);

//client_pc_unix.c
//void SendtoRTSP(CLIENT_PC_PRIVATE_HANDLE*,unsigned short int);

//client_pc_deal.c

#ifdef __cplusplus
}
#endif
#endif  
