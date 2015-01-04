/*
 * Device interface header file 
 *
 */

#ifndef __PSIA_IFACE_H__
#define __PSIA_IFACE_H__

#define MAX_USERS 10

typedef struct {
	const char *id;
	const char *input;
	const char *whiteBalanceMode;
	const char *whiteBalanceLevel;
	const char *exposureMode;
	const char *exposureTarget;
	const char *exposureAutoMin;
	const char *exposureAutoMax;
	const char *gainLevel;
	const char *brightnessLevel;
	const char *contrastLevel;
	const char *sharpnessLevel;
	const char *saturationLevel;
	const char *dayNightFilterType;
}VideoInputChannel_psia;

typedef struct {
	const char *id;
	const char *enabled;
	const char *videoInputID;
	const char *timeStampEnabled;
	const char *dateTimeFormat;
	const char *backgroundColor;
	const char *fontColor;
	const char *fontSize;
	const char *displayText;
	const char *horizontalAlignType;
	const char *verticalAlignType;
}TextOverlay_psia;

typedef struct {
	const char *id;
	const char *enabled;
	const char *videoInputID;
	const char *imageName;
	const char *positionX;
	const char *positionY;
	const char *transparentColorEnabled;
	const char *transparentColor;	
}ImageOverlay_psia;

typedef struct {	
	const char *enabled;	
}PrivacyMask_psia;

typedef struct {
	const char *id;
	const char *enabled;
	const char *audioMode;
	const char *microphoneEnabled;
	const char *microphoneSource;
	const char *microphoneVolume;
	const char *speakerEnabled;
	const char *speakerVolume;
}AudioChannel_psia;

typedef struct {
	const char *device_name;
	const char *device_id;
	const char *model;
	const char *mac_address;
	const char *firmware_version;
	const char *firmware_released_date;
	const char *boot_version;
	const char *hardware_version;
}DeviceInfo_psia;

typedef struct {
	const char *channelName;
	const char *rtspPortNo;
	const char *maxPacketSize;		
	const char *enabled;
	const char *disabled;
	const char *videoInputChannelID;
	const char *videoCodecType;
	const char *videoScanType;
	const char *fixedQuality;
	const char *videoResolutionWidth;
	const char *videoResolutionHeight;
	const char *videoQualityControlType;
    const char *constantBitRate;
	const char *maxFrameRate;
	const char *rotationDegree;
	const char *mirrorEnabled;
	const char *audioEnabled;	
	const char *audioInputChannelID;
	const char *audioCompressionType;
	const char *audioBitRate;
	const char *audioSamplingRate;
}StreamingChannel_psia;

typedef struct {
	const char *timemode;
	const char *localtime;
	const char *timezone;	
}Time_psia;

typedef struct {
	const char *hostname;
	const char *ip_address;
	const char *ipV6_address;
	const char *portNo;	
}NtpServer_psia;

typedef struct {
	const char *ip_version;
	const char *addressing_type;
	const char *ip_address;
	const char *subnet_mask;
	const char *gateway;
	const char *primaryDNS;
}IPAddress_psia;

typedef struct {
	const char *upnp_enabled;
	const char *zconf_enabled;
	const char *mcast_enabled;
	const char *ipAddress;	     // zeroconf mcast addr
	const char *portNo;		     // zeroconf port no
	const char *ttl;             // zeroconf ttl
}Discovery_psia;

typedef struct {
	char id[8];
	char userName[32];
	char password[32];
	char privilegeLevel[2]; //0, 1, 2, 3
}User_psia;

typedef struct {
	const char *id;
	const char *eventType;
	const char *eventDescription;		
	const char *inputIOPortID;
	const char *intervalBetweenEvents;
}EventTrigger_psia;

typedef struct {
	const char *id;
	const char *notificationMethod;
	const char *notificationRecurrence;		
	const char *notificationInterval;
}EventTriggerNotification_psia;

