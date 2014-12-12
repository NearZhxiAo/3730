
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <time.h>

#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <file_list.h>
#include "web_translate.h"
#include <sys_env_type.h>
#include "file_msg_drv.h"
#include <net_config.h>

//20110411
#include "boa_private.h"

static char *nicname = "eth0";

ARG_HASH_TABLE *arg_hash;

/***************************************************************************
 *                                                                         *
 ***************************************************************************/
int para_date(char *data, char *arg)
{
	time_t tnow;
	struct tm *tmnow;

	tzset();
	time(&tnow);
	tmnow = localtime(&tnow);

	if ( *arg == '0' ) {
		return sprintf(data, "%d", tmnow->tm_year+1900);
	}
	else if ( *arg == '1' ) {
		return sprintf(data, "%d", tmnow->tm_mon+1);
	}
	else if ( *arg == '2' ) {
		return sprintf(data, "%d", tmnow->tm_mday);
	}
	else if ( *arg == '\0' )
		return sprintf(data, "%04d/%02d/%02d", tmnow->tm_year+1900, tmnow->tm_mon+1, tmnow->tm_mday);
	return -1;
}
/***************************************************************************
 *                                                                         *
 ***************************************************************************/
int para_time(char *data, char *arg)
{
	time_t tnow;
	struct tm *tmnow;

	tzset();
	time(&tnow);
	tmnow = localtime(&tnow);

	if ( *arg == '0' ) {
		return sprintf(data, "%d", tmnow->tm_hour);
	}
	else if ( *arg == '1' ) {
		return sprintf(data, "%d", tmnow->tm_min);
	}
	else if ( *arg == '2' ) {
		return sprintf(data, "%d", tmnow->tm_sec);
	}
	else if ( *arg == '\0' )
		return sprintf(data, "%02d:%02d:%02d", tmnow->tm_hour, tmnow->tm_min, tmnow->tm_sec);
	return -1;
}
/***************************************************************************
 *                                                                         *
 ***************************************************************************/
int para_sntpip(char *data, char *arg)
{
	SysInfo* pSysInfo = GetSysInfo();
	if(pSysInfo == NULL)
		return -1;
	return sprintf(data, "%s", pSysInfo->lan_config.net.ntp_server);
}
/***************************************************************************
 *                                                                         *
 ***************************************************************************/
int para_sntptimezone(char *data, char *arg)
{
	SysInfo* pSysInfo = GetSysInfo();
	if(pSysInfo == NULL)
		return -1;
	return sprintf(data, "%d", pSysInfo->lan_config.net.ntp_timezone);
}
/***************************************************************************
 *                                                                         *
 ***************************************************************************/
int para_sntpfrequency(char *data, char *arg)
{
	SysInfo* pSysInfo = GetSysInfo();
	if(pSysInfo == NULL)
		return -1;
	return sprintf(data, "%d", pSysInfo->lan_config.net.ntp_frequency);
}
/***************************************************************************
 *                                                                         *
 ***************************************************************************/
int para_dhcpenable(char *data, char *arg)
{
	SysInfo* pSysInfo = GetSysInfo();
	if(pSysInfo == NULL)
		return -1;
	return sprintf(data, "%d", pSysInfo->lan_config.net.dhcp_enable);
}
/***************************************************************************
 *                                                                         *
 ***************************************************************************/
int para_httpport(char *data, char *arg)
{
succeed_type iret = succeed_type_failed;
net_param gateway;
iret = Gbshm_paramGet(global_boa_private_handle.gbshm_handle, PARA_CMD_SYSTEM_NET_PARAM, &gateway, sizeof(gateway));
SYS_INFO("\n\r!!!!!!!!!!!!!!!!!!!GBSHM_PARA_GET httpport = %d iret = %d!!!!!!!!!!!!!!!!!!!!!!!!\r\n",gateway.httpport, iret);
	return sprintf(data,"%d", gateway.httpport);

}
/***************************************************************************
 *                                                                         *
 ***************************************************************************/
int para_gateway(char *data, char *arg)
{
	succeed_type iret = succeed_type_failed;
	net_param gateway;
	iret = Gbshm_paramGet(global_boa_private_handle.gbshm_handle, PARA_CMD_SYSTEM_NET_PARAM, &gateway, sizeof(gateway));
	SYS_INFO("\n\r!!!!!!!!!!!!!!!!!!!GBSHM_PARA_GET gateway.ip = %s iret = %d!!!!!!!!!!!!!!!!!!!!!!!!\r\n",gateway.ipgateway, iret);
	return sprintf(data, gateway.ipgateway,strlen(gateway.ipgateway));
}
/***************************************************************************
 *                                                                         *
 ***************************************************************************/
int para_netmask(char *data, char *arg)
{
	succeed_type iret = succeed_type_failed;
	net_param gateway;
	iret = Gbshm_paramGet(global_boa_private_handle.gbshm_handle, PARA_CMD_SYSTEM_NET_PARAM, &gateway, sizeof(gateway));
	SYS_INFO("\n\r!!!!!!!!!!!!!!!!!!!GBSHM_PARA_GET netmask.ip = %s iret = %d!!!!!!!!!!!!!!!!!!!!!!!!\r\n",gateway.ipmask, iret);
	return sprintf(data, gateway.ipmask,strlen(gateway.ipmask));
}
/***************************************************************************
 *                                                                         *
 ***************************************************************************/
int para_netip(char *data, char *arg)
{
	 succeed_type iret = succeed_type_failed;
	 net_param gateway;
	 iret = Gbshm_paramGet(global_boa_private_handle.gbshm_handle, PARA_CMD_SYSTEM_NET_PARAM, &gateway, sizeof(gateway));
	 SYS_INFO("\n\r!!!!!!!!!!!!!!!!!!!GBSHM_PARA_GET netip = %s iret = %d!!!!!!!!!!!!!!!!!!!!!!!!\r\n",gateway.ipaddr, iret);
	 return sprintf(data ,gateway.ipaddr,strlen(gateway.ipaddr));

}
/***************************************************************************
 *                                                                         *
 ***************************************************************************/
int para_dnsip(char *data, char *arg)
{

	NET_IPV4 dns;

	dns.int32 = net_get_dns();


	if ( *arg == '0' )
		return sprintf(data, "%d", dns.str[0]);
	else if ( *arg == '1' )
		return sprintf(data, "%d", dns.str[1]);
	else if ( *arg == '2' )
		return sprintf(data, "%d", dns.str[2]);
	else if ( *arg == '3' )
		return sprintf(data, "%d", dns.str[3]);
	else if ( *arg == '\0' )

		//dns.str[0] = 192;
		//dns.str[1] = 168;
		//dns.str[2] = 2;
		//dns.str[3] = 187;
		return sprintf(data, "%03d.%03d.%03d.%03d", dns.str[0], dns.str[1], dns.str[2], dns.str[3]);
	return -1;
}
/***************************************************************************
 *                                                                         *
 ***************************************************************************/
int para_osdtextinfo(char *data, char *arg)
{
	SysInfo* pSysInfo = GetSysInfo();
	if(pSysInfo == NULL)
		return -1;
	return sprintf(data, "%s", pSysInfo->lan_config.osdtext);
}
/***************************************************************************
 *                                                                         *
 ***************************************************************************/
int para_regusr(char *data, char *arg)
{
	SysInfo* pSysInfo = GetSysInfo();
	if(pSysInfo == NULL)
		return -1;
	return sprintf(data, "%s", pSysInfo->lan_config.regusr);
}
/***************************************************************************
 *                                                                         *
 ***************************************************************************/
int para_title(char *data, char *arg)
{
	SysInfo* pSysInfo = GetSysInfo();
	if(pSysInfo == NULL)
		return -1;
	return sprintf(data, "%s", pSysInfo->lan_config.title);
}
/***************************************************************************
 *                                                                         *
 ***************************************************************************/
int para_maxtitlelen(char *data, char *arg)
{
	return sprintf(data, "%d", MAX_LANCAM_TITLE_LEN);
}
/***************************************************************************
 *                                                                         *
 ***************************************************************************/
int para_mpeg4resolution(char *data, char *arg)
{
	SysInfo* pSysInfo = GetSysInfo();
	if(pSysInfo == NULL)
		return -1;
	return sprintf(data, "%d", pSysInfo->lan_config.net.mpeg4resolution);
}
/***************************************************************************
 *                                                                         *
 ***************************************************************************/
int para_liveresolution(char *data, char *arg)
{
	SysInfo* pSysInfo = GetSysInfo();
	if(pSysInfo == NULL)
		return -1;
	return sprintf(data, "%d", pSysInfo->lan_config.net.liveresolution);
}
/***************************************************************************
 *                                                                         *
 ***************************************************************************/
int para_mpeg4quality(char *data, char *arg)
{
	SysInfo* pSysInfo = GetSysInfo();
	if(pSysInfo == NULL)
		return -1;
	return sprintf(data, "%d", pSysInfo->lan_config.net.mpeg4quality);
}
/***************************************************************************
 *                                                                         *
 ***************************************************************************/
int para_livequality(char *data, char *arg)
{
	SysInfo* pSysInfo = GetSysInfo();
	if(pSysInfo == NULL)
		return -1;
	return sprintf(data, "%d", pSysInfo->lan_config.net.jpegquality);
}
/***************************************************************************
 *                                                                         *
 ***************************************************************************/
int para_maxfqdnlen(char *data, char *arg)
{
	return sprintf(data, "%d", MAX_FQDN_LENGTH - 1);
}
int para_maxdomainname(char *data, char *arg)
{
	return sprintf(data, "%d", MAX_DOMAIN_NAME);
}
/***************************************************************************
 *                                                                         *
 ***************************************************************************/
int para_ftpip(char *data, char *arg)
{
	SysInfo* pSysInfo = GetSysInfo();
	if(pSysInfo == NULL)
		return -1;
	return sprintf(data, "%s", pSysInfo->ftp_config.servier_ip);
}
/***************************************************************************
 *                                                                         *
 ***************************************************************************/
int para_ftppiport(char *data, char *arg)
{
	SysInfo* pSysInfo = GetSysInfo();
	if(pSysInfo == NULL)
		return -1;
	return sprintf(data, "%d", pSysInfo->ftp_config.port);
}
/***************************************************************************
 *                                                                         *
 ***************************************************************************/
int para_ftpuser(char *data, char *arg)
{
	SysInfo* pSysInfo = GetSysInfo();
	if(pSysInfo == NULL)
		return -1;
	return sprintf(data, "%s", pSysInfo->ftp_config.username);
}
/***************************************************************************
 *                                                                         *
 ***************************************************************************/
int para_ftppassword(char *data, char *arg)
{
	SysInfo* pSysInfo = GetSysInfo();
	if(pSysInfo == NULL)
		return -1;
	return sprintf(data, "%s", pSysInfo->ftp_config.password);
}
/***************************************************************************
 *                                                                         *
 ***************************************************************************/
int para_ftppapath(char *data, char *arg)
{
	SysInfo* pSysInfo = GetSysInfo();
	if(pSysInfo == NULL)
		return -1;
	return sprintf(data, "%s", pSysInfo->ftp_config.foldername);
}
/***************************************************************************
 *                                                                         *
 ***************************************************************************/
int para_maxftpuserlen(char *data, char *arg)
{
	return sprintf(data, "%d", USER_LEN - 1);
}
/***************************************************************************
 *                                                                         *
 ***************************************************************************/
int para_maxftppwdlen(char *data, char *arg)
{
	return sprintf(data, "%d", PASSWORD_LEN - 1);
}
/***************************************************************************
 *                                                                         *
 ***************************************************************************/
int para_maxftppathlen(char *data, char *arg)
{
	return sprintf(data, "%d", MAX_FILE_NAME - 1);
}
/***************************************************************************
 *                                                                         *
 ***************************************************************************/
int para_smtpip(char *data, char *arg)
{
	SysInfo* pSysInfo = GetSysInfo();
	if(pSysInfo == NULL)
		return -1;
	return sprintf(data, "%s", pSysInfo->smtp_config.servier_ip);
}
/***************************************************************************
 *                                                                         *
 ***************************************************************************/
int para_smtpauth(char *data, char *arg)
{
	SysInfo* pSysInfo = GetSysInfo();
	if(pSysInfo == NULL)
		return -1;
	return sprintf(data, "%d", pSysInfo->smtp_config.authentication);
}
/***************************************************************************
 *                                                                         *
 ***************************************************************************/
int para_smtpuser(char *data, char *arg)
{
	SysInfo* pSysInfo = GetSysInfo();
	if(pSysInfo == NULL)
		return -1;
	return sprintf(data, "%s", pSysInfo->smtp_config.username);
}
/***************************************************************************
 *                                                                         *
 ***************************************************************************/
int para_maxsmtpuser(char *data, char *arg)
{
	return sprintf(data, "%d", USER_LEN - 1);
}
/***************************************************************************
 *                                                                         *
 ***************************************************************************/
int para_smtppwd(char *data, char *arg)
{
	SysInfo* pSysInfo = GetSysInfo();
	if(pSysInfo == NULL)
		return -1;
	return sprintf(data, "%s", pSysInfo->smtp_config.password);
}
/***************************************************************************
 *                                                                         *
 ***************************************************************************/
int para_maxsmtppwd(char *data, char *arg)
{
	return sprintf(data, "%d", PASSWORD_LEN - 1);
}
/***************************************************************************
 *                                                                         *
 ***************************************************************************/
int para_smtpsender(char *data, char *arg)
{
	SysInfo* pSysInfo = GetSysInfo();
	if(pSysInfo == NULL)
		return -1;
	return sprintf(data, "%s", pSysInfo->smtp_config.sender_email);
}
/***************************************************************************
 *                                                                         *
 ***************************************************************************/
int para_maxsmtpsender(char *data, char *arg)
{
	return sprintf(data, "%d", MAX_STRING_LENGTH - 1);
}
/***************************************************************************
 *                                                                         *
 ***************************************************************************/
int para_emailuser(char *data, char *arg)
{
	SysInfo* pSysInfo = GetSysInfo();
	if(pSysInfo == NULL)
		return -1;
	return sprintf(data, "%s", pSysInfo->smtp_config.receiver_email);
}
/***************************************************************************
 *                                                                         *
 ***************************************************************************/
int para_maxemailuserlen(char *data, char *arg)
{
	return sprintf(data, "%d", MAX_STRING_LENGTH - 1);
}
/***************************************************************************
 *                                                                         *
 ***************************************************************************/
int para_supportmpeg4(char *data, char *arg)
{
	SysInfo* pSysInfo = GetSysInfo();
	if(pSysInfo == NULL)
		return -1;
	return sprintf(data, "%d", pSysInfo->lan_config.net.supportmpeg4);
}
/***************************************************************************
 *                                                                         *
 ***************************************************************************/
int para_format(char *data, char *arg)
{
	SysInfo* pSysInfo = GetSysInfo();
	if(pSysInfo == NULL)
		return -1;
	return sprintf(data, "%d", pSysInfo->lan_config.net.imageformat);
}
/***************************************************************************
 *                                                                         *
 ***************************************************************************/
int para_imagesource(char *data, char *arg)
{
/*
	SysInfo* pSysInfo = GetSysInfo();
	if(pSysInfo == NULL)
		return -1;
*/
 //    int video_input;
     succeed_type iret = succeed_type_failed;
	 SYS_INFO("\n\r!!!!!!!!!!!!!!!!!!!!!Entering !para_imagesource!!!!!!!!!!!!!!!!!!!!!!\r\n");
	 video_enc_param imagesource;
	 SYS_INFO("\n\r!!!!!!!!!!global_boa_private_handle.gbshm_handle == %x!!!!!!!!!!!!!!\r\n",global_boa_private_handle.gbshm_handle);
	 iret = Gbshm_paramGet(global_boa_private_handle.gbshm_handle , PARA_CMD_SYSTEM_VIDEO_ENC_PARAM, &imagesource, sizeof(imagesource));
	 SYS_INFO("\n\r!!!!!!!!!!!!!!!!!!!GBSHM_PARA_GET imagesource = %d iret = %d!!!!!!!!!!!!!!!!!!!!!!!!\r\n",imagesource.video_input_mode, iret);
	 switch(imagesource.video_input_mode)
	 {
	     case VIDEO_INPUT_PAL:
		 	return sprintf(data ,"%d",0);
		 case VIDEO_INPUT_NTSC:
		 	return sprintf(data ,"%d",1);
	 }
//	 video_input = (int)imagesource.video_input_mode - 1;
	 
}
/***************************************************************************
 * NEW FEATURE                                                             *
 ***************************************************************************/
int para_advancemode(char *data, char *arg)
{
	SysInfo* pSysInfo = GetSysInfo();
	if(pSysInfo == NULL)
		return -1;
	return sprintf(data, "%d", pSysInfo->lan_config.AdvanceMode);
}
int para_m41sfeature(char *data, char *arg)
{
	SysInfo* pSysInfo = GetSysInfo();
	if(pSysInfo == NULL)
		return -1;
	return sprintf(data, "%d", pSysInfo->lan_config.M41SFeature);
}
int para_m42sfeature(char *data, char *arg)
{
	SysInfo* pSysInfo = GetSysInfo();
	if(pSysInfo == NULL)
		return -1;

	return sprintf(data, "%d", pSysInfo->lan_config.M42SFeature);
}
int para_jpgsfeature(char *data, char *arg)
{
	SysInfo* pSysInfo = GetSysInfo();
	if(pSysInfo == NULL)
		return -1;
	return sprintf(data, "%d", pSysInfo->lan_config.JPGSFeature);
}
int para_advfeaturename(char *data, char *arg)
{

	if ( *arg == '0' )
		return sprintf(data, "OFF");
	else if ( *arg == '1' )
		return sprintf(data, "LDC");
	else if ( *arg == '2' )
		return sprintf(data, "VS");
	else if ( *arg == '3')
		return sprintf(data, "NF");
	else if ( *arg == '4')
		return sprintf(data, "LDC+VS");
	else if ( *arg == '5')
		return sprintf(data, "LDC+NF");
	else if ( *arg == '6')
		return sprintf(data, "VS+NF");
	else if ( *arg == '7')
		return sprintf(data, "LDC+VS+NF");
	else if ( *arg == 'a' || *arg == '\0' )
		return sprintf(data, "OFF;LDC;VS;NF;LDC+VS;LDC+NF;VS+NF;LDC+VS+NF");

	return -1;
}

int para_preprocess(char *data, char *arg)
{
	SysInfo* pSysInfo = GetSysInfo();
	if(pSysInfo == NULL)
		return -1;
	//return sprintf(data, "%d", pSysInfo->lan_config.AdvanceMode);
	unsigned char retv = (pSysInfo->lan_config.AdvanceMode) & 0x03;
	return sprintf(data, "%d", retv);
}
int para_noisefilt(char *data, char *arg)
{
	//unsigned char retv = (AdvmodeX >> 2) & 0x03;
	SysInfo* pSysInfo = GetSysInfo();
	if(pSysInfo == NULL)
		return -1;
	unsigned char retv = ((pSysInfo->lan_config.AdvanceMode) >> 2) & 0x03;
	return sprintf(data, "%d", retv);
}
int para_preprocessname(char *data, char *arg)
{

	if ( *arg == '0' )
		return sprintf(data, "OFF");
	else if ( *arg == '1' )
		return sprintf(data, "VS");
	else if ( *arg == '2' )
		return sprintf(data, "LDC");
	else if ( *arg == '3')
		return sprintf(data, "VS+LDC");
	else if ( *arg == 'a' || *arg == '\0' )
		return sprintf(data, "OFF;VS;LDC;VS+LDC");

	return -1;
}
int para_noisefiltname(char *data, char *arg)
{

	if ( *arg == '0' )
		return sprintf(data, "OFF");
	else if ( *arg == '1' )
		return sprintf(data, "SNF");
	else if ( *arg == '2' )
		return sprintf(data, "TNF");
	else if ( *arg == '3')
		return sprintf(data, "SNF+TNF");
	else if ( *arg == 'a' || *arg == '\0' )
		return sprintf(data, "OFF;SNF;TNF;SNF+TNF");

	return -1;
}
/***************************************************************************
 *                                                                         *
 ***************************************************************************/
