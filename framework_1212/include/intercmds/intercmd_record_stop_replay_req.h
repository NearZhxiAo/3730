#ifndef _INTERCMD_RECORD_STOP_REPLAY_REQ_H__
#define _INTERCMD_RECORD_STOP_REPLAY_REQ_H__

#include "client_inter_comm.h"
#ifdef __cplusplus
extern "C"{
#endif

typedef struct record_stop_replay_req
{
    PE_CHANNEL_ID chann_id;
    PE_CHANNEL_INFO channel_info;
}record_stop_replay_req;

#ifdef __cplusplus
}
#endif



#endif