typedef struct {
	const char *beginDateTime;
	const char *endDateTime;
}EventSchedule_psia;

typedef struct {
	const char *uploadVideoClipEnabled;
	const char *senderEmailAddress;
	const char *receiverEmailAddress;		
	const char *videoClipFormatType;
}EvenNotificationMethods_psia;

typedef struct {
	const char *id;
	const char *authenticationMode;
	const char *addressingFormatType;		
	const char *hostName;
	const char *ipAddress;
	const char *accountName;
	const char *password;	
}MailingNotification_psia;

typedef struct {
	const char *id;
	const char *addressingFormatType;	
	const char *ipAddress;
	const char *portNo;
	const char *userName;
	const char *password;
	const char *uploadPath;	
}FTPNotification_psia;

typedef struct {
	const char *id;
	const char *url;	
	const char *protocolType;
	const char *portNo;	
}HttpHostNotification_psia;

typedef struct {
	const char *id;
	const char *enabled;	
	const char *samplingInterval;
	const char *startTriggerTime;
	const char *endTriggerTime;
	const char *directionSensitivity;	
	const char *regionType;	
	const char *rowGranularity;
	const char *columnGranularity;
}MotionDetection_psia;

typedef struct {
	const char *id;
	const char *enabled;	
	const char *maskEnabled;
	const char *sensitivityLevel;
	const char *detectionThreshold;
	const char *positionX;	
	const char *positionY;
}MotionDetectionRegion_psia;


/* Status */
extern void add_response_status(struct request *req, node_t *node);
extern int get_http_status(struct request *req);

/* System/Network */
extern int retrieve_system_network_interfaces(struct request *req, node_t *node);
extern int retrieve_system_network_interfaces_id(struct request *req, node_t *node);
extern int update_system_network_interfaces_id(struct request *req, node_t *node);
extern int retrieve_system_network_interfaces_id_ipaddress(struct request *req, node_t *node);
extern int update_system_network_interfaces_id_ipaddress(struct request *req, node_t *node);
extern int retrieve_system_network_interfaces_id_discovery(struct request *req, node_t *node);
extern int update_system_network_interfaces_id_discovery(struct request *req, node_t *node);
extern int get_network_count(void);
extern void get_network_url(int id, char *idstr);
extern int get_network_index(char *idstr);

/* System/Video */
extern int retrieve_system_video_channels(struct request *req, node_t *node);
extern int retrieve_system_video_channels_id(struct request *req, node_t *node);
extern int update_system_video_channels_id(struct request *req, node_t *node);
extern int retrieve_system_video_channels_id_overlays(struct request *req, node_t *node);
extern int update_system_video_channels_id_overlays(struct request *req, node_t *node);
extern int delete_system_video_channels_id_overlays(struct request *req, node_t *node);
extern int retrieve_system_video_channels_id_privacymask(struct request *req, node_t *node);
extern int update_system_video_channels_id_privacymask(struct request *req, node_t *node);
extern int retrieve_system_video_channels_id_overlays_text(struct request *req, node_t *node);
extern int update_system_video_channels_id_overlays_text(struct request *req, node_t *node);
extern int create_system_video_channels_id_overlays_text(struct request *req, node_t *node);
extern int delete_system_video_channels_id_overlays_text(struct request *req, node_t *node);
extern int retrieve_system_video_channels_id_overlays_image(struct request *req, node_t *node);
extern int update_system_video_channels_id_overlays_image(struct request *req, node_t *node);
extern int delete_system_video_channels_id_overlays_image(struct request *req, node_t *node);
extern int retrieve_system_video_channels_id_overlays_text_id(struct request *req, node_t *node);
extern int update_system_video_channels_id_overlays_text_id(struct request *req, node_t *node);
extern int delete_system_video_channels_id_overlays_text_id(struct request *req, node_t *node);
extern int retrieve_system_video_channels_id_overlays_image_id(struct request *req, node_t *node);
extern int update_system_video_channels_id_overlays_image_id(struct request *req, node_t *node);
extern int delete_system_video_channels_id_overlays_image_id(struct request *req, node_t *node);