int para_osdwin(char *data, char *arg)
{
	SysInfo* pSysInfo = GetSysInfo();
	if(pSysInfo == NULL)
		return -1;
	return sprintf(data, "%d", pSysInfo->lan_config.osdwin);
}
/***************************************************************************
 *                                                                         *
 ***************************************************************************/
int para_histogram(char *data, char *arg)
{
	SysInfo* pSysInfo = GetSysInfo();
	if(pSysInfo == NULL)
		return -1;
	return sprintf(data, "%d", pSysInfo->lan_config.histogram);
}
/***************************************************************************
 *                                                                         *
 ***************************************************************************/
int para_gbce(char *data, char *arg)
{
	SysInfo* pSysInfo = GetSysInfo();
	if(pSysInfo == NULL)
		return -1;
	return sprintf(data, "%d", pSysInfo->lan_config.gbce);
}
/***************************************************************************
 *                                                                         *
 ***************************************************************************/
int para_devicename(char *data, char *arg)
{
	SysInfo* pSysInfo = GetSysInfo();
	if(pSysInfo == NULL)
		return -1;
	return sprintf(data, "%s", pSysInfo->devicename);
}
/***************************************************************************
 *                                                                         *
 ***************************************************************************/
int para_defaultstorage(char *data, char *arg)
{
	SysInfo* pSysInfo = GetSysInfo();
	if(pSysInfo == NULL)
		return -1;
	return sprintf(data, "%d", pSysInfo->lan_config.net.defaultstorage);
}
/***************************************************************************
 *                                                                         *
 ***************************************************************************/
int para_sdinsert(char *data, char *arg)
{
	SysInfo* pSysInfo = GetSysInfo();
	if(pSysInfo == NULL)
		return -1;
	return sprintf(data, "%d", pSysInfo->sdcard_config.sdinsert);
}
/***************************************************************************
 *                                                                         *
 ***************************************************************************/
int para_cfinsert(char *data, char *arg)
{
	return sprintf(data, "%d", 0);
}
/***************************************************************************
 *                                                                         *
 ***************************************************************************/
int para_defaultcardgethtm(char *data, char *arg)
{
	SysInfo* pSysInfo = GetSysInfo();
	if(pSysInfo == NULL)
		return -1;
	return sprintf(data, "%s", pSysInfo->lan_config.net.defaultcardgethtm);
}
/***************************************************************************
 *                                                                         *
 ***************************************************************************/
int para_brandurl(char *data, char *arg)
{
	SysInfo* pSysInfo = GetSysInfo();
	if(pSysInfo == NULL)
		return -1;
	return sprintf(data, "%s", pSysInfo->lan_config.net.brandurl);
}
/***************************************************************************
 *                                                                         *
 ***************************************************************************/
int para_brandname(char *data, char *arg)
{
	SysInfo* pSysInfo = GetSysInfo();
	if(pSysInfo == NULL)
		return -1;
	return sprintf(data, "%s", pSysInfo->lan_config.net.brandname);
}
/***************************************************************************
 *                                                                         *
 ***************************************************************************/
int para_supporttstamp(char *data, char *arg)
{
	SysInfo* pSysInfo = GetSysInfo();
	if(pSysInfo == NULL)
		return -1;
	return sprintf(data, "%d", pSysInfo->lan_config.net.supporttstamp);
}
/***************************************************************************
 *                                                                         *
 ***************************************************************************/
int para_mpeg4xsize(char *data, char *arg)
{
	SysInfo* pSysInfo = GetSysInfo();
	if(pSysInfo == NULL)
		return -1;
	return sprintf(data, "%d", pSysInfo->lan_config.Mpeg41Xsize);
}
/***************************************************************************
 *                                                                         *
 ***************************************************************************/
int para_mpeg4ysize(char *data, char *arg)
{
	SysInfo* pSysInfo = GetSysInfo();
	if(pSysInfo == NULL)
		return -1;
	return sprintf(data, "%d", pSysInfo->lan_config.Mpeg41Ysize);
}
/***************************************************************************
 *                                                                         *
 ***************************************************************************/
int para_jpegxsize(char *data, char *arg)
{
	SysInfo* pSysInfo = GetSysInfo();
	if(pSysInfo == NULL)
		return -1;
	return sprintf(data, "%d", pSysInfo->lan_config.JpegXsize);
}
/***************************************************************************
 *                                                                         *
 ***************************************************************************/
int para_jpegysize(char *data, char *arg)
{
	SysInfo* pSysInfo = GetSysInfo();
	if(pSysInfo == NULL)
		return -1;
	return sprintf(data, "%d", pSysInfo->lan_config.JpegYsize);
}
/***************************************************************************
 *                                                                         *
 ***************************************************************************/
int para_socketauthority(char *data, char *arg)
{
	return sprintf(data, "%d", 0);
}
/***************************************************************************
 *                                                                         *
 ***************************************************************************/
int para_authoritychange(char *data, char *arg)
{
	return sprintf(data, "%d", 0);
}
/***************************************************************************
 *                                                                         *
 ***************************************************************************/
int para_supportmotion(char *data, char *arg)
{
	SysInfo* pSysInfo = GetSysInfo();
	if(pSysInfo == NULL)
		return -1;
	return sprintf(data, "%d", pSysInfo->lan_config.net.supportmotion);
}
/***************************************************************************
 *                                                                         *
 ***************************************************************************/
int para_supportwireless(char *data, char *arg)
{
	SysInfo* pSysInfo = GetSysInfo();
	if(pSysInfo == NULL)
		return -1;
	return sprintf(data, "%d", pSysInfo->lan_config.net.supportwireless);
}
/***************************************************************************
 *                                                                         *
 ***************************************************************************/
int para_serviceftpclient(char *data, char *arg)
{
	SysInfo* pSysInfo = GetSysInfo();
	if(pSysInfo == NULL)
		return -1;
	return sprintf(data, "%d", pSysInfo->lan_config.net.serviceftpclient);
}
/***************************************************************************
 *                                                                         *
 ***************************************************************************/
int para_servicesmtpclient(char *data, char *arg)
{
	SysInfo* pSysInfo = GetSysInfo();
	if(pSysInfo == NULL)
		return -1;
	return sprintf(data, "%d", pSysInfo->lan_config.net.servicesmtpclient);
}
/***************************************************************************
 *                                                                         *
 ***************************************************************************/
int para_servicepppoe(char *data, char *arg)
{
	SysInfo* pSysInfo = GetSysInfo();
	if(pSysInfo == NULL)
		return -1;
	return sprintf(data, "%d", pSysInfo->lan_config.net.servicepppoe);
}
/***************************************************************************
 *                                                                         *
 ***************************************************************************/
int para_servicesntpclient(char *data, char *arg)
{
	SysInfo* pSysInfo = GetSysInfo();
	if(pSysInfo == NULL)
		return -1;
	return sprintf(data, "%d", pSysInfo->lan_config.net.servicesntpclient);
}
/***************************************************************************
 *                                                                         *
 ***************************************************************************/
int para_serviceddnsclient(char *data, char *arg)
{
	SysInfo* pSysInfo = GetSysInfo();
	if(pSysInfo == NULL)
		return -1;
	return sprintf(data, "%d", pSysInfo->lan_config.net.serviceddnsclient);
}
/***************************************************************************
 *                                                                         *
 ***************************************************************************/
int para_supportmaskarea(char *data, char *arg)
{
	SysInfo* pSysInfo = GetSysInfo();
	if(pSysInfo == NULL)
		return -1;
	return sprintf(data, "%d", pSysInfo->lan_config.net.supportmaskarea);
}
/***************************************************************************
 *                                                                         *
 ***************************************************************************/
int para_machinecode(char *data, char *arg)
{
	SysInfo* pSysInfo = GetSysInfo();
	if(pSysInfo == NULL)
		return -1;
	return sprintf(data, "0x%x", pSysInfo->DeviceType);
}
/***************************************************************************
 *                                                                         *
 ***************************************************************************/
int para_maxchannel(char *data, char *arg)
{
	SysInfo* pSysInfo = GetSysInfo();
	if(pSysInfo == NULL)
		return -1;
	return sprintf(data, "%d", pSysInfo->lan_config.net.maxchannel);
}
/***************************************************************************
 *                                                                         *
 ***************************************************************************/
int para_supportrs485(char *data, char *arg)
{
	SysInfo* pSysInfo = GetSysInfo();
	if(pSysInfo == NULL)
		return -1;
	return sprintf(data, "%d", pSysInfo->lan_config.net.supportrs485);
}
/***************************************************************************
 *                                                                         *
 ***************************************************************************/
int para_supportrs232(char *data, char *arg)
{
	SysInfo* pSysInfo = GetSysInfo();
	if(pSysInfo == NULL)
		return -1;
	return sprintf(data, "%d", pSysInfo->lan_config.net.supportrs232);
}
/***************************************************************************
 *                                                                         *
 ***************************************************************************/
int para_ftppath(char *data, char *arg)
{
	SysInfo* pSysInfo = GetSysInfo();
	if(pSysInfo == NULL)
		return -1;
	return sprintf(data, "%s", pSysInfo->ftp_config.foldername);
}
/***************************************************************************
 *                                                                         *
 ***************************************************************************/
int para_layoutnum(char *data, char *arg)
{
	SysInfo* pSysInfo = GetSysInfo();
	if(pSysInfo == NULL)
		return -1;
	if ( *arg == '0' )
		return sprintf(data, "1");
	return sprintf(data, "%d", pSysInfo->lan_config.net.layoutnum);
}
/***************************************************************************
 *                                                                         *
 ***************************************************************************/
int para_supportmui(char *data, char *arg)
{
	SysInfo* pSysInfo = GetSysInfo();
	if(pSysInfo == NULL)
		return -1;
	return sprintf(data, "%d", pSysInfo->lan_config.net.supportmui);
}
/***************************************************************************
 *                                                                         *
 ***************************************************************************/
int para_mui(char *data, char *arg)
{
	SysInfo* pSysInfo = GetSysInfo();
	if(pSysInfo == NULL)
		return -1;
	return sprintf(data, "%d", pSysInfo->lan_config.net.mui);
}
/***************************************************************************
 *                                                                         *
 ***************************************************************************/
int para_supportsequence(char *data, char *arg)
{
	SysInfo* pSysInfo = GetSysInfo();
	if(pSysInfo == NULL)
		return -1;
	return sprintf(data, "%d", pSysInfo->lan_config.net.supportsequence);
}
/***************************************************************************
 *                                                                         *
 ***************************************************************************/
int para_quadmodeselect(char *data, char *arg)
{
	SysInfo* pSysInfo = GetSysInfo();
	if(pSysInfo == NULL)
		return -1;
	return sprintf(data, "%d", pSysInfo->lan_config.net.quadmodeselect);
}
/***************************************************************************
 *                                                                         *
 ***************************************************************************/
int para_serviceipfilter(char *data, char *arg)
{
	SysInfo* pSysInfo = GetSysInfo();
	if(pSysInfo == NULL)
		return -1;
	return sprintf(data, "%d", pSysInfo->lan_config.net.serviceipfilter);
}
/***************************************************************************
 *                                                                         *
 ***************************************************************************/
int para_oemflag0(char *data, char *arg)
{
	SysInfo* pSysInfo = GetSysInfo();
	if(pSysInfo == NULL)
		return -1;
	return sprintf(data, "%d", pSysInfo->lan_config.net.oemflag0);
}
/***************************************************************************
 *                                                                         *
 ***************************************************************************/
int para_supportdncontrol(char *data, char *arg)
{
	SysInfo* pSysInfo = GetSysInfo();
	if(pSysInfo == NULL)
		return -1;
	return sprintf(data, "%d", pSysInfo->lan_config.net.supportdncontrol);
}
/***************************************************************************
 *                                                                         *
 ***************************************************************************/
int para_supportavc(char *data, char *arg)
{
	SysInfo* pSysInfo = GetSysInfo();
	if(pSysInfo == NULL)
		return -1;
	return sprintf(data, "%d", pSysInfo->lan_config.net.supportavc);
}
/***************************************************************************
 *                                                                         *
 ***************************************************************************/
int para_supportaudio(char *data, char *arg)
{
	SysInfo* pSysInfo = GetSysInfo();
	if(pSysInfo == NULL)
		return -1;
	return sprintf(data, "%d", pSysInfo->lan_config.net.supportaudio);
}
/***************************************************************************
 *                                                                         *
 ***************************************************************************/
int para_supportptzpage(char *data, char *arg)
{
	SysInfo* pSysInfo = GetSysInfo();
	if(pSysInfo == NULL)
		return -1;
	return sprintf(data, "%d", pSysInfo->lan_config.net.supportptzpage);
}
/***************************************************************************
 *                                                                         *
 ***************************************************************************/
int para_dhcpconfig(char *data, char *arg)
{
	SysInfo* pSysInfo = GetSysInfo();
	if(pSysInfo == NULL)
		return -1;
	return sprintf(data, "%d", pSysInfo->lan_config.net.dhcp_config);
}
/***************************************************************************
 *                                                                         *
 ***************************************************************************/
int para_audioenable(char *data, char *arg)
{
	SysInfo* pSysInfo = GetSysInfo();
	if(pSysInfo == NULL)
		return -1;
	return sprintf(data, "%d", pSysInfo->lan_config.net.audioenable);
}
/***************************************************************************
 *                                                                         *
 ***************************************************************************/
int para_daylight(char *data, char *arg)
{
	SysInfo* pSysInfo = GetSysInfo();
	if(pSysInfo == NULL)
		return -1;
	return sprintf(data, "%d", pSysInfo->lan_config.net.daylight_time);
}
/***************************************************************************
 *                                                                         *
 ***************************************************************************/
int para_timeformat(char *data, char *arg)
{
	SysInfo* pSysInfo = GetSysInfo();
	if(pSysInfo == NULL)
		return -1;
	return sprintf(data, "%d", pSysInfo->lan_config.net.time_format);
}
/***************************************************************************
 *                                                                         *
 ***************************************************************************/
int para_pppoeenable(char *data, char *arg)
{
	return sprintf(data, "0");
}
/***************************************************************************
 *                                                                         *
 ***************************************************************************/
int para_mpeg4desired(char *data, char *arg)
{
	return sprintf(data, "1");
}
/***************************************************************************
 *                                                                         *
 ***************************************************************************/
int para_mpeg4cenable(char *data, char *arg)
{
	return sprintf(data, "0");
}
/***************************************************************************
 *                                                                         *
 ***************************************************************************/
int para_mpeg4cvalue(char *data, char *arg)
{
     succeed_type iret = succeed_type_failed;
	 video_enc_param imagesource;
	 unsigned int bitrate;
	 iret = Gbshm_paramGet(global_boa_private_handle.gbshm_handle , PARA_CMD_SYSTEM_VIDEO_ENC_PARAM, &imagesource, sizeof(imagesource));
	 SYS_INFO("\n\r!!!!!!!!!!!!!!!!!!!GBSHM_PARA_GET imagempeg4cvalue = %d iret = %d!!!!!!!!!!!!!!!!!!!!!!!!\r\n",imagesource.video_enc_bitrate, iret);
	 bitrate = imagesource.video_enc_bitrate / 1000;
	 return sprintf(data ,"%d",bitrate);
}

/***************************************************************************
 *                                                                         *
 ***************************************************************************/
int para_mpeg42cvalue(char *data, char *arg)
{
	SysInfo* pSysInfo = GetSysInfo();
	if(pSysInfo == NULL)
		return -1;
	return sprintf(data, "%d", pSysInfo->lan_config.nMpeg42bitrate/1000);
}
/***************************************************************************
 *                                                                         *
 ***************************************************************************/
int para_mpeg4resname(char *data, char *arg)
{
	if ( *arg == '0' )
		return sprintf(data, "1280x720");
	else if ( *arg == '1' )
		return sprintf(data, "640x480");
	else if ( *arg == 'a' || *arg == '\0' )
		return sprintf(data, "1280x720;640x480");
	return -1;
}
/***************************************************************************
 *                                                                         *
 ***************************************************************************/
int para_mpeg42resname(char *data, char *arg)
{
	if ( *arg == '0' )
		return sprintf(data, "640x360");
	else if ( *arg == '1' )
		return sprintf(data, "352x192");
	else if ( *arg == 'a' || *arg == '\0' )
		return sprintf(data, "640x360;352x192");
	return -1;
}
/***************************************************************************
 *                                                                         *
 ***************************************************************************/
int para_resolutionname(char *data, char *arg)
{
	if ( *arg == '0' )
		return sprintf(data, "720");
	else if ( *arg == '1' )
		return sprintf(data, "VGA");
	else if ( *arg == '2' )
		return sprintf(data, "CIF");
	else if ( *arg == '3' )
		return sprintf(data, "640x360");
	else if ( *arg == 'a' || *arg == '\0' )
		return sprintf(data, "720;VGA;CIF;640x360");
	return -1;
}
/***************************************************************************
 *here                                                                     *
 ***************************************************************************/
int para_videocodecresname(char *data, char *arg)
{
	/*
	   char *resname_list[] =
	   {
	   "H264:720;H264:SXGA;H264:D1;H264:1080", "MPEG4:720;MPEG4:SXGA;MPEG4:D1;MPEG4:1080", "JPG:1600x1200;JPG:2048x1536",
	   "H264:720,JPEG:VGA;H264:D1,JPEG:D1;H264:720,JPEG:720", "MPEG4:720,JPEG:VGA;MPEG4:D1,JPEG:D1;MPEG4:720,JPEG:720",
	   "H264:720,H264:CIF;H264:D1,H264:D1;H264:1080,H264:CIF", "MPEG4:720,MPEG4:CIF;MPEG4:D1,MPEG4:D1;MPEG4:1080,MPEG4:CIF",
	   "H264:D1,MPEG4:D1",
	   "H264:720,JPEG:VGA,H264:CIF", "MPEG4:720,JPEG:VGA,MPEG4:CIF",
	   };
	 */
	char *resname_list[] =
	{ 

		"H264:D1;H264:CIF", "JPEG4:D1;JPEG:CIF",
	};
	int tblsize = sizeof(resname_list)/sizeof(resname_list[0]);
	//fprintf(stderr, "\nresname_list size: %d\n", tblsize);
	int i = 0;
	char rtn_msg[360] = "\0";
	if ( *arg == 'a' || *arg == '\0' )
	{
		for (i = 0; i < tblsize; i++)
		{
			if (i > 0)
				strcat(rtn_msg, "@");
			strcat(rtn_msg, resname_list[i]);
		}
		return sprintf(data, "%s", rtn_msg);
	}
	else if (*arg >= '0' && *arg <= '9')
	{
		i = atoi(arg);
		if (i < tblsize)
			return sprintf(data, "%s", resname_list[i]);
	}
	return -1;
}
/***************************************************************************
 *                                                                         *
 ***************************************************************************/
int para_videocodecmodename(char *data, char *arg)
{
	char *modename_list[] =
	{
		"Single H.264", "Single MPEG4", "MegaPixel JPEG",
		"H.264 + JPEG", "MPEG4 + JPEG",
		"Dual H.264", "Dual MPEG4", "H264 + MPEG4",
		"Dual H264 + JPEG", "Dual MPEG4 + JPEG",
	};
	int tblsize = sizeof(modename_list)/sizeof(modename_list[0]);
	int i = 0;
	char rtn_msg[256] = "\0";
	if ( *arg == 'a' || *arg == '\0' )
	{
		for (i = 0; i < tblsize; i++)
		{
			if (i > 0)
				strcat(rtn_msg, ";");
			strcat(rtn_msg, modename_list[i]);
		}
		return sprintf(data, "%s", rtn_msg);
	}
	else if (*arg >= '0' && *arg <= '9')
	{
		i = atoi(arg);
		if (i < tblsize)
			return sprintf(data, "%s", modename_list[i]);
	}
	return -1;
}
/***************************************************************************
 *  2010-11-12                                                                       *
 ***************************************************************************/
