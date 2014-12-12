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
* @file sys_env_type.h
* @brief System evironment structure and Global definition.
*/
#ifndef __SYS_ENV_TYPE_H__
#define __SYS_ENV_TYPE_H__

#include <asm/types.h>
#include <netinet/in.h>
#include <time.h>

#define MAX_DOMAIN_NAME			40 		///< Maximum length of domain name. Ex: www.xxx.com
#define MAX_LANCAM_TITLE_LEN	11 		///< Maximum length of LANCAM title.
#define OSDTEXTLEN				16
#define MAX_FQDN_LENGTH			256 	///< Maximum length of FQDN.
#define MAX_STRING_LENGTH		256 	///< Maximum length of normal string.
#define MAC_LENGTH				6 		///< Length of MAC address.
#define ACOUNT_NUM				10 		///< How many acounts which are stored in system.
#define SCHDULE_NUM				10 		///< How many schedules will be stored in system.
#define MAX_FILE_NAME			128		///< Maximum length of file name.

#define USER_LEN				33 		///< Maximum of acount username length.
#define PASSWORD_LEN			33 		///< Maximum of acount password length.
#define IP_STR_LEN				20		///< IP string length

#define MOTION_BLK_LEN			(10) 	///< Motion block size

#define OSD_MSG_FALSE			(0)
#define OSD_MSG_TRUE			(1)
#define OSD_PARAMS_UPDATED		(4)

#define FLG_UI_EXT 				(1 << 0)
#define FLG_UI_MOTION 			(1 << 1)
#define FLG_UI_RECORD 			(1 << 2)
#define FLG_UI_AVI 				(1 << 3)
#define FLG_UI_JPG 				(1 << 4)

/**
* @brief Infomation of network status and user settings.
*/
typedef struct
{
	struct in_addr	ip; ///< IP address in static IP mode
	struct in_addr	netmask; ///< netmask in static IP mode
	struct in_addr	gateway; ///< gateway in static IP mode
	struct in_addr	dns; ///< DNS IP in static IP mode
	__u16		http_port; ///< HTTP port in web site.
	int			dhcp_enable; ///< current DHCP status.
	int			dhcp_config; ///< DHCP config
	char		ntp_server[MAX_DOMAIN_NAME+1]; ///< NTP server FQDN
	__u8		ntp_timezone; ///< current time zone
	__u8		ntp_frequency; ///< NTP server query frequence
	__u8		time_format; ///< time display format
	__u8		daylight_time; ///< daylight saving time mode
	__u8		MAC[MAC_LENGTH]; ///< hardware MAC address
	__u8		mpeg4resolution; ///< MPEG4_1 resolution
	__u8		mpeg42resolution; ///< MPEG4_2 resolution
	__u8		liveresolution; ///< live resolution
	__u8		mpeg4quality; ///< MPEG4 quality
	__u8		jpegquality; ///< jpeg quality
	__u8		supportmpeg4; ///< support MPEG4
	__u8		imageformat; ///< image format
	__u8		imagesource; ///< image source (NTSC/PAL)
	__u8		audiosource;///2011.4.2 add
	__u8		defaultstorage; ///< default storage
	char		defaultcardgethtm[MAX_STRING_LENGTH]; ///< default card get page
	char		brandurl[MAX_STRING_LENGTH]; ///< brand URL
	char		brandname[MAX_STRING_LENGTH]; ///< brand name
	__u8		supporttstamp; ///< support time stamp
	__u8		supportmotion; ///< support motion detection
	__u8		supportwireless; ///< support wireless
	__u8		serviceftpclient; ///< ftp client support
	__u8		servicesmtpclient; ///< smtp client support
	__u8		servicepppoe; ///< PPPoE support
	__u8		servicesntpclient; ///< sntp client support
	__u8		serviceddnsclient; ///< DNS client support
	__u8		supportmaskarea; ///< mask area support
	__u8		maxchannel; ///< max channel
	__u8		supportrs485; ///< RS485 support
	__u8		supportrs232; ///< RS232 support
	__u8		layoutnum; ///< layout No.
	__u8		supportmui; ///< support MUI
	__u8		mui; ///< MUI
	__u8		supportsequence; ///< support sequence
	__s8		quadmodeselect; ///< quadmode select
	__u8		serviceipfilter; ///< service IP filter
	__u8		oemflag0; ///< OEM flag
	__u8		supportdncontrol; ///< Daynight control support
	__u8		supportavc; ///< avc support
	__u8		supportaudio; ///< support audio
	__u8		supportptzpage; ///< PTZ support
	__u8		audioenable; ///< audio enable
	__u8		multicast_enable; ///< multicast enable
} Network_Config_Data;