/* System/Audio */
extern int retrieve_system_audio_channels(struct request *req, node_t *node);
extern int retrieve_system_audio_channels_id(struct request *req, node_t *node);
extern int update_system_audio_channels_id(struct request *req, node_t *node);

/* System */
extern int do_system_reboot(struct request *req, node_t *node);
extern int do_system_updatefirmware(struct request *req, node_t *node);
extern int retrieve_system_configurationdata(struct request *req, node_t *node);
extern int update_system_configurationdata(struct request *req, node_t *node);
extern int do_system_factory_reset(struct request *req, node_t *node);
extern int retrieve_system_deviceinfo(struct request *req, node_t *node);
extern int update_system_deviceinfo(struct request *req, node_t *node);
extern int retrieve_system_supportreport(struct request *req, node_t *node);
extern int retrieve_system_status(struct request *req, node_t *node);
extern int retrieve_system_time(struct request *req, node_t *node);
extern int update_system_time(struct request *req, node_t *node);
extern int retrieve_system_time_localtime(struct request *req, node_t *node);
extern int update_system_time_localtime(struct request *req, node_t *node);
extern int retrieve_system_time_timezone(struct request *req, node_t *node);
extern int update_system_time_timezone(struct request *req, node_t *node);
extern int retrieve_system_time_ntpservers(struct request *req, node_t *node);
extern int update_system_time_ntpservers(struct request *req, node_t *node);
extern int create_system_time_ntpservers(struct request *req, node_t *node);
extern int delete_system_time_ntpservers(struct request *req, node_t *node);
extern int retrieve_system_time_ntpservers_id(struct request *req, node_t *node);
extern int update_system_time_ntpservers_id(struct request *req, node_t *node);
extern int delete_system_time_ntpservers_id(struct request *req, node_t *node);
extern int get_ntpserver_count(void);
extern void get_ntpserver_url(int id, char *idstr);
extern int get_ntpserver_index(char *idstr);

/* Security */
extern int get_user_count(void);
extern void get_user_url(int i, char *idstr);
extern int retieve_security_aaa_users(struct request *req, node_t *node);
extern int update_security_aaa_users(struct request *req, node_t *node);
extern int create_security_aaa_users(struct request *req, node_t *node);
extern int delete_security_aaa_users(struct request *req, node_t *node);
extern int retieve_security_aaa_users_id(struct request *req, node_t *node);
extern int update_security_aaa_users_id(struct request *req, node_t *node);
extern int delete_security_aaa_users_id(struct request *req, node_t *node);

/* Streaming */
extern int get_channel_count(void);
extern void get_channel_url(int id, char *idstr);
extern int get_channel_index(char *idstr);
extern int retrieve_streaming_status(struct request *req, node_t *node);
extern int retrieve_streaming_channels(struct request *req, node_t *node);
extern int retrieve_streaming_channels_id(struct request *req, node_t *node);
extern int retrieve_streaming_channels_id_status(struct request *req, node_t *node);
extern int retrieve_streaming_channels_id_http(struct request *req, node_t *node);
extern int retrieve_streaming_channels_id_picture(struct request *req, node_t *node);
extern int retrieve_streaming_channels_id_http_capabilities(struct request *req, node_t *node);
extern int retrieve_streaming_channels_id_picture_capabilities(struct request *req, node_t *node);