int para_videocodecmode1(char *data, char *arg)
{
	/*
	   SysInfo* pSysInfo = GetSysInfo();
	   if(pSysInfo == NULL)
	   return -1;
	   int i=0;
	   i=pSysInfo->lan_config.nVideocodecmode;
	   if(i==0)
	   return sprintf(data,"H.264");
	   else if(i==1)
	   return sprintf(data,"MPEG4");
	   else if(i==2)
	   return sprintf(data,"JPEG");
	   else if(i==3)
	   return sprintf(data,"H.264;JPEG");
	   else if(i==4)
	   return sprintf(data,"MPEG4;JPEG");
	   else if(i==5)
	   return sprintf(data,"H.264");
	   else if(i==6)
	   return sprintf(data,"MPEG4");
	   else if(i==7)
	   return sprintf(data,"H.264;MPEG4");
	   else if(i==8)
	   return sprintf(data,"H.264;JPEG");
	   else if(i==9)
	   return sprintf(data,"MPEG4;JPEG");
	 */

	return sprintf(data,"%d",2);
	//return sprintf(data, "h264;mpeg4;jpeg");
}
/***************************************************************************
 *                                                                         *
 ***************************************************************************/
int para_videoresmenu(char *data, char *arg)
{
	 succeed_type iret = succeed_type_failed;
	 video_enc_param coderes;
	 iret = Gbshm_paramGet(global_boa_private_handle.gbshm_handle , PARA_CMD_SYSTEM_VIDEO_ENC_PARAM, &coderes, sizeof(coderes));
	 SYS_INFO("\n\r!!!!!!!!!!!!!!!!!!!GBSHM_PARA_GET imageres = %d iret = %d!!!!!!!!!!!!!!!!!!!!!!!!\r\n",coderes.video_enc_resolution, iret);
     switch(coderes.video_enc_resolution)
	 {
         case VIDEO_ENC_D1:
		 	return sprintf(data ,"%d",0);
		 case VIDEO_ENC_CIF:
		 	return sprintf(data ,"%d",1);
	 }
}
/**************************************************************************
 *                                                                        *
 **************************************************************************/
 int para_videocodecmenu(char *data, char *arg)
{
     succeed_type iret = succeed_type_failed;
	 video_enc_param codemenu;
	 iret = Gbshm_paramGet(global_boa_private_handle.gbshm_handle , PARA_CMD_SYSTEM_VIDEO_ENC_PARAM, &codemenu, sizeof(codemenu));
	 SYS_INFO("\n\r!!!!!!!!!!!!!!!!!!!GBSHM_PARA_GET imagemenu = %d iret = %d!!!!!!!!!!!!!!!!!!!!!!!!\r\n",codemenu.video_enc_mode, iret);
     switch(codemenu.video_enc_mode)
	 {
         case VIDEO_ENC_H264:
		 	return sprintf(data ,"%d",0);
		 case VIDEO_ENC_JPEG:
		 	return sprintf(data ,"%d",1);
	 }
}
/**************************************************************************
 *                                                                        *
 **************************************************************************/
int para_videoframemenu(char *data, char *arg)
{
     succeed_type iret = succeed_type_failed;
	 video_enc_param codeframe;
	 iret = Gbshm_paramGet(global_boa_private_handle.gbshm_handle , PARA_CMD_SYSTEM_VIDEO_ENC_PARAM, &codeframe, sizeof(codeframe));
	 SYS_INFO("\n\r!!!!!!!!!!!!!!!!!!!GBSHM_PARA_GET imageframe = %d iret = %d!!!!!!!!!!!!!!!!!!!!!!!!\r\n",codeframe.video_enc_framerate, iret);
     switch(codeframe.video_enc_framerate)
	 {
	     case VIDEO_ENC_30FPS:
		 	return sprintf(data ,"%d",0);
		 case VIDEO_ENC_25FPS:
		 	return sprintf(data ,"%d",0);
		 case VIDEO_ENC_15FPS:
		 	return sprintf(data ,"%d",1);
		 case VIDEO_ENC_7FPS:
		 	return sprintf(data ,"%d",2);
	 }
}
/**************************************************************************
 *                                                                        *
 **************************************************************************/
int para_videokeyframe(char *data, char *arg)
{
     succeed_type iret = succeed_type_failed;
	 video_enc_param keyframe;
	 iret = Gbshm_paramGet(global_boa_private_handle.gbshm_handle , PARA_CMD_SYSTEM_VIDEO_ENC_PARAM, &keyframe, sizeof(keyframe));
	 SYS_INFO("\n\r!!!!!!!!!!!!!!!!!!!GBSHM_PARA_GET imageframe = %d iret = %d!!!!!!!!!!!!!!!!!!!!!!!!\r\n",keyframe.keyFrameInterval, iret);
	 return sprintf(data ,"%d",keyframe.keyFrameInterval);
}
/***************************************************************************
 *                                                                         *
 ***************************************************************************/
int para_mpeg4qualityname(char *data, char *arg)
{
	if ( *arg == '0' )
		return sprintf(data, "high");
	else if ( *arg == '1' )
		return sprintf(data, "normal");
	else if ( *arg == '2' )
		return sprintf(data, "low");
	else if ( *arg == 'a' || *arg == '\0' )
		return sprintf(data, "high;normal;low");
	return -1;
}
/***************************************************************************
 *                                                                         *
 ***************************************************************************/
int para_qualityname(char *data, char *arg)
{
	if ( *arg == '0' )
		return sprintf(data, "high");
	else if ( *arg == '1' )
		return sprintf(data, "normal");
	else if ( *arg == '2' )
		return sprintf(data, "low");
	else if ( *arg == 'a' || *arg == '\0' )
		return sprintf(data, "high;normal;low");
	return -1;
}
/***************************************************************************
 *                                                                         *
 ***************************************************************************/
int para_motionylimit(char *data, char *arg)
{
	return sprintf(data, "720");
}
/***************************************************************************
 *                                                                         *
 ***************************************************************************/
int para_ratecontrol(char *data, char *arg)
{
	SysInfo* pSysInfo = GetSysInfo();
	if(pSysInfo == NULL)
		return -1;
	return sprintf(data, "%d", pSysInfo->lan_config.nRateControl);
}
int para_ratecontrolname(char *data, char *arg)
{
	if ( *arg == '0' )
		return sprintf(data, "OFF");
	else if ( *arg == '1' )
		return sprintf(data, "CVBR");
	else if ( *arg == '2' )
		return sprintf(data, "CBR");
	else if ( *arg == 'a' || *arg == '\0' )
		return sprintf(data, "OFF;CVBR;CBR");
	return -1;
}

/***************************************************************************
 *                                                                         *
 ***************************************************************************/
/*static char *night_ratename[3][3] = {{"30fps","----","----"},
  {"----","15fps","----"},
  {"----","----","7.5fps"}};*/
static char *night_ratename[4] = {"30","15","5","Auto"};
//static char *night_ratename2[4] = {"15","7.5","2.5","Auto"};
static char *day_ratename[4][5] = 	{{"30fps","15fps","7.5fps","3fps","1fps"},
	{"----","15fps","7.5fps","3fps","1fps"},
	{"24fps","12fps","6fps","3fps","1fps"},
	{"----","12fps","6fps","3fps","1fps"}};
int para_mpeg41framerate(char *data, char *arg)
{
	SysInfo* pSysInfo = GetSysInfo();
	if(pSysInfo == NULL)
		return -1;
	return sprintf(data, "%d", pSysInfo->lan_config.nMpeg41frmrate);
}
int para_mpeg41frameratename(char *data, char *arg)
{
	char **ratename = NULL;
	SysInfo* pSysInfo = GetSysInfo();
	if(pSysInfo == NULL)
		return -1;
	unsigned char daynight = pSysInfo->lan_config.nDayNight;
	if (pSysInfo->lan_config.Supportstream2 == 0 && pSysInfo->lan_config.Supportstream5 == 0)
		return sprintf(data, "N/A");
	if (daynight == 0)
		//ratename = night_ratename;
		return sprintf(data, "%s", night_ratename[3]);
	else
		ratename = day_ratename[0];
	if ( *arg == '0' )
		return sprintf(data, "%s", ratename[0]);
	else if ( *arg == '1' )
		return sprintf(data, "%s", ratename[1]);
	else if ( *arg == '2' )
		return sprintf(data, "%s", ratename[2]);
	else if ( *arg == 'a' || *arg == '\0' )
	{
		return sprintf(data, "%s;%s;%s", ratename[0], ratename[1], ratename[2]);
	}
	return -1;
}
/***************************************************************************
 *                                                                         *
 ***************************************************************************/
int para_mpeg42framerate(char *data, char *arg)
{
	SysInfo* pSysInfo = GetSysInfo();
	if(pSysInfo == NULL)
		return -1;
	return sprintf(data, "%d", pSysInfo->lan_config.nMpeg42frmrate);
}
int para_mpeg42frameratename(char *data, char *arg)
{
	char **ratename = NULL;
	SysInfo* pSysInfo = GetSysInfo();
	if(pSysInfo == NULL)
		return -1;
	unsigned char daynight = pSysInfo->lan_config.nDayNight;
	//unsigned char frate = pSysInfo->lan_config.nMpeg41frmrate;
	if (pSysInfo->lan_config.Supportstream3 == 0 && pSysInfo->lan_config.Supportstream6 == 0)
		return sprintf(data, "N/A");
	if (daynight == 0) {
		/*if (pSysInfo->lan_config.nVideocodecmode == 0)
		  ratename = night_ratename2;
		  else
		  ratename = night_ratename;
		  return sprintf(data, "%s", ratename[frate]);*/
		return sprintf(data, "%s", night_ratename[3]);
	}
	else if (pSysInfo->lan_config.nVideocodecmode == 8)
		ratename = day_ratename[1];
	else
		ratename = day_ratename[0];

	if ( *arg == '0' )
		return sprintf(data, "%s", ratename[0]);
	else if ( *arg == '1' )
		return sprintf(data, "%s", ratename[1]);
	else if ( *arg == '2' )
		return sprintf(data, "%s", ratename[2]);
	else if ( *arg == 'a' || *arg == '\0' )
		return sprintf(data, "%s;%s;%s", ratename[0], ratename[1], ratename[2]);
	return -1;
}
/***************************************************************************
 *                                                                         *
 ***************************************************************************/
int para_jpegframerate(char *data, char *arg)
{
	SysInfo* pSysInfo = GetSysInfo();
	if(pSysInfo == NULL)
		return -1;
	return sprintf(data, "%d", pSysInfo->lan_config.nJpegfrmrate);
}
int para_jpegframeratename(char *data, char *arg)
{
	char **ratename = NULL;
	SysInfo* pSysInfo = GetSysInfo();
	if(pSysInfo == NULL)
		return -1;
	unsigned char daynight = pSysInfo->lan_config.nDayNight;
	//unsigned char frate = pSysInfo->lan_config.nMpeg41frmrate;
	if (pSysInfo->lan_config.Supportstream1 == 0)
		return sprintf(data, "N/A");
	if (daynight == 0) {
		return sprintf(data, "%s", night_ratename[3]);
	}
	else if (pSysInfo->lan_config.nVideocodecmode == 8)
		ratename = day_ratename[1]; //limited
	else if (pSysInfo->lan_config.nVideocodecmode == 2) {
		if (pSysInfo->lan_config.nVideocodecres == 0)
			ratename = day_ratename[2];
		else //if (pSysInfo->lan_config.nVideocodecres == 1)
			ratename = day_ratename[1]; //limited
	}
	else
		ratename = day_ratename[0];

	if ( *arg == '0' )
		return sprintf(data, "%s", ratename[0]);
	else if ( *arg == '1' )
		return sprintf(data, "%s", ratename[1]);
	else if ( *arg == '2' )
		return sprintf(data, "%s", ratename[2]);
	else if ( *arg == '3' )
		return sprintf(data, "%s", ratename[3]);
	else if ( *arg == '4' )
		return sprintf(data, "%s", ratename[4]);
	else if ( *arg == 'a' || *arg == '\0' )
		return sprintf(data, "%s;%s;%s;%s;%s", ratename[0], ratename[1], ratename[2], ratename[3], ratename[4]);
	return -1;
}
/***************************************************************************
 *                                                                         *
 ***************************************************************************/
int para_quadmodeselectname(char *data, char *arg)
{
	return sprintf(data, "30fps");
}
/***************************************************************************
 *                                                                         *
 ***************************************************************************/
int para_videomodename(char *data, char *arg)
{
	if ( *arg == '0' )
		return sprintf(data, "JPEG");
	else if ( *arg == '1' )
		return sprintf(data, "MPEG");
	else if ( *arg == '2' )
		return sprintf(data, "JPEG+MPEG");
	else if ( *arg == '3' )
		return sprintf(data, "MPEG+MPEG");
	else if ( *arg == '4' )
		return sprintf(data, "JPEG+MPEG+MPEG");
	else if ( *arg == 'a' || *arg == '\0' )
		return sprintf(data, "JPEG;MPEG;JPEG+MPEG;MPEG+MPEG;JPEG+MPEG+MPEG");
	return -1;
}
/***************************************************************************
 *                                                                         *
 ***************************************************************************/
int para_videomode(char *data, char *arg)
{
	SysInfo* pSysInfo = GetSysInfo();
	if(pSysInfo == NULL)
		return -1;
	return sprintf(data, "%d", pSysInfo->lan_config.nVideomode);
}
/***************************************************************************
 *                                                                         *
 ***************************************************************************/
int para_supportagc(char *data, char *arg)
{
	return sprintf(data, "0");
}
/***************************************************************************
 *                                                                         *
 ***************************************************************************/
int para_agc(char *data, char *arg)
{
	return sprintf(data, "0");
}
/***************************************************************************
 *                                                                         *
 ***************************************************************************/
int para_colorkiller(char *data, char *arg)
{
	return sprintf(data, "128");
}
/***************************************************************************
 *                                                                         *
 ***************************************************************************/
int para_fluorescent(char *data, char *arg)
{
	return sprintf(data, "128");
}
/***************************************************************************
 *                                                                         *
 ***************************************************************************/
int para_mirror(char *data, char *arg)
{
	return sprintf(data, "0");
}
/***************************************************************************
 *                                                                         *
 ***************************************************************************/
int para_blc(char *data, char *arg)
{
	SysInfo* pSysInfo = GetSysInfo();
	if(pSysInfo == NULL)
		return -1;
	return sprintf(data, "%d", pSysInfo->lan_config.nBacklightControl);
}
/***************************************************************************
 *                                                                         *
 ***************************************************************************/
int para_kelvin(char *data, char *arg)
{
	return sprintf(data, "0");
}
/***************************************************************************
 *                                                                         *
 ***************************************************************************/
int para_supporthue(char *data, char *arg)
{
	return sprintf(data, "0");
}
/***************************************************************************
 *                                                                         *
 ***************************************************************************/
int para_supportexposure(char *data, char *arg)
{
	return sprintf(data, "0");
}
/***************************************************************************
 *                                                                         *
 ***************************************************************************/
int para_supportfluorescent(char *data, char *arg)
{
	return sprintf(data, "0");
}
/***************************************************************************
 *                                                                         *
 ***************************************************************************/
int para_supportmirros(char *data, char *arg)
{
	return sprintf(data, "0");
}
/***************************************************************************
 *                                                                        *
 ***************************************************************************/
int para_supportkelvin(char *data, char *arg)
{
	return sprintf(data, "0");
}
/***************************************************************************
 *                                                                         *
 ***************************************************************************/
int para_senseup(char *data, char *arg)
{
	return sprintf(data, "1");
}
/***************************************************************************
 *                                                                         *
 ***************************************************************************/
int para_supportsenseup(char *data, char *arg)
{
	return sprintf(data, "0");
}
/***************************************************************************
 *                                                                         *
 ***************************************************************************/
int para_supportmaxagcgain(char *data, char *arg)
{
	return sprintf(data, "0");
}
/***************************************************************************
 *                                                                         *
 ***************************************************************************/
int para_supportblcmode(char *data, char *arg)
{
	return sprintf(data, "0");

	/*	SysInfo* pSysInfo = GetSysInfo();
		if(pSysInfo == NULL)
		return -1;
		return sprintf(data, "%d", pSysInfo->lan_config.nBacklightControl);*/
}
/***************************************************************************
 *                                                                         *
 ***************************************************************************/
int para_blcmode(char *data, char *arg)
{
	SysInfo* pSysInfo = GetSysInfo();
	if(pSysInfo == NULL)
		return -1;
	return sprintf(data, "%d", pSysInfo->lan_config.nBacklightControl);
}
/***************************************************************************
 *                                                                         *
 ***************************************************************************/
int para_supporthspeedshutter(char *data, char *arg)
{
	return sprintf(data, "0");
}
/***************************************************************************
 *                                                                         *
 ***************************************************************************/
int para_hspeedshutter(char *data, char *arg)
{
	return sprintf(data, "0");
}
/***************************************************************************
 *                                                                         *
 ***************************************************************************/
int para_awbname(char *data, char *arg)
{
	if ( *arg == '0' )
		return sprintf(data, "auto");
	else if ( *arg == '1' )
		return sprintf(data, "indoor");
	else if ( *arg == '2' )
		return sprintf(data, "outdoor");
	else if ( *arg == 'a' || *arg == '\0' )
		return sprintf(data, "auto;indoor;outdoor");
	return -1;
}
/***************************************************************************
 *                                                                         *
 ***************************************************************************/
int para_awb(char *data, char *arg)
{
	SysInfo* pSysInfo = GetSysInfo();
	if(pSysInfo == NULL)
		return -1;
	return sprintf(data, "%d", pSysInfo->lan_config.nWhiteBalance);
}
/***************************************************************************
 *                                                                         *
 ***************************************************************************/
int para_tvcable(char *data, char *arg)
{
	SysInfo* pSysInfo = GetSysInfo();
	if(pSysInfo == NULL)
		return -1;
	return sprintf(data, "%d", pSysInfo->lan_config.nTVcable);
}
/***************************************************************************
 *                                                                         *
 ***************************************************************************/
char *binskip[3] = {"binning","skipping","window"};
int para_binningname(char *data, char *arg)
{
	/*SysInfo* pSysInfo = GetSysInfo();
	  if(pSysInfo == NULL)
	  return -1;
	  if (pSysInfo->lan_config.nVideocodecmode == 1 &&
	  pSysInfo->lan_config.nVideocodecres == 1)
	  binname = binskip[1];
	  else
	  binname = binskip[0];*/
	if ( *arg == '0' )
		return sprintf(data, "%s", binskip[0]);
	else if ( *arg == '1' )
		return sprintf(data, "%s", binskip[1]);
	else if ( *arg == '2' )
		return sprintf(data, "%s", binskip[2]);
	else if ( *arg == 'a' || *arg == '\0' )
		return sprintf(data, "%s;%s;%s", binskip[0], binskip[1], binskip[2]);
	return -1;
}
/***************************************************************************
 *                                                                         *
 ***************************************************************************/
int para_binning(char *data, char *arg)
{
	SysInfo* pSysInfo = GetSysInfo();
	if(pSysInfo == NULL)
		return -1;
	return sprintf(data, "%d", pSysInfo->lan_config.nBinning);
}
/***************************************************************************
 *                                                                         *
 ***************************************************************************/
int para_maxagcgainname(char *data, char *arg)
{
	return sprintf(data, "dark");
}
/***************************************************************************
 *                                                                         *
 ***************************************************************************/
int para_maxagcgain(char *data, char *arg)
{
	return sprintf(data, "0");
}
/***************************************************************************
 *                                                                         *
 ***************************************************************************/
int para_daynightname(char *data, char *arg)
{
	char *namelist[] = {"night","day"};
	if ( *arg == 'a' || *arg == '\0' )
		return sprintf(data, "%s;%s", namelist[0], namelist[1]);
	else if (*arg >= '0' && *arg <= '1') {
		int idx = atoi(arg);
		return sprintf(data, "%s", namelist[idx]);
	}
	return -1;
}
/***************************************************************************
 * dan                                                                     *
 ***************************************************************************/
int para_daynight(char *data, char *arg)
{
	SysInfo* pSysInfo = GetSysInfo();
	if(pSysInfo == NULL)
		return -1;
	return sprintf(data, "%d", pSysInfo->lan_config.nDayNight);
}
/***************************************************************************
 *                                                                         *
 ***************************************************************************/
int para_blcmodename(char *data, char *arg)
{
	if ( *arg == '0' )
		return sprintf(data, "normal");
	else if ( *arg == '1' )
		return sprintf(data, "blcmode");
	else if ( *arg == 'a' || *arg == '\0' )
		return sprintf(data, "normal;blcmode");
	return -1;
}
/***************************************************************************
 *                                                                         *
 ***************************************************************************/
