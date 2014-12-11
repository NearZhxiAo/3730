/*
 * =====================================================================================
 *       Copyright (C), 2008, islab@ZJU
 *       �ļ���     :  dvs_unixcom.h
 *       �ļ�����   :  DVR File Header
 *       �汾��Ϣ   :  1.0
 *       ��������   :  2008��01��16�� 10ʱ05��26�� CST
 *       �汾��¼   :  none
 *       ������     :  mipsel-linux-gcc/g++
 *       ������Ϣ   :  Dr Islab (Islab), islab@zju.edu.cn.
 *       ��˾����   :  ZJU Digital ISLab.
 *       ����˵��   :
 * =====================================================================================
 */
#ifndef  LIB_UNIX_INC
#define  LIB_UNIX_INC
#ifdef __cplusplus
extern "C"{
#endif

#include "system_debug.h"
#include "intercmdtype.hin"
#include "intercmd.h"
typedef unsigned int BLOCK_BOOL;
#define FALSE 0
#define TRUE  1

#define CMDINFO_LEN 128	

#define ACK_REQUIRE	0x55555555
#define ACK_NO_REQUIRE	0xAAAAAAAA
typedef struct _unix_cmd_struct
{
    unsigned int cmd;				//交互命令字
    unsigned int cmdlen;			//交互命令信息长度
    unsigned char cmdinfo[CMDINFO_LEN];		//交互命令信息
    unsigned int ret_ack;			//是否需要回应标志, 在构造数据包发送的时候根据需求进行
}UNIX_CMD_STRUCT;

#define UNIX_ACK_FLAG	0x55555555
#define UNIX_NACK_FLAG	0xAAAAAAAA
typedef struct _unix_cmdack_struct
{
    unsigned int ack_flag;
    unsigned int ack_resv;
}UNIX_CMDACK_STRUCT;

/*
   本地域套接字创建函数
   localname：本地绑定的文件名，每个模块对应的文件名在intercmd.h中定义,UNIXFILE_SOCKET_XXX
   sockfd：域套接字创建返回的fd
   block：阻塞标志字，表明对此socket fd是不是阻塞进行使用
 */
succeed_type UNIX_CREATE(char *localname,int *sockfd,BLOCK_BOOL block);

succeed_type UNIX_DESTROY(char *localname,int sockfd);

/*
   本地域套接字接收函数
   sockfd：之前创建返回的sockfd
   remotename：指针，接收函数返回后里面放置的是接收到谁的信息指示
   pinfo：接收到信息的指针，里面存放的是接收到信息的命令字以及信息本身
 */
succeed_type UNIX_RECV(int sockfd, char *remotename, UNIX_CMD_STRUCT *pinfo);

/*
   本地域套接字发送函数
   pinfo：其中的cmd必须填写，里面的cmd对应于intercmd.h中的PARACMD_XXX，里面cmdinfo对应的是它的数据结构
 */
succeed_type UNIX_SEND(int sockfd,  UNIX_CMD_STRUCT *pinfo);



#ifdef __cplusplus
}
#endif

#endif   /* ----- #ifndef DVS_UNIXCOM_INC  ----- */


