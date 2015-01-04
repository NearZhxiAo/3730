#ifndef _INTERCMD_RECORD_STOP_REQ_H__
#define _INTERCMD_RECORD_STOP_REQ_H__

#include "client_inter_comm.h"
#ifdef __cplusplus
extern "C"{
#endif

typedef struct record_stop_req
{
    PE_UCHAR record_type;
    PE_CHANNEL_ID chann_id;
    PE_UCHAR option;
    unsigned char  guid[16];
}record_stop_req;

#ifdef __cplusplus
}
#endif



#endif