int para_backlight(char *data, char *arg)
{
	SysInfo* pSysInfo = GetSysInfo();
	if(pSysInfo == NULL)
		return -1;
	return sprintf(data, "%d", pSysInfo->lan_config.nBackLight);
}
/***************************************************************************
 *                                                                         *
 ***************************************************************************/
int para_brightness(char *data, char *arg)
{
	 succeed_type iret = succeed_type_failed;
	 ad_setting_param adsetting;
	 iret = Gbshm_paramGet(global_boa_private_handle.gbshm_handle , PARA_CMD_SYSTEM_AD_SETTING_PARAM, &adsetting, sizeof(adsetting));
	 SYS_INFO("\n\r!!!!!!!!!!!!!!!!!!!GBSHM_PARA_GET ad_setting_bright = %d iret = %d!!!!!!!!!!!!!!!!!!!!!!!!\r\n",adsetting.ad_bright, iret);
	 return sprintf(data ,"%d",adsetting.ad_bright);
}
/***************************************************************************
 *                                                                         *
 ***************************************************************************/
int para_contrast(char *data, char *arg)
{
/*
	SysInfo* pSysInfo = GetSysInfo();
	if(pSysInfo == NULL)
		return -1;
	return sprintf(data, "%d", pSysInfo->lan_config.nContrast);
*/
     succeed_type iret = succeed_type_failed;
	 ad_setting_param adsetting;
	 iret = Gbshm_paramGet(global_boa_private_handle.gbshm_handle , PARA_CMD_SYSTEM_AD_SETTING_PARAM, &adsetting, sizeof(adsetting));
	 SYS_INFO("\n\r!!!!!!!!!!!!!!!!!!!GBSHM_PARA_GET ad_setting_contrast = %d iret = %d!!!!!!!!!!!!!!!!!!!!!!!!\r\n",adsetting.ad_contrast, iret);
	 return sprintf(data ,"%d",adsetting.ad_contrast);
}
/***************************************************************************
 *                                                                         *
 ***************************************************************************/
int para_hue(char *data, char *arg)
{
	return sprintf(data, "128");
}
/***************************************************************************
 *                                                                         *
 ***************************************************************************/
int para_saturation(char *data, char *arg)
{
     succeed_type iret = succeed_type_failed;
	 ad_setting_param adsetting;
	 iret = Gbshm_paramGet(global_boa_private_handle.gbshm_handle , PARA_CMD_SYSTEM_AD_SETTING_PARAM, &adsetting, sizeof(adsetting));
	 SYS_INFO("\n\r!!!!!!!!!!!!!!!!!!!GBSHM_PARA_GET ad_setting_saturation = %d iret = %d!!!!!!!!!!!!!!!!!!!!!!!!\r\n",adsetting.ad_saturation, iret);
	 return sprintf(data ,"%d",adsetting.ad_saturation);
}
/***************************************************************************
 *                                                                         *
 ***************************************************************************/
int para_exposure(char *data, char *arg)
{
	SysInfo* pSysInfo = GetSysInfo();
	if(pSysInfo == NULL)
		return -1;
	return sprintf(data, "%d", pSysInfo->lan_config.nSaturation);
}
/***************************************************************************
 *here                                                                     *
 ***************************************************************************/
int para_stream1xsize(char *data, char *arg)
{
	SysInfo* pSysInfo = GetSysInfo();
	if(pSysInfo == NULL)
		return -1;
	return sprintf(data, "%d", pSysInfo->lan_config.JpegXsize);
}
/***************************************************************************
 *                                                                         *
 ***************************************************************************/
int para_stream1ysize(char *data, char *arg)
{
	SysInfo* pSysInfo = GetSysInfo();
	if(pSysInfo == NULL)
		return -1;
	return sprintf(data, "%d", pSysInfo->lan_config.JpegYsize);
}
/***************************************************************************
 *                                                                         *
 ***************************************************************************/
int para_stream2xsize(char *data, char *arg)
{
	SysInfo* pSysInfo = GetSysInfo();
	if(pSysInfo == NULL)
		return -1;
	return sprintf(data, "%d", pSysInfo->lan_config.Mpeg41Xsize);
}
/***************************************************************************
 *                                                                         *
 ***************************************************************************/
int para_stream2ysize(char *data, char *arg)
{
	SysInfo* pSysInfo = GetSysInfo();
	if(pSysInfo == NULL)
		return -1;
	return sprintf(data, "%d", pSysInfo->lan_config.Mpeg41Ysize);
}
/***************************************************************************
 *                                                                         *
 ***************************************************************************/
int para_stream3xsize(char *data, char *arg)
{
	SysInfo* pSysInfo = GetSysInfo();
	if(pSysInfo == NULL)
		return -1;
	return sprintf(data, "%d", pSysInfo->lan_config.Mpeg42Xsize);
}
/***************************************************************************
 *                                                                         *
 ***************************************************************************/
int para_stream3ysize(char *data, char *arg)
{
	SysInfo* pSysInfo = GetSysInfo();
	if(pSysInfo == NULL)
		return -1;
	return sprintf(data, "%d", pSysInfo->lan_config.Mpeg42Ysize);
}
/***************************************************************************
 * Added in DM365 IPCAM                                                    *
 ***************************************************************************/
int para_stream5xsize(char *data, char *arg)
{
	SysInfo* pSysInfo = GetSysInfo();
	if(pSysInfo == NULL)
		return -1;
	return sprintf(data, "%d", pSysInfo->lan_config.Avc1Xsize);
}
/***************************************************************************
 *                                                                         *
 ***************************************************************************/
int para_stream5ysize(char *data, char *arg)
{
	SysInfo* pSysInfo = GetSysInfo();
	if(pSysInfo == NULL)
		return -1;
	return sprintf(data, "%d", pSysInfo->lan_config.Avc1Ysize);
}
/***************************************************************************
 *                                                                         *
 ***************************************************************************/
int para_stream6xsize(char *data, char *arg)
{
	SysInfo* pSysInfo = GetSysInfo();
	if(pSysInfo == NULL)
		return -1;
	return sprintf(data, "%d", pSysInfo->lan_config.Avc2Xsize);
}
/***************************************************************************
 *                                                                         *
 ***************************************************************************/
int para_stream6ysize(char *data, char *arg)
{
	SysInfo* pSysInfo = GetSysInfo();
	if(pSysInfo == NULL)
		return -1;
	return sprintf(data, "%d", pSysInfo->lan_config.Avc2Ysize);
}
/***************************************************************************
 *                                                                         *
 ***************************************************************************/
int para_stream1name(char *data, char *arg)
{
	SysInfo* pSysInfo = GetSysInfo();
	if(pSysInfo == NULL)
		return -1;
	return sprintf(data, "JPG(%dx%d)",	pSysInfo->lan_config.JpegXsize,
			pSysInfo->lan_config.JpegYsize);
}
/***************************************************************************
 *                                                                         *
 ***************************************************************************/
int para_stream2name(char *data, char *arg)
{
	SysInfo* pSysInfo = GetSysInfo();
	if(pSysInfo == NULL)
		return -1;
	return sprintf(data, "MPEG4(%dx%d)",	pSysInfo->lan_config.Mpeg41Xsize,
			pSysInfo->lan_config.Mpeg41Ysize);
}
/***************************************************************************
 *                                                                         *
 ***************************************************************************/
int para_stream3name(char *data, char *arg)
{
	SysInfo* pSysInfo = GetSysInfo();
	if(pSysInfo == NULL)
		return -1;
	return sprintf(data, "MPEG4(%dx%d)",	pSysInfo->lan_config.Mpeg42Xsize,
			pSysInfo->lan_config.Mpeg42Ysize);
}
/***************************************************************************
 *                                                                         *
 ***************************************************************************/
int para_stream4name(char *data, char *arg)
{
	SysInfo* pSysInfo = GetSysInfo();
	if(pSysInfo == NULL)
		return -1;
	return sprintf(data, "JPG-2(%dx%d)",	pSysInfo->lan_config.JpegXsize,
			pSysInfo->lan_config.JpegYsize);
}
/***************************************************************************
 *                                                                         *
 ***************************************************************************/
int para_stream5name(char *data, char *arg)
{
	SysInfo* pSysInfo = GetSysInfo();
	if(pSysInfo == NULL)
		return -1;
	return sprintf(data, "H.264(%dx%d)",	pSysInfo->lan_config.Avc1Xsize,
			pSysInfo->lan_config.Avc1Ysize);
}
/***************************************************************************
 *                                                                         *
 ***************************************************************************/
int para_stream6name(char *data, char *arg)
{
	SysInfo* pSysInfo = GetSysInfo();
	if(pSysInfo == NULL)
		return -1;
	return sprintf(data, "H.264(%dx%d)",	pSysInfo->lan_config.Avc2Xsize,
			pSysInfo->lan_config.Avc2Ysize);
}
/***************************************************************************
 *                                                                         *
 ***************************************************************************/
int para_sharpness(char *data, char *arg)
{
	 succeed_type iret = succeed_type_failed;
	 ad_setting_param adsetting;
	 iret = Gbshm_paramGet(global_boa_private_handle.gbshm_handle , PARA_CMD_SYSTEM_AD_SETTING_PARAM, &adsetting, sizeof(adsetting));
	 SYS_INFO("\n\r!!!!!!!!!!!!!!!!!!!GBSHM_PARA_GET ad_setting_hue = %d iret = %d!!!!!!!!!!!!!!!!!!!!!!!!\r\n",adsetting.ad_hue, iret);
	 return sprintf(data ,"%d",adsetting.ad_hue);
}
/***************************************************************************
 *                                                                         *
 ***************************************************************************/
int para_timezonename(char *data, char *arg)
{
	static char *TZname[] = {
		"GMT-12 Eniwetok, Kwajalein",
		"GMT-11 Midway Island, Samoa",
		"GMT-10 Hawaii",
		"GMT-09 Alaska",
		"GMT-08 Pacific Time (US & Canada), Tijuana",
		"GMT-07 Mountain Time (US & Canada), Arizona",
		"GMT-06 Central Time (US & Canada), Mexico City, Tegucigalpa, Saskatchewan",
		"GMT-05 Eastern Time (US & Canada), Indiana(East), Bogota, Lima",
		"GMT-04 Atlantic Time (Canada), Caracas, La Paz",
		"GMT-03 Brasilia, Buenos Aires, Georgetown",
		"GMT-02 Mid-Atlantic",
		"GMT-01 Azores, Cape Verdes Is.",
		"GMT+00 GMT, Dublin, Edinburgh, London, Lisbon, Monrovia, Casablanca",
		"GMT+01 Berlin, Stockholm, Rome, Bern, Brussels, Vienna, Paris, Madrid, Amsterdam, Prague, Warsaw, Budapest",
		"GMT+02 Athens, Helsinki, Istanbul, Cairo, Eastern Europe, Harare, Pretoria, Israel",
		"GMT+03 Baghdad, Kuwait, Nairobi, Riyadh, Moscow, St. Petersburg, Kazan, Volgograd",
		"GMT+04 Abu Dhabi, Muscat, Tbilisi",
		"GMT+05 Islamabad, Karachi, Ekaterinburg, Tashkent",
		"GMT+06 Alma Ata, Dhaka",
		"GMT+07 Bangkok, Jakarta, Hanoi",
		"GMT+08 Taipei, Beijing, Chongqing, Urumqi, Hong Kong, Perth, Singapore",
		"GMT+09 Tokyo, Osaka, Sapporo, Seoul, Yakutsk",
		"GMT+10 Brisbane, Melbourne, Sydney, Guam, Port Moresby, Vladivostok, Hobart",
		"GMT+11 Magadan, Solomon Is., New Caledonia",
		"GMT+12 Fiji, Kamchatka, Marshall Is., Wellington, Auckland"
	};

	if ( *arg == '\0' ) {
		SysInfo* pSysInfo = GetSysInfo();
		if(pSysInfo == NULL)
			return -1;
		if (pSysInfo->lan_config.net.ntp_timezone <= 24)
			return sprintf(data, "%s", TZname[pSysInfo->lan_config.net.ntp_timezone]);
		return -1;
	}
	else {
		int tz = atoi(arg);
		if ((tz >= 0) && (tz <= 24))
			return sprintf(data, "%s", TZname[tz]);
		return -1;
	}
}
/***************************************************************************
 *                                                                         *
 ***************************************************************************/
int para_timeformatname(char *data, char *arg)
{
	if ( *arg == '0' )
		return sprintf(data, "YYYY/MM/DD");
	else if ( *arg == '1' )
		return sprintf(data, "MM/DD/YYYY");
	else if ( *arg == '2' )
		return sprintf(data, "DD/MM/YYYY");
	else if ( *arg == 'a' || *arg == '\0' )
		return sprintf(data, "YYYY/MM/DD;MM/DD/YYYY;DD/MM/YYYY");
	return -1;
}
/***************************************************************************
 *                                                                         *
 ***************************************************************************/
int para_maxnamelen(char *data, char *arg)
{
	return sprintf(data, "%d", USER_LEN - 1);
}
/***************************************************************************
 *                                                                         *
 ***************************************************************************/
int para_maxpwdlen(char *data, char *arg)
{
	return sprintf(data, "%d", PASSWORD_LEN - 1);
}
/***************************************************************************
 *                                                                         *
 ***************************************************************************/
int para_authorityadmin(char *data, char *arg)
{
	return sprintf(data, "%d", AUTHORITY_ADMIN);
}
/***************************************************************************
 *                                                                         *
 ***************************************************************************/
int para_authorityoperator(char *data, char *arg)
{
	return sprintf(data, "%d", AUTHORITY_OPERATOR);
}
/***************************************************************************
 *                                                                         *
 ***************************************************************************/
int para_authorityviewer(char *data, char *arg)
{
	return sprintf(data, "%d", AUTHORITY_VIEWER);
}
/***************************************************************************
 *                                                                         *
 ***************************************************************************/
int para_minnamelen(char *data, char *arg)
{
	return sprintf(data, "%d", 4);
}
/***************************************************************************
 *                                                                         *
 ***************************************************************************/
int para_user(char *data, char *arg)
{
	int i, count = 0;
	SysInfo* pSysInfo = GetSysInfo();
	if(pSysInfo == NULL)
		return -1;
	if ( *arg >= '0' && *arg <= '9' )
		return sprintf(data, "%s", pSysInfo->acounts[*arg - '0'].user);
	else if ( *arg == 'a' || *arg == '\0' ){
		for(i = 0; i< ACOUNT_NUM;i++)
			count += sprintf(data + count, "user%d:%s\n", i + 1, pSysInfo->acounts[i].user);
		return count;
	}
	return -1;
}
/***************************************************************************
 *                                                                         *
 ***************************************************************************/
int para_authority(char *data, char *arg)
{
	int i, count = 0;
	SysInfo* pSysInfo = GetSysInfo();
	if(pSysInfo == NULL)
		return -1;
	if ( *arg >= '0' && *arg <= '9' )
		return sprintf(data, "%d", pSysInfo->acounts[*arg - '0'].authority);
	else if ( *arg == 'a' || *arg == '\0' ){
		for(i = 0; i< ACOUNT_NUM;i++)
			count += sprintf(data + count, "authority%d:%d\n", i+1,
					pSysInfo->acounts[i].authority);
		return count;
	}
	return -1;
}
/***************************************************************************
 *                                                                         *
 ***************************************************************************/
int para_mpeg42resolution(char *data, char *arg)
{
	SysInfo* pSysInfo = GetSysInfo();
	if(pSysInfo == NULL)
		return -1;
	return sprintf(data, "%d", pSysInfo->lan_config.net.mpeg42resolution);
}
/***************************************************************************
 *                                                                         *
 ***************************************************************************/
int para_schedule(char *data, char *arg)
{
	SysInfo* pSysInfo = GetSysInfo();
	Schedule_t *pSchedule;
	int ret = 0, i, arg_i = atoi(arg);

	if(pSysInfo == NULL)
		return -1;
	if ( *arg == 'a' || *arg == '\0' ){
		for(i = 0; i < SCHDULE_NUM; i ++){
			pSchedule = &(pSysInfo->lan_config.aSchedules[i]);
			ret += sprintf(data + ret,
					"%02d%d%02d%02d%02d%02d%02d%02d%02d\n", i,
					pSchedule -> bStatus, pSchedule -> nDay,
					pSchedule -> tStart.nHour, pSchedule -> tStart.nMin,
					pSchedule -> tStart.nSec, pSchedule -> tDuration.nHour,
					pSchedule -> tDuration.nMin, pSchedule -> tDuration.nSec);
		}
		return ret;
	} else if (arg_i >= 0 && arg_i < SCHDULE_NUM){
		pSchedule = &(pSysInfo->lan_config.aSchedules[arg_i]);
		return sprintf(data, "%d%02d%02d%02d%02d%02d%02d%02d",
				pSchedule -> bStatus, pSchedule -> nDay,
				pSchedule -> tStart.nHour, pSchedule -> tStart.nMin,
				pSchedule -> tStart.nSec, pSchedule -> tDuration.nHour,
				pSchedule -> tDuration.nMin, pSchedule -> tDuration.nSec);
	}
	return -1;
}
/***************************************************************************
 *                                                                         *
 ***************************************************************************/
int para_sdformatName(char *data, char *arg)
{
	int supportMpeg4=0,supportMpeg4cif=0 ,supportH264=0, supportH264cif=0,supportJPG=0;
	int supportAVI=0;
	SysInfo* pSysInfo = GetSysInfo();
	if(pSysInfo == NULL)
		return -1;
	else {
		supportJPG = pSysInfo->lan_config.Supportstream1;
		supportMpeg4 = pSysInfo->lan_config.Supportstream2;
		supportMpeg4cif = pSysInfo->lan_config.Supportstream3;
		supportH264 = pSysInfo->lan_config.Supportstream5;
		supportH264cif = pSysInfo->lan_config.Supportstream6;
	}
	supportAVI = supportH264|supportH264cif|supportMpeg4|supportMpeg4cif;
	if ((supportAVI&&supportJPG)){
		if ( *arg == '0' )
			return sprintf(data, "AVI");
		else if(*arg =='1')
			return sprintf(data,"JPEG");
		else if ( *arg == 'a' || *arg == '\0' )
			return sprintf(data, "AVI;JPEG");
	}
	else if (supportAVI){
		if ( *arg == '0' )
			return sprintf(data, "AVI");
		else if ( *arg == 'a' || *arg == '\0' )
			return sprintf(data, "AVI");
	}
	else if (supportJPG){
		if ( *arg == '0' )
			return sprintf(data, "JPEG");
		else if ( *arg == 'a' || *arg == '\0' )
			return sprintf(data, "JPEG");
	}
	return -1;
}
/***************************************************************************
 *                                                                         *
 ***************************************************************************/
int para_formatName(char *data, char *arg)
{
	int supportMpeg4=0,supportMpeg4cif=0 ,supportH264=0, supportH264cif=0,supportJPG=0;
	int supportAVI=0;
	SysInfo* pSysInfo = GetSysInfo();
	if(pSysInfo == NULL)
		return -1;
	else {
		supportJPG = pSysInfo->lan_config.Supportstream1;
		supportMpeg4 = pSysInfo->lan_config.Supportstream2;
		supportMpeg4cif = pSysInfo->lan_config.Supportstream3;
		supportH264 = pSysInfo->lan_config.Supportstream5;
		supportH264cif = pSysInfo->lan_config.Supportstream6;
	}
	if (pSysInfo->sdcard_config.sdinsert==3) {
		if(supportJPG) {
			if ( *arg == '0' )
				return sprintf(data, "JPEG");
			else if ( *arg == 'a' || *arg == '\0' )
				return sprintf(data, "JPEG");
		}else return sprintf(data, "");
	}
	supportAVI = supportH264|supportH264cif|supportMpeg4|supportMpeg4cif;
	if ((supportAVI&&supportJPG)){
		if ( *arg == '0' )
			return sprintf(data, "AVI");
		else if(*arg =='1')
			return sprintf(data,"JPEG");
		else if ( *arg == 'a' || *arg == '\0' )
			return sprintf(data, "AVI;JPEG");
	}
	else if (supportAVI){
		if ( *arg == '0' )
			return sprintf(data, "AVI");
		else if ( *arg == 'a' || *arg == '\0' )
			return sprintf(data, "AVI");
	}
	else if (supportJPG){
		if ( *arg == '0' )
			return sprintf(data, "JPEG");
		else if ( *arg == 'a' || *arg == '\0' )
			return sprintf(data, "JPEG");
	}
	return -1;
}
/***************************************************************************
 *                                                                         *
 ***************************************************************************/
