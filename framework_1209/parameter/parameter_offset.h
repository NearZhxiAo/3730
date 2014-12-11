#ifndef _PARAMETER_OFFSET_AUTOGEN_
#define _PARAMETER_OFFSET_AUTOGEN_
/*  This file is a autogenerate file .
 *  DONOT EDIT THIS FILE!!
 *  Edit parameter_internal.ddf and parameter_offset.tpl instead
 */
#include "parametertype.hin"
#ifdef __cplusplus
extern "C"{
#endif

enum para_cmd_index_array{
	
   PARA_CMD_SYSTEM_GBSHM_MUTEX_INDEX ,
   PARA_CMD_SYSTEM_TEST_VAR_INDEX ,
   PARA_CMD_SYSTEM_VIDEO_ENC_PARAM_INDEX ,
   PARA_CMD_SYSTEM_STORAGE_SIZE_PARAM_INDEX ,
   PARA_CMD_SYSTEM_NET_PARAM_INDEX ,
   PARA_CMD_SYSTEM_ALERT_DI_PARAM_INDEX ,
   PARA_CMD_SYSTEM_ALERT_VLOSS_PARAM_INDEX ,
   PARA_CMD_SYSTEM_ALERT_MOTION_PARAM_INDEX ,
   PARA_CMD_SYSTEM_STORAGE_SETTING_PARAM_INDEX ,
   PARA_CMD_SYSTEM_AD_SETTING_PARAM_INDEX ,
   PARA_CMD_SYSTEM_TIME_SETTING_PARAM_INDEX ,
   PARA_CMD_SYSTEM_RS485_SETTING_PARAM_INDEX ,
   PARA_CMD_SYSTEM_PTZ_AREA_SETTING_PARAM_INDEX ,
   PARA_CMD_SYSTEM_ALG_SETTING_PARAM_INDEX ,
   PARA_CMD_SYSTEM_WORKING_STATE_PARAM_INDEX ,
   PARA_CMD_SYSTEM_END_PARAM_INDEX ,
   PARA_CMD_SYSTEM_RUNTIME_DEVICE_STATUS_INDEX ,
   PARA_CMD_SYSTEM_RUNTIME_DEVICE_VERSION_INDEX 
   ,PARA_CMD_INDEX_END
};
static unsigned int para_cmd_size_array[]={
   sizeof(system_gbshm_mutex_paratype) * (1),
   sizeof(system_test_var_paratype) * (1),
   sizeof(system_video_enc_param_paratype) * (1),
   sizeof(system_storage_size_param_paratype) * (1),
   sizeof(system_net_param_paratype) * (1),
   sizeof(system_alert_di_param_paratype) * (1),
   sizeof(system_alert_vloss_param_paratype) * (1),
   sizeof(system_alert_motion_param_paratype) * (1),
   sizeof(system_storage_setting_param_paratype) * (1),
   sizeof(system_ad_setting_param_paratype) * (1),
   sizeof(system_time_setting_param_paratype) * (1),
   sizeof(system_rs485_setting_param_paratype) * (1),
   sizeof(system_ptz_area_setting_param_paratype) * (1),
   sizeof(system_alg_setting_param_paratype) * (1),
   sizeof(system_working_state_param_paratype) * (1),
   sizeof(system_end_param_paratype) * (1),
   sizeof(system_runtime_device_status_paratype) * (1),
   sizeof(system_runtime_device_version_paratype) * (1)
};

typedef struct para_cmd_to_para_index_
{
	unsigned int para_index;
	unsigned int para_cmd;
	unsigned int para_argument_count;
}para_cmd_to_para_index;

static para_cmd_to_para_index localparacmd_to_paraindex[] = { 
	{ PARA_CMD_SYSTEM_GBSHM_MUTEX_INDEX, 
	  PARA_CMD_SYSTEM_GBSHM_MUTEX,
	  1
	}
	,
	{ PARA_CMD_SYSTEM_TEST_VAR_INDEX, 
	  PARA_CMD_SYSTEM_TEST_VAR,
	  1
	}
	,
	{ PARA_CMD_SYSTEM_VIDEO_ENC_PARAM_INDEX, 
	  PARA_CMD_SYSTEM_VIDEO_ENC_PARAM,
	  1
	}
	,
	{ PARA_CMD_SYSTEM_STORAGE_SIZE_PARAM_INDEX, 
	  PARA_CMD_SYSTEM_STORAGE_SIZE_PARAM,
	  1
	}
	,
	{ PARA_CMD_SYSTEM_NET_PARAM_INDEX, 
	  PARA_CMD_SYSTEM_NET_PARAM,
	  1
	}
	,
	{ PARA_CMD_SYSTEM_ALERT_DI_PARAM_INDEX, 
	  PARA_CMD_SYSTEM_ALERT_DI_PARAM,
	  1
	}
	,
	{ PARA_CMD_SYSTEM_ALERT_VLOSS_PARAM_INDEX, 
	  PARA_CMD_SYSTEM_ALERT_VLOSS_PARAM,
	  1
	}
	,
	{ PARA_CMD_SYSTEM_ALERT_MOTION_PARAM_INDEX, 
	  PARA_CMD_SYSTEM_ALERT_MOTION_PARAM,
	  1
	}
	,
	{ PARA_CMD_SYSTEM_STORAGE_SETTING_PARAM_INDEX, 
	  PARA_CMD_SYSTEM_STORAGE_SETTING_PARAM,
	  1
	}
	,
	{ PARA_CMD_SYSTEM_AD_SETTING_PARAM_INDEX, 
	  PARA_CMD_SYSTEM_AD_SETTING_PARAM,
	  1
	}
	,
	{ PARA_CMD_SYSTEM_TIME_SETTING_PARAM_INDEX, 
	  PARA_CMD_SYSTEM_TIME_SETTING_PARAM,
	  1
	}
	,
	{ PARA_CMD_SYSTEM_RS485_SETTING_PARAM_INDEX, 
	  PARA_CMD_SYSTEM_RS485_SETTING_PARAM,
	  1
	}
	,
	{ PARA_CMD_SYSTEM_PTZ_AREA_SETTING_PARAM_INDEX, 
	  PARA_CMD_SYSTEM_PTZ_AREA_SETTING_PARAM,
	  1
	}
	,
	{ PARA_CMD_SYSTEM_ALG_SETTING_PARAM_INDEX, 
	  PARA_CMD_SYSTEM_ALG_SETTING_PARAM,
	  1
	}
	,
	{ PARA_CMD_SYSTEM_WORKING_STATE_PARAM_INDEX, 
	  PARA_CMD_SYSTEM_WORKING_STATE_PARAM,
	  1
	}
	,
	{ PARA_CMD_SYSTEM_END_PARAM_INDEX, 
	  PARA_CMD_SYSTEM_END_PARAM,
	  1
	}
	,
	{ PARA_CMD_SYSTEM_RUNTIME_DEVICE_STATUS_INDEX, 
	  PARA_CMD_SYSTEM_RUNTIME_DEVICE_STATUS,
	  1
	}
	,
	{ PARA_CMD_SYSTEM_RUNTIME_DEVICE_VERSION_INDEX, 
	  PARA_CMD_SYSTEM_RUNTIME_DEVICE_VERSION,
	  1
	}
	
};

static inline int  parameter_indexfind(unsigned int cmd)
{
	int index = 0;
	for ( index = 0; index < PARA_CMD_INDEX_END; index++ )
	{
		if ( localparacmd_to_paraindex[index].para_cmd == cmd )
		{
			return localparacmd_to_paraindex[index].para_index;
		}
	}
	return -1;
}

static inline int  parameter_argument_count(unsigned int cmd)
{
	int index = 0;
	for ( index = 0; index < PARA_CMD_INDEX_END; index++ )
	{
		if ( localparacmd_to_paraindex[index].para_cmd == cmd )
		{
			return localparacmd_to_paraindex[index].para_argument_count;
		}
	}
	return -1;
}

static inline int  parameter_offset( unsigned int cmd )	{	
    unsigned int total = 0,i = 0;	
	int index = 0;
    if ( (cmd) > PARA_CMD_END )
    {	
        return -1;	
    }	
	index = parameter_indexfind(cmd);	
	if ( index == -1 )
	{
		return -1;
	}
    for ( i = 0; i < (index); i++ )	
    {	
        total += para_cmd_size_array[i];	
    }	
    return total;	
}

static inline int  parameter_size( unsigned int cmd )	{	
	int index = 0;
    if ( (cmd) > PARA_CMD_END )
    {	
        return -1;	
    }	
	index = parameter_indexfind(cmd);	
	if ( index == -1 )
	{
		return -1;
	}
	return para_cmd_size_array[index] / localparacmd_to_paraindex[index].para_argument_count;
}



#define	 PARA_CMD_SYSTEM_GBSHM_MUTEX_OFFSET	parameter_offset(PARA_CMD_SYSTEM_GBSHM_MUTEX )

#define	 PARA_CMD_SYSTEM_TEST_VAR_OFFSET	parameter_offset(PARA_CMD_SYSTEM_TEST_VAR )

#define	 PARA_CMD_SYSTEM_VIDEO_ENC_PARAM_OFFSET	parameter_offset(PARA_CMD_SYSTEM_VIDEO_ENC_PARAM )

#define	 PARA_CMD_SYSTEM_STORAGE_SIZE_PARAM_OFFSET	parameter_offset(PARA_CMD_SYSTEM_STORAGE_SIZE_PARAM )

#define	 PARA_CMD_SYSTEM_NET_PARAM_OFFSET	parameter_offset(PARA_CMD_SYSTEM_NET_PARAM )

#define	 PARA_CMD_SYSTEM_ALERT_DI_PARAM_OFFSET	parameter_offset(PARA_CMD_SYSTEM_ALERT_DI_PARAM )

#define	 PARA_CMD_SYSTEM_ALERT_VLOSS_PARAM_OFFSET	parameter_offset(PARA_CMD_SYSTEM_ALERT_VLOSS_PARAM )

#define	 PARA_CMD_SYSTEM_ALERT_MOTION_PARAM_OFFSET	parameter_offset(PARA_CMD_SYSTEM_ALERT_MOTION_PARAM )

#define	 PARA_CMD_SYSTEM_STORAGE_SETTING_PARAM_OFFSET	parameter_offset(PARA_CMD_SYSTEM_STORAGE_SETTING_PARAM )

#define	 PARA_CMD_SYSTEM_AD_SETTING_PARAM_OFFSET	parameter_offset(PARA_CMD_SYSTEM_AD_SETTING_PARAM )

#define	 PARA_CMD_SYSTEM_TIME_SETTING_PARAM_OFFSET	parameter_offset(PARA_CMD_SYSTEM_TIME_SETTING_PARAM )

#define	 PARA_CMD_SYSTEM_RS485_SETTING_PARAM_OFFSET	parameter_offset(PARA_CMD_SYSTEM_RS485_SETTING_PARAM )

#define	 PARA_CMD_SYSTEM_PTZ_AREA_SETTING_PARAM_OFFSET	parameter_offset(PARA_CMD_SYSTEM_PTZ_AREA_SETTING_PARAM )

#define	 PARA_CMD_SYSTEM_ALG_SETTING_PARAM_OFFSET	parameter_offset(PARA_CMD_SYSTEM_ALG_SETTING_PARAM )

#define	 PARA_CMD_SYSTEM_WORKING_STATE_PARAM_OFFSET	parameter_offset(PARA_CMD_SYSTEM_WORKING_STATE_PARAM )

#define	 PARA_CMD_SYSTEM_END_PARAM_OFFSET	parameter_offset(PARA_CMD_SYSTEM_END_PARAM )

#define	 PARA_CMD_SYSTEM_RUNTIME_DEVICE_STATUS_OFFSET	parameter_offset(PARA_CMD_SYSTEM_RUNTIME_DEVICE_STATUS )

#define	 PARA_CMD_SYSTEM_RUNTIME_DEVICE_VERSION_OFFSET	parameter_offset(PARA_CMD_SYSTEM_RUNTIME_DEVICE_VERSION )


#ifdef __cplusplus
}
#endif

#endif // _PARAMETER_OFFSET_AUTOGEN_



