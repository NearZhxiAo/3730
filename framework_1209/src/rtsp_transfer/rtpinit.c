#include<stdlib.h>
#include<time.h>

#include "RTP_HEAD.h"
#include "rtp_ctrl_type.h"
#include "string.h"


char buf[MTU];

/*12 bytes of rtp header init*/
void InitRtpHead(RTP_DATA_HEAD *prtpDataHdr, RTP_HEAD_INFO* prtp_head_info)
{
	unsigned char version,extension;
	unsigned char padding,ccount;
	unsigned char marker,payload;
	// 	unsigned int T,S,width,reserve,height;

	version = 2;
	padding = 0;
	extension = 0;
	ccount = 0; 
	//extension = prtp_head_info->Extension;
	marker = prtp_head_info->Marker; //当一个视频帧结束时，必须填1
	payload = prtp_head_info->Payload;//H264=98

	prtpDataHdr->VPEC = ((version << 6)|(padding << 5)|(extension<<4)|ccount);
	prtpDataHdr->Mrk_type = ((marker<<7) | payload);
  
	prtpDataHdr->Snumber = htons(prtp_head_info->seqnumber);
	prtpDataHdr->Timestamp = htonl(prtp_head_info->timestamp);	
	if((PAYLOAD_H264 == payload) || (PAYLOAD_MPEG4 == payload))
	{
		memcpy(prtpDataHdr->Ssrc, prtp_head_info->SSRC, 4);
    }
	else
	{
		memcpy(prtpDataHdr->Ssrc, prtp_head_info->AUDIO_SSRC, 4);
    }
}


void InitRtpExtenHead_Audio(RTP_Exten_HEAD *prtpExtenHdr, RTP_HEAD_INFO* prtp_head_info)
{
	unsigned char version,extension;
	unsigned char padding,ccount;
	unsigned char marker,payload;
	unsigned short T,S,width,reserve,height;
	unsigned int CodeType_byte1 = 'P';
	unsigned int CodeType_byte2 = 'C';
	unsigned int CodeType_byte3 = 'M';
	unsigned int CodeType_byte4 = 'A';
	version = 2;
	padding = 0;
	extension = 1; //扩展头Falgs 填1；
	ccount = 0;
	marker = prtp_head_info->Marker; //当一个视频帧结束时，必须填1
	//payload = prtp_head_info->Payload; //h264=98
	marker = 1;		//
	payload = PAYLOAD_PCMA; //
	T = 1; //为0表示时戳遵循RTP标准；为1表示时戳的单位为毫秒
	S = 1;  //标准解码器能解码的码流置1
	//width = prtp_head_info->v_width; //图像宽度，以像素为单位.音频填0
	width = 0; //图像宽度，以像素为单位.音频填0
	reserve = 0;
	//height = prtp_head_info->v_height;//图像高度，以像素为单位.音频填0
	height = 0;//图像高度，以像素为单位.音频填0
	prtpExtenHdr->VPEC = ((version << 6)|(padding << 5)|(extension<<4)|ccount);
	prtpExtenHdr->Mrk_type = ((marker<<7) | payload);
	prtpExtenHdr->Snumber = htons(prtp_head_info->seqnumber);
    prtpExtenHdr->Timestamp = htonl(prtp_head_info->timestamp);	
	memcpy(prtpExtenHdr->Ssrc, prtp_head_info->AUDIO_SSRC, 4);
	prtpExtenHdr->Def_by_profile = htons(0x7376);
	prtpExtenHdr->Length = htons(3);
	prtpExtenHdr->TSWid = htons((T<<15) | (S<<14) | width);
	prtpExtenHdr->ResHgt = htons((reserve<<14) | height);
	prtpExtenHdr->Ext_time_stamp = htons(0);
	prtpExtenHdr->FTY_code = htons(0x7376); //缺省
	prtpExtenHdr->CodeType = htonl((CodeType_byte1<<24)|(CodeType_byte2<<16)
			| (CodeType_byte3<<8) | CodeType_byte4);

}

void InitRtpExtenHead(RTP_Exten_HEAD *prtpExtenHdr, RTP_HEAD_INFO* prtp_head_info)
{
	unsigned char version,extension;
	unsigned char padding,ccount;
	unsigned char marker,payload;
	unsigned short T,S,width,reserve,height;
	unsigned int CodeType_byte1 = 'H';
	unsigned int CodeType_byte2 = '2';
	unsigned int CodeType_byte3 = '6';
	unsigned int CodeType_byte4 = '4';
	version = 2;
	padding = 0;
	extension = 1; //扩展头Falgs 填1；
	ccount = 0;
	marker = prtp_head_info->Marker; //当一个视频帧结束时，必须填1
	payload = prtp_head_info->Payload; //h264=98
	T = 0; //为0表示时戳遵循RTP标准；为1表示时戳的单位为毫秒
	S = 1;  //标准解码器能解码的码流置1
	width = prtp_head_info->v_width; //图像宽度，以像素为单位.音频填0
	reserve = 0;
	height = prtp_head_info->v_height;//图像高度，以像素为单位.音频填0
	prtpExtenHdr->VPEC = ((version << 6)|(padding << 5)|(extension<<4)|ccount);
	prtpExtenHdr->Mrk_type = ((marker<<7) | payload);
	prtpExtenHdr->Snumber = htons(prtp_head_info->seqnumber);
    prtpExtenHdr->Timestamp = htonl(prtp_head_info->timestamp);	
	memcpy(prtpExtenHdr->Ssrc, prtp_head_info->SSRC, 4);
	prtpExtenHdr->Def_by_profile = htons(0x7376);
	prtpExtenHdr->Length = htons(3);
	prtpExtenHdr->TSWid = htons((T<<15) | (S<<14) | width);
	prtpExtenHdr->ResHgt = htons((reserve<<14) | height);
	prtpExtenHdr->Ext_time_stamp = htons(0);
	prtpExtenHdr->FTY_code = htons(0x7376); //缺省
	prtpExtenHdr->CodeType = htonl((CodeType_byte1<<24)|(CodeType_byte2<<16)
			| (CodeType_byte3<<8) | CodeType_byte4);

}