int para_sdfileformat(char *data, char *arg)
{
	SysInfo* pSysInfo = GetSysInfo();
	if(pSysInfo == NULL)
		return -1;
	return sprintf(data, "%d", pSysInfo->sdcard_config.sdfileformat);
}
/***************************************************************************
 *                                                                         *
 ***************************************************************************/
int para_ftpfileformat(char *data, char *arg)
{
	SysInfo* pSysInfo = GetSysInfo();
	if(pSysInfo == NULL)
		return -1;
	return sprintf(data, "%d", pSysInfo->ftp_config.ftpfileformat);
}

/***************************************************************************
 *                                                                         *
 ***************************************************************************/
int para_attfileformat(char *data, char *arg)
{
	SysInfo* pSysInfo = GetSysInfo();
	if(pSysInfo == NULL)
		return -1;
	return sprintf(data, "%d", pSysInfo->smtp_config.attfileformat);
}
/***************************************************************************
 *                                                                         *
 ***************************************************************************/
int para_aviduration(char *data, char *arg)
{
	SysInfo* pSysInfo = GetSysInfo();
	if(pSysInfo == NULL)
		return -1;
	return sprintf(data, "%d", pSysInfo->lan_config.aviduration);
}
/***************************************************************************
 *                                                                         *
 ***************************************************************************/
int para_avidurationname(char *data, char *arg)
{
	SysInfo* pSysInfo = GetSysInfo();
	if(pSysInfo == NULL)
		return -1;
	if(pSysInfo->sdcard_config.sdinsert==3){
		if( *arg == '0' )
			return sprintf(data, "--");
		else if( *arg == 'a' || *arg == '\0' )
			return sprintf(data, "--");
	}
	if ( *arg == '0' )
		return sprintf(data, "5");
	else if ( *arg == '1' )
		return sprintf(data, "10");
	else if ( *arg == 'a' || *arg == '\0' )
		return sprintf(data, "5;10");
	return -1;
}
/***************************************************************************
 *                                                                         *
 ***************************************************************************/
int para_aviformat(char *data, char *arg)
{
	SysInfo* pSysInfo = GetSysInfo();
	if(pSysInfo == NULL)
		return -1;
	return sprintf(data, "%d", pSysInfo->lan_config.aviformat);
}
/***************************************************************************
 *                                                                         *
 ***************************************************************************/
int para_aviformatname(char *data, char *arg)
{
	SysInfo* pSysInfo = GetSysInfo();
	if(pSysInfo == NULL)
		return -1;
	int supportMpeg4 = pSysInfo->lan_config.Supportstream2;
	int supportMpeg4cif = pSysInfo->lan_config.Supportstream3;
	int supportH264 = pSysInfo->lan_config.Supportstream5;
	int supportH264cif = pSysInfo->lan_config.Supportstream6;
	int mpeg41Xsize= pSysInfo->lan_config.Mpeg41Xsize;
	int mpeg41Ysize= pSysInfo->lan_config.Mpeg41Ysize;
	int mpeg42Xsize=pSysInfo->lan_config.Mpeg42Xsize;
	int mpeg42Ysize=pSysInfo->lan_config.Mpeg42Ysize;
	int avc1Xsize=pSysInfo->lan_config.Avc1Xsize;
	int avc1Ysize=pSysInfo->lan_config.Avc1Ysize;
	int avc2Xsize=pSysInfo->lan_config.Avc2Xsize;
	int avc2Ysize=pSysInfo->lan_config.Avc2Ysize;

	if ( supportH264 ) {
		if ( supportH264cif ){
			return sprintf(data,"H.264(%d x %d);H.264(%d x %d)", avc1Xsize,avc1Ysize,avc2Xsize,avc2Ysize);
		}
		else if( supportMpeg4cif ){
			return sprintf(data,"H.264(%d x %d);MPEG4(%d x %d)",avc1Xsize,avc1Ysize,mpeg42Xsize,mpeg42Ysize);
		}else return sprintf(data,"H.264(%d x %d)",avc1Xsize,avc1Ysize);
	}
	else if ( supportMpeg4 ){
		if ( supportH264cif ){
			return sprintf(data,"MPEG4(%d x %d);H.264(%d x %d)",mpeg41Xsize,mpeg41Ysize,avc2Xsize,avc2Ysize);
		}
		else if( supportMpeg4cif ){
			return sprintf(data,"MPEG4(%d x %d);MPEG4(%d x %d)",mpeg41Xsize,mpeg41Ysize,mpeg42Xsize,mpeg42Ysize);
		}else return sprintf(data,"MPEG4(%d x %d)",mpeg41Xsize,mpeg41Ysize);
	}
	return -1;
}
/***************************************************************************
 *                                                                         *
 ***************************************************************************/
int para_sdleft(char *data, char *arg)
{
	long long freespace;
	freespace=GetDiskfreeSpace("/mnt/mmc/");

	return sprintf(data, "%lld",freespace*1024);
}
/***************************************************************************
 *                                                                         *
 ***************************************************************************/
int para_sdused(char *data, char *arg)
{
	long long usedspace;
	usedspace=GetDiskusedSpace("/mnt/mmc/");
	return sprintf(data,"%lld",usedspace*1024);
}
/***************************************************************************
 *                                                                         *
 ***************************************************************************/
int para_motionblock(char *data, char *arg)
{
	SysInfo* pSysInfo = GetSysInfo();

	if(pSysInfo == NULL)
		return -1;
	return sprintf(data, "%s", pSysInfo->motion_config.motionblock);
}
/***************************************************************************
 *                                                                         *
 ***************************************************************************/
int para_motionenable(char *data, char *arg)
{
	SysInfo* pSysInfo = GetSysInfo();

	if(pSysInfo == NULL)
		return -1;
	return sprintf(data, "%d", pSysInfo->motion_config.motionenable);
}
/***************************************************************************
 *                                                                         *
 ***************************************************************************/
int para_motionlevel(char *data, char *arg)
{
	return sprintf(data, "%d", 0);

}
/***************************************************************************
 *                                                                         *
 ***************************************************************************/
int para_motionsensitivity(char *data, char *arg)
{
	SysInfo* pSysInfo = GetSysInfo();

	if(pSysInfo == NULL)
		return -1;
	return sprintf(data, "%d", pSysInfo->motion_config.motionlevel);

}
/***************************************************************************
 *                                                                         *
 ***************************************************************************/
int para_motionname(char *data, char *arg)
{
	if ( *arg == '0' )
		return sprintf(data, "LOW");
	else if ( *arg == '1' )
		return sprintf(data, "MEDIUM");
	else if ( *arg == '2' )
		return sprintf(data, "HIGH");
	else if ( *arg == 'a' || *arg == '\0' )
		return sprintf(data, "LOW;MEDIUM;HIGH");

	return -1;
}
/***************************************************************************
 *                                                                         *
 ***************************************************************************/
int para_motioncenable(char *data, char *arg)
{
	SysInfo* pSysInfo = GetSysInfo();

	if(pSysInfo == NULL)
		return -1;
	return sprintf(data, "%d", pSysInfo->motion_config.motioncenable);
}
/***************************************************************************
 *                                                                         *
 ***************************************************************************/
int para_ipncptz(char *data, char *arg)
{
	return 0;
}

/***************************************************************************
 *                                                                         *
 ***************************************************************************/
int para_motioncvalue(char *data, char *arg)
{
	SysInfo* pSysInfo = GetSysInfo();

	if(pSysInfo == NULL)
		return -1;
	return sprintf(data, "%d", pSysInfo->motion_config.motioncvalue);
}
/***************************************************************************
 *                                                                         *
 ***************************************************************************/
int para_minmotionthreshold(char *data, char *arg)
{
	return sprintf(data, "1");
}
/***************************************************************************
 *                                                                         *
 ***************************************************************************/
int para_maxmotionthreshold(char *data, char *arg)
{
	return sprintf(data, "99");
}
/***************************************************************************
 *                                                                         *
 ***************************************************************************/
int para_lostalarm(char *data, char *arg)
{
	SysInfo* pSysInfo = GetSysInfo();
	if(pSysInfo == NULL)
		return -1;
	return sprintf(data, "%d", pSysInfo->lan_config.lostalarm);
}
/***************************************************************************
 *                                                                         *
 ***************************************************************************/
int para_sdaenable(char *data, char *arg)
{
	SysInfo* pSysInfo = GetSysInfo();
	if(pSysInfo == NULL)
		return -1;
	return sprintf(data, "%d", pSysInfo->lan_config.bSdaEnable);
}
/***************************************************************************
 *                                                                         *
 ***************************************************************************/
int para_aftpenable(char *data, char *arg)
{
	SysInfo* pSysInfo = GetSysInfo();
	if(pSysInfo == NULL)
		return -1;
	return sprintf(data, "%d", pSysInfo->lan_config.bAFtpEnable);
}
/***************************************************************************
 *                                                                         *
 ***************************************************************************/
int para_asmtpenable(char *data, char *arg)
{
	SysInfo* pSysInfo = GetSysInfo();
	if(pSysInfo == NULL)
		return -1;
	return sprintf(data, "%d", pSysInfo->lan_config.bASmtpEnable);
}
/***************************************************************************
 *                                                                         *
 ***************************************************************************/
int para_recordduration(char *data, char *arg)
{
	if ( *arg == '0' )
		return sprintf(data, "10 seconds");
	else if ( *arg == '1' )
		return sprintf(data, "30 seconds");
	else if ( *arg == '2' )
		return sprintf(data, "1 minute");
	else if ( *arg == '3' )
		return sprintf(data, "5 minutes");
	else if ( *arg == '4' )
		return sprintf(data, "10 minutes");
	else if ( *arg == '5' )
		return sprintf(data, "Non-Stop");
	else if ( *arg == 'a' || *arg == '\0' )
		return sprintf(data, "10 seconds;30 seconds;1 minute;5 minutes;10 minutes;Non-Stop");
	return -1;
}
/***************************************************************************
 *                                                                         *
 ***************************************************************************/
int para_smtpminattach(char *data, char *arg)
{
	return sprintf(data, "1");
}
/***************************************************************************
 *                                                                         *
 ***************************************************************************/
int para_smtpmaxattach(char *data, char *arg)
{
	return sprintf(data, "20");
}
/***************************************************************************
 *                                                                         *
 ***************************************************************************/
int para_asmtpattach(char *data, char *arg)
{
	SysInfo* pSysInfo = GetSysInfo();

	if(pSysInfo == NULL)
		return -1;
	return sprintf(data, "%d", pSysInfo->smtp_config.attachments);
}
/***************************************************************************
 *                                                                         *
 ***************************************************************************/
int para_rftpenable(char *data, char *arg)
{
	SysInfo* pSysInfo = GetSysInfo();

	if(pSysInfo == NULL)
		return -1;
	return sprintf(data, "%d", pSysInfo->ftp_config.rftpenable);
}
/***************************************************************************
 *                                                                         *
 ***************************************************************************/
int para_sdrenable(char *data, char *arg)
{
	SysInfo* pSysInfo = GetSysInfo();

	if(pSysInfo == NULL)
		return -1;
	return sprintf(data, "%d", pSysInfo->sdcard_config.sdrenable);
}
/***************************************************************************
 *                                                                         *
 ***************************************************************************/
int para_alarmduration(char *data, char *arg)
{
	SysInfo* pSysInfo = GetSysInfo();
	if(pSysInfo == NULL)
		return -1;
	return sprintf(data, "%d", pSysInfo->lan_config.nAlarmDuration);
}
/***************************************************************************
 *                                                                         *
 ***************************************************************************/
int para_motionxlimit(char *data, char *arg)
{
	return sprintf(data, "%d", 1280);
}
/***************************************************************************
 *                                                                         *
 ***************************************************************************/
int para_motionxblock(char *data, char *arg)
{
	return sprintf(data, "%d", 4);
}
/***************************************************************************
 *                                                                         *
 ***************************************************************************/
int para_motionyblock(char *data, char *arg)
{
	return sprintf(data, "%d", 3);
}
/***************************************************************************
 *                                                                         *
 ***************************************************************************/
int para_supportstream1(char *data, char *arg)
{
	SysInfo* pSysInfo = GetSysInfo();
	if(pSysInfo == NULL)
		return -1;
	return sprintf(data, "%d", pSysInfo->lan_config.Supportstream1);
}
/***************************************************************************
 *                                                                         *
 ***************************************************************************/
int para_supportstream2(char *data, char *arg)
{
	SysInfo* pSysInfo = GetSysInfo();
	if(pSysInfo == NULL)
		return -1;
	return sprintf(data, "%d", pSysInfo->lan_config.Supportstream2);
}
/***************************************************************************
 *                                                                         *
 ***************************************************************************/
int para_supportstream3(char *data, char *arg)
{
	SysInfo* pSysInfo = GetSysInfo();
	if(pSysInfo == NULL)
		return -1;
	return sprintf(data, "%d", pSysInfo->lan_config.Supportstream3);
}
/***************************************************************************
 *                                                                         *
 ***************************************************************************/
int para_supportstream4(char *data, char *arg)
{
	SysInfo* pSysInfo = GetSysInfo();
	if(pSysInfo == NULL)
		return -1;
	return sprintf(data, "%d", pSysInfo->lan_config.Supportstream4);
}
/***************************************************************************
 *                                                                         *
 ***************************************************************************/
int para_supportstream5(char *data, char *arg)
{
	SysInfo* pSysInfo = GetSysInfo();
	if(pSysInfo == NULL)
		return -1;
	return sprintf(data, "%d", pSysInfo->lan_config.Supportstream5);
}
/***************************************************************************
 *                                                                         *
 ***************************************************************************/
int para_supportstream6(char *data, char *arg)
{
	SysInfo* pSysInfo = GetSysInfo();
	if(pSysInfo == NULL)
		return -1;
	return sprintf(data, "%d", pSysInfo->lan_config.Supportstream6);
}
/***************************************************************************
 *                                                                         *
 ***************************************************************************/

int para_image2a(char *data, char *arg)
{
	SysInfo* pSysInfo = GetSysInfo();
	if(pSysInfo == NULL)
		return -1;

	return sprintf(data, "%d", pSysInfo->lan_config.nAEWswitch);
}

int para_img2aname(char *data, char *arg)
{
	if ( *arg == '0' )
		return sprintf(data, "NONE");
	else if ( *arg == '1' )
		return sprintf(data, "APPRO");
	else if ( *arg == '2' )
		return sprintf(data, "TI");
	else if ( *arg == 'a' || *arg == '\0' ) {
		return sprintf(data, "NONE;APPRO;TI");
	}
	return -1;
}
/***************************************************************************
 *                                                                         *
 ***************************************************************************/

int para_image2atype(char *data, char *arg)
{
	SysInfo* pSysInfo = GetSysInfo();
	if(pSysInfo == NULL)
		return -1;

	return sprintf(data, "%d", pSysInfo->lan_config.nAEWtype);
}

int para_img2atypename(char *data, char *arg)
{
	if ( *arg == '0' )
		return sprintf(data, "OFF");
	else if ( *arg == '1' )
		return sprintf(data, "AE");
	else if ( *arg == '2' )
		return sprintf(data, "AWB");
	else if ( *arg == '3' )
		return sprintf(data, "AEWB");
	else if ( *arg == 'a' || *arg == '\0' ) {
		return sprintf(data, "OFF;AE;AWB;AEWB");
	}
	return -1;
}
/***************************************************************************
 *                                                                         *
 ***************************************************************************/

int para_democfg(char *data, char *arg)
{
	SysInfo* pSysInfo = GetSysInfo();
	if(pSysInfo == NULL)
		return -1;

	return sprintf(data, "%d", pSysInfo->lan_config.democfg);
}

int para_democfgname(char *data, char *arg)
{
	if ( *arg == '0' )
		return sprintf(data, "NONE");
	else if ( *arg == '1' )
		return sprintf(data, "VS DEMO");
	else if ( *arg == '2' )
		return sprintf(data, "VNF DEMO");
	else if ( *arg == '3' )
		return sprintf(data, "LDC DEMO");
	else if ( *arg == '4' )
		return sprintf(data, "FD DEMO");
	else if ( *arg == '5' )
		return sprintf(data, "FD ROI DEMO");
	else if ( *arg == '6' )
		return sprintf(data, "ROI CENTER DEMO");
	else if ( *arg == 'a' || *arg == '\0' )
		return sprintf(data, "NONE;VS DEMO;VNF DEMO;LDC DEMO;FD DEMO;FD ROI DEMO;ROI CENTER DEMO");

	return -1;
}

/***************************************************************************
 *                                                                         *
 ***************************************************************************/

int para_rotctrl(char *data, char *arg)
{
	SysInfo* pSysInfo = GetSysInfo();
	if(pSysInfo == NULL)
		return -1;

	return sprintf(data, "%d", pSysInfo->lan_config.rotation);
}

int para_rotctrlname(char *data, char *arg)
{
#if 1
	if ( *arg == '0' )
		return sprintf(data, "OFF");
	else if ( *arg == '1' )
		return sprintf(data, "LEFT");
	else if ( *arg == '2' )
		return sprintf(data, "UPSIDE_DOWN");
	else if ( *arg == '3')
		return sprintf(data, "RIGHT");
	else if ( *arg == 'a' || *arg == '\0' )
		return sprintf(data, "OFF;LEFT;UPSIDE_DOWN;RIGHT");
#else
	if ( *arg == '0' )
		return sprintf(data, "OFF");
	else if ( *arg == '2' )
		return sprintf(data, "UPSIDE_DOWN");
	else if ( *arg == 'a' || *arg == '\0' )
		return sprintf(data, "OFF;UPSIDE_DOWN");
#endif

	return -1;
}
/***************************************************************************
 *                                                                         *
 ***************************************************************************/

int para_mirctrl(char *data, char *arg)
{
	SysInfo* pSysInfo = GetSysInfo();
	if(pSysInfo == NULL)
		return -1;

	return sprintf(data, "%d", pSysInfo->lan_config.mirror);
}

int para_mirctrlname(char *data, char *arg)
{
	if ( *arg == '0' )
		return sprintf(data, "OFF");
	else if ( *arg == '1' )
		return sprintf(data, "HORIZONTAL");
	else if ( *arg == '2' )
		return sprintf(data, "VERTICAL");
	else if ( *arg == '3')
		return sprintf(data, "BOTH");
	else if ( *arg == 'a' || *arg == '\0' )
		return sprintf(data, "OFF;HORIZONTAL;VERTICAL;BOTH");

	return -1;
}
/***************************************************************************
 *                                                                         *
 ***************************************************************************/

int para_fdetect(char *data, char *arg)
{
	SysInfo* pSysInfo = GetSysInfo();
	if(pSysInfo == NULL)
		return -1;

	return sprintf(data, "%d", pSysInfo->lan_config.fdetect);
}

int para_fdetectname(char *data, char *arg)
{

	if ( *arg == '0' )
		return sprintf(data, "OFF");
	else if ( *arg == '1' )
		return sprintf(data, "FACE DETECT");
	else if ( *arg == '2' )
		return sprintf(data, "PRIVACY MASK");
	else if ( *arg == '3' )
		return sprintf(data, "RECOGNIZE USER");   //ANR
	else if ( *arg == '4' )
		return sprintf(data, "REGISTER USER");    //ANR
	else if ( *arg == '5' )
		return sprintf(data, "CLEAR ALL USERS");   //ANR
	else if ( *arg == 'a' || *arg == '\0' )
		return sprintf(data, "OFF;FACE DETECT;PRIVACY MASK;RECOGNIZE USER;REGISTER USER;CLEAR ALL USERS");

	return -1;
}
/***************************************************************************
 *                                                                         *
 ***************************************************************************/

int para_osdwinnum(char *data, char *arg)
{
	SysInfo* pSysInfo = GetSysInfo();
	if(pSysInfo == NULL)
		return -1;

	return sprintf(data, "%d", pSysInfo->lan_config.osdwinnum);
}

