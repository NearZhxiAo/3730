#include "av_encoder_private.h"
extern succeed_type av_encoder_exit(AV_ENCODER_PRIVATE_HANDLE *phandle);

static succeed_type av_encoder_unix_deal( AV_ENCODER_PRIVATE_HANDLE *phandle, UNIX_CMD_STRUCT *pcmd)
{
	int status;
	int framerate = 0x00;
	unsigned int *pinfo = NULL;
	unsigned int bitrate = 0x00;					
	unsigned int keyFrameInterval = 0x00;
	unsigned int bright = 0x00;
	unsigned int hue = 0x00;
	unsigned int saturation = 0x00;
	unsigned int contrast = 0x00;

	switch(pcmd->cmd)	//根据不同的交互命令进行不同的处理
	{
		case PARACMD_STORAGE_STORAGE_TO_AVENC_DEC_FRAME:
			{
				phandle->current_state = AVENC_DEC;
				break;
			}
		case PARACMD_SERIAL_MANAGE_SERIAL_TO_AVENC_INPUT_INFRARED:
		case PARACMD_SERIAL_MANAGE_SERIAL_TO_AVENC_INPUT_CCD:
		case PARACMD_QT_QT_TO_AVENC_INPUT_INFRARED: //需要将输入状态作为共享内存的参数放到共享内存中，QT和AVENC共享此参数
		case PARACMD_QT_QT_TO_AVENC_INPUT_CCD:
			{
				phandle->cam_input_switch = 1;
				SYS_INFO("msg from qt infrared...\r\n");
				break;
			}
		case PARACMD_QT_QT_TO_AVENC_JPEG_PIC:
			{
				phandle->Is_jpeg_pic = 1;

				break;
			}
		case PARACMD_QT_QT_TO_AVENC_START_ENC:
			{
				phandle->current_state = AVENC_ENC;
				break;
			}
		case PARACMD_QT_QT_TO_AVENC_STOP_ENC:
			{
				phandle->current_state = AVENC_IDLE;
				break;
			}

#if 0
		case PARACMD_BOA_BOATOAVENC_PARA_SETFRAMERATE:

			pinfo = (VIDEO_ENC_FRAMERATE *)pcmd->cmdinfo;
			if(*pinfo == phandle->videoencparam.video_enc_framerate)
			{ 
				break;
			} 
			switch(*pinfo)
			{
				case VIDEO_ENC_30FPS:
					if(phandle->videoencparam.video_input_mode == VIDEO_INPUT_PAL)
					{
						framerate = 25;
						phandle->videoencparam.video_enc_framerate = VIDEO_ENC_25FPS;    
					}	
					else
					{
						framerate = 30;
						phandle->videoencparam.video_enc_framerate = VIDEO_ENC_30FPS;  
					}					
					break;
				case VIDEO_ENC_25FPS:
					framerate = 25;
					phandle->videoencparam.video_enc_framerate = VIDEO_ENC_25FPS;
					break;
				case VIDEO_ENC_15FPS:
					framerate = 15;
					phandle->videoencparam.video_enc_framerate = VIDEO_ENC_15FPS;
					break;
				case VIDEO_ENC_7FPS:
					framerate = 7;
					phandle->videoencparam.video_enc_framerate = VIDEO_ENC_7FPS;
					break;
				default:
					break;
			}
			printf("boa to enc set framrate ->%d.\r\n",framerate);

#if 0
			status = ALG_FRAMERATE(phandle->pvideoalg_handle, framerate*1000);
			if(status != OSA_SOK)
			{
				SYS_ERROR("Set Framerate failed .\r\n");
				return succeed_type_failed;
			}
#endif
			break;
		case PARACMD_BOA_BOATOAVENC_PARA_SETBITRATE:

			pinfo = (unsigned int *)pcmd->cmdinfo;
			if((*pinfo == phandle->videoencparam.video_enc_bitrate) || (*pinfo <= 0))
			{ 
				break;
			}
			bitrate = *pinfo;
			phandle->videoencparam.video_enc_bitrate = bitrate;
			printf("boa to enc set bitrate ->%d.\r\n",bitrate);
#endif
#if 0
			status = ALG_BITRATE(phandle->pvideoalg_handle, bitrate);
			if(status != OSA_SOK)
			{
				SYS_ERROR("Set Bitrate failed. \r\n");
				return succeed_type_failed;
			}
#endif
			break;        
		default:
			break;
	}
	return status; 
}

succeed_type av_encoder_unix(AV_ENCODER_PRIVATE_HANDLE *phandle)
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
	SYS_INFO("Rcv Event from %s in av_encoder.\r\n", remotename); //??

	//2. deal the receiving event.
	av_encoder_unix_deal(phandle, &unix_rcv_cmd);
	return succeed_type_succeed;
}