char* fillRtpHead(char*databuf, RTP_HEAD_INFO* prtp_head_info, unsigned int* psend_len, unsigned int *offset)
{
	char *ptr = NULL;
#ifndef USE_NEW_AUDIO
	RTP_DATA_HEAD rtpHead;
	InitRtpHead(&rtpHead, prtp_head_info); //prtp_head_info->Payload = 98--H264
#else
	RTP_Exten_HEAD rtpHeadExt;
	InitRtpExtenHead_Audio(&rtpHeadExt, prtp_head_info);
#endif
	
	memset(buf,0,MTU);
	ptr = buf;
	
	if(TCP == prtp_head_info->trans_protocol)
	{
	    unsigned short magic = htons(0x7E7E); //包头标识
#ifndef USE_NEW_AUDIO
	    unsigned short length = htons(sizeof(rtpHead) + (prtp_head_info->data_len)); //数据传输单元长度
#else
	    unsigned short length = htons(sizeof(rtpHeadExt) + (prtp_head_info->data_len)); //数据传输单元长度
#endif
      memcpy(ptr,&magic,sizeof(unsigned short));
	    ptr += sizeof(unsigned short);
	    *psend_len += sizeof(unsigned short);
	    memcpy(ptr,&length,sizeof(unsigned short));
	    ptr += sizeof(unsigned short);
	    *psend_len += sizeof(unsigned short);
	    
	}
#ifndef USE_NEW_AUDIO	
	memcpy(ptr, &rtpHead, sizeof(rtpHead));
	ptr += sizeof(rtpHead);  //填rtp头
	*psend_len += sizeof(rtpHead);
#else
	memcpy(ptr, &rtpHeadExt, sizeof(rtpHeadExt));
	ptr += sizeof(rtpHeadExt);  //填rtp头
	*psend_len += sizeof(rtpHeadExt);
#endif
	*offset = ptr - buf;
	memcpy(ptr, databuf, (prtp_head_info->data_len));        //填数据
	*psend_len += (prtp_head_info->data_len);

	return buf;
}



char* fillRtpExtenHead(char*databuf, RTP_HEAD_INFO* prtp_head_info, unsigned int* psend_len, unsigned int *offset)
{
    RTP_Exten_HEAD rtpExtenHean;
    InitRtpExtenHead(&rtpExtenHean, prtp_head_info);
    char *ptr = NULL;

	  memset(buf,0,MTU);
	  ptr = buf;
	  
	  if(TCP == prtp_head_info->trans_protocol)
	  {
	    unsigned short magic = htons(0x7E7E); //包头标识
	    unsigned short length = htons(sizeof(rtpExtenHean) + (prtp_head_info->data_len)); //数据传输单元长度
	    memcpy(ptr,&magic,sizeof(unsigned short));
	    ptr += sizeof(unsigned short);
	    *psend_len += sizeof(unsigned short);
	    memcpy(ptr,&length,sizeof(unsigned short));
	    ptr += sizeof(unsigned short);
	    *psend_len += sizeof(unsigned short);
	    
	  }
	  
	  memcpy(ptr, &rtpExtenHean, sizeof(rtpExtenHean));
	  ptr += sizeof(rtpExtenHean);  //填rtp头
	  *psend_len += sizeof(rtpExtenHean);	 
	  *offset = ptr - buf; 
	  memcpy(ptr, databuf, prtp_head_info->data_len);        //填数据
	  *psend_len += (prtp_head_info->data_len);  

	return buf;
}


char* fillRtpCtrlHead(RTP_HEAD_INFO* prtp_head_info, unsigned short rtpCtrltype)
{
  RTP_CTRL_HEAD rtpCtrlHead;
  InitRtpHead(&(rtpCtrlHead.rtphead),prtp_head_info);
  rtpCtrlHead.CtrlCmdType = rtpCtrltype;
  rtpCtrlHead.CmdParaLen = 16;
  //rtpCtrlHead.CmdPara = {0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0};

  char *ptr = NULL;
  unsigned short magic = htons(0x7E7E); //包头标识
	unsigned short length = htons(sizeof(rtpCtrlHead) + RTP_Msg_Len); //数据传输单元长度
	memset(buf,0,MTU);
	
	ptr = buf;
	memcpy(ptr,&magic,sizeof(unsigned short));
	ptr += sizeof(unsigned short);
	memcpy(ptr,&length,sizeof(unsigned short));
	ptr += sizeof(unsigned short);
	memcpy(ptr, &rtpCtrlHead, sizeof(rtpCtrlHead));//填rtpd控制头
	return buf;

}