int para_osdwinnumname(char *data, char *arg)
{
	if ( *arg == '0' )
		return sprintf(data, "Logo Window");
	else if ( *arg == '1' )
		return sprintf(data, "Text Window");
#if 0
	else if ( *arg == '2' )
		return sprintf(data, "Window_3");
	else if ( *arg == '3')
		return sprintf(data, "Window_4");
	else if ( *arg == '4')
		return sprintf(data, "Window_5");
	else if ( *arg == 'a' || *arg == '\0' )
		return sprintf(data, "Window_1;Window_2;Window_3;Window_4;Window_5");
#else
	else if ( *arg == 'a' || *arg == '\0' )
		return sprintf(data, "Logo Window;Text Window");
#endif

	return -1;
}
/***************************************************************************
 *                                                                         *
 ***************************************************************************/

int para_osdstream(char *data, char *arg)
{
	SysInfo* pSysInfo = GetSysInfo();
	if(pSysInfo == NULL)
		return -1;

	return sprintf(data, "%d", pSysInfo->lan_config.osdstream);
}

int para_osdstreamname(char *data, char *arg)
{
	if ( *arg == '0' )
		return sprintf(data, "Stream_1");
	else if ( *arg == '1' )
		return sprintf(data, "Stream_2");
	else if ( *arg == '2' )
		return sprintf(data, "Stream_3");
	else if ( *arg == 'a' || *arg == '\0' )
		return sprintf(data, "Stream_1;Stream_2;Stream_3");

	return -1;
}
/***************************************************************************
 *                                                                         *
 ***************************************************************************/
int para_kernelversion(char *data, char *arg)
{
	FILE *fptr;
	char TempBuff[80];
	char *pStr[3];
	int buffsize = sizeof(TempBuff);
	int cnt = 0;

	fptr=fopen("/proc/version","r");
	if (fptr==NULL){
		fprintf(stderr,"\n Can't get Version information ");
		return -1;
	}
	fread( TempBuff, buffsize-1, 1,fptr);
	TempBuff[buffsize-1] = '\0';

	pStr[0] = strtok(TempBuff, " ");

	for( cnt = 1;cnt < 3;cnt++ )
	{
		pStr[cnt] = strtok(NULL, " ");
	}
	fclose(fptr);
	return sprintf(data, "%s %s %s ",pStr[0],pStr[1],pStr[2]);

}
/***************************************************************************
 *                                                                         *
 ***************************************************************************/
int para_biosversion(char *data, char *arg)
{
	int fd0;
	size_t size, cnt = 0, length;
	char readbuff[1024], *strptr, *target;
	fd0 = open("/dev/mtd0", O_RDONLY);
	if (fd0 == -1) {
		dbg("=======Open mtd0 failed========\n");
		return sprintf(data, "Open mtd0 failed");
	}
	size = lseek(fd0, 0x240000, SEEK_SET);
	if (size == -1) {
		dbg("=======lseek failed========\n");
		return sprintf(data, "lseek failed");
	}
	size = read(fd0, readbuff, 1024);
	dbg("#######size = %d\n", size);
	readbuff[1023] = '\0';
	strptr = readbuff;
	while (cnt < size) {
		length = strlen(strptr) + 1;
		if (strncmp(strptr, "ver", 3) == 0)
			break;
		strptr += length;
		cnt += length;
	}
	target = strtok(strptr, "=");
	//	fprintf(stderr,"====stringA: %s\n", target);
	target = strtok(NULL, "=");
	//	fprintf(stderr,"====stringB: %s\n", target);
	close(fd0);
	return sprintf(data, "%s", target);
}
/***************************************************************************
 *                                                                         *
 ***************************************************************************/
int para_waitserver(char *data, char *arg)
{
	return sprintf(data, "0");
}
/***************************************************************************
 *                                                                         *
 ***************************************************************************/
int para_supportcolorkiller(char *data, char *arg)
{
	return sprintf(data, "1");
}
int para_supportAWB(char *data, char *arg)
{
	return sprintf(data, "1");
}
int para_supportbrightness(char *data, char *arg)
{
	return sprintf(data, "1");
}
int para_supportcontrast(char *data, char *arg)
{
	return sprintf(data, "1");
}
int para_supportsaturation(char *data, char *arg)
{
	return sprintf(data, "1");
}
int para_supportbacklight(char *data, char *arg)
{
	return sprintf(data, "1");
}
int para_supportsharpness(char *data, char *arg)
{
	return sprintf(data, "1");
}
/***************************************************************************
 *                                                                         *
 ***************************************************************************/

int para_motion_blk(char *data, char *arg)
{
	SysInfo* pSysInfo = GetSysInfo();

	if(pSysInfo == NULL)
		return -1;

	strncpy(data, pSysInfo->motion_config.motionblock,MOTION_BLK_LEN);
	return strlen(data);
}
/***************************************************************************
 *                                                                         *
 ***************************************************************************/
int para_gioinenable(char *data, char *arg)
{
	SysInfo* pSysInfo = GetSysInfo();

	if(pSysInfo == NULL)
		return -1;
	return sprintf(data, "%d", pSysInfo->lan_config.gioinenable);
}
/***************************************************************************
 *                                                                         *
 ***************************************************************************/
int para_giointype(char *data, char *arg)
{
	SysInfo* pSysInfo = GetSysInfo();

	if(pSysInfo == NULL)
		return -1;
	return sprintf(data, "%d", pSysInfo->lan_config.giointype);
}
/***************************************************************************
 *                                                                         *
 ***************************************************************************/
int para_gioinname(char *data, char *arg)
{
	if ( *arg == '0' )
		return sprintf(data, "Low");
	else if ( *arg == '1' )
		return sprintf(data, "High");
	else if ( *arg == 'a' || *arg == '\0' )
		return sprintf(data, "Low;High");
	return -1;
}
/***************************************************************************
 *                                                                         *
 ***************************************************************************/
int para_giooutenable(char *data, char *arg)
{
	SysInfo* pSysInfo = GetSysInfo();

	if(pSysInfo == NULL)
		return -1;
	return sprintf(data, "%d", pSysInfo->lan_config.giooutenable);
}
/***************************************************************************
 *                                                                         *
 ***************************************************************************/
int para_gioouttype(char *data, char *arg)
{
	SysInfo* pSysInfo = GetSysInfo();

	if(pSysInfo == NULL)
		return -1;
	return sprintf(data, "%d", pSysInfo->lan_config.gioouttype);
}
/***************************************************************************
 *                                                                         *
 ***************************************************************************/
int para_giooutname(char *data, char *arg)
{
	if ( *arg == '0' )
		return sprintf(data, "Low");
	else if ( *arg == '1' )
		return sprintf(data, "High");
	else if ( *arg == 'a' || *arg == '\0' )
		return sprintf(data, "Low;High");
	return -1;
}
/***************************************************************************
 *                                                                         *
 ***************************************************************************/
int para_softwareversion(char *data, char *arg)
{
	return sprintf(data, "%s", APP_VERSION);
}
/***************************************************************************
 *                                                                         *
 ***************************************************************************/
int para_tstampenable(char *data, char *arg)
{
	SysInfo* pSysInfo = GetSysInfo();

	if(pSysInfo == NULL)
		return -1;
	return sprintf(data, "%d", pSysInfo->lan_config.tstampenable);
}
/***************************************************************************
 *                                                                         *
 ***************************************************************************/
int para_tstampcolorname(char *data, char *arg)
{
	if ( *arg == '0' )
		return sprintf(data, "white");
	else if ( *arg == 'a' || *arg == '\0' )
		return sprintf(data, "white");
	return -1;
}
/***************************************************************************
 *                                                                         *
 ***************************************************************************/
int para_tstamplocname(char *data, char *arg)
{
	if ( *arg == '0' )
		return sprintf(data, "leftup");
	else if ( *arg == 'a' || *arg == '\0' )
		return sprintf(data, "leftup");
	return -1;
}
/***************************************************************************
 *                                                                         *
 ***************************************************************************/
int para_tstampformatname(char *data, char *arg)
{
	if ( *arg == '0' )
		return sprintf(data, "YYYY/MM/DD");
	else if ( *arg == '1' )
		return sprintf(data, "MM/DD/YYYY");
	else if ( *arg == '2' )
		return sprintf(data, "DD/MM/YYYY");
	else if ( *arg == 'a' || *arg == '\0' )
		return sprintf(data, "YYYY/MM/DD;MM/DD/YYYY;DD/MM/YYYY");
	return -1;
}
/***************************************************************************
 *                                                                         *
 ***************************************************************************/
int para_tstampformat(char *data, char *arg)
{
	SysInfo* pSysInfo = GetSysInfo();

	if(pSysInfo == NULL)
		return -1;
	return sprintf(data, "%d", pSysInfo->lan_config.tstampformat);
}
/***************************************************************************
 *                                                                         *
 ***************************************************************************/
int para_audioinmax(char *data, char *arg)
{
	return sprintf(data, "%d", 4);
}
/***************************************************************************
 *                                                                         *
 ***************************************************************************/
int para_audioinvolume(char *data, char *arg)
{
	SysInfo* pSysInfo = GetSysInfo();

	if(pSysInfo == NULL)
		return -1;
	return sprintf(data, "%d", pSysInfo->lan_config.audioinvolume);
}
/***************************************************************************
 *                                                                         *
 ***************************************************************************/
int para_event_location(char *data, char *arg)
{
	extern unsigned int event_location;

	return sprintf(data, "%d", event_location);
}
/***************************************************************************
 *                                                                         *
 ***************************************************************************/
int para_get_event(char *data, char *arg)
{
	extern unsigned int event_location;
	LogEntry_t tLog;
	int i = atoi(arg) - 1 + event_location;
	struct tm* ptm;
	if(GetSysLog(0, i, &tLog)){
		/* No event */
		return sprintf(data, "");
	}
	ptm = &tLog.time;
	return sprintf(data, "%d-%02d-%02d %02d:%02d:%02d %s",
			ptm->tm_year + 1900, ptm->tm_mon + 1, ptm->tm_mday,
			ptm->tm_hour, ptm->tm_min, ptm->tm_sec, tLog.event);
}
/***************************************************************************
 *                                                                         *
 ***************************************************************************/
int para_multicast(char *data, char *arg)
{
	SysInfo* pSysInfo = GetSysInfo();
	if(pSysInfo == NULL)
		return -1;
	return sprintf(data, "%d", pSysInfo->lan_config.net.multicast_enable);
}
/***************************************************************************
 *                                                                         *
 ***************************************************************************/
//2010-09-24  14:00
int para_ntpswitch(char *data, char *arg)
{
	// SysInfo* pSysInfo = GetSysInfo();

	//if(pSysInfo == NULL)
	//	return -1;
	return sprintf(data, "%d", 1);
}
int para_smtpswitch(char *data, char *arg)
{
	// SysInfo* pSysInfo = GetSysInfo();

	//if(pSysInfo == NULL)
	//	return -1;
	return sprintf(data, "%d", 1);
}
int para_rtspswitch(char *data, char *arg)
{
	// SysInfo* pSysInfo = GetSysInfo();

	//if(pSysInfo == NULL)
	//  return -1;
	return sprintf(data, "%d", 1);
}
int para_h2641(char *data, char *arg)
{
	// SysInfo* pSysInfo = GetSysInfo();

	//if(pSysInfo == NULL)
	//	return -1;
	return sprintf(data, "%d", 1);
}
int para_h2642(char *data, char *arg)
{
	// SysInfo* pSysInfo = GetSysInfo();

	//if(pSysInfo == NULL)
	//	return -1;
	return sprintf(data, "%d", 1);
}
int para_mpeg41(char *data, char *arg)
{
	// SysInfo* pSysInfo = GetSysInfo();

	//if(pSysInfo == NULL)
	//	return -1;
	return sprintf(data, "%d", 1);
}
int para_mpeg42(char *data, char *arg)
{
	// SysInfo* pSysInfo = GetSysInfo();

	//if(pSysInfo == NULL)
	//  return -1;
	return sprintf(data, "%d", 1);
}
int para_jpeg(char *data, char *arg)
{
	// SysInfo* pSysInfo = GetSysInfo();

	//if(pSysInfo == NULL)
	//  return -1;
	return sprintf(data, "%d", 1);
}
int para_input(char *data, char *arg)
{
	// SysInfo* pSysInfo = GetSysInfo();

	//if(pSysInfo == NULL)
	//	return -1;
	return sprintf(data, "%d", 1);
}

int para_yellow_line(char *data, char *arg)
{
	// SysInfo* pSysInfo = GetSysInfo();

	//if(pSysInfo == NULL)
	//  return -1;
	return sprintf(data, "%d", 1);
}
int para_red(char *data, char *arg)
{
	// SysInfo* pSysInfo = GetSysInfo();

	//if(pSysInfo == NULL)
	//  return -1;
	return sprintf(data, "%d", 1);
}
int para_superspeed(char *data, char *arg)
{
	// SysInfo* pSysInfo = GetSysInfo();

	//if(pSysInfo == NULL)
	//  return -1;
	return sprintf(data, "%d", 1);
}

int para_di_do_action(char *data, char *arg)
{
	// SysInfo* pSysInfo = GetSysInfo();

	//if(pSysInfo == NULL)
	//  return -1;
	return sprintf(data, "%d", 1);
}
int para_yellow_do_action(char *data, char *arg)
{
	// SysInfo* pSysInfo = GetSysInfo();

	//if(pSysInfo == NULL)
	//  return -1;
	return sprintf(data, "%d", 1);
}
int para_red_do_action(char *data, char *arg)
{
	// SysInfo* pSysInfo = GetSysInfo();

	//if(pSysInfo == NULL)
	//  return -1;
	return sprintf(data, "%d", 1);
}
int para_sup_do_action(char *data, char *arg)
{
	// SysInfo* pSysInfo = GetSysInfo();

	//if(pSysInfo == NULL)
	//  return -1;
	return sprintf(data, "%d", 1);
}
int para_smtp_action(char *data, char *arg)
{
	// SysInfo* pSysInfo = GetSysInfo();

	//if(pSysInfo == NULL)
	//  return -1;
	return sprintf(data, "%d", 1);
}
int para_yellow_smtp_action(char *data, char *arg)
{
	// SysInfo* pSysInfo = GetSysInfo();

	//if(pSysInfo == NULL)
	//  return -1;
	return sprintf(data, "%d", 1);
}
int para_red_smtp_action(char *data, char *arg)
{
	// SysInfo* pSysInfo = GetSysInfo();

	//if(pSysInfo == NULL)
	//  return -1;
	return sprintf(data, "%d", 1);
}
int para_sup_smtp_action(char *data, char *arg)
{
	// SysInfo* pSysInfo = GetSysInfo();

	//if(pSysInfo == NULL)
	//  return -1;
	return sprintf(data, "%d", 1);
}
int para_di_record_action(char *data, char *arg)
{
	// SysInfo* pSysInfo = GetSysInfo();

	//if(pSysInfo == NULL)
	//  return -1;
	return sprintf(data, "%d", 1);
}
int para_yellow_record_action(char *data, char *arg)
{
	// SysInfo* pSysInfo = GetSysInfo();

	//if(pSysInfo == NULL)
	//  return -1;
	return sprintf(data, "%d", 1);
}
int para_red_record_action(char *data, char *arg)
{
	// SysInfo* pSysInfo = GetSysInfo();

	//if(pSysInfo == NULL)
	//  return -1;
	return sprintf(data, "%d", 1);
}
int para_sup_record_action(char *data, char *arg)
{
	// SysInfo* pSysInfo = GetSysInfo();

	//if(pSysInfo == NULL)
	//  return -1;
	return sprintf(data, "%d", 1);
}

int para_storageswitch(char *data, char *arg)
{
	// SysInfo* pSysInfo = GetSysInfo();

	//if(pSysInfo == NULL)
	//  return -1;
	return sprintf(data, "%d", 1);
}
int para_time_storage_switch(char *data, char *arg)
{
	// SysInfo* pSysInfo = GetSysInfo();

	//if(pSysInfo == NULL)
	//  return -1;
	return sprintf(data, "%d", 1);
}
int para_perstorage(char *data, char *arg)
{
	// SysInfo* pSysInfo = GetSysInfo();

	//if(pSysInfo == NULL)
	//  return -1;
	return sprintf(data, "%d", 1);
}
int para_storage_time_set(char *data, char *arg)
{
     succeed_type iret = succeed_type_failed;
	 storage_setting_param storage_set;
	 iret = Gbshm_paramGet(global_boa_private_handle.gbshm_handle , PARA_CMD_SYSTEM_STORAGE_SETTING_PARAM, &storage_set, sizeof(storage_set));
     switch(storage_set.storage_pre_record_time)
	 {
         case 5:
		 	return sprintf(data ,"%d",0);
		 case 10:
		 	return sprintf(data ,"%d",1);
		 case 30:
		 	return sprintf(data ,"%d",2);
		 case 60:
		 	return sprintf(data ,"%d",3);
	 }
}
int para_event_storage_switch(char *data, char *arg)
{
     return sprintf(data ,"%d",1);
	 succeed_type iret = succeed_type_failed;
	 storage_setting_param storage;
	 iret = Gbshm_paramGet(global_boa_private_handle.gbshm_handle , PARA_CMD_SYSTEM_STORAGE_SETTING_PARAM, &storage, sizeof(storage));
     switch(storage.storage_switch)
	 {
         case STORAGE_SWITCH_ON:
		 	return sprintf(data ,"%d",1);
		 case STORAGE_SWITCH_OFF:
		 	return sprintf(data ,"%d",0);
	 }
}
int para_3gstatus(char *data, char *arg)
{
	succeed_type iret = succeed_type_failed;
	runtime_device_status g3status;
	iret = Gbshm_paramGet(global_boa_private_handle.gbshm_handle, PARA_CMD_SYSTEM_RUNTIME_DEVICE_STATUS, &g3status, sizeof(g3status));
	SYS_INFO("\n\r!!!!!!!!!!!!!!!!!!!GBSHM_PARA_GET gateway.ip = %s iret = %d!!!!!!!!!!!!!!!!!!!!!!!!\r\n",g3status.device_status_online, iret);
    switch(g3status.device_status_online)
	 {
         case DEVICE_STATUS_ON:
		 	return sprintf(data ,"%d",1);
		 case DEVICE_STATUS_OFF:
		 	return sprintf(data ,"%d",0);
	 }
}

