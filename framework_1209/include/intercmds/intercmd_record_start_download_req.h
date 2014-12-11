#ifndef _INTERCMD_RECORD_START_DOWNLOAD_REQ_H__
#define _INTERCMD_RECORD_START_DOWNLOAD_REQ_H__

#include "client_inter_comm.h"
#ifdef __cplusplus
extern "C"{
#endif

typedef struct record_start_download_req
{
    PE_CHANNEL_ID chann_id;
    PE_TIME_FILE time_file;
    PE_NET_SCHEDULE_ADDRESS addr;
    unsigned char guid[16];
    unsigned char ip[64];
    PE_CHANNEL_INFO channel_info;
}record_start_download_req;

#ifdef __cplusplus
}
#endif



#endif
