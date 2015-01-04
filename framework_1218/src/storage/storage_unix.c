/*******************************************************************************
 * COPYRIGHT
 ********************************************************************************
/*
 * =====================================================================================
 *
 *       Filename:  storage_unix.c
 *
 *    Description:
 *
 *        Version:
 *        Created:
 *       Revision:
 *       Compiler:
 *
 *         Author:
 *        Company:
 *
 * =====================================================================================
 */
/********************************* Source Code Start ********************************/
#include "storage_private.h"

LIBVSSHM_UINT_DATA_I storage_uint_data;

#if 0
static succeed_type storage_unix_alert_deal(STORAGE_PRIVATE_HANDLE *phandle, UNIX_CMD_STRUCT *pcmd)
{
	alert_event alert_value;
	memcpy(&alert_value, pcmd->cmdinfo, pcmd->cmdlen);
	SYS_INFO("storage_unix_alert_deal recv cmd event_type = %x", alert_value.event_type);
	switch(alert_value.event_type)
	{
		case ALERT_EVENT_TYPE_START:
			break;
		case ALERT_EVENT_TYPE_DI:
			phandle->enalert_event_type=ALERT_EVENT_TYPE_DI;
			break;
		case ALERT_EVENT_TYPE_TIME:
			{
				storage_setting_param storage_setting;
				GBSHM_PARA_GET(phandle->pgbshm_obj, PARA_CMD_SYSTEM_STORAGE_SETTING_PARAM,\
						&storage_setting, sizeof(storage_setting));
				phandle->storage_store_duration = ((long long)storage_setting.storage_pre_record_time / 60) * 100;
				SYS_WARNING("Storage_duration time Changed!!duration = %lld\r\n",\
						phandle->storage_store_duration);
				break;
			}
		case ALERT_EVENT_TYPE_END:
			break;
		default:
			phandle->enalert_event_type=ALERT_EVENT_TYPE_END;
			break;
	}
	return succeed_type_succeed;

}
static succeed_type storage_unix_avenc_buffer_new_msg(STORAGE_PRIVATE_HANDLE *phandle,\
		UNIX_CMD_STRUCT *pcmd)
{
	succeed_type iret = succeed_type_failed;
	buffer_new_msg *pstorage_buff = NULL;

	pstorage_buff = (buffer_new_msg*)(pcmd->cmdinfo);
	memset(&storage_uint_data, 0x00, sizeof(storage_uint_data));
	iret = VSSHM_GET_DATA(phandle->pvsshm_obj, pstorage_buff->buffer_id, (void *)&storage_uint_data);
	if(iret != succeed_type_succeed)
	{
		SYS_ERROR("VSSHM_GET failed!! \r\n");
	}
	else
	{
		VSSHM_GET_COMPLETE(phandle->pvsshm_obj, pstorage_buff->buffer_id);
		//	phandle->pfile_name = "20110402010000000";

		//	SYS_INFO("data name = %s\r\n",phandle->afile_name);
#if 1 
		if( 1 == phandle->sdcard_checked_if)
		{
			pthread_mutex_lock(&(phandle->g_mutex_t));
			phandle->pstorage_video_data = &storage_uint_data;
			phandle->IsStorageData_Available = 1;
			pthread_mutex_unlock(&(phandle->g_mutex_t));
		//	if(AddRecIntoDataFile(phandle,&storage_uint_data) != succeed_type_succeed)
		//	{
		//		SYS_ERROR("\n\r---------AddRecIntoDataFile failed---------\r\n");
		//	}
		}
#endif
	}
	SYS_INFO("VSSHM_GET buffer_id[%d]!! \r\n",pstorage_buff->buffer_id);
	return succeed_type_succeed;

}
static succeed_type storage_unix_avenc_audio_buffer_new_msg(STORAGE_PRIVATE_HANDLE *phandle,\
		UNIX_CMD_STRUCT *pcmd)
{
	succeed_type iret = succeed_type_failed;
	audio_buffer_new_msg *pstorage_buff = NULL;

	pstorage_buff = (audio_buffer_new_msg*)(pcmd->cmdinfo);
	memset(&storage_uint_data, 0x00, sizeof(storage_uint_data));
	iret = ASSHM_GET_DATA(phandle->passhm_obj, pstorage_buff->buffer_id,(void *)&storage_uint_data);
	if(iret != succeed_type_succeed)
	{
		SYS_ERROR("VSSHM_GET failed!! \r\n");
	}
	else
	{
		ASSHM_GET_COMPLETE(phandle->passhm_obj, pstorage_buff->buffer_id);
		//	phandle->pfile_name = "20110402010000000";

		//	SYS_INFO("data name = %s\r\n",phandle->afile_name);
#if 0
		if(AddRecIntoDataFile(phandle,&storage_uint_data) != succeed_type_succeed)
		{
			SYS_ERROR("\n\r---------AddRecIntoDataFile failed---------\r\n");
		}
#endif
	}
	SYS_INFO("ASSHM_GET buffer_id[%d]!! \r\n",pstorage_buff->buffer_id);
	return succeed_type_succeed;

}
#endif
static succeed_type storage_unix_clientpc_record_start_req_deal(STORAGE_PRIVATE_HANDLE *phandle, UNIX_CMD_STRUCT *pcmd)
{


	return succeed_type_succeed;

}
static succeed_type storage_unix_clientpc_record_stop_req_deal(STORAGE_PRIVATE_HANDLE *phandle, UNIX_CMD_STRUCT *pcmd)
{

	return succeed_type_succeed;

}
static succeed_type storage_unix_clientpc_record_delete_file_deal(STORAGE_PRIVATE_HANDLE *phandle, UNIX_CMD_STRUCT *pcmd)
{

	return succeed_type_succeed;

}
static succeed_type storage_unix_clientpc_record_lock_file_deal(STORAGE_PRIVATE_HANDLE *phandle, UNIX_CMD_STRUCT *pcmd)
{

	return succeed_type_succeed;

}