int para_videostatus(char *data, char *arg)
{
	succeed_type iret = succeed_type_failed;
	runtime_device_status videostatus;
	iret = Gbshm_paramGet(global_boa_private_handle.gbshm_handle, PARA_CMD_SYSTEM_RUNTIME_DEVICE_STATUS, &videostatus, sizeof(videostatus));
	SYS_INFO("\n\r!!!!!!!!!!!!!!!!!!!GBSHM_PARA_GET gateway.ip = %s iret = %d!!!!!!!!!!!!!!!!!!!!!!!!\r\n",videostatus.device_status_video, iret);
    switch(videostatus.device_status_video)
	 {
         case DEVICE_STATUS_ON:
		 	return sprintf(data ,"%d",1);
		 case DEVICE_STATUS_OFF:
		 	return sprintf(data ,"%d",0);
	 }
}
int para_alarmstatus(char *data, char *arg)
{
	succeed_type iret = succeed_type_failed;
	runtime_device_status alarmstatus;
	iret = Gbshm_paramGet(global_boa_private_handle.gbshm_handle, PARA_CMD_SYSTEM_RUNTIME_DEVICE_STATUS, &alarmstatus, sizeof(alarmstatus));
	SYS_INFO("\n\r!!!!!!!!!!!!!!!!!!!GBSHM_PARA_GET gateway.ip = %s iret = %d!!!!!!!!!!!!!!!!!!!!!!!!\r\n",alarmstatus.device_status_di, iret);
    switch(alarmstatus.device_status_di)
	 {
         case DEVICE_STATUS_ON:
		 	return sprintf(data ,"%d",1);
		 case DEVICE_STATUS_OFF:
		 	return sprintf(data ,"%d",0);
	 }
}
int para_alalrm_line(char *data, char *arg)
{
	// SysInfo* pSysInfo = GetSysInfo();

	//if(pSysInfo == NULL)
	//  return -1;
	return sprintf(data, "%d", 1);
}
int para_baud_rate(char *data, char *arg)
{
	//SysInfo* pSysInfo = GetSysInfo();
	//if(pSysInfo == NULL)
	//return -1;
	return sprintf(data, "%d", 0);
}
int para_sdata_bit(char *data, char *arg)
{
	SysInfo* pSysInfo = GetSysInfo();
	if(pSysInfo == NULL)
		return -1;
	return sprintf(data, "%d", 0);
}
int para_parity_check(char *data, char *arg)
{
	SysInfo* pSysInfo = GetSysInfo();
	if(pSysInfo == NULL)
		return -1;
	return sprintf(data, "%d", 0);
}
int para_stop_bit(char *data, char *arg)
{
	SysInfo* pSysInfo = GetSysInfo();
	if(pSysInfo == NULL)
		return -1;return sprintf(data, "%d", 0);
}
int para_center_ip_add(char *data, char *arg)
{
	succeed_type iret = succeed_type_failed;
	net_param centerip;
	iret = Gbshm_paramGet(global_boa_private_handle.gbshm_handle, PARA_CMD_SYSTEM_NET_PARAM, &centerip, sizeof(centerip));
//	printf("\n\r!!!!!!!!!!!!!!!!!!!GBSHM_PARA_GET centerip = %s iret = %d!!!!!!!!!!!!!!!!!!!!!!!!\r\n",centerip.center_ipaddr, iret);
	return sprintf(data ,centerip.center_ipaddr,strlen(centerip.center_ipaddr));
	
}
int para_center_port(char *data, char *arg)
{
	succeed_type iret = succeed_type_failed;
	net_param gateway;
	iret = Gbshm_paramGet(global_boa_private_handle.gbshm_handle, PARA_CMD_SYSTEM_NET_PARAM, &gateway, sizeof(gateway));
//	printf("\n\r!!!!!!!!!!!!!!!!!!!GBSHM_PARA_GET centerport = %d iret = %d!!!!!!!!!!!!!!!!!!!!!!!!\r\n",gateway.center_port, iret);
	return sprintf(data,"%d", gateway.center_port);
}
int para_audiosource(char *data, char *arg)
{
	 succeed_type iret = succeed_type_failed;
	 SYS_INFO("\n\r!!!!!!!!!!!!!!!!!!!!!Entering !para_imagesource!!!!!!!!!!!!!!!!!!!!!!\r\n");
	 audio_param audio_source;
	 SYS_INFO("\n\r!!!!!!!!!!global_boa_private_handle.gbshm_handle == %x!!!!!!!!!!!!!!\r\n",global_boa_private_handle.gbshm_handle);
	 iret = Gbshm_paramGet(global_boa_private_handle.gbshm_handle , PARA_CMD_SYSTEM_AUDIO_PARAM, &audio_source, sizeof(audio_source));
	 SYS_INFO("\n\r!!!!!!!!!!!!!!!!!!!GBSHM_PARA_GET imagesource = %d iret = %d!!!!!!!!!!!!!!!!!!!!!!!!\r\n",audio_source.audio_mode, iret);
	 switch(audio_source.audio_mode)
	 {
	     case AUDIO_MODE_AMR_G711:
		 	return sprintf(data ,"%d",0);
		 case AUDIO_MODE_AAC_G711:
		 	return sprintf(data ,"%d",1);
	 }
}

int para_center_id(char *data, char *arg)
{
	succeed_type iret = succeed_type_failed;
	net_param center_id;
	iret = Gbshm_paramGet(global_boa_private_handle.gbshm_handle, PARA_CMD_SYSTEM_NET_PARAM, &center_id, sizeof(center_id));
	SYS_INFO("\n\r!!!!!!!!!!!!!!!!!!!GBSHM_PARA_GET centerid = %s iret = %d!!!!!!!!!!!!!!!!!!!!!!!!\r\n",center_id.center_loginid, iret);
	return sprintf(data ,center_id.center_loginid,strlen(center_id.center_loginid));
}
/***************************************************************************
 *                                                                           *
 ****************************************************************************/
#define HASH_TABLE_SIZE	(sizeof(HttpArgument)/sizeof(HTML_ARGUMENT))

HTML_ARGUMENT HttpArgument [] =
{
	{ "date"            , para_date                 , AUTHORITY_VIEWER   , NULL },
	{ "time"            , para_time                 , AUTHORITY_VIEWER   , NULL },
	{ "sntpip"          , para_sntpip               , AUTHORITY_OPERATOR , NULL },
	{ "sntptimezone"    , para_sntptimezone         , AUTHORITY_OPERATOR , NULL },
	{ "sntpfrequency"   , para_sntpfrequency        , AUTHORITY_OPERATOR , NULL },
	{ "dhcpenable"      , para_dhcpenable           , AUTHORITY_OPERATOR , NULL },
	{ "dnsip"           , para_dnsip                , AUTHORITY_OPERATOR , NULL },
	{ "gateway"         , para_gateway              , AUTHORITY_OPERATOR , NULL },
	{ "httpport"        , para_httpport             , AUTHORITY_OPERATOR , NULL },
	{ "netip"           , para_netip                , AUTHORITY_OPERATOR , NULL },
	{ "netmask"         , para_netmask              , AUTHORITY_OPERATOR , NULL },
	{ "title"           , para_title                , AUTHORITY_VIEWER   , NULL },
	{ "regusrname"      , para_regusr               , AUTHORITY_VIEWER   , NULL },
	{ "osdtextinfo"    	, para_osdtextinfo          , AUTHORITY_VIEWER   , NULL },
	{ "maxtitlelen"     , para_maxtitlelen          , AUTHORITY_OPERATOR , NULL },
	{ "liveresolution"  , para_liveresolution       , AUTHORITY_VIEWER   , NULL },
	{ "mpeg4quality"    , para_mpeg4quality         , AUTHORITY_OPERATOR , NULL },
	{ "livequality"     , para_livequality          , AUTHORITY_VIEWER   , NULL },
	{ "maxfqdnlen"      , para_maxfqdnlen           , AUTHORITY_OPERATOR , NULL },
	{ "maxdomainname"   , para_maxdomainname        , AUTHORITY_OPERATOR , NULL },

	{ "ftpip"           , para_ftpip                , AUTHORITY_OPERATOR , NULL },
	{ "ftppiport"       , para_ftppiport            , AUTHORITY_OPERATOR , NULL },
	{ "ftpuser"         , para_ftpuser              , AUTHORITY_OPERATOR , NULL },
	{ "ftppassword"     , para_ftppassword          , AUTHORITY_OPERATOR , NULL },
	{ "ftppath"         , para_ftppath              , AUTHORITY_OPERATOR , NULL },
	{ "maxftpuserlen"   , para_maxftpuserlen        , AUTHORITY_OPERATOR , NULL },
	{ "maxftppwdlen"    , para_maxftppwdlen         , AUTHORITY_OPERATOR , NULL },
	{ "maxftppathlen"   , para_maxftppathlen        , AUTHORITY_OPERATOR , NULL },

	{ "smtpip"          , para_smtpip               , AUTHORITY_OPERATOR , NULL },
	{ "smtpauth"        , para_smtpauth             , AUTHORITY_OPERATOR , NULL },
	{ "smtpuser"        , para_smtpuser             , AUTHORITY_OPERATOR , NULL },
	{ "maxsmtpuser"     , para_maxsmtpuser          , AUTHORITY_OPERATOR , NULL },
	{ "smtppwd"         , para_smtppwd              , AUTHORITY_OPERATOR , NULL },
	{ "maxsmtppwd"      , para_maxsmtppwd           , AUTHORITY_OPERATOR , NULL },
	{ "smtpsender"      , para_smtpsender           , AUTHORITY_OPERATOR , NULL },
	{ "maxsmtpsender"   , para_maxsmtpsender        , AUTHORITY_OPERATOR , NULL },
	{ "emailuser"       , para_emailuser            , AUTHORITY_OPERATOR , NULL },
	{ "maxemailuserlen" , para_maxemailuserlen      , AUTHORITY_OPERATOR , NULL },

	{ "supportmpeg4"    , para_supportmpeg4         , AUTHORITY_VIEWER   , NULL },
	{ "format"          , para_format               , AUTHORITY_VIEWER   , NULL },
	{ "imagesource"     , para_imagesource          , AUTHORITY_VIEWER   , NULL },
	{ "advmode"     	, para_advancemode         	, AUTHORITY_OPERATOR	, NULL },
	{ "m41sfeature"     , para_m41sfeature         	, AUTHORITY_OPERATOR	, NULL },
	{ "m42sfeature"		, para_m42sfeature   		, AUTHORITY_OPERATOR	, NULL },
	{ "jpgsfeature"		, para_jpgsfeature			, AUTHORITY_OPERATOR	, NULL },
	{ "advfeaturename"	, para_advfeaturename		, AUTHORITY_OPERATOR	, NULL },
	{ "preprocess"		, para_preprocess			, AUTHORITY_OPERATOR	, NULL },
	{ "preprocessname"	, para_preprocessname		, AUTHORITY_OPERATOR	, NULL },
	{ "noisefilt"		, para_noisefilt			, AUTHORITY_OPERATOR	, NULL },
	{ "noisefiltname"	, para_noisefiltname		, AUTHORITY_OPERATOR	, NULL },
	{ "osdwin"     		, para_osdwin          		, AUTHORITY_VIEWER   , NULL },
	{ "histogram"     	, para_histogram          		, AUTHORITY_VIEWER   , NULL },
	{ "gbce"     		, para_gbce          		, AUTHORITY_VIEWER   , NULL },
	{ "devicename"      , para_devicename           , AUTHORITY_VIEWER   , NULL },
	{ "defaultstorage"  , para_defaultstorage       , AUTHORITY_VIEWER   , NULL },
	{ "sdinsert"        , para_sdinsert             , AUTHORITY_VIEWER   , NULL },
	{ "cfinsert"        , para_cfinsert             , AUTHORITY_VIEWER   , NULL },
	{ "defaultcardgethtm", para_defaultcardgethtm   , AUTHORITY_VIEWER   , NULL },
	{ "brandurl"         , para_brandurl            , AUTHORITY_VIEWER   , NULL },
	{ "brandname"        , para_brandname           , AUTHORITY_VIEWER   , NULL },
	{ "supporttstamp"    , para_supporttstamp       , AUTHORITY_VIEWER   , NULL },
	{ "mpeg4xsize"       , para_mpeg4xsize          , AUTHORITY_VIEWER   , NULL },
	{ "mpeg4ysize"       , para_mpeg4ysize          , AUTHORITY_VIEWER   , NULL },
	{ "jpegxsize"        , para_jpegxsize           , AUTHORITY_VIEWER   , NULL },
	{ "jpegysize"        , para_jpegysize           , AUTHORITY_VIEWER   , NULL },
	{ "socketauthority"  , para_socketauthority     , AUTHORITY_VIEWER   , NULL },
	{ "authoritychange"  , para_authoritychange     , AUTHORITY_VIEWER   , NULL },
	{ "supportmotion"    , para_supportmotion       , AUTHORITY_VIEWER   , NULL },
	{ "supportwireless"  , para_supportwireless     , AUTHORITY_VIEWER   , NULL },
	{ "serviceftpclient" , para_serviceftpclient    , AUTHORITY_VIEWER   , NULL },
	{ "servicesmtpclient", para_servicesmtpclient   , AUTHORITY_VIEWER   , NULL },
	{ "servicepppoe"     , para_servicepppoe        , AUTHORITY_VIEWER   , NULL },
	{ "servicesntpclient", para_servicesntpclient   , AUTHORITY_VIEWER   , NULL },
	{ "serviceddnsclient", para_serviceddnsclient   , AUTHORITY_VIEWER   , NULL },
	{ "supportmaskarea"  , para_supportmaskarea     , AUTHORITY_VIEWER   , NULL },
	{ "machinecode"      , para_machinecode         , AUTHORITY_VIEWER   , NULL },
	{ "maxchannel"       , para_maxchannel          , AUTHORITY_VIEWER   , NULL },
	{ "supportrs485"     , para_supportrs485        , AUTHORITY_VIEWER   , NULL },
	{ "supportrs232"     , para_supportrs232        , AUTHORITY_VIEWER   , NULL },
	{ "layoutnum"        , para_layoutnum           , AUTHORITY_VIEWER   , NULL },
	{ "supportmui"       , para_supportmui          , AUTHORITY_VIEWER   , NULL },
	{ "mui"              , para_mui                 , AUTHORITY_VIEWER   , NULL },
	{ "supportsequence"  , para_supportsequence     , AUTHORITY_VIEWER   , NULL },
	{ "quadmodeselect"   , para_quadmodeselect      , AUTHORITY_VIEWER   , NULL },
	{ "serviceipfilter"  , para_serviceipfilter     , AUTHORITY_VIEWER   , NULL },
	{ "oemflag0"         , para_oemflag0            , AUTHORITY_VIEWER   , NULL },
	{ "supportdncontrol" , para_supportdncontrol    , AUTHORITY_VIEWER   , NULL },
	{ "supportavc"       , para_supportavc          , AUTHORITY_VIEWER   , NULL },
	{ "supportaudio"     , para_supportaudio        , AUTHORITY_VIEWER   , NULL },
	{ "supportptzpage"   , para_supportptzpage      , AUTHORITY_VIEWER   , NULL },

	{ "dhcpconfig"			, para_dhcpconfig			, AUTHORITY_OPERATOR , NULL },
	{ "audioenable"			, para_audioenable			, AUTHORITY_VIEWER   , NULL },
	{ "daylight"			, para_daylight				, AUTHORITY_VIEWER   , NULL },
	{ "timeformat"			, para_timeformat			, AUTHORITY_VIEWER   , NULL },
	{ "pppoeenable"			, para_pppoeenable			, AUTHORITY_OPERATOR , NULL },
	{ "mpeg4desired"		, para_mpeg4desired			, AUTHORITY_OPERATOR , NULL },
	{ "mpeg4cenable"		, para_mpeg4cenable			, AUTHORITY_OPERATOR , NULL },
	{ "mpeg4cvalue"			, para_mpeg4cvalue			, AUTHORITY_OPERATOR , NULL },
	{ "mpeg42cvalue"		, para_mpeg42cvalue			, AUTHORITY_OPERATOR , NULL },
	{ "mpeg4framerate"		, para_mpeg41framerate		, AUTHORITY_OPERATOR , NULL },
	{ "mpeg4frameratename"	, para_mpeg41frameratename	, AUTHORITY_OPERATOR , NULL },
	{ "mpeg42framerate"		, para_mpeg42framerate		, AUTHORITY_OPERATOR , NULL },
	{ "mpeg42frameratename"	, para_mpeg42frameratename	, AUTHORITY_OPERATOR , NULL },
	{ "jpegframerate"		, para_jpegframerate		, AUTHORITY_OPERATOR , NULL },
	{ "jpegframeratename"	, para_jpegframeratename	, AUTHORITY_OPERATOR , NULL },
	{ "mpeg4resname"		, para_mpeg4resname			, AUTHORITY_OPERATOR , NULL },
	{ "mpeg4resolution"		, para_mpeg4resolution      , AUTHORITY_OPERATOR , NULL },
	{ "mpeg42resname"		, para_mpeg42resname		, AUTHORITY_OPERATOR , NULL },
	{ "mpeg42resolution"	, para_mpeg42resolution		, AUTHORITY_OPERATOR , NULL },
	{ "resolutionname"		, para_resolutionname		, AUTHORITY_OPERATOR , NULL },
	{ "mpeg4qualityname"	, para_mpeg4qualityname		, AUTHORITY_OPERATOR , NULL },
	{ "qualityname"			, para_qualityname			, AUTHORITY_VIEWER   , NULL },
	{ "videocodecresname"	, para_videocodecresname	, AUTHORITY_OPERATOR , NULL },
	{ "videoresmenu"		, para_videoresmenu		, AUTHORITY_OPERATOR , NULL },
	{ "videocodecmodename"	, para_videocodecmodename	, AUTHORITY_OPERATOR , NULL },
	{ "videocodecmenu"		, para_videocodecmenu		, AUTHORITY_OPERATOR , NULL },
	{ "videoframemenu"		, para_videoframemenu		, AUTHORITY_OPERATOR , NULL },
	{ "videokeyframe"		, para_videokeyframe		, AUTHORITY_OPERATOR , NULL },
	//2010-09-24 14:13
	{ "videocodecmode1"		, para_videocodecmode1		, AUTHORITY_OPERATOR , NULL },
	{ "waitserver"			, para_waitserver			, AUTHORITY_OPERATOR , NULL },
	{ "supportcolorkiller"	, para_supportcolorkiller	, AUTHORITY_VIEWER   , NULL },
	{ "supportAWB"			, para_supportAWB			, AUTHORITY_VIEWER   , NULL },
	{ "supportbrightness"	, para_supportbrightness	, AUTHORITY_VIEWER   , NULL },
	{ "supportcontrast"		, para_supportcontrast		, AUTHORITY_VIEWER   , NULL },
	{ "supportsaturation"	, para_supportsaturation	, AUTHORITY_VIEWER   , NULL },
	{ "supportbacklight"	, para_supportbacklight		, AUTHORITY_VIEWER   , NULL },
	{ "supportsharpness"	, para_supportsharpness		, AUTHORITY_VIEWER   , NULL },

	{ "quadmodeselectname"	, para_quadmodeselectname	, AUTHORITY_OPERATOR , NULL },
	{ "videomodename"		, para_videomodename		, AUTHORITY_VIEWER , NULL },
	{ "videomode"		    , para_videomode		    , AUTHORITY_VIEWER , NULL },
	{ "supportagc"		, para_supportagc			, AUTHORITY_VIEWER , NULL },
	{ "agc"					, para_agc					, AUTHORITY_OPERATOR , NULL },
	{ "colorkiller"			, para_daynight			, AUTHORITY_OPERATOR , NULL },
	{ "fluorescent"		, para_fluorescent			, AUTHORITY_VIEWER , NULL },
	{ "mirror"			    , para_mirror				, AUTHORITY_OPERATOR , NULL },
	{ "blc"				, para_blc					, AUTHORITY_VIEWER , NULL },
	{ "kelvin"			, para_kelvin				, AUTHORITY_VIEWER , NULL },
	{ "supporthue"		, para_supporthue			, AUTHORITY_VIEWER , NULL },
	{ "supportexposure"		, para_supportexposure		, AUTHORITY_VIEWER , NULL },
	{ "supportfluorescent"	, para_supportfluorescent	, AUTHORITY_VIEWER , NULL },
	{ "supportmirros"		, para_supportmirros		, AUTHORITY_VIEWER , NULL },
	{ "supportkelvin"		, para_supportkelvin		, AUTHORITY_VIEWER , NULL },
	{ "senseup"			, para_senseup				, AUTHORITY_VIEWER , NULL },
	{ "supportsenseup"		, para_supportsenseup		, AUTHORITY_VIEWER , NULL },
	{ "supportmaxagcgain"	, para_supportmaxagcgain	, AUTHORITY_VIEWER , NULL },
	{ "supportblcmode"		, para_supportblcmode		, AUTHORITY_VIEWER , NULL },
	{ "blcmode"			, para_blcmode				, AUTHORITY_VIEWER , NULL },
	{ "supporthspeedshutter"	, para_supporthspeedshutter	, AUTHORITY_VIEWER , NULL },
	{ "hspeedshutter"		, para_hspeedshutter		, AUTHORITY_VIEWER , NULL },
	{ "awbname"			, para_awbname				, AUTHORITY_VIEWER , NULL },
	{ "awb"				    , para_awb					, AUTHORITY_OPERATOR , NULL },
	{ "binningname"		, para_binningname				, AUTHORITY_VIEWER , NULL },
	{ "binning"			, para_binning					, AUTHORITY_VIEWER , NULL },
	{ "maxagcgainname"		, para_maxagcgainname		, AUTHORITY_VIEWER , NULL },
	{ "maxagcgain"		, para_maxagcgain			, AUTHORITY_VIEWER , NULL },
	{ "daynightname"		, para_daynightname			, AUTHORITY_VIEWER , NULL },
	{ "tvcable"			, para_tvcable				, AUTHORITY_VIEWER , NULL },
	{ "blcmodename"		, para_blcmodename			, AUTHORITY_VIEWER , NULL },
	{ "backlight"			, para_backlight			, AUTHORITY_VIEWER , NULL },
	{ "brightness"			, para_brightness			, AUTHORITY_VIEWER , NULL },
	{ "contrast"			, para_contrast				, AUTHORITY_VIEWER , NULL },
	{ "hue"				    , para_hue					, AUTHORITY_VIEWER , NULL },
	{ "saturation"			, para_saturation			, AUTHORITY_VIEWER , NULL },
	{ "exposure"			, para_exposure				, AUTHORITY_VIEWER , NULL },
	{ "sharpness"			, para_sharpness			, AUTHORITY_OPERATOR , NULL },
	{ "timezonename"		, para_timezonename			, AUTHORITY_OPERATOR , NULL },
	{ "timeformatname"		, para_timeformatname		, AUTHORITY_OPERATOR , NULL },
	{ "schedule"			, para_schedule				, AUTHORITY_OPERATOR , NULL },

	{ "motionblock"		    , para_motionblock			, AUTHORITY_OPERATOR , NULL },
	{ "motionenable"		    , para_motionenable			, AUTHORITY_OPERATOR , NULL },
	{ "motionsensitivity"		, para_motionsensitivity	, AUTHORITY_OPERATOR , NULL },
	{ "motionname"		    , para_motionname			, AUTHORITY_OPERATOR , NULL },
	{ "motioncenable"		    , para_motioncenable		, AUTHORITY_OPERATOR , NULL },
	{ "motioncvalue"      	, para_motioncvalue			, AUTHORITY_OPERATOR , NULL },
	{ "motionlevel"		    , para_motionlevel			, AUTHORITY_OPERATOR , NULL },
	{ "minmotionthreshold"	, para_minmotionthreshold	, AUTHORITY_OPERATOR , NULL },
	{ "maxmotionthreshold"	, para_maxmotionthreshold	, AUTHORITY_OPERATOR , NULL },
	{ "lostalarm"			    , para_lostalarm			, AUTHORITY_OPERATOR , NULL },
	{ "sdaenable"			    , para_sdaenable			, AUTHORITY_OPERATOR , NULL },
	{ "aftpenable"			, para_aftpenable			, AUTHORITY_OPERATOR , NULL },
	{ "asmtpenable"			, para_asmtpenable			, AUTHORITY_OPERATOR , NULL },
	{ "recordduration"		, para_recordduration		, AUTHORITY_OPERATOR , NULL },
	{ "alarmduration"			, para_alarmduration		, AUTHORITY_OPERATOR , NULL },
	{ "motionxlimit"			, para_motionxlimit	    	, AUTHORITY_OPERATOR , NULL },
	{ "motionylimit"	    	, para_motionylimit			, AUTHORITY_OPERATOR , NULL },
	{ "motionxblock"			, para_motionxblock		    , AUTHORITY_OPERATOR , NULL },
	{ "motionyblock"			, para_motionyblock		    , AUTHORITY_OPERATOR , NULL },
	{ "ipncptz"			    , para_ipncptz		        , AUTHORITY_OPERATOR , NULL },

	{ "maxnamelen"	    	, para_maxnamelen			, AUTHORITY_VIEWER , NULL },
	{ "maxpwdlen"		        , para_maxpwdlen			, AUTHORITY_VIEWER , NULL },
	{ "authorityadmin"		, para_authorityadmin		, AUTHORITY_VIEWER , NULL },
	{ "authorityoperator"		, para_authorityoperator	, AUTHORITY_VIEWER , NULL },
	{ "authorityviewer"		, para_authorityviewer		, AUTHORITY_VIEWER , NULL },

	{ "minnamelen"		    , para_minnamelen			, AUTHORITY_VIEWER , NULL },
	{ "user"			    , para_user				    , AUTHORITY_OPERATOR , NULL },
	{ "authority"		    , para_authority	   	    , AUTHORITY_OPERATOR , NULL },

	{ "sdleft"		       , para_sdleft			, AUTHORITY_OPERATOR , NULL },
	{ "sdused"		       , para_sdused			, AUTHORITY_OPERATOR , NULL },

	{ "sdfileformat"	    , para_sdfileformat		    , AUTHORITY_OPERATOR , NULL },
	{ "sdfileformatname"    , para_sdformatName		, AUTHORITY_OPERATOR , NULL },
	{ "aviduration"	        , para_aviduration		, AUTHORITY_OPERATOR , NULL },
	{ "avidurationname"	    , para_avidurationname	, AUTHORITY_OPERATOR , NULL },
	{ "aviformat"	        , para_aviformat		, AUTHORITY_OPERATOR , NULL },
	{ "aviformatname"	    , para_aviformatname	, AUTHORITY_OPERATOR , NULL },
	{ "ftpfileformat"	    , para_ftpfileformat		, AUTHORITY_OPERATOR , NULL },
	{ "ftpfileformatname"   , para_formatName		, AUTHORITY_OPERATOR , NULL },
	{ "attfileformat"	    , para_attfileformat	, AUTHORITY_OPERATOR , NULL },
	{ "attfileformatname"   , para_formatName		, AUTHORITY_OPERATOR , NULL },
	{ "stream1xsize"		, para_stream1xsize	  	, AUTHORITY_VIEWER , NULL },
	{ "stream1ysize"		, para_stream1ysize	  	, AUTHORITY_VIEWER , NULL },
	{ "stream2xsize"		, para_stream2xsize	  	, AUTHORITY_VIEWER , NULL },
	{ "stream2ysize"		, para_stream2ysize	  	, AUTHORITY_VIEWER , NULL },
	{ "stream3xsize"		, para_stream3xsize	  	, AUTHORITY_VIEWER , NULL },
	{ "stream3ysize"		, para_stream3ysize	  	, AUTHORITY_VIEWER , NULL },
	{ "stream4xsize"		, para_stream1xsize	  	, AUTHORITY_VIEWER , NULL },
	{ "stream4ysize"		, para_stream1ysize	  	, AUTHORITY_VIEWER , NULL },
	{ "stream5xsize"		, para_stream5xsize	  	, AUTHORITY_VIEWER , NULL },
	{ "stream5ysize"		, para_stream5ysize	  	, AUTHORITY_VIEWER , NULL },
	{ "stream6xsize"		, para_stream6xsize	  	, AUTHORITY_VIEWER , NULL },
	{ "stream6ysize"		, para_stream6ysize	  	, AUTHORITY_VIEWER , NULL },
	{ "stream1name"			, para_stream1name	  	, AUTHORITY_VIEWER , NULL },
	{ "stream2name"			, para_stream2name	  	, AUTHORITY_VIEWER , NULL },
	{ "stream3name"			, para_stream3name	  	, AUTHORITY_VIEWER , NULL },
	{ "stream4name"			, para_stream4name	  	, AUTHORITY_VIEWER , NULL },
	{ "stream5name"			, para_stream5name	  	, AUTHORITY_VIEWER , NULL },
	{ "stream6name"			, para_stream6name	  	, AUTHORITY_VIEWER , NULL },

	{ "supportstream1"		, para_supportstream1	, AUTHORITY_VIEWER , NULL },
	{ "supportstream2"		, para_supportstream2	, AUTHORITY_VIEWER , NULL },
	{ "supportstream3"		, para_supportstream3	, AUTHORITY_VIEWER , NULL },
	{ "supportstream4"		, para_supportstream4	, AUTHORITY_VIEWER , NULL },
	{ "supportstream5"		, para_supportstream5	, AUTHORITY_VIEWER , NULL },
	{ "supportstream6"		, para_supportstream6	, AUTHORITY_VIEWER , NULL },
	{ "img2a"				, para_image2a			, AUTHORITY_VIEWER , NULL },
	{ "img2aname"			, para_img2aname		, AUTHORITY_VIEWER , NULL },
	{ "img2atype"			, para_image2atype		, AUTHORITY_VIEWER , NULL },
	{ "img2atypename"		, para_img2atypename	, AUTHORITY_VIEWER , NULL },
	{ "democfg"				, para_democfg 			, AUTHORITY_VIEWER , NULL },
	{ "democfgname"			, para_democfgname		, AUTHORITY_VIEWER , NULL },
	{ "rotctrl"				, para_rotctrl 			, AUTHORITY_VIEWER , NULL },
	{ "rotctrlname"			, para_rotctrlname		, AUTHORITY_VIEWER , NULL },
	{ "mirctrl"				, para_mirctrl 			, AUTHORITY_VIEWER , NULL },
	{ "mirctrlname"			, para_mirctrlname		, AUTHORITY_VIEWER , NULL },
	{ "fdetect"				, para_fdetect 			, AUTHORITY_VIEWER , NULL },
	{ "fdetectname"			, para_fdetectname		, AUTHORITY_VIEWER , NULL },
	{ "osdstream"			, para_osdstream   		, AUTHORITY_VIEWER , NULL },
	{ "osdstreamname"		, para_osdstreamname	, AUTHORITY_VIEWER , NULL },
	{ "osdwinnum"			, para_osdwinnum   		, AUTHORITY_VIEWER , NULL },
	{ "osdwinnumname"		, para_osdwinnumname	, AUTHORITY_VIEWER , NULL },
	{ "ratecontrol"			, para_ratecontrol		, AUTHORITY_VIEWER , NULL },
	{ "ratecontrolname"		, para_ratecontrolname	, AUTHORITY_VIEWER , NULL },

	{ "kernelversion"		, para_kernelversion	, AUTHORITY_VIEWER , NULL },
	{ "biosversion"			, para_biosversion		, AUTHORITY_VIEWER , NULL },

	{ "smtpminattach"       , para_smtpminattach	, AUTHORITY_OPERATOR , NULL },
	{ "smtpmaxattach"       , para_smtpmaxattach	, AUTHORITY_OPERATOR , NULL },
	{ "asmtpattach"		    , para_asmtpattach		, AUTHORITY_OPERATOR , NULL },

	{ "rftpenable"          , para_rftpenable		, AUTHORITY_OPERATOR , NULL },
	{ "sdrenable"		    , para_sdrenable		, AUTHORITY_OPERATOR , NULL },
	//	{ "cfrenable"		    , para_cfrenable  		, AUTHORITY_OPERATOR , NULL },
	{ "multicast"		    , para_multicast  		, AUTHORITY_OPERATOR , NULL },

	{ "gioinenable"		    , para_gioinenable 		, AUTHORITY_OPERATOR , NULL },
	{ "giointype"		    , para_giointype  		 , AUTHORITY_OPERATOR , NULL },
	{ "gioinname"		    , para_gioinname  		 , AUTHORITY_OPERATOR , NULL },
	{ "giooutenable"	    , para_giooutenable 	 , AUTHORITY_OPERATOR , NULL },
	{ "gioouttype"		    , para_gioouttype  		 , AUTHORITY_OPERATOR , NULL },
	{ "giooutname"		    , para_giooutname  		 , AUTHORITY_OPERATOR , NULL },
	{ "softwareversion"	    , para_softwareversion   , AUTHORITY_OPERATOR , NULL },
	{ "tstampenable"	    , para_tstampenable	     , AUTHORITY_OPERATOR , NULL },
	{ "tstampcolorname"	    , para_tstampcolorname	 , AUTHORITY_OPERATOR , NULL },
	{ "tstamplocname"	    , para_tstamplocname	 , AUTHORITY_OPERATOR , NULL },
	{ "tstampformatname"    , para_tstampformatname  , AUTHORITY_OPERATOR , NULL },
	{ "tstampformat"        , para_tstampformat      , AUTHORITY_OPERATOR , NULL },
	{ "audioinmax"          , para_audioinmax        , AUTHORITY_OPERATOR , NULL },
	{ "audioinvolume"       , para_audioinvolume     , AUTHORITY_OPERATOR , NULL },
	{ "eventlocation"	    , para_event_location  		, AUTHORITY_OPERATOR , NULL },
	{ "event"	    , para_get_event  		, AUTHORITY_OPERATOR , NULL },

	//2010-09-24  13:49
	{ "smtpSwitch"			, para_smtpswitch			, AUTHORITY_OPERATOR,NULL }, 
	{ "ntpSwitch"			, para_ntpswitch 			, AUTHORITY_OPERATOR, NULL },
	{ "rtspSwitch"			, para_rtspswitch			, AUTHORITY_OPERATOR,  NULL },
	{ "h2641"				, para_h2641 		, AUTHORITY_OPERATOR,  NULL },
	{ "h2642"				, para_h2642 		, AUTHORITY_OPERATOR,  NULL },
	{ "mpeg41"				, para_mpeg41			, AUTHORITY_OPERATOR,  NULL },
	{ "mpeg42"				, para_mpeg42			, AUTHORITY_OPERATOR,  NULL },
	{ "jpeg"				, para_jpeg			, AUTHORITY_OPERATOR,  NULL },
	{ "input"				, para_input 		, AUTHORITY_OPERATOR,  NULL },
	{ "yellow_line" 		, para_yellow_line			, AUTHORITY_OPERATOR, NULL },
	{ "red" 				, para_red			, AUTHORITY_OPERATOR,  NULL },
	{ "superspeed"			, para_superspeed			, AUTHORITY_OPERATOR,  NULL },
	{ "di_do_action"		, para_di_do_action			, AUTHORITY_OPERATOR, NULL },
	{ "yellow_do_action"		, para_yellow_do_action			, AUTHORITY_OPERATOR,  NULL },
	{ "red_do_action"		, para_red_do_action 		, AUTHORITY_OPERATOR,  NULL },
	{ "sup_do_action"		, para_sup_do_action 		, AUTHORITY_OPERATOR,  NULL },
	{ "di_smtp_action"		, para_smtp_action			, AUTHORITY_OPERATOR, NULL },
	{ "yellow_smtp_action"		, para_yellow_smtp_action			, AUTHORITY_OPERATOR,  NULL },
	{ "red_smtp_action" 	, para_red_smtp_action			, AUTHORITY_OPERATOR, NULL },
	{ "sup_smtp_action" 	, para_sup_smtp_action			, AUTHORITY_OPERATOR,  NULL },
	{ "di_record_action"	, para_di_record_action			, AUTHORITY_OPERATOR,  NULL },
	{ "yellow_record_action", para_yellow_record_action			, AUTHORITY_OPERATOR,  NULL },
	{ "red_record_action"	, para_red_record_action 		, AUTHORITY_OPERATOR,  NULL },
	{ "sup_record_action"	, para_sup_record_action 		, AUTHORITY_OPERATOR,  NULL },

	{ "storageswitch"		, para_storageswitch 		, AUTHORITY_OPERATOR,  NULL },
	{ "time_storage_switch" , para_time_storage_switch			, AUTHORITY_OPERATOR,  NULL },
	{ "perstorage"			, para_perstorage			, AUTHORITY_OPERATOR,  NULL },
	{ "StorageTime"			, para_storage_time_set			, AUTHORITY_OPERATOR,  NULL },
	{ "EventStorageSwitch", para_event_storage_switch			, AUTHORITY_OPERATOR,  NULL },
	{ "g3netstatus" 		, para_3gstatus		, AUTHORITY_VIEWER,  NULL },
	{ "videostatus" 		, para_videostatus	, AUTHORITY_VIEWER,  NULL },
	{ "alarmstatus" 		, para_alarmstatus	, AUTHORITY_VIEWER,  NULL }, 
	{ "alarm_line" 		, para_alalrm_line			, AUTHORITY_OPERATOR, NULL },
	{ "serial_baud_rate"    , para_baud_rate         , AUTHORITY_OPERATOR , NULL },
	{ "serial_sdata_bit"    , para_sdata_bit         , AUTHORITY_OPERATOR , NULL },
	{ "serial_parity_check"    , para_parity_check         , AUTHORITY_OPERATOR , NULL },
	{ "serial_stop_bit"    , para_stop_bit         , AUTHORITY_OPERATOR , NULL },
	{ "center_port"        , para_center_port             , AUTHORITY_OPERATOR , NULL },
	{ "center_ip_add"           , para_center_ip_add                , AUTHORITY_OPERATOR , NULL },
	{ "audiosource"     , para_audiosource          ,AUTHORITY_VIEWER, NULL },
	{ "center_id"   , para_center_id            , AUTHORITY_OPERATOR , NULL },
};

