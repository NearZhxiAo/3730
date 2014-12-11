#ifndef _NET_CMDSTRUCT_H_
#define _NET_CMDSTRUCT_H_

#ifdef __cplusplus
extern "C"{
#endif

#include <stdio.h>
#include <stdlib.h>
#include <string.h>


typedef enum _CMD_DIR
{
    CMD_DIR_START,
    CMD_DIR_CLIENTTOSERVER,
    CMD_DIR_SERVERTOCLIENT,
    CMD_DIR_END
}CMD_DIR;

#define CMD_SIZE_PERPACKET	1300
#define CMD_MAGIC_START	0x22114433
#define CMD_MAGIC_END	0x11223344

typedef  struct  _CMD_TRANSFER
{
    CMD_DIR   cmd_direction;  
    int      magicnum_start;  
	int      cmd_num;         
	int      contents_len;	  
    char     cmd_contents[CMD_SIZE_PERPACKET]; 
    int      magicnum_end;    
}CMD_TRANSFER;


extern int CONSTRUCT_CMD(int cmd, char *psend, int sendlen, CMD_TRANSFER *pout);


extern int DECONSTRUCT_CMD(CMD_TRANSFER *pinput, char *ptrout, int *ptrcmd);


#define DATA_SIZE_PERPACKET	1300
#define DATA_MAGIC_START	0x11112222
#define DATA_MAGIC_END	0x22221111
#define PACKET_INDEX_LAST	0x1FFFFFFF

typedef  struct  _H264_ENC_DATA
{
    int   startmagic;   
    int   frame_index;  
    int   packet_index; 
    int   frame_len;    
    int   packet_len;   
    char  packet_contents[DATA_SIZE_PERPACKET];
    int   endmagic;      
}H264_ENC_DATA;


extern int CONSTRUCT_DATA(char *psend, int sendlen, H264_ENC_DATA *pout, int totallen, int packetindex);
extern int DESTRUCT_DATA( H264_ENC_DATA *pinput, char *pout, int *frame_index, int *packet_index, int *packet_size, int *frame_size);

#ifdef __cplusplus
}
#endif

#endif