static succeed_type storage_unix_clientpc_file_list_deal(STORAGE_PRIVATE_HANDLE *phandle, UNIX_CMD_STRUCT *pcmd)
{

	return succeed_type_succeed;
}
static succeed_type storage_unix_clientpc_to_clean_file_deal(STORAGE_PRIVATE_HANDLE *phandle, UNIX_CMD_STRUCT *pcmd)
{

	return succeed_type_succeed;
}
static succeed_type storage_unix_avenc_jpeg_buffer_new_msg_deal(STORAGE_PRIVATE_HANDLE *phandle, UNIX_CMD_STRUCT *pcmd)
{

	succeed_type iret = succeed_type_failed;
	avenc_to_storage_jpeg_buffer_new_msg *pstorage_buff = NULL;
	SYS_INFO("get a unix msg\r\n");

	pstorage_buff = (avenc_to_storage_jpeg_buffer_new_msg*)(pcmd->cmdinfo);
//	memset(&storage_uint_data, 0x00, sizeof(storage_uint_data));
	// do storage jpeg pic
	//pthread_mutex_lock(&(phandle->g_mutex_t));
	phandle->jpeg_buffer_id = pstorage_buff->buffer_id;
	phandle->IsJpegStore = 1;
	//pthread_mutex_unlock(&(phandle->g_mutex_t));

	return succeed_type_succeed;
}
static succeed_type storage_unix_avenc_video_buffer_new_msg_deal(STORAGE_PRIVATE_HANDLE *phandle, UNIX_CMD_STRUCT *pcmd)
{

	succeed_type iret = succeed_type_failed;
	avenc_to_storage_video_buffer_new_msg *pstorage_buff = NULL;

	pstorage_buff = (avenc_to_storage_video_buffer_new_msg*)(pcmd->cmdinfo);
	memset(&storage_uint_data, 0x00, sizeof(storage_uint_data));
	iret = VSSHM_GET_DATA(phandle->pvsshm_obj, pstorage_buff->buffer_id, (void *)&storage_uint_data);
	if(iret != succeed_type_succeed)
	{
		SYS_ERROR("VSSHM_GET failed!! \r\n");
	}
	else
	{
//		VSSHM_GET_COMPLETE(phandle->pvsshm_obj, pstorage_buff->buffer_id);
		//	phandle->pfile_name = "20110402010000000";

		//	SYS_INFO("data name = %s\r\n",phandle->afile_name);
#if 1 
		if( 1 == phandle->sdcard_checked_if)
		{
			pthread_mutex_lock(&(phandle->g_mutex_t));
			phandle->pstorage_video_data = &storage_uint_data;
			phandle->IsStorageData_Available = 1;
			pthread_mutex_unlock(&(phandle->g_mutex_t));
			//	if(AddRecIntoDataFile(phandle,&storage_uint_data) != succeed_type_succeed)
			//	{
			//		SYS_ERROR("\n\r---------AddRecIntoDataFile failed---------\r\n");
			//	}
		}
#endif
	}
	SYS_INFO("VSSHM_GET buffer_id[%d]!! \r\n",pstorage_buff->buffer_id);
	return succeed_type_succeed;
}

static succeed_type storage_unix_avenc_viddec_frame_ack_deal(STORAGE_PRIVATE_HANDLE *phandle, UNIX_CMD_STRUCT *pcmd)
{


	return succeed_type_succeed;
}

static succeed_type storage_unix_avenc_start_storage_deal(STORAGE_PRIVATE_HANDLE *phandle, UNIX_CMD_STRUCT *pcmd)
{

	return succeed_type_succeed;
}

