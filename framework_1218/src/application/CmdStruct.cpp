#include "CmdStruct.h"
static int frame_index = 1;

int CONSTRUCT_CMD(int cmd, char *psend, int sendlen, CMD_TRANSFER *pout)
{
	if (NULL == pout)
	{
		return 0;
	}
    pout->cmd_direction = (CMD_DIR)CMD_DIR_SERVERTOCLIENT; 
	
	pout->magicnum_start = CMD_MAGIC_START;
	pout->cmd_num = cmd;
	memset(pout->cmd_contents, 0, sizeof(pout->cmd_contents));
	if(psend)
	{
		memcpy(pout->cmd_contents, psend, sendlen);
	}
	pout->contents_len = sendlen;
	pout->magicnum_end = CMD_MAGIC_END;
	return 1;
}


int DECONSTRUCT_CMD(CMD_TRANSFER *pinput, char *ptrout, int *ptrcmd)
{
	if (NULL == pinput)
	{
		return 0;
	}
	else if ((CMD_MAGIC_START != pinput->magicnum_start) || (CMD_MAGIC_END != pinput->magicnum_end))
	{
		return 0;
	}
	else
	{
		if (ptrcmd)
		{
			*ptrcmd = pinput->cmd_num;
		}
		if(ptrout)
		{
			memcpy(ptrout, pinput->cmd_contents, pinput->contents_len);
		}
	}
	return 1;
}

int CONSTRUCT_DATA(char *psend, int sendlen, H264_ENC_DATA *pout, int totallen, int packetindex)
{
	if (NULL == pout)
	{
		return 0;
	}
	pout->startmagic = DATA_MAGIC_START;
	if(PACKET_INDEX_LAST == packetindex)
	{
		frame_index++;
	}
	pout->frame_index = frame_index;
	pout->packet_index = packetindex;
	pout->frame_len = totallen;
	pout->packet_len = sendlen;
	memset(pout->packet_contents, 0, sizeof(pout->packet_contents));
	if(psend)
	{
		memcpy(pout->packet_contents, psend, sendlen);
	}
	pout->endmagic = DATA_MAGIC_END;
    
	return 1;
}

int DESTRUCT_DATA( H264_ENC_DATA *pinput, char *pout, int *frame_index, int *packet_index, int *packet_size, int *frame_size)
{
	if (NULL == pinput)
	{
		return 0;
	}
	else if ((DATA_MAGIC_START != pinput->startmagic) || (DATA_MAGIC_END != pinput->endmagic))
	{
		return 0;
	}
	else
	{
		if(frame_index)
		{
			*frame_index = pinput->frame_index;
		}
		if (packet_index)
		{
			*packet_index = pinput->packet_index;
		}
		if (packet_size)
		{
			*packet_size = pinput->packet_len;
		}
		if (pout)
		{
			memcpy(pout, pinput->packet_contents, *packet_size);
		}
		if(frame_size)
		{
			*frame_size = pinput->frame_len;
		}
	}
	return 1;
}