/**
* @brief Infomation about ftp configuration.
*/
typedef struct
{
	char		servier_ip[MAX_DOMAIN_NAME+1]; ///< FTP server address
	char		username[USER_LEN]; ///< FTP login username
	char		password[PASSWORD_LEN]; ///< FTP login password
	char		foldername[MAX_FILE_NAME]; ///< FTP upload folder
	int			image_acount; ///< Image count
	int			pid; ///< PID
	__u16		port; ///< FTP port
 	__u8		rftpenable; ///< RFTP enable
	__u8        ftpfileformat; ///< file format
} Ftp_Config_Data;

/**
* @brief Infomation about SMTP configuration.
*/
typedef struct
{
	char		servier_ip[MAX_DOMAIN_NAME+1]; ///< SMTP server address
	char		username[USER_LEN]; ///< SMTP username
	char		password[PASSWORD_LEN]; ///< SMTP password
	__u8		authentication; ///< SMTP authentication
	char		sender_email[MAX_STRING_LENGTH]; ///< sender E-mail address
	char		receiver_email[MAX_STRING_LENGTH]; ///< receiver E-mail address
	char		CC[MAX_STRING_LENGTH]; ///< CC E-mail address
	char		subject[MAX_STRING_LENGTH]; ///< mail subject
	char		text[MAX_STRING_LENGTH]; ///< mail text
	__u8		attachments; ///< mail attachment
	__u8		view; ///< smtp view
	__u8		asmtpattach; ///< attatched file numbers
	__u8        attfileformat; ///< attachment file format
} Smtp_Config_Data;

/**
* @brief custom data structure for time.
*/
typedef struct{
	__u8	nHour;	///< Hour from 0 to 23.
	__u8	nMin;	///< Minute from 0 to 59.
	__u8	nSec;	///< Second from 0 to 59.
} Time_t;

/**
* @brief custom data structure for schedule entry.
*/
typedef struct{
	__u8	bStatus;	///< schedule status ( 0:disable 1:enable }
	__u8	nDay;		///< schedule day of week (1:Mon 2:Tue 3:Wed 4:Thr 5:Fri 6:Sat 7:Sun 8:Everyday)
	Time_t	tStart;		///< schedule start time
	Time_t	tDuration;	///< schedule duration
} Schedule_t;