static succeed_type storage_unix_avenc_stop_storage_deal(STORAGE_PRIVATE_HANDLE *phandle, UNIX_CMD_STRUCT *pcmd)
{
	phandle->Is_encode_stop = 0;
	phandle->IsStorageData_Available = 0;
	return succeed_type_succeed;
}

static succeed_type storage_unix_qt_start_loopback_deal(STORAGE_PRIVATE_HANDLE *phandle, UNIX_CMD_STRUCT *pcmd)
{

	return succeed_type_succeed;
}

static succeed_type storage_unix_qt_stop_loopback_deal(STORAGE_PRIVATE_HANDLE *phandle, UNIX_CMD_STRUCT *pcmd)
{

	return succeed_type_succeed;
}

static succeed_type storage_unix_qt_check_file_list_deal(STORAGE_PRIVATE_HANDLE *phandle, UNIX_CMD_STRUCT *pcmd)
{


	return succeed_type_succeed;
}

static succeed_type storage_unix_qt_pause_loopback_deal(STORAGE_PRIVATE_HANDLE *phandle, UNIX_CMD_STRUCT *pcmd)
{

	return succeed_type_succeed;
}

static succeed_type storage_unix_deal( STORAGE_PRIVATE_HANDLE *phandle, UNIX_CMD_STRUCT *pcmd)
{
	if(NULL == phandle || NULL == pcmd)
	{
		SYS_ERROR("NULL == phandle || NULL == pcmd");
	}

	switch(pcmd->cmd)
	{
		case PARACMD_CLIENTPC_CLIENTPC_TO_STORAGE_FILE_LIST:
			{
				storage_unix_clientpc_file_list_deal(phandle, pcmd);

				break;
			}
		case	PARACMD_CLIENTPC_CLIENTPC_TO_STORAGE_CLEAN:
			{

				storage_unix_clientpc_to_clean_file_deal(phandle, pcmd);
				break;
			}

		case	PARACMD_AVENC_AVENC_TO_STORAGE_JPEG_BUFFER_NEW_MSG:
			{
				storage_unix_avenc_jpeg_buffer_new_msg_deal(phandle, pcmd);
				break;
			}
		case	PARACMD_AVENC_AVENC_TO_STORAGE_VIDEO_BUFFER_NEW_MSG :
			{
				storage_unix_avenc_video_buffer_new_msg_deal(phandle, pcmd);

				break;
			}
		case	PARACMD_AVENC_AVENC_TO_STORAGE_VIDDEC_FRAME_ACK :
			{
				storage_unix_avenc_viddec_frame_ack_deal(phandle, pcmd);

				break;
			}
		case	PARACMD_AVENC_AVENC_TO_STORAGE_START_STORAGE :
			{
				storage_unix_avenc_start_storage_deal(phandle, pcmd);

				break;
			}
		case	PARACMD_AVENC_AVENC_TO_STORAGE_STOP_STORAGE :
			{
				storage_unix_avenc_stop_storage_deal(phandle, pcmd);

				break;
			}
		case    PARACMD_QT_QT_TO_STORAGE_START_LOOPBACK:
			{
				storage_unix_qt_start_loopback_deal(phandle, pcmd);

				break;
			}
		case	PARACMD_QT_QT_TO_STORAGE_STOP_LOOPBACK:
			{
				storage_unix_qt_stop_loopback_deal(phandle, pcmd);

				break;
			}
		case	PARACMD_QT_QT_TO_STORAGE_CHECK_FILE_LIST:
			{
				storage_unix_qt_check_file_list_deal(phandle, pcmd);

				break;
			}
		case	PARACMD_QT_QT_TO_STORAGE_PAUSE_LOOPBACK:
			{
				storage_unix_qt_pause_loopback_deal(phandle, pcmd);

				break;
			}

		default:
			break;
	}
	return succeed_type_succeed;
}

succeed_type storage_unix(STORAGE_PRIVATE_HANDLE *phandle)
{
	unsigned char remotename[128];
	UNIX_CMD_STRUCT unix_rcv_cmd;

	memset(remotename, 0x00, 128);
	memset(&unix_rcv_cmd, 0x00, sizeof(UNIX_CMD_STRUCT));

	//1. recv the unix event
	if ( UNIX_RECV( phandle->unix_fd, remotename, &unix_rcv_cmd) == succeed_type_failed )
	{
		SYS_ERROR("UNIX RECV failed.\r\n");
		return succeed_type_failed;
	}
	//SYS_INFO("Rcv Event from %s in storage.\r\n", remotename);

	//2. deal the receiving event.
	storage_unix_deal(phandle, &unix_rcv_cmd);
	return succeed_type_succeed;
}

/********************************* Source Code End ********************************/
