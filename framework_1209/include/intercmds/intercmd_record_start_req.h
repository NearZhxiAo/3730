#ifndef _INTERCMD_RECORD_START_REQ_H__
#define _INTERCMD_RECORD_START_REQ_H__

#include "client_inter_comm.h"
#ifdef __cplusplus
extern "C"{
#endif

typedef struct record_start_req
{
    PE_UCHAR record_type;
    PE_CHANNEL_ID chann_id;
    PE_CHANNEL_INFO chan_info;
    PE_RECORDSTART_REQ record_req;
    PE_LONG record_timelen;
    unsigned char guid[16];
    PE_NET_SCHEDULE_ADDRESS schedule_address;
}record_start_req;

#ifdef __cplusplus
}
#endif



#endif