/* Custom/Event */
extern int get_event_count(void);
extern int retrieve_custom_event(struct request *req, node_t *node);
extern int update_custom_event(struct request *req, node_t *node);
extern int retrieve_custom_event_triggers(struct request *req, node_t *node);
extern int update_custom_event_triggers(struct request *req, node_t *node);
extern int create_custom_event_triggers(struct request *req, node_t *node);
extern int delete_custom_event_triggers(struct request *req, node_t *node);
extern int retrieve_custom_event_triggers_id(struct request *req, node_t *node);
extern int update_custom_event_triggers_id(struct request *req, node_t *node);
extern int delete_custom_event_triggers_id(struct request *req, node_t *node);
extern int retrieve_custom_event_triggers_id_notifications(struct request *req, node_t *node);
extern int update_custom_event_triggers_id_notifications(struct request *req, node_t *node);
extern int create_custom_event_triggers_id_notifications(struct request *req, node_t *node);
extern int delete_custom_event_triggers_id_notifications(struct request *req, node_t *node);
extern int retrieve_custom_event_triggers_id_notifications_id(struct request *req, node_t *node);
extern int update_custom_event_triggers_id_notifications_id(struct request *req, node_t *node);
extern int delete_custom_event_triggers_id_notifications_id(struct request *req, node_t *node);
extern int retrieve_custom_event_schedule(struct request *req, node_t *node);
extern int update_custom_event_schedule(struct request *req, node_t *node);
extern int retrieve_custom_event_notification(struct request *req, node_t *node);
extern int update_custom_event_notification(struct request *req, node_t *node);
extern int retrieve_custom_event_notification_mailing(struct request *req, node_t *node);
extern int update_custom_event_notification_mailing(struct request *req, node_t *node);
extern int create_custom_event_notification_mailing(struct request *req, node_t *node);
extern int delete_custom_event_notification_mailing(struct request *req, node_t *node);
extern int retrieve_custom_event_notification_mailing_id(struct request *req, node_t *node);
extern int update_custom_event_notification_mailing_id(struct request *req, node_t *node);
extern int delete_custom_event_notification_mailing_id(struct request *req, node_t *node);
extern int retrieve_custom_event_notification_ftp(struct request *req, node_t *node);
extern int update_custom_event_notification_ftp(struct request *req, node_t *node);
extern int create_custom_event_notification_ftp(struct request *req, node_t *node);
extern int delete_custom_event_notification_ftp(struct request *req, node_t *node);
extern int retrieve_custom_event_notification_ftp_id(struct request *req, node_t *node);
extern int update_custom_event_notification_ftp_id(struct request *req, node_t *node);
extern int delete_custom_event_notification_ftp_id(struct request *req, node_t *node);
extern int retrieve_custom_event_notification_httpHost(struct request *req, node_t *node);
extern int update_custom_event_notification_httpHost(struct request *req, node_t *node);
extern int create_custom_event_notification_httpHost(struct request *req, node_t *node);
extern int delete_custom_event_notification_httpHost(struct request *req, node_t *node);
extern int retrieve_custom_event_notification_httpHost_id(struct request *req, node_t *node);
extern int update_custom_event_notification_httpHost_id(struct request *req, node_t *node);
extern int delete_custom_event_notification_httpHost_id(struct request *req, node_t *node);

/* Custom/Analytics */
extern int get_region_count(void);
extern int retrieve_custom_analytics_motiondetection(struct request *req, node_t *node);
extern int retrieve_custom_analytics_motiondetection_id(struct request *req, node_t *node);
extern int update_custom_analytics_motiondetection_id(struct request *req, node_t *node);
extern int retrieve_custom_analytics_motiondetection_id_regions(struct request *req, node_t *node);
extern int update_custom_analytics_motiondetection_id_regions(struct request *req, node_t *node);
extern int create_custom_analytics_motiondetection_id_regions(struct request *req, node_t *node);
extern int delete_custom_analytics_motiondetection_id_regions(struct request *req, node_t *node);
extern int retrieve_custom_analytics_motiondetection_id_regions_id(struct request *req, node_t *node);
extern int update_custom_analytics_motiondetection_id_regions_id(struct request *req, node_t *node);
extern int delete_custom_analytics_motiondetection_id_regions_id(struct request *req, node_t *node);

/* Initialization and cleanup */
extern int iface_init(struct request *req);
extern void iface_cleanup(struct request *req);

#endif /* __PSIA_IFACE_H__ */