/***************************************************************************
 *                                                                         *
 ***************************************************************************/
unsigned int arg_hash_cal_value(char *name)
{
	unsigned int value = 0;

	while (*name)
		value = value * 37 + (unsigned int)(*name++);
	return value;
}

/***************************************************************************
 *                                                                         *
 ***************************************************************************/
void arg_hash_insert_entry(ARG_HASH_TABLE *table, HTML_ARGUMENT *arg)
{
	if (table->harg) {
		arg->next = table->harg;
	}
	table->harg = arg;
}

/***************************************************************************
 *                                                                         *
 ***************************************************************************/
int arg_hash_table_init(void)
{
	int i;

	if ( (arg_hash = (ARG_HASH_TABLE *)calloc(sizeof(ARG_HASH_TABLE), MAX_ARG_HASH_SIZE)) == NULL) {
		return -1;
	}
	for (i=0; i<HASH_TABLE_SIZE; i++) {
		arg_hash_insert_entry(arg_hash+(arg_hash_cal_value(HttpArgument[i].name)%MAX_ARG_HASH_SIZE), HttpArgument+i);
	}
	return 0;
}

/***************************************************************************
 *                                                                         *
 ***************************************************************************/
int TranslateWebPara(AUTHORITY authority, char *target, char *para, char *subpara)
{
	HTML_ARGUMENT *htmp;
	htmp = arg_hash[arg_hash_cal_value(para)%MAX_ARG_HASH_SIZE].harg;

	while (htmp) {
		if ( strcasecmp(htmp->name, para) == 0 ) {
			if (authority > htmp->authority) {
				dbg("[%s.%s] permission denied!!!\n", para, subpara);
				return -1;
			}
			return (*htmp->handler) (target, subpara);
		}
		htmp = htmp->next;
	}
	dbg("[%s.%s] not found\n", para, subpara);
	return -1;
}

/***************************************************************************
 *                                                                         *
 ***************************************************************************/

void arg_hash_table_cleanup()
{
	free(arg_hash);
}

/***************************************************************************
 *                                                                         *
 ***************************************************************************/

int ShowAllWebValue(char *data, int max_size, AUTHORITY authority)
{
	HTML_ARGUMENT *htmp;
	int i, total_size = 0, size;
	char buf[1024];
	for(i = 0;i < HASH_TABLE_SIZE; i++){
		htmp = &HttpArgument[i];
		if (authority > htmp->authority)
			continue;
		if(htmp ->handler == NULL)
			continue;
		size = sprintf(buf, "%s=", htmp->name);
		if(total_size + size + 1 > max_size){
			total_size = sprintf(data, "Not enogh size to show");
			break;
		}
		total_size += sprintf(data+total_size, "%s", buf);
		size = (*htmp->handler) (buf, "");
		if(size < 0){
			size = sprintf(buf, "Error return");
		}
		if(total_size + size + 1 > max_size){
			total_size = sprintf(data, "Not enogh size to show");
			break;
		}
		total_size += sprintf(data+total_size, "%s<br>", buf);
	}
	return total_size;
}

/***************************************************************************
 *                                                                         *
 ***************************************************************************/

static int ShowPara(char* buf, int index, char* name, AUTHORITY authority)
{
	char strAuthority[5][20] = {"ADMINISTRATOR","OPERATOR","VIEWER","NONE","ERROR"};
	int a_index;
	switch(authority){
		case AUTHORITY_ADMIN:
			a_index = 0;
			break;
		case AUTHORITY_OPERATOR:
			a_index = 1;
			break;
		case AUTHORITY_VIEWER:
			a_index = 2;
			break;
		case AUTHORITY_NONE:
			a_index = 3;
			break;
		default:
			a_index = 4;
			break;
	}
	return sprintf(buf, "%3d.%-25s%s\n", index, name, strAuthority[a_index]);
}

/***************************************************************************
 *                                                                         *
 ***************************************************************************/

int ShowAllPara(char *data, int max_size, AUTHORITY authority)
{
	HTML_ARGUMENT *htmp;
	int i, total_size = 0, size,count = 0;
	char buf[80];
	for(i = 0;i < HASH_TABLE_SIZE; i++){
		htmp = &HttpArgument[i];
		if(htmp ->handler == NULL)
			continue;
		size = ShowPara(buf, ++count, htmp->name, htmp->authority);
		if(total_size + size + 1 > max_size){
			total_size = sprintf(data, "Not enogh size to show");
			break;
		}
		total_size += sprintf(data+total_size, "%s", buf);
	}
	return total_size;
}


