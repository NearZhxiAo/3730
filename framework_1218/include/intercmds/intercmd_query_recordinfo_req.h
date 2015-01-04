#ifndef _INTERCMD_QUERY_RECORDINFO_REQ_H__
#define _INTERCMD_QUERY_RECORDINFO_REQ_H__

#include "client_inter_comm.h"
#ifdef __cplusplus
extern "C"{
#endif

typedef struct query_recordinfo_req
{
    PE_CHANNEL_ID chann_id;
    unsigned char ip[64];
    unsigned char qos;
    PE_TIME_FILE time_file;
}query_recordinfo_req;

#ifdef __cplusplus
}
#endif



#endif
