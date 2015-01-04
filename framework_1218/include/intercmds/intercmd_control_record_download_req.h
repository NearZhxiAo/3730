#ifndef _INTERCMD_CONTROL_RECORD_DOWNLOAD_REQ_H__
#define _INTERCMD_CONTROL_RECORD_DOWNLOAD_REQ_H__

#include "client_inter_comm.h"
#ifdef __cplusplus
extern "C"{
#endif

typedef struct control_record_download_req
{
    PE_CHANNEL_ID chann_id;
    PE_CTRL_CMD playback_cmd;
    unsigned char guid[16];
}control_record_download_req;

#ifdef __cplusplus
}
#endif



#endif
