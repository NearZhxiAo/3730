#ifndef _PARAMETER_AUTOGEN_
#define _PARAMETER_AUTOGEN_
/*	This file is a autogenerate file .
 *	DONOT EDIT THIS FILE!!
 *  Edit parameter_internal.ddf and parameter.tpl instead
 */
#ifdef __cplusplus
extern "C"{
#endif
#include <stdio.h>
#include <stdlib.h>

enum unxi_filename_type {
	
	UNIX_FILENAME_SYSTEM
	,
	UNIX_FILENAME_STORAGE
	,
	UNIX_FILENAME_TEST_TWO_PROCESS
	
	,UNIX_FILENAME_END
};



#define UNIX_SOCKET_SYSTEM  "/var/unix_socket_SYSTEM"

#define UNIX_SOCKET_STORAGE  "/var/unix_socket_STORAGE"

#define UNIX_SOCKET_TEST_TWO_PROCESS  "/var/unix_socket_TEST_TWO_PROCESS"

#define MAX_UNIXFILENAME_LEN	64
static char find_unix_name[UNIX_FILENAME_END][MAX_UNIXFILENAME_LEN] = {
	UNIX_SOCKET_SYSTEM
	,
	UNIX_SOCKET_STORAGE
	,
	UNIX_SOCKET_TEST_TWO_PROCESS
	
};

enum para_cat_type {
	
	PARA_CATEGORY_SYSTEM
	,
	PARA_CATEGORY_STORAGE
	,
	PARA_CATEGORY_TEST_TWO_PROCESS
	
};
#define MAX_CATEGORY_BIT		(6)
#define MAX_CATEGORY_PARA_NUM	(1<<(MAX_CATEGORY_BIT))
enum para_cmd_indicator_type {
	PARA_CMD_SYSTEM_GBSHM_MUTEX = (PARA_CATEGORY_SYSTEM << MAX_CATEGORY_BIT) + 1,
	PARA_CMD_SYSTEM_TEST_VAR = (PARA_CATEGORY_SYSTEM << MAX_CATEGORY_BIT) + 2,
	PARA_CMD_SYSTEM_VIDEO_ENC_PARAM = (PARA_CATEGORY_SYSTEM << MAX_CATEGORY_BIT) + 3,
	PARA_CMD_SYSTEM_STORAGE_SIZE_PARAM = (PARA_CATEGORY_SYSTEM << MAX_CATEGORY_BIT) + 4,
	PARA_CMD_SYSTEM_NET_PARAM = (PARA_CATEGORY_SYSTEM << MAX_CATEGORY_BIT) + 5,
	PARA_CMD_SYSTEM_ALERT_DI_PARAM = (PARA_CATEGORY_SYSTEM << MAX_CATEGORY_BIT) + 6,
	PARA_CMD_SYSTEM_ALERT_VLOSS_PARAM = (PARA_CATEGORY_SYSTEM << MAX_CATEGORY_BIT) + 7,
	PARA_CMD_SYSTEM_ALERT_MOTION_PARAM = (PARA_CATEGORY_SYSTEM << MAX_CATEGORY_BIT) + 8,
	PARA_CMD_SYSTEM_STORAGE_SETTING_PARAM = (PARA_CATEGORY_SYSTEM << MAX_CATEGORY_BIT) + 9,
	PARA_CMD_SYSTEM_AD_SETTING_PARAM = (PARA_CATEGORY_SYSTEM << MAX_CATEGORY_BIT) + 10,
	PARA_CMD_SYSTEM_TIME_SETTING_PARAM = (PARA_CATEGORY_SYSTEM << MAX_CATEGORY_BIT) + 11,
	PARA_CMD_SYSTEM_RS485_SETTING_PARAM = (PARA_CATEGORY_SYSTEM << MAX_CATEGORY_BIT) + 12,
	PARA_CMD_SYSTEM_PTZ_AREA_SETTING_PARAM = (PARA_CATEGORY_SYSTEM << MAX_CATEGORY_BIT) + 13,
	PARA_CMD_SYSTEM_ALG_SETTING_PARAM = (PARA_CATEGORY_SYSTEM << MAX_CATEGORY_BIT) + 14,
	PARA_CMD_SYSTEM_WORKING_STATE_PARAM = (PARA_CATEGORY_SYSTEM << MAX_CATEGORY_BIT) + 15,
	PARA_CMD_SYSTEM_END_PARAM = (PARA_CATEGORY_SYSTEM << MAX_CATEGORY_BIT) + 16,
	PARA_CMD_SYSTEM_RUNTIME_DEVICE_STATUS = (PARA_CATEGORY_SYSTEM << MAX_CATEGORY_BIT) + 17,
	PARA_CMD_SYSTEM_RUNTIME_DEVICE_VERSION = (PARA_CATEGORY_SYSTEM << MAX_CATEGORY_BIT) + 18
	,PARA_CMD_END
};


static inline void produce_para_cmd_value(void)
{
	printf("/**********************parameter_value define**********************/\n");
	printf("enum para_cmd_value \n{ \n");
	
	printf("PARA_CMD_SYSTEM_GBSHM_MUTEX = 0x%x,", PARA_CMD_SYSTEM_GBSHM_MUTEX);
	printf("      /* %s */         \n",  "mutex for gbshm");
	
	printf("PARA_CMD_SYSTEM_TEST_VAR = 0x%x,", PARA_CMD_SYSTEM_TEST_VAR);
	printf("      /* %s */         \n",  "mutex for gbshm");
	
	printf("PARA_CMD_SYSTEM_VIDEO_ENC_PARAM = 0x%x,", PARA_CMD_SYSTEM_VIDEO_ENC_PARAM);
	printf("      /* %s */         \n",  "video encoder parameters");
	
	printf("PARA_CMD_SYSTEM_STORAGE_SIZE_PARAM = 0x%x,", PARA_CMD_SYSTEM_STORAGE_SIZE_PARAM);
	printf("      /* %s */         \n",  "sd card storage parameters");
	
	printf("PARA_CMD_SYSTEM_NET_PARAM = 0x%x,", PARA_CMD_SYSTEM_NET_PARAM);
	printf("      /* %s */         \n",  "net parameters");
	
	printf("PARA_CMD_SYSTEM_ALERT_DI_PARAM = 0x%x,", PARA_CMD_SYSTEM_ALERT_DI_PARAM);
	printf("      /* %s */         \n",  "di parameters");
	
	printf("PARA_CMD_SYSTEM_ALERT_VLOSS_PARAM = 0x%x,", PARA_CMD_SYSTEM_ALERT_VLOSS_PARAM);
	printf("      /* %s */         \n",  "vloss parameters");
	
	printf("PARA_CMD_SYSTEM_ALERT_MOTION_PARAM = 0x%x,", PARA_CMD_SYSTEM_ALERT_MOTION_PARAM);
	printf("      /* %s */         \n",  "motion parameters");
	
	printf("PARA_CMD_SYSTEM_STORAGE_SETTING_PARAM = 0x%x,", PARA_CMD_SYSTEM_STORAGE_SETTING_PARAM);
	printf("      /* %s */         \n",  "storage setting parameters");
	
	printf("PARA_CMD_SYSTEM_AD_SETTING_PARAM = 0x%x,", PARA_CMD_SYSTEM_AD_SETTING_PARAM);
	printf("      /* %s */         \n",  "ad setting parameters");
	
	printf("PARA_CMD_SYSTEM_TIME_SETTING_PARAM = 0x%x,", PARA_CMD_SYSTEM_TIME_SETTING_PARAM);
	printf("      /* %s */         \n",  "time setting parameters");
	
	printf("PARA_CMD_SYSTEM_RS485_SETTING_PARAM = 0x%x,", PARA_CMD_SYSTEM_RS485_SETTING_PARAM);
	printf("      /* %s */         \n",  "time setting parameters");
	
	printf("PARA_CMD_SYSTEM_PTZ_AREA_SETTING_PARAM = 0x%x,", PARA_CMD_SYSTEM_PTZ_AREA_SETTING_PARAM);
	printf("      /* %s */         \n",  "ptz area setting parameters");
	
	printf("PARA_CMD_SYSTEM_ALG_SETTING_PARAM = 0x%x,", PARA_CMD_SYSTEM_ALG_SETTING_PARAM);
	printf("      /* %s */         \n",  "ptz area setting parameters");
	
	printf("PARA_CMD_SYSTEM_WORKING_STATE_PARAM = 0x%x,", PARA_CMD_SYSTEM_WORKING_STATE_PARAM);
	printf("      /* %s */         \n",  "system working state parameters");
	
	printf("PARA_CMD_SYSTEM_END_PARAM = 0x%x,", PARA_CMD_SYSTEM_END_PARAM);
	printf("      /* %s */         \n",  "time setting parameters");
	
	printf("PARA_CMD_SYSTEM_RUNTIME_DEVICE_STATUS = 0x%x,", PARA_CMD_SYSTEM_RUNTIME_DEVICE_STATUS);
	printf("      /* %s */         \n",  "runtime device status");
	
	printf("PARA_CMD_SYSTEM_RUNTIME_DEVICE_VERSION = 0x%x,", PARA_CMD_SYSTEM_RUNTIME_DEVICE_VERSION);
	printf("      /* %s */         \n",  "runtime device version information");
	
	printf("};");
}
enum para_cmd_category_base {
	PARA_CMD_SYSTEM_BASE = (PARA_CATEGORY_SYSTEM << MAX_CATEGORY_BIT) + 1
	,
	PARA_CMD_STORAGE_BASE = (PARA_CATEGORY_STORAGE << MAX_CATEGORY_BIT) + 1
	,
	PARA_CMD_TEST_TWO_PROCESS_BASE = (PARA_CATEGORY_TEST_TWO_PROCESS << MAX_CATEGORY_BIT) + 1
	
};

enum para_cmd_category_end {
    PARA_CMD_SYSTEM_END = PARA_CMD_SYSTEM_BASE  + (1<<MAX_CATEGORY_BIT)
    ,
    PARA_CMD_STORAGE_END = PARA_CMD_STORAGE_BASE  + (1<<MAX_CATEGORY_BIT)
    ,
    PARA_CMD_TEST_TWO_PROCESS_END = PARA_CMD_TEST_TWO_PROCESS_BASE  + (1<<MAX_CATEGORY_BIT)
    
};

#define PARA_CMD_BASE_GET(PARA_CMD)			(((PARA_CMD >> MAX_CATEGORY_BIT) << MAX_CATEGORY_BIT) + 1)
#define PARA_CATEGORY_GET(PARA_CMD)			((PARA_CMD >> MAX_CATEGORY_BIT))

#ifdef __cplusplus
}
#endif

#endif // _PARAMETER_AUTOGEN_