/**
* @brief IPCAM configuration data.
*/
typedef struct
{
	char			title[MAX_LANCAM_TITLE_LEN+1];	///< camera title
	__u8			nWhiteBalance;					///< white balance mode
	__u8			nDayNight;						///< Daynight mode
	__u8			nTVcable;						///< TV-out setting
	__u8			nBinning;						///< binning/skipping setting
	__u8			nBacklightControl;				///< backlight control setting
	__u8			nBackLight;						///< backlight value
	__u8			nBrightness;					///< brightness value
	__u8			nContrast;						///< contrast value
	__u8			nSaturation;					///< saturation value
	__u8			nSharpness;						///< sharpness value
	Schedule_t		aSchedules[SCHDULE_NUM];		///< schedule data
	__u8            lostalarm;						///< ethernet lost alarm
	__u8			bSdaEnable;						///< alarm save into SD card
	__u8			bAFtpEnable;					///< alarm upload via FTP
	__u8			bASmtpEnable;					///< alarm upload via SMTP
	__u8			nVideomode;						///< video mode
	__u8			nVideocodecmode;				///< selected codec mode
	__u8			nVideocodecres;					///< selected codec resolution
	__u8			nRateControl;					///< bitrate control setting
	__u8			nMpeg41frmrate;					///< MPEG4_1 frame rate
	__u8			nMpeg42frmrate;					///< MPEG4_2 frame rate
	__u8			nJpegfrmrate;					///< JPEG frame rate
	int				nMpeg41bitrate;					///< MPEG4_1 bitrate
	int				nMpeg42bitrate;					///< MPEG4_2 bitrate
	int				Mpeg41Xsize;					///< MPEG4_1 horizontal resolution
	int				Mpeg41Ysize;					///< MPEG4_1 vertical resolution
	int				Mpeg42Xsize;					///< MPEG4_2 horizontal resolution
	int				Mpeg42Ysize;					///< MPEG4_2 vertical resolution
	int				JpegXsize;						///< JPEG horizontal resolution
	int				JpegYsize;						///< JPEG vertical resolution
	int				Avc1Xsize;						///< H264_1 horizontal resolution
	int				Avc1Ysize;						///< H264_1 vertical resolution
	int				Avc2Xsize;						///< H264_2 horizontal resolution
	int				Avc2Ysize;						///< H264_2 vertical resolution
	__u8			Supportstream1;					///< support stream1 (JPEG)
	__u8			Supportstream2;					///< support stream2 (MPEG4_1)
	__u8			Supportstream3;					///< support stream3 (MPEG4_2)
	__u8			Supportstream4;					///< support stream4
	__u8			Supportstream5;					///< support stream5 (AVC_1)
	__u8			Supportstream6;					///< support stream6 (AVC_2)
	__u8			nAlarmDuration;					///< alarm duration
	__u8			nAEWswitch;						///< 2A engine selection
	__u8			gioinenable;					///< GIO input enable
	__u8			giointype;						///< GIO input type
	__u8			giooutenable;					///< GIO output enable
	__u8			gioouttype;						///< GIO output type
	__u8            tstampenable;					///< enable timestamp
	__u8            tstampformat;					///< timestamp format
	__u8            audioinvolume;					///< audio volume
	__u8            aviduration;					///< avi record duration
	__u8			aviformat;						///< avirecord format.
	__u16			alarmstatus;					///< Current alarm status.
	Network_Config_Data	net;						///< network status and user settings
	__u8			rotation; 						///< rotation
	__u8			mirror; 						///< mirroring
	__u8		    AdvanceMode; 					    ///< video stabilization (ON/OFF)
	__u8		    M41SFeature; 					    ///< video stabilization (ON/OFF)
	__u8			M42SFeature; 							///< noise filter
	__u8			JPGSFeature;					///< video codec
	__u8		    fdetect; 					    ///< facedetect (ON/OFF)
	__u8			democfg;						///< demo config
	__u8			regusr[MAX_LANCAM_TITLE_LEN+1];	///< register user name
	__u8			osdstream;						///< register user name
	__u8			osdwinnum;						///< register user name
	__u8			osdwin;							///< register user name
	__u8			osdtext[OSDTEXTLEN];			///< register user name
	__u8			nAEWtype;						///< 2A engine selection
	__u8			histogram;						///< Histogram switch ON/OFF
	__u8			gbce;							///< GBCE switch ON/OFF
	__u8			dummy;							///< dummy for end of config
} Lancam_Config_Data;

/**
* @brief SD card configuration data.
*/
typedef struct
{
/**  __u8			sdrecordtype;
  __u8			sdcount;
  __u8			sdrate;
  __u8			sdduration;
  __u8			aviprealarm;*/
   //__u8         sdformat;
   __u8			sdfileformat;	///< file format saved into SD card
   __u8			sdrenable;		///< enable SD card recording
   __u8			sdinsert;		///< SD card inserted
}Sdcard_Config_Data;

