#ifndef _INTERCMD_CALL_START_H__
#define _INTERCMD_CALL_START_H__

#include "client_inter_comm.h"
#ifdef __cplusplus
extern "C"{
#endif

typedef struct call_start
{
    PE_CHANNEL_ID pe_channel_id;
    PE_NETADDRESS pe_netaddress;
}call_start;

#ifdef __cplusplus
}
#endif



#endif
