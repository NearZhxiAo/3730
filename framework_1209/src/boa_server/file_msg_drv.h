/* ===========================================================================
* @path $(IPNCPATH)\include
*
* @desc
* .
* Copyright (c) Appro Photoelectron Inc.  2008
*
* Use of this software is controlled by the terms and conditions found
* in the license agreement under which this software has been supplied
*
* =========================================================================== */
/**
* @file file_msg_drv.h
* @brief File message driver
*/
#ifndef _FILE_MSG_DRV_H_
#define _FILE_MSG_DRV_H_
#include <Msg_Def.h>
#include <sys_env_type.h>
/**
* @defgroup FILE_MSG_DRV File message driver
* @brief APIs to control file manager.

* This is the communication interface of file manager. Every process needs a
*  message ID (which is defined in @ref File_Msg_Def.h) to communicate with
* file manager. More detail please refer to (@ref FILE_MSG_DRV_HOW).
* @{
*/
/* This function should be called at process innitial !!! */
int InitFileMsgDrv(int key,int iProcId);
void CleanupFileMsgDrv();
/* API */
void SendFileQuitCmd();
SysInfo *GetSysInfo();
int GetSysLog(int nPageNum, int nItemNum, LogEntry_t * pBuf);
int fSetNetMask(struct in_addr netmask);
int fSetIp(struct in_addr ip);
int fSetHttpPort(unsigned short port);
int fSetTitle(void *buf, int len);
int fSetRegUsr(void *buf, int len);
int fSetOSDText(void *buf, int len);
int fSetGateway(struct in_addr gateway);
int fSetDhcpEnable(int enable);
int fSetFtpFqdn(void *buf, int len);
int fSetFtpPort(unsigned short port);
int fSetFtpUsername(void *buf, int len);
int fSetFtpPassword(void *buf, int len);
int fSetFtpFoldname(void *buf, int len);
int fSetFtpImageacount(int imageacount);
int fSetFtpPid(int pid);
int fSetSmtpServerIp(void * buf, int len);
int fSetSmtpAuthentication(unsigned char authentication);
int fSetSmtpUsername(void * buf, int len);
int fSetSmtpPassword(void * buf, int len);
int fSetSmtpSenderEmail(void * buf, int len);
int fSetSmtpReceiverEmail(void * buf, int len);
int fSetSmtpCC(void * buf, int len);
int fSetSmtpSubject(void * buf, int len);
int fSetSmtpText(void * buf, int len);
int fSetSmtpAttachments(unsigned char attachments);
int fSetSmtpView(unsigned char view);
int fSetDns(struct in_addr ip);
int fSetSntpServer(void *buf, int len);
int fSetTimeFormat(unsigned char value);
int fSetDaylightTime(unsigned char value);
int fSetTimeZone(unsigned char value);
int fSetCamDayNight(unsigned char value);
int fSetCamWhiteBalance(unsigned char value);
int fSetCamBacklight(unsigned char value);
int fSetCamBrightness(unsigned char value);
int fSetCamContrast(unsigned char value);
int fSetCamSaturation(unsigned char value);
int fSetCamSharpness(unsigned char value);
int fSetJpegQuality(unsigned char value);
int fSetRotation(unsigned char value);
int fSetMirror(unsigned char value);
int fSetAdvMode(unsigned char value);
int fSetM41SFeature(unsigned char value);
int fSetM42SFeature(unsigned char value);
int fSetJPGSFeature(unsigned char value);
int fSetFaceDetect(unsigned char value);
int fSetDemoCfg(unsigned char value);
int fSetOSDWin(unsigned char value);
int fSetHistogram(unsigned char value);
int fSetGBCE(unsigned char value);
int fSetOSDWinNum(unsigned char value);
int fSetOSDStream(unsigned char value);
int fSetVideoMode(unsigned char value);
int fSetVideoCodecMode(unsigned char value);
int fSetVideoCodecRes(unsigned char value);
int fSetImageFormat(unsigned char value);
int fSetResolution(unsigned char value);
int fSetMPEG4Res(unsigned char value);
int fSetMPEG42Res(unsigned char value);
int fSetSchedule(int index,Schedule_t* pSchedule);
int fSetLostAlarm(unsigned char value);
int fSetSDAlarmEnable(unsigned char value);
int fSetFTPAlarmEnable(unsigned char value);
int fSetSMTPAlarmEnable(unsigned char value);
int fSetAlarmDuration(unsigned char value);
int fSetImageSource(unsigned char value);
int fSetTVcable(unsigned char value);
int fSetBinning(unsigned char value);
int fSetBlc(unsigned char value);
int fSetMP41bitrate(int value);
int fSetMP42bitrate(int value);
int fSetEncFramerate(unsigned char stream, unsigned char value);
int fSetRateControl(unsigned char value);
int fSetAVIDuration(unsigned char value);
int fSetAVIFormat(unsigned char value);
int fSetFTPFileFormat(unsigned char value);
int fSetSDFileFormat(unsigned char value);
int fSetAttFileFormat(unsigned char value);
int fSetAudioEnable(unsigned char value);
int fSetASmtpAttach(unsigned char value);
int fSetRftpenable(unsigned char value);
int fSetSdReEnable(unsigned char value);
int fSetImageAEW(unsigned char value);
int fSetImageAEWType(unsigned char value);
int fSetVideoSize(unsigned char stream, int Xvalue, int Yvalue);
int fSetStreamActive(unsigned char stream1, unsigned char stream2, unsigned char stream3,
                     unsigned char stream4, unsigned char stream5, unsigned char stream6);
int fSetMotionEnable(unsigned char value);
int fSetMotionCEnable(unsigned char value);
int fSetMotionLevel(unsigned char value);
int fSetMotionCValue(unsigned char value);
int fSetMotionBlock(char *pValue,int len);
int fSetSDInsert(unsigned char value);
int fSetAcount(int index, Acount_t *acout);
int fSetIpncPtz(unsigned char value);
int fSetGIOInEnable(unsigned char value);
int fSetGIOInType(unsigned char value);
int fSetGIOOutEnable(unsigned char value);
int fSetGIOOutType(unsigned char value);
int fSetTStampEnable(unsigned char value);
int fSetTStampFormat(unsigned char value);
int fSetTAudioinVolume(unsigned char value);
int fResetSysInfo();
int fSetSysLog(LogEntry_t* value);
int fSetAlarmStatus(unsigned short value);
int fSetPtzSupport(unsigned char value);
int fSetNetMulticast(unsigned char value);
/** @} */
#endif /* _FILE_MSG_DRV_H_ */
/**
* @page FILE_MSG_DRV_HOW How to use file message driver?

* 1. Add new message ID in \ref File_Msg_Def.h if needed.\n
* 2. Call the API InitFileMsgDrv(int key,int iProcId) with the ID you added to initialize file
*  message driver.\n
* 3. Now you can use any file message driver functions as you want.\n
* 4. Call the API CleanupFileMsgDrv() to cleanup file message driver.
* @section FILE_MSG_DRV_HOW_EX Example
* @code
#include <file_msg_drv.h>
int main()
{
	SysInfo *pSysInfo;
	if( InitFileMsgDrv(FILE_MSG_KEY, FILE_SYS_MSG) != 0)
		return -1;
	pSysInfo = GetSysInfo();
	// more file message driver API
	CleanupFileMsgDrv();
	return 0;
}
* @endcode
* \b See \b also \ref SYS_MSG_DRV_HOW
*/
