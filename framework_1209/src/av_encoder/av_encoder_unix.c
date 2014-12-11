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

		case PARACMD_PTZ_PTZ_START_PRESET_ACK:
			if ( phandle->current_state == AVENC_ALARM_BEGIN_DEAL)
			{
				phandle->current_state = AVENC_ALARM_JPEG;
			}
			else
			{
			}
			break;

		case PARACMD_PTZ_PTZ_START_SETPRESET_ACK:
			if (phandle->current_state == AVENC_ALARM_WAIT)  //added 2012-1-6 zsx
			{
				phandle->current_state = AVENC_IDLE;
			}
			else
			{
			}
			break;

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
#if 0
			status = ALG_BITRATE(phandle->pvideoalg_handle, bitrate);
			if(status != OSA_SOK)
			{
				SYS_ERROR("Set Bitrate failed. \r\n");
				return succeed_type_failed;
			}
#endif
			break;        
		case PARACMD_BOA_BOATOAVENC_PARA_SETKEYFRAMEINTERVAL:

			pinfo = (unsigned int *)pcmd->cmdinfo;
			if((*pinfo == phandle->videoencparam.keyFrameInterval) || (*pinfo <= 0))
			{
				break;
			}	
			keyFrameInterval = *pinfo;
			phandle->videoencparam.keyFrameInterval = keyFrameInterval;
			printf("boa to enc set keyFrameInterval ->%d.\r\n",keyFrameInterval);
#if 0
			status = ALG_KEYFRAMEINTERVAL(phandle->pvideoalg_handle, keyFrameInterval);
			if(status != OSA_SOK)
			{
				SYS_ERROR("Set KeyFrameInterval failed. \r\n");
				return succeed_type_failed;
			}
#endif
			break;   						
		case PARACMD_BOA_BOATOAVENC_PARA_SETVIDEOENCMODE:

			pinfo = (VIDEO_ENC_MODE *)pcmd->cmdinfo;
			if(*pinfo == VIDEO_ENC_JPEG)
			{
				printf("do not support VIDEO_ENC_JPEG.\r\n"); 
			}
			break;  		   		
		case PARACMD_BOA_BOATOAVENC_PARA_SETVIDEOINPUTMODE: 

			pinfo = (VIDEO_INPUT_MODE *)pcmd->cmdinfo; 
			if(*pinfo == phandle->videoencparam.video_input_mode)
			{
				break; 
			}
			printf("boa to enc change VIDEOINPUTMODE(N/P).\r\n");

			break; 
		case PARACMD_BOA_BOATOAVENC_PARA_SETVIDEOENCRESOLUTION:

			pinfo = (VIDEO_ENC_RESOLUTION *)pcmd->cmdinfo; 
			if(*pinfo == phandle->videoencparam.video_enc_resolution)
			{
				break;			
			}
			printf("boa to enc change VIDEOENCRESOLUTION(D1/CIF).\r\n");

			break; 				
		case PARACMD_BOA_BOATOAVENC_PARA_SETBRIGHTNESS:

			pinfo = (unsigned int *)pcmd->cmdinfo;
			if((*pinfo == phandle->videoadsettingparam.ad_bright) || (*pinfo < 0x00) || (*pinfo > 0xff))
			{ 
				break;
			}
			bright= *pinfo;
			phandle->videoadsettingparam.ad_bright = bright;
			printf("boa to enc set bright ->%d.\r\n",bright);

			break;        

		case PARACMD_BOA_BOATOAVENC_PARA_SETSATURATION:

			pinfo = (unsigned int *)pcmd->cmdinfo;
			if((*pinfo == phandle->videoadsettingparam.ad_saturation) || (*pinfo < 0x00) || (*pinfo > 0xff))
			{ 
				break;
			}
			saturation = *pinfo;
			phandle->videoadsettingparam.ad_saturation = saturation;
			printf("boa to enc set saturation->%d.\r\n",saturation);

			break; 
		case PARACMD_BOA_BOATOAVENC_PARA_SETHUE:

			pinfo = (unsigned int *)pcmd->cmdinfo;
			if(*pinfo == phandle->videoadsettingparam.ad_hue)
			{ 
				break;
			}
			hue = *pinfo;
			phandle->videoadsettingparam.ad_hue = hue;
			printf("boa to enc set hue ->%d.\r\n",hue);

			break; 
		case PARACMD_BOA_BOATOAVENC_PARA_SETCONTRAST:

			pinfo = (unsigned int *)pcmd->cmdinfo;
			if((*pinfo == phandle->videoadsettingparam.ad_contrast) || (*pinfo < 0) || (*pinfo > 0xcf))
			{ 
				break;
			}
			contrast = *pinfo;
			phandle->videoadsettingparam.ad_contrast = contrast;
			printf("boa to enc set contrast ->%d.\r\n",contrast);

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
	SYS_INFO("Rcv Event from %s in av_encoder.\r\n", remotename); 

	//2. deal the receiving event.
	av_encoder_unix_deal(phandle, &unix_rcv_cmd);
	return succeed_type_succeed;
}