/**
* @brief IPCAM user account data.
*/
typedef struct
{
	char	user[USER_LEN];			///< username
	char	password[PASSWORD_LEN];	///< password
	__u8	authority;				///< user authority
}Acount_t;

/**
* @brief motion detection configuration data.
*/
typedef struct
{
  __u8      motionenable;				///< motion detection enable
  __u8      motioncenable;				///< customized sensitivity enable
  __u8      motionlevel;				///< predefined sensitivity level
  __u8      motioncvalue;				///< customized sensitivity value
  __u8	 motionblock[MOTION_BLK_LEN];	///< motion detection block data
}Motion_Config_Data;


/**
* @brief structure for PTZ control data.
*/
typedef struct
{
  char      ptzdata[300];
}Ptz_Data;


/**
* @brief PTZ configuration data.
*/
typedef struct
{
  char      ptzzoomin[6];	///< zoom-in
  char      ptzzoomout[7];	///< zoom-out
  char      ptzpanup[2];	///< pan-up
  char      ptzpandown[4];	///< pan-down
  char	ptzpanleft[4];		///< pan-left
  char	ptzpanright[5];		///< pan-right
}Ptz_Config_Data;

/**
* @brief event log data structure.
*/
typedef struct LogEntry_t{
	char event[50];		///< event description
	struct tm time;		///< event log time
}LogEntry_t;


//**************************************************************************
//2010-10-16 Ìí

typedef struct {
 __u32  ftpswitch;          
 __u32  smtpswitch;
 __u32  ntpswitch;
 __u32  rtspswitch;
}Net_config_switch;

typedef struct {
 __u32  h2641;
 __u32  h2642;
 __u32  mpeg41;
 __u32  mpeg42;
 __u32  jpeg;
}Net_rtsp_port;
typedef struct {
 __u32  input;	         
 __u32  yellow_line;        
 __u32  red;                
 __u32  superspeed;         
 __u32  di_do_action;       
 __u32  yellow_do_action;   
 __u32  red_do_action;      
 __u32  sup_do_action;      
 __u32  di_smtp_action;     
 __u32  yellow_smtp_action; 
 __u32  red_smtp_action;    
 __u32  sup_smtp_action;    
 __u32  di_record_action;   
 __u32  yellow_record_action;
 __u32  red_record_action;   
 __u32  sup_record_action;   
}event_switch;

typedef struct{
	__u32  storageswitch;     
	__u32  perstorage_switch;  
	__u32  time_storage_switch; 
	__u32  StorageSwitch; 
	__u32  pretime;             
	__u32  delaytime;      
	__u32  StorageTimeSet;
	
}event_storage;
//*************************************************************************
/**
* @brief system info main data structure.
*/
typedef struct SysInfo{
	Lancam_Config_Data lan_config;		///< IPCAM configuration data
	char fqdn[MAX_FQDN_LENGTH];			///< IPCAM FQDN
	unsigned short DeviceType;			///< IPCAM device type
	Ftp_Config_Data ftp_config;			///< IPCAM FQDN
	Smtp_Config_Data smtp_config;		///< ftp configuration data
	Sdcard_Config_Data sdcard_config;	///< SD card configuration data
	Motion_Config_Data motion_config;	///< motion detection configuration data
	Ptz_Config_Data ptz_config;			///< PTZ configuration data
	char devicename[MAX_STRING_LENGTH];	///< device name
	Acount_t	acounts[ACOUNT_NUM];	///< user account data
	LogEntry_t	tCurLog;				///< event log
	//2010-10-16Ìí¼Ó
//	Net_config_switch  net_switch;       
//	Net_rtsp_port     rtsp_port;        
//	event_switch  evn_switch;    
//	event_storage  evn_storage;
}SysInfo;
#endif /* __SYS_ENV_TYPE_H__ */
