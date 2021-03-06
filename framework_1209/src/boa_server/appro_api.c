#include <string.h>
#include <stdlib.h>
#include <asm/types.h>
#include <time.h>
#include <sys/ioctl.h>
#include <arpa/inet.h>
#include <net/if.h>
#include "boa.h"
#include "appro_api.h"
#include "net_config.h"
#include <sysctrl.h>
#include "file_list.h"
#include <sys_env_type.h>
#include <sysctrl_struct.h>
#include <file_msg_drv.h>
#include <system_default.h>

//20110411
#include "boa_private.h"

static char *nicname = "eth0";

unsigned int event_location = 0;

static char gf_Restarting = 0;
static char gf_CmdBlocking = 0;
static char codecNumberValue=0;
static char codecMenuValue=0;
static char codecSetCount=0;
static char alarm_status = 0;
unsigned char lower_ascii[256] = {
	0x00, 0x01, 0x02, 0x03, 0x04, 0x05, 0x06, 0x07,
	0x08, 0x09, 0x0a, 0x0b, 0x0c, 0x0d, 0x0e, 0x0f,
	0x10, 0x11, 0x12, 0x13, 0x14, 0x15, 0x16, 0x17,
	0x18, 0x19, 0x1a, 0x1b, 0x1c, 0x1d, 0x1e, 0x1f,
	0x20, 0x21, 0x22, 0x23, 0x24, 0x25, 0x26, 0x27,
	0x28, 0x29, 0x2a, 0x2b, 0x2c, 0x2d, 0x2e, 0x2f,
	0x30, 0x31, 0x32, 0x33, 0x34, 0x35, 0x36, 0x37,
	0x38, 0x39, 0x3a, 0x3b, 0x3c, 0x3d, 0x3e, 0x3f,
	0x40, 0x61, 0x62, 0x63, 0x64, 0x65, 0x66, 0x67,
	0x68, 0x69, 0x6a, 0x6b, 0x6c, 0x6d, 0x6e, 0x6f,
	0x70, 0x71, 0x72, 0x73, 0x74, 0x75, 0x76, 0x77,
	0x78, 0x79, 0x7a, 0x5b, 0x5c, 0x5d, 0x5e, 0x5f,
	0x60, 0x61, 0x62, 0x63, 0x64, 0x65, 0x66, 0x67,
	0x68, 0x69, 0x6a, 0x6b, 0x6c, 0x6d, 0x6e, 0x6f,
	0x70, 0x71, 0x72, 0x73, 0x74, 0x75, 0x76, 0x77,
	0x78, 0x79, 0x7a, 0x7b, 0x7c, 0x7d, 0x7e, 0x7f,
	0x80, 0x81, 0x82, 0x83, 0x84, 0x85, 0x86, 0x87,
	0x88, 0x89, 0x8a, 0x8b, 0x8c, 0x8d, 0x8e, 0x8f,
	0x90, 0x91, 0x92, 0x93, 0x94, 0x95, 0x96, 0x97,
	0x98, 0x99, 0x9a, 0x9b, 0x9c, 0x9d, 0x9e, 0x9f,
	0xa0, 0xa1, 0xa2, 0xa3, 0xa4, 0xa5, 0xa6, 0xa7,
	0xa8, 0xa9, 0xaa, 0xab, 0xac, 0xad, 0xae, 0xaf,
	0xb0, 0xb1, 0xb2, 0xb3, 0xb4, 0xb5, 0xb6, 0xb7,
	0xb8, 0xb9, 0xba, 0xbb, 0xbc, 0xbd, 0xbe, 0xbf,
	0xc0, 0xc1, 0xc2, 0xc3, 0xc4, 0xc5, 0xc6, 0xc7,
	0xc8, 0xc9, 0xca, 0xcb, 0xcc, 0xcd, 0xce, 0xcf,
	0xd0, 0xd1, 0xd2, 0xd3, 0xd4, 0xd5, 0xd6, 0xd7,
	0xd8, 0xd9, 0xda, 0xdb, 0xdc, 0xdd, 0xde, 0xdf,
	0xe0, 0xe1, 0xe2, 0xe3, 0xe4, 0xe5, 0xe6, 0xe7,
	0xe8, 0xe9, 0xea, 0xeb, 0xec, 0xed, 0xee, 0xef,
	0xf0, 0xf1, 0xf2, 0xf3, 0xf4, 0xf5, 0xf6, 0xf7,
	0xf8, 0xf9, 0xfa, 0xfb, 0xfc, 0xfd, 0xfe, 0xff
};

net_param net_setting;
video_enc_param video_setting;
ad_setting_param ad_setting;
audio_param audio_setting;
storage_setting_param storage_setting;

extern int TranslateWebPara(AUTHORITY authority, char *target, char *para, char *subpara);

void strtolower(unsigned char *str)
{
	while (*str) {
		*str = lower_ascii[*str];
		str++;
	}
}

typedef union __NET_IPV4 {
	__u32	int32;
	__u8	str[4];
} NET_IPV4;

typedef struct __NET_IP_STR{
	__u8    str[15];
}NET_IP_STR;
/***************************************************************************
 *                                                                         *
 ***************************************************************************/
int ipstrsplit( char *pAddress, NET_IP_STR *ptr_ip )
{
	unsigned int firstflag = 1;
	int iLoop0 = 0;
	int iLoop1 = 0;
	for(iLoop0 = 0; iLoop0 < 12; iLoop0++)
	{
		if ( (firstflag == 0) && (pAddress[iLoop0] == '0' ))
		{
			continue;
		}
		if ( pAddress[iLoop0] != '0' )
		{
			firstflag = 1;
		}
		else
		{
			firstflag = 0;
		}
		if(iLoop0 == 3 || iLoop0 == 6 || iLoop0 == 9)
		{
			ptr_ip->str[iLoop1++] = 0x2e;
		}
		ptr_ip->str[iLoop1++] = pAddress[iLoop0];	
	}
	ptr_ip->str[iLoop1] = '\0';
}
/***************************************************************************
 *                                                                         *
 ***************************************************************************/

 /***************************************************************************
 *                                                                         *
 ***************************************************************************/
static int netsplit( char *pAddress, void *ip )
{
	unsigned int ret;
	NET_IPV4 *ipaddr = (NET_IPV4 *)ip;

	if ((ret = atoi(pAddress + 9)) > 255)
		return FALSE;
	ipaddr->str[3] = ret;

	*( pAddress + 9 ) = '\0';
	if ((ret = atoi(pAddress + 6)) > 255)
		return FALSE;
	ipaddr->str[2] = ret;

	*( pAddress + 6 ) = '\0';
	if ((ret = atoi(pAddress + 3)) > 255)
		return FALSE;
	ipaddr->str[1] = ret;

	*( pAddress + 3 ) = '\0';
	if ((ret = atoi(pAddress + 0)) > 255)
		return FALSE;
	ipaddr->str[0] = ret;

	return TRUE;
}

/***************************************************************************
 *                                                                         *
 ***************************************************************************/
int ipv4_str_to_num(char *data, struct in_addr *ipaddr)
{
	if ( strchr(data, '.') == NULL )
		return netsplit(data, ipaddr);
	return inet_aton(data, ipaddr);
}

/***************************************************************************
 *                                                                         *
 ***************************************************************************/
#define NTPCLIENT_EXE	"/opt/ipnc/ntpclient"
#include <pthread.h>
static int ntp_check = 0;
void *ntpThrFxn(void *arg)
{
	char buf[256];
	char *server_name = arg;

	sprintf(buf, NTPCLIENT_EXE " -s -i 5 -h %s\n", server_name);
	dbg("run_ntpclient: %s\n", buf);
	system(buf);
	system("hwclock -uw");
	ntp_check = 1;

	return (void *) 0;
}


int run_ntpclient(char *server_name)
{
	//int ret;
	char buf[256];
	pthread_t ntpThread;
	ntp_check = 0;
	int timout_cnt = 8;
	int ret = 0;
	unsigned char v2;
	//void  *pRet;

#if 1
	if (pthread_create(&ntpThread,NULL, ntpThrFxn, server_name))
	{
		return -1;
	}

	while(timout_cnt--)
	{
		//dbg("timout_cnt = %d\n",timout_cnt);
		if( ntp_check == 0 )
		{
			sleep(1);
		}
		else
		{
			break;
		}
	}
	if( ntp_check == 0 )
	{
		sprintf(buf, "killall -9 ntpclient");
		system(buf);
		ret = -7;
	}
	pthread_join(ntpThread, NULL);
	ControlSystemData(SFIELD_SCHEDULE_RESUME, (void *)&v2, sizeof(v2));
	return ret;
	/*if( pthread_join(ntpThread, &pRet) == 0 )
	  {
	  return 0;
	  }else{
	  return -1;
	  }*/

#else
	//snprintf(buf, 256, NTPCLIENT_EXE " -s -h %s", server_name);
	sprintf(buf, NTPCLIENT_EXE " -s -i 5 -h %s&\n", server_name);
	dbg("run_ntpclient: %s\n", buf);
	ret = system(buf);
	system("hwclock -uw");
	sleep(10);

	if ((ret < 0) || (ret == 127))
		return -1;
	return 0;
#endif
}
/***************************************************************************
 *                                                                         *
 ***************************************************************************/
char *req_bufptr(request * req)
{
	return (req->buffer + req->buffer_end);
}

/***************************************************************************
 *                                                                         *
 ***************************************************************************/
void get_dhcpclient(request *req, COMMAND_ARGUMENT *argm)
{
	int value = 0;

	do {
		if (ControlSystemData(SFIELD_GET_DHCPC_ENABLE, (void *)&value, sizeof(value)) < 0)
			break;;
		req->buffer_end += sprintf(req_bufptr(req), OPTION_OK "%s=%d\n", argm->name, value);
		return;
	} while (0);
	req->buffer_end += sprintf(req_bufptr(req), OPTION_NG "%s\n", argm->name);
}

void set_dhcpclient(request *req, COMMAND_ARGUMENT *argm)
{
	int sys_dhcp;
	int value = atoi(argm->value);
#ifdef ISLAB_DEBUG
	printf("\r\nFILE: %s LINE: %d....\n", __FILE__, __LINE__);
#endif
	do {
		if ( (value != TRUE) && (value != FALSE) )
			break;
		//		if(ControlSystemData(SFIELD_GET_DHCP_CONFIG, (void *)&sys_dhcp, sizeof(sys_dhcp)) < 0)
		//			break;
		if(sys_dhcp == 0)
			break;
		if (ControlSystemData(SFIELD_GET_DHCPC_ENABLE, (void *)&sys_dhcp, sizeof(sys_dhcp)) < 0)
			break;

		if (sys_dhcp != value) {
			if (value == TRUE)
				net_enable_dhcpcd();
			else
				net_disable_dhcpcd();

			ControlSystemData(SFIELD_SET_DHCPC_ENABLE, (void *)&value, sizeof(value));
		}
		req->buffer_end += sprintf(req_bufptr(req), OPTION_OK "%s\n", argm->name);
		return;
	} while (0);
#ifdef ISLAB_DEBUG
	printf("\r\nFILE:%s LINE:%d...\n",__FILE__,__LINE__);
#endif
	req->buffer_end += sprintf(req_bufptr(req), OPTION_NG "%s\n", argm->name);

}

/***************************************************************************
 *                                                                         *
 ***************************************************************************/
void get_netip(request *req, COMMAND_ARGUMENT *argm)
{
	struct in_addr ip;
	printf("\r\nFILE: %s LINE: %d....\n", __FILE__, __LINE__);
	do {
		//		if (ControlSystemData(SFIELD_GET_IP, (void *)&ip.s_addr, sizeof(ip.s_addr)) < 0)
		if ( (ip.s_addr = net_get_ifaddr(nicname)) == -1)
			break;
		req->buffer_end += sprintf(req_bufptr(req), OPTION_OK "%s=%s\n", argm->name, inet_ntoa(ip));
		printf("\r\nFILE: %s LINE: %d....\n", __FILE__, __LINE__);
		printf("####get_netip-dowhile(0):%s\n",req->buffer_end);
		return;
	} while (0);
	req->buffer_end += sprintf(req_bufptr(req), OPTION_NG "%s\n", argm->name);
	printf("\r\nFILE: %s LINE: %d....\n", __FILE__, __LINE__);
	printf("####get_netip-dowhile(1):%s\n",req->buffer_end);
}

void set_netip(request *req, COMMAND_ARGUMENT *argm)
{
	UNIX_CMD_STRUCT cmd;
	boatoflash_para_save para_save;
	NET_IP_STR ipaddr;
	
	para_save.testflag = 0xFF;
	cmd.cmd = PARACMD_BOA_BOATOFLASH_PARA_SAVE;
	cmd.cmdlen = sizeof(para_save);
	memcpy(cmd.cmdinfo, &para_save, sizeof(para_save));
	cmd.ret_ack = 0;
    if ( strchr(argm -> value, '.') == NULL )
    {
		NET_IPV4 localnet;
		memset(&localnet, 0x00, sizeof(localnet));
		if (netsplit(argm->value, &localnet) == 0)
		{
              SYS_INFO("\n\r#########ERROR##########\r\n");
		}
		sprintf(net_setting.ipaddr,"%d.%d.%d.%d",localnet.str[0], localnet.str[1],localnet.str[2],localnet.str[3]);

   // 	if(ipstrsplit(argm->value,&ipaddr) == 0)
     //         SYS_INFO("\n\r#########ERROR##########\r\n");
	//	sprintf(net_setting.ipaddr,"%s",ipaddr.str);
		printf("\n\r!!!!!!!!!!!!!!!!!!!GBSHM_PARA_GET argm->value = %s,net.ip = %s !!!!!!!!!!!!!!!!!!!!!!!!\r\n",argm->value,net_setting.ipaddr);
	}
	else
		sprintf(net_setting.ipaddr,argm -> value,strlen(argm -> value));
//	UNIX_SEND(global_boa_private_handle.unix_fd, &cmd);	
}

/***************************************************************************
 *                                                                         *
 ***************************************************************************/
void get_netmask(request *req, COMMAND_ARGUMENT *argm)
{
	struct in_addr ip;

	do {
		//		if (ControlSystemData(SFIELD_GET_NETMASK, (void *)&ip.s_addr, sizeof(ip.s_addr)) < 0)
		if ( (ip.s_addr = net_get_netmask(nicname)) == -1)
			break;
		req->buffer_end += sprintf(req_bufptr(req), OPTION_OK "%s=%s\n", argm->name, inet_ntoa(ip));
		return;
	} while (0);
	req->buffer_end += sprintf(req_bufptr(req), OPTION_NG "%s\n", argm->name);
}

void set_netmask(request *req, COMMAND_ARGUMENT *argm)
{
	UNIX_CMD_STRUCT cmd;
	boatoflash_para_save para_save;
	NET_IP_STR ipaddr;
	
	para_save.testflag = 0xFF;
	cmd.cmd = PARACMD_BOA_BOATOFLASH_PARA_SAVE;
	cmd.cmdlen = sizeof(para_save);
	memcpy(cmd.cmdinfo, &para_save, sizeof(para_save));
	cmd.ret_ack = 0;
    if ( strchr(argm -> value, '.') == NULL )
    {
		NET_IPV4 localnet;
		memset(&localnet, 0x00, sizeof(localnet));
		if (netsplit(argm->value, &localnet) == 0)
		{
              SYS_INFO("\n\r#########ERROR##########\r\n");
		}
		sprintf(net_setting.ipmask,"%d.%d.%d.%d",localnet.str[0], localnet.str[1],localnet.str[2],localnet.str[3]);

    //	if(ipstrsplit(argm->value,&ipaddr) == 0)
    //         SYS_INFO("\n\r#########ERROR##########\r\n");
	//	sprintf(net_setting.ipmask,"%s",ipaddr.str);
		printf("\n\r!!!!!!!!!!!!!!!!!!!GBSHM_PARA_GET argm->value = %s,netmask.ip = %s !!!!!!!!!!!!!!!!!!!!!!!!\r\n",argm->value,net_setting.ipmask);
	}
	else
		sprintf(net_setting.ipmask,argm -> value,strlen(argm -> value));
//	UNIX_SEND(global_boa_private_handle.unix_fd, &cmd);	
}

void set_aviduration(request *req, COMMAND_ARGUMENT *argm)
{
	__u8 value = atoi(argm->value);
	SysInfo* pSysInfo = GetSysInfo();
	if (pSysInfo->sdcard_config.sdinsert==1)
		return;
	do {

		ControlSystemData(SFIELD_SET_AVIDURATION, (void *)&value, sizeof(value));
		req->buffer_end += sprintf(req_bufptr(req), OPTION_OK "%s\n", argm->name);
		return;
	} while (0);

	req->buffer_end += sprintf(req_bufptr(req), OPTION_NG "%s\n", argm->name);
}
void set_aviformat(request *req, COMMAND_ARGUMENT *argm)
{
	__u8 value = atoi(argm->value);
	do {
		ControlSystemData(SFIELD_SET_AVIFORMAT, (void *)&value, sizeof(value));
		req->buffer_end += sprintf(req_bufptr(req), OPTION_OK "%s\n", argm->name);
		return;
	} while (0);

	req->buffer_end += sprintf(req_bufptr(req), OPTION_NG "%s\n", argm->name);
}
void set_ftpfileformat(request *req, COMMAND_ARGUMENT *argm)
{
	__u8 value = atoi(argm->value);

	do {

		ControlSystemData(SFIELD_SET_FTPFILEFORMAT, (void *)&value, sizeof(value));
		req->buffer_end += sprintf(req_bufptr(req), OPTION_OK "%s\n", argm->name);
		return;
	} while (0);

	req->buffer_end += sprintf(req_bufptr(req), OPTION_NG "%s\n", argm->name);
}
void set_sdfileformat(request *req, COMMAND_ARGUMENT *argm)
{
	__u8 value = atoi(argm->value);

	do {

		ControlSystemData(SFIELD_SET_SDFILEFORMAT, (void *)&value, sizeof(value));
		req->buffer_end += sprintf(req_bufptr(req), OPTION_OK "%s\n", argm->name);
		return;
	} while (0);

	req->buffer_end += sprintf(req_bufptr(req), OPTION_NG "%s\n", argm->name);
}
void set_attfileformat(request *req, COMMAND_ARGUMENT *argm)
{
	__u8 value = atoi(argm->value);

	do {

		ControlSystemData(SFIELD_SET_ATTFILEFORMAT, (void *)&value, sizeof(value));
		req->buffer_end += sprintf(req_bufptr(req), OPTION_OK "%s\n", argm->name);
		return;
	} while (0);

	req->buffer_end += sprintf(req_bufptr(req), OPTION_NG "%s\n", argm->name);
}
void set_audioenable(request *req, COMMAND_ARGUMENT *argm)
{
	__u8 value = atoi(argm->value);

	do {

		ControlSystemData(SFIELD_SET_AUDIOENABLE, (void *)&value, sizeof(value));
		req->buffer_end += sprintf(req_bufptr(req), OPTION_OK "%s\n", argm->name);
		return;
	} while (0);

	req->buffer_end += sprintf(req_bufptr(req), OPTION_NG "%s\n", argm->name);
}

int CheckAudioStatus(void)
{
	SysInfo* pSysInfo = GetSysInfo();
	return pSysInfo->lan_config.net.audioenable;
}

void get_audioenable(request *req, COMMAND_ARGUMENT *argm)
{
	do {
		SysInfo* pSysInfo = GetSysInfo();
		if(pSysInfo == NULL)
			break;
		req->buffer_end += sprintf(req_bufptr(req), OPTION_OK "%s=%d\n", argm->name, pSysInfo->lan_config.net.audioenable);
		return;
	} while (0);
	req->buffer_end += sprintf(req_bufptr(req), OPTION_NG "%s\n", argm->name);
}

void set_asmtpattach(request *req, COMMAND_ARGUMENT *argm)
{
	__u8 value = atoi(argm->value);

	do {

		ControlSystemData(SFIELD_SET_SMTP_ATTACHMENTS, (void *)&value, sizeof(value));
		req->buffer_end += sprintf(req_bufptr(req), OPTION_OK "%s\n", argm->name);
		return;
	} while (0);

	req->buffer_end += sprintf(req_bufptr(req), OPTION_NG "%s\n", argm->name);
}

void set_rftpenable(request *req, COMMAND_ARGUMENT *argm)
{
	__u8 value = atoi(argm->value);

	do {

		ControlSystemData(SFIELD_SET_FTP_RFTPENABLE, (void *)&value, sizeof(value));
		req->buffer_end += sprintf(req_bufptr(req), OPTION_OK "%s\n", argm->name);
		return;
	} while (0);

	req->buffer_end += sprintf(req_bufptr(req), OPTION_NG "%s\n", argm->name);
}

void set_sdrenable(request *req, COMMAND_ARGUMENT *argm)
{
	__u8 value = atoi(argm->value);

	do {

		ControlSystemData(SFIELD_SET_SD_SDRENABLE, (void *)&value, sizeof(value));
		req->buffer_end += sprintf(req_bufptr(req), OPTION_OK "%s\n", argm->name);
		return;
	} while (0);

	req->buffer_end += sprintf(req_bufptr(req), OPTION_NG "%s\n", argm->name);
}

/***************************************************************************
 *                                                                         *
 ***************************************************************************/
void get_dnsip(request *req, COMMAND_ARGUMENT *argm)
{
	struct in_addr ip;

	/*	do {
		if (ControlSystemData(SFIELD_GET_DNS, (void *)&ip.s_addr, sizeof(ip.s_addr)) < 0)
		break;
		req->buffer_end += sprintf(req_bufptr(req), OPTION_OK "%s=%s\n", argm->name, inet_ntoa(ip));
		return;
		} while (0);
		req->buffer_end += sprintf(req_bufptr(req), OPTION_NG "%s\n", argm->name);
	 */
	ip.s_addr = net_get_dns();
	req->buffer_end += sprintf(req_bufptr(req), OPTION_OK "%s=%s\n", argm->name, inet_ntoa(ip));
}

void set_dnsip(request *req, COMMAND_ARGUMENT *argm)
{
	struct in_addr ipaddr, sys_ip;
#ifdef ISLAB_DEBUG
	printf("\r\nFILE: %s LINE: %d....\n", __FILE__, __LINE__);
#endif
	do {
		if (ipv4_str_to_num(argm->value, &ipaddr) == 0)
			break;

		//		if (ControlSystemData(SFIELD_GET_DNS, (void *)&sys_ip.s_addr, sizeof(sys_ip.s_addr)) < 0)
		//			break;
		sys_ip.s_addr = net_get_dns();

		if (sys_ip.s_addr != ipaddr.s_addr) {
			if (net_set_dns(inet_ntoa(ipaddr)) < 0)
				break;

			ControlSystemData(SFIELD_SET_DNS, (void *)&ipaddr.s_addr, sizeof(ipaddr.s_addr));
			//			argm->flags |= HTTP_OPTION_CHANGED;

		}
#ifdef ISLAB_DEBUG
		printf("\r\nFILE: %s LINE: %d....\n", __FILE__, __LINE__);
#endif
		req->buffer_end += sprintf(req_bufptr(req), OPTION_OK "%s\n", argm->name);
		return;
	} while (0);
	req->buffer_end += sprintf(req_bufptr(req), OPTION_NG "%s\n", argm->name);

}

/***************************************************************************
 *                                                                         *
 ***************************************************************************/
void get_gateway(request *req, COMMAND_ARGUMENT *argm)
{
	struct in_addr ip;

	/*	do {
		if (ControlSystemData(SFIELD_GET_GATEWAY, (void *)&ip.s_addr, sizeof(ip.s_addr)) < 0)
		break;
		req->buffer_end += sprintf(req_bufptr(req), OPTION_OK "%s=%s\n", argm->name, inet_ntoa(ip));
		return;
		} while (0);
		req->buffer_end += sprintf(req_bufptr(req), OPTION_NG "%s\n", argm->name);
	 */
	ip.s_addr = net_get_gateway();
	req->buffer_end += sprintf(req_bufptr(req), OPTION_OK "%s=%s\n", argm->name, inet_ntoa(ip));
}

void set_gateway(request *req, COMMAND_ARGUMENT *argm)
{
	UNIX_CMD_STRUCT cmd;
	boatoflash_para_save para_save;
	NET_IP_STR ipaddr;
	
	para_save.testflag = 0xFF;
	cmd.cmd = PARACMD_BOA_BOATOFLASH_PARA_SAVE;
	cmd.cmdlen = sizeof(para_save);
	memcpy(cmd.cmdinfo, &para_save, sizeof(para_save));
	cmd.ret_ack = 0;
    if ( strchr(argm -> value, '.') == NULL )
    {
		NET_IPV4 localnet;
		memset(&localnet, 0x00, sizeof(localnet));
		if (netsplit(argm->value, &localnet) == 0)
		{
              SYS_INFO("\n\r#########ERROR##########\r\n");
		}
		sprintf(net_setting.ipgateway,"%d.%d.%d.%d",localnet.str[0], localnet.str[1],localnet.str[2],localnet.str[3]);

    //	if(ipstrsplit(argm->value,&ipaddr) == 0)
    //          SYS_INFO("\n\r#########ERROR##########\r\n");
	//	sprintf(net_setting.ipgateway,"%s",ipaddr.str);
		printf("\n\r!!!!!!!!!!!!!!!!!!!GBSHM_PARA_GET argm->value = %s,gateway.ip = %s !!!!!!!!!!!!!!!!!!!!!!!!\r\n",argm->value,net_setting.ipgateway);
	}
	else
		sprintf(net_setting.ipgateway,argm -> value,strlen(argm -> value));
//	UNIX_SEND(global_boa_private_handle.unix_fd, &cmd);	

}

/***************************************************************************
 *                                                                         *
 ***************************************************************************/
void get_httpport(request *req, COMMAND_ARGUMENT *argm)
{	

#ifdef ISLAB_DEBUG
	printf("entering get_httpport....\n");
#endif	
	unsigned short value;

	do {
		if (ControlSystemData(SFIELD_GET_HTTPPORT, (void *)&value, sizeof(value)) < 0)
			break;
		req->buffer_end += sprintf(req_bufptr(req), OPTION_OK "%s=%d\n", argm->name, value);
		return;
	} while (0);
	req->buffer_end += sprintf(req_bufptr(req), OPTION_NG "%s\n", argm->name);
#ifdef ISLAB_DEBUG
	printf("get_httpport ok!\n");
#endif

}

void set_httpport(request *req, COMMAND_ARGUMENT *argm)
{
    UNIX_CMD_STRUCT cmd;
	boatoflash_para_save para_save;
	
	para_save.testflag = 0xFF;
	cmd.cmd = PARACMD_BOA_BOATOFLASH_PARA_SAVE;
	cmd.cmdlen = sizeof(para_save);
	memcpy(cmd.cmdinfo, &para_save, sizeof(para_save));
	cmd.ret_ack = 0;

    SYS_INFO("\n\r###########argm->value = %s#############\r\n",argm->value);

    net_setting.httpport = atoi(argm -> value);
	SYS_INFO("\n\r!!!!!!!!!!!!!!!!!!!GBSHM_PARA_GET argm->value = %s,httpport = %d !!!!!!!!!!!!!!!!!!!!!!!!\r\n",argm->value,net_setting.httpport);
//	UNIX_SEND(global_boa_private_handle.unix_fd, &cmd);		
}

/***************************************************************************
 *                                                                         *
 ***************************************************************************/
void get_sntpip(request *req, COMMAND_ARGUMENT *argm)
{
	char value[256];
	int len;

	do {
		if ((len = ControlSystemData(SFIELD_GET_SNTP_FQDN, (void *)value, sizeof(value))) < 0)
			break;
		value[len] = '\0';
		req->buffer_end += sprintf(req_bufptr(req), OPTION_OK "%s=%s\n", argm->name, value);
		return;
	} while (0);
	req->buffer_end += sprintf(req_bufptr(req), OPTION_NG "%s\n", argm->name);
}

void set_sntpfqdn(request *req, COMMAND_ARGUMENT *argm)
{
	char sys_value[256];

	do {
		if (ControlSystemData(SFIELD_GET_SNTP_FQDN, (void *)sys_value, sizeof(sys_value)) < 0)
			break;

		if (strcmp(sys_value, argm->value)) {
			ControlSystemData(SFIELD_SET_SNTP_SERVER, (void *)argm->value, strlen(argm->value));
		}
		req->buffer_end += sprintf(req_bufptr(req), OPTION_OK "%s\n", argm->name);
		return;
	} while (0);
	req->buffer_end += sprintf(req_bufptr(req), OPTION_NG "%s\n", argm->name);

}

/***************************************************************************
 *                                                                         *
 ***************************************************************************/
void get_date(request *req, COMMAND_ARGUMENT *argm)
{
	static char wday_name[7][3] = {
		"Sun", "Mon", "Tue", "Wed", "Thu", "Fri", "Sat"};
	time_t tnow;
	struct tm *tmnow;
	int string_len;

	time(&tnow);
	tmnow = localtime(&tnow);

	string_len = sprintf(req_bufptr(req), OPTION_OK "%s=%04d-%02d-%02d %.3s\n", argm->name,
			tmnow->tm_year+1900, tmnow->tm_mon+1, tmnow->tm_mday, wday_name[tmnow->tm_wday]);
	req->buffer_end += string_len;
}

void get_time(request *req, COMMAND_ARGUMENT *argm)
{
	time_t tnow;
	struct tm *tmnow;
	int string_len;

	time(&tnow);
	tmnow = localtime(&tnow);

	string_len = sprintf(req_bufptr(req), OPTION_OK "%s=%02d:%02d:%02d\n", argm->name,
			tmnow->tm_hour, tmnow->tm_min, tmnow->tm_sec);
	req->buffer_end += string_len;
}

/***************************************************************************
 *                                                                         *
 ***************************************************************************/
int sys_set_date(int year, int month, int day)
{
	struct tm *tm;
	time_t now;
	unsigned char v2;

	now = time(NULL);
	tm = localtime(&now);

	year = (year>1900) ? year-1900 : 0;
	tm->tm_year = year;
	month = (month>0) ? month-1 : 0;
	tm->tm_mon = month;
	tm->tm_mday = day;

	if ((now = mktime(tm)) < 0)
		return -1;
	ControlSystemData(SFIELD_SCHEDULE_STOP, (void *)&v2, sizeof(v2));
	stime(&now);
	system("hwclock -uw");
	ControlSystemData(SFIELD_SCHEDULE_RESUME, (void *)&v2, sizeof(v2));
	return 0;
}

int sys_set_time(int hour, int min, int sec)
{
	struct tm *tm;
	time_t now;
	unsigned char v2;

	now = time(NULL);
	tm = localtime(&now);

	tm->tm_hour = hour;
	tm->tm_min = min;
	tm->tm_sec = sec + 3;

	if ((now = mktime(tm)) < 0)
		return -1;
	ControlSystemData(SFIELD_SCHEDULE_STOP, (void *)&v2, sizeof(v2));
	stime(&now);
	system("hwclock -uw");
	ControlSystemData(SFIELD_SCHEDULE_RESUME, (void *)&v2, sizeof(v2));
	return 0;
}

void set_system_date(request *req, COMMAND_ARGUMENT *argm)
{
	int year, month, day;

	do {
		if (sscanf(argm->value, "%d/%d/%d", &year, &month, &day) != 3)
			break;

		if (sys_set_date(year, month, day) < 0)
			break;
		req->buffer_end += sprintf(req_bufptr(req), OPTION_OK "%s\n", argm->name);
		return;
	} while (0);
	req->buffer_end += sprintf(req_bufptr(req), OPTION_NG "%s\n", argm->name);
}

void set_system_time(request *req, COMMAND_ARGUMENT *argm)
{
	int hour, min, sec;

	do {
		if (sscanf(argm->value, "%d:%d:%d", &hour, &min, &sec) != 3)
			break;

		if (sys_set_time(hour, min, sec) < 0)
			break;
		req->buffer_end += sprintf(req_bufptr(req), OPTION_OK "%s\n", argm->name);
		return;
	} while (0);
	req->buffer_end += sprintf(req_bufptr(req), OPTION_NG "%s\n", argm->name);
}

void set_time_frequency(request *req, COMMAND_ARGUMENT *argm)
{
	char value[256];
	int ret;
	unsigned char v2;

	do {
		if (ControlSystemData(SFIELD_GET_SNTP_FQDN, (void *)value, sizeof(value)) < 0)
			break;
		ControlSystemData(SFIELD_SCHEDULE_STOP, (void *)&v2, sizeof(v2));
		ret = run_ntpclient(value);
		if (ret < 0)
		{
			if (ret == -7)
			{
				req->buffer_end += sprintf(req_bufptr(req), "TIMEOUT %s\n", argm->name);
				return;
			}
			break;
		}
		req->buffer_end += sprintf(req_bufptr(req), OPTION_OK "%s\n", argm->name);
		return;
	} while (0);
	req->buffer_end += sprintf(req_bufptr(req), OPTION_NG "%s\n", argm->name);
}

void set_day_night(request *req, COMMAND_ARGUMENT *argm)
{
	__u8 value = atoi(argm->value);

	do {
		if ( value > 1 )
			break;

		ControlSystemData(SFIELD_SET_DAY_NIGHT, (void *)&value, sizeof(value));
		req->buffer_end += sprintf(req_bufptr(req), OPTION_OK "%s\n", argm->name);
		return;
	} while (0);
	req->buffer_end += sprintf(req_bufptr(req), OPTION_NG "%s\n", argm->name);

}

void set_white_balance(request *req, COMMAND_ARGUMENT *argm)
{
	__u8 value = atoi(argm->value);

	do {
		if ( value > 2 )
			break;

		ControlSystemData(SFIELD_SET_WHITE_BALANCE, (void *)&value, sizeof(value));
		req->buffer_end += sprintf(req_bufptr(req), OPTION_OK "%s\n", argm->name);
		return;
	} while (0);
	req->buffer_end += sprintf(req_bufptr(req), OPTION_NG "%s\n", argm->name);

}

void set_backlight(request *req, COMMAND_ARGUMENT *argm)
{
	__u8 value = atoi(argm->value);

	do {

		ControlSystemData(SFIELD_SET_BACKLIGHT, (void *)&value, sizeof(value));
		req->buffer_end += sprintf(req_bufptr(req), OPTION_OK "%s\n", argm->name);
		return;
	} while (0);
	req->buffer_end += sprintf(req_bufptr(req), OPTION_NG "%s\n", argm->name);

}

void set_brightness(request *req, COMMAND_ARGUMENT *argm)
{

	UNIX_CMD_STRUCT cmd;
	brightnesstype setbrightness;
	ad_setting.ad_bright = atoi(argm -> value);
	printf("\n\r!!!!!!!!!!!!!!!!!!!GBSHM_PARA_GET argm->value = %s,video_bright = %d !!!!!!!!!!!!!!!!!!!!!!!!\r\n",argm->value,ad_setting.ad_bright);
	setbrightness = ad_setting.ad_bright;
	cmd.cmd = PARACMD_BOA_BOATOCORE_PARA_SETBRIGHTNESS;
	cmd.cmdlen = sizeof(setbrightness);
	memcpy(cmd.cmdinfo, &setbrightness, sizeof(setbrightness));
	cmd.ret_ack = 0;
	Unix_send(global_boa_private_handle.unix_fd, &cmd);	

}

void set_contrast(request *req, COMMAND_ARGUMENT *argm)
{

	UNIX_CMD_STRUCT cmd;
	contrasttype setcontrast;
	ad_setting.ad_contrast = atoi(argm -> value);
	printf("\n\r!!!!!!!!!!!!!!!!!!!GBSHM_PARA_GET argm->value = %s,video_contrast = %d !!!!!!!!!!!!!!!!!!!!!!!!\r\n",argm->value,ad_setting.ad_contrast);
	setcontrast = ad_setting.ad_contrast;
	cmd.cmd = PARACMD_BOA_BOATOCORE_PARA_SETCONTRAST;
	cmd.cmdlen = sizeof(setcontrast);
	memcpy(cmd.cmdinfo, &setcontrast, sizeof(setcontrast));
	cmd.ret_ack = 0;
	Unix_send(global_boa_private_handle.unix_fd, &cmd);
}

void set_saturation(request *req, COMMAND_ARGUMENT *argm)
{

	UNIX_CMD_STRUCT cmd;
	saturationtype setsaturation;
	ad_setting.ad_saturation = atoi(argm -> value);
	printf("\n\r!!!!!!!!!!!!!!!!!!!GBSHM_PARA_GET argm->value = %s,video_saturation = %d !!!!!!!!!!!!!!!!!!!!!!!!\r\n",argm->value,ad_setting.ad_saturation);
	setsaturation = ad_setting.ad_saturation;
	cmd.cmd = PARACMD_BOA_BOATOCORE_PARA_SETSATURATION;
	cmd.cmdlen = sizeof(setsaturation);
	memcpy(cmd.cmdinfo, &setsaturation, sizeof(setsaturation));
	cmd.ret_ack = 0;
	Unix_send(global_boa_private_handle.unix_fd, &cmd);
}

void set_sharpness(request *req, COMMAND_ARGUMENT *argm)
{

	UNIX_CMD_STRUCT cmd,cmd1;
    huetype sethue;
	boatoflash_para_save para_save;
	para_save.testflag = 0xFF;
	cmd.cmd = PARACMD_BOA_BOATOFLASH_PARA_SAVE;
	cmd.cmdlen = sizeof(para_save);
	memcpy(cmd.cmdinfo, &para_save, sizeof(para_save));
	cmd.ret_ack = 0;
	ad_setting.ad_hue = atoi(argm -> value);
	printf("\n\r!!!!!!!!!!!!!!!!!!!GBSHM_PARA_GET argm->value = %s,ad_hue = %d !!!!!!!!!!!!!!!!!!!!!!!!\r\n",argm->value,ad_setting.ad_hue);
	sethue = ad_setting.ad_hue;
	cmd1.cmd = PARACMD_BOA_BOATOCORE_PARA_SETHUE;
	cmd1.cmdlen = sizeof(sethue);
	memcpy(cmd1.cmdinfo, &sethue, sizeof(sethue));
	cmd1.ret_ack = 0;
	Gbshm_paramSet(global_boa_private_handle.gbshm_handle, PARA_CMD_SYSTEM_AD_SETTING_PARAM, &ad_setting, sizeof(ad_setting));
	Unix_send(global_boa_private_handle.unix_fd, &cmd1);
	Unix_send(global_boa_private_handle.unix_fd, &cmd);	
	
}

void set_jpgquality(request *req, COMMAND_ARGUMENT *argm) /*here*/
{
	__u8 value = atoi(argm->value);
#ifdef ISLAB_DEBUG
	printf("\r\n FILE:%s  LINE:%d\n",__FILE__,__LINE__);
#endif
	do {

		//	ControlSystemData(SFIELD_SET_JQUALITY, (void *)&value, sizeof(value));
		req->buffer_end += sprintf(req_bufptr(req), OPTION_OK "%s\n", argm->name);
		return;
	} while (0);
	req->buffer_end += sprintf(req_bufptr(req), OPTION_NG "%s\n", argm->name);

}

void set_rotctrl(request *req, COMMAND_ARGUMENT *argm)
{
	__u8 value = atoi(argm->value);
#ifdef ISLAB_DEBUG
	printf("???????????????????????????set_rotctrl???????????????????????????????????\n");
#endif
	do {
		ControlSystemData(SFIELD_SET_ROTCTRL, (void *)&value, sizeof(value));
		req->buffer_end += sprintf(req_bufptr(req), OPTION_OK "%s\n", argm->name);
		return;
	} while (0);

	req->buffer_end += sprintf(req_bufptr(req), OPTION_NG "%s\n", argm->name);
}

void set_mirctrl(request *req, COMMAND_ARGUMENT *argm)
{
	__u8 value = atoi(argm->value);
#ifdef ISLAB_DEBUG
	printf("??????????????????????????set_mirctrl????????????????????????????????\n");
#endif
	do {
		ControlSystemData(SFIELD_SET_MIRROR, (void *)&value, sizeof(value));
		req->buffer_end += sprintf(req_bufptr(req), OPTION_OK "%s\n", argm->name);
		return;
	} while (0);

	req->buffer_end += sprintf(req_bufptr(req), OPTION_NG "%s\n", argm->name);
}

void set_fdetect(request *req, COMMAND_ARGUMENT *argm)
{
	__u8 value = atoi(argm->value);

	do {
		ControlSystemData(SFIELD_SET_FDETECT, (void *)&value, sizeof(value));
		req->buffer_end += sprintf(req_bufptr(req), OPTION_OK "%s\n", argm->name);
		return;
	} while (0);

	req->buffer_end += sprintf(req_bufptr(req), OPTION_NG "%s\n", argm->name);
}
/* 20090623 new feature for Advance mode */
void set_advmodeX(request *req, COMMAND_ARGUMENT *argm)
{
	static unsigned char done = 0, temp = 0;
	__u8 value = atoi(argm->value);

	if (done < 2)
	{
		if (strcmp(argm->name, "preprocess") == 0)
			temp |= (value & 0x03);
		else
			temp |= ((value & 0x03) << 2);
		done++;
	}
	if (done == 2)
	{
		ControlSystemData(SFIELD_SET_ADVMODE, (void *)&temp, sizeof(temp));
		done = temp = 0;
	}
	req->buffer_end += sprintf(req_bufptr(req), OPTION_OK "%s\n", argm->name);
	return;
}
void set_advmode(request *req, COMMAND_ARGUMENT *argm)
{
	__u8 value = atoi(argm->value);
#ifdef ISLAB_DEBUG
	printf("?????????????????????set_advmode?????????????????????????\n");
#endif
	do {
		ControlSystemData(SFIELD_SET_ADVMODE, (void *)&value, sizeof(value));
		req->buffer_end += sprintf(req_bufptr(req), OPTION_OK "%s\n", argm->name);
		return;
	} while (0);

	req->buffer_end += sprintf(req_bufptr(req), OPTION_NG "%s\n", argm->name);
}

void set_m41sfeature(request *req, COMMAND_ARGUMENT *argm)
{
	__u8 value = atoi(argm->value);

	do {
		ControlSystemData(SFIELD_SET_M41SFEATURE, (void *)&value, sizeof(value));
		req->buffer_end += sprintf(req_bufptr(req), OPTION_OK "%s\n", argm->name);
		return;
	} while (0);

	req->buffer_end += sprintf(req_bufptr(req), OPTION_NG "%s\n", argm->name);
}

void set_m42sfeature(request *req, COMMAND_ARGUMENT *argm)
{
	__u8 value = atoi(argm->value);

	do {
		ControlSystemData(SFIELD_SET_M42SFEATURE, (void *)&value, sizeof(value));
		req->buffer_end += sprintf(req_bufptr(req), OPTION_OK "%s\n", argm->name);
		return;
	} while (0);

	req->buffer_end += sprintf(req_bufptr(req), OPTION_NG "%s\n", argm->name);
}

void set_jpgsfeature(request *req, COMMAND_ARGUMENT *argm)
{
	__u8 value = atoi(argm->value);

	do {
		ControlSystemData(SFIELD_SET_JPGSFEATURE, (void *)&value, sizeof(value));
		req->buffer_end += sprintf(req_bufptr(req), OPTION_OK "%s\n", argm->name);
		return;
	} while (0);
	req->buffer_end += sprintf(req_bufptr(req), OPTION_NG "%s\n", argm->name);

}

void videomode(request *req, COMMAND_ARGUMENT *argm)
{
	if (argm->value[0]) {
		__u8 value = atoi(argm->value);

		do {
			if (value >= 5)
				break;
			ControlSystemData(SFIELD_SET_VIDEO_MODE, (void *)&value, sizeof(value));
			req->buffer_end += sprintf(req_bufptr(req), OPTION_OK "%s\n", argm->name);
			return;
		} while (0);
		req->buffer_end += sprintf(req_bufptr(req), OPTION_NG "%s\n", argm->name);
	}
	else {
		SysInfo* pSysInfo = GetSysInfo();
		do {
			if(pSysInfo == NULL)
				break;;
			req->buffer_end += sprintf(req_bufptr(req), OPTION_OK "%s=%d\n", argm->name, pSysInfo->lan_config.nVideomode);
			return;
		} while (0);
		req->buffer_end += sprintf(req_bufptr(req), OPTION_NG "%s\n", argm->name);
	}
}

void get_videomode(request *req, COMMAND_ARGUMENT *argm)
{
	SysInfo* pSysInfo = GetSysInfo();
	do {
		if(pSysInfo == NULL)
			break;;
		req->buffer_end += sprintf(req_bufptr(req), OPTION_OK "%s=%d\n", argm->name, pSysInfo->lan_config.nVideomode);
		return;
	} while (0);
	req->buffer_end += sprintf(req_bufptr(req), OPTION_NG "%s\n", argm->name);
}

void set_democfg(request *req, COMMAND_ARGUMENT *argm)
{
	__u8 value = atoi(argm->value);
#ifdef ISLAB_DEBUG
	printf("?????????????????????set_democfg?????????????????????????????????\n");
#endif
	do {
		ControlSystemData(SFIELD_SET_DEMOCFG, (void *)&value, sizeof(value));
		req->buffer_end += sprintf(req_bufptr(req), OPTION_OK "%s\n", argm->name);
		return;
	} while (0);

	req->buffer_end += sprintf(req_bufptr(req), OPTION_NG "%s\n", argm->name);
}

void set_osdwin(request *req, COMMAND_ARGUMENT *argm)
{
	__u8 value = atoi(argm->value);

	do {
		ControlSystemData(SFIELD_SET_OSDWIN, (void *)&value, sizeof(value));
		req->buffer_end += sprintf(req_bufptr(req), OPTION_OK "%s\n", argm->name);
		return;
	} while (0);

	req->buffer_end += sprintf(req_bufptr(req), OPTION_NG "%s\n", argm->name);
}

void set_histogram(request *req, COMMAND_ARGUMENT *argm)
{
	__u8 value = atoi(argm->value);

	do {
		ControlSystemData(SFIELD_SET_HISTOGRAM, (void *)&value, sizeof(value));
		req->buffer_end += sprintf(req_bufptr(req), OPTION_OK "%s\n", argm->name);
		return;
	} while (0);

	req->buffer_end += sprintf(req_bufptr(req), OPTION_NG "%s\n", argm->name);
}

void set_gbce(request *req, COMMAND_ARGUMENT *argm)
{
	__u8 value = atoi(argm->value);

	do {
		ControlSystemData(SFIELD_SET_GBCE, (void *)&value, sizeof(value));
		req->buffer_end += sprintf(req_bufptr(req), OPTION_OK "%s\n", argm->name);
		return;
	} while (0);

	req->buffer_end += sprintf(req_bufptr(req), OPTION_NG "%s\n", argm->name);
}


void set_osdwinnum(request *req, COMMAND_ARGUMENT *argm)
{
	__u8 value = atoi(argm->value);

	do {
		ControlSystemData(SFIELD_SET_OSDWINNUM, (void *)&value, sizeof(value));
		req->buffer_end += sprintf(req_bufptr(req), OPTION_OK "%s\n", argm->name);
		return;
	} while (0);

	req->buffer_end += sprintf(req_bufptr(req), OPTION_NG "%s\n", argm->name);
}

void set_osdstream(request *req, COMMAND_ARGUMENT *argm)
{
	__u8 value = atoi(argm->value);

	do {
		ControlSystemData(SFIELD_SET_OSDSTREAM, (void *)&value, sizeof(value));
		req->buffer_end += sprintf(req_bufptr(req), OPTION_OK "%s\n", argm->name);
		return;
	} while (0);

	req->buffer_end += sprintf(req_bufptr(req), OPTION_NG "%s\n", argm->name);
}

void set_cameratitle(request *req, COMMAND_ARGUMENT *argm)
{
	char sys_camtitle[256] = {0};

	do {
		//	if (ControlSystemData(SFIELD_GET_TITLE, (void *)sys_camtitle, sizeof(sys_camtitle)) < 0)
		//		break;

		printf("\r\n-++++++++++++++++++++++----------------set_cameratitle--------------- start\r\n");
		if (strcmp(sys_camtitle, argm->value)) {
			ControlSystemData(SFIELD_SET_TITLE, (void *)argm->value, strlen(argm->value));
		}
		printf("\r\n-++++++++++++++++++++++----------------set_cameratitle--------------- end\r\n");
		req->buffer_end += sprintf(req_bufptr(req), OPTION_OK "%s\n", argm->name);
		return;
	} while (0);
	req->buffer_end += sprintf(req_bufptr(req), OPTION_NG "%s\n", argm->name);
}

void set_regusrname(request *req, COMMAND_ARGUMENT *argm)
{
	char sys_regusr[256] = {0};

	do {
		if (ControlSystemData(SFIELD_GET_REGUSR, (void *)sys_regusr, sizeof(sys_regusr)) < 0)
			break;
		if (strcmp(sys_regusr, argm->value)) {
			ControlSystemData(SFIELD_SET_REGUSR, (void *)argm->value, strlen(argm->value));
		}
		req->buffer_end += sprintf(req_bufptr(req), OPTION_OK "%s\n", argm->name);
		return;
	} while (0);
	req->buffer_end += sprintf(req_bufptr(req), OPTION_NG "%s\n", argm->name);
}

void set_osdtextname(request *req, COMMAND_ARGUMENT *argm)
{
	//char sys_osdtext[256] = {0};

	do {
#if 0
		if (ControlSystemData(SFIELD_GET_OSDTEXT, (void *)sys_osdtext, sizeof(sys_osdtext)) < 0)
			break;

		if (strcmp(sys_osdtext, argm->value)) {
			ControlSystemData(SFIELD_SET_OSDTEXT, (void *)argm->value, strlen(argm->value));
		}
#else
		ControlSystemData(SFIELD_SET_OSDTEXT, (void *)argm->value, strlen(argm->value));
#endif

		req->buffer_end += sprintf(req_bufptr(req), OPTION_OK "%s\n", argm->name);
		return;
	} while (0);
	req->buffer_end += sprintf(req_bufptr(req), OPTION_NG "%s\n", argm->name);
}

void set_imageformat(request *req, COMMAND_ARGUMENT *argm)
{
	__u8 value = atoi(argm->value);

	do {

		ControlSystemData(SFIELD_SET_IMAGEFORMAT, (void *)&value, sizeof(value));
		req->buffer_end += sprintf(req_bufptr(req), OPTION_OK "%s\n", argm->name);
		return;
	} while (0);
	req->buffer_end += sprintf(req_bufptr(req), OPTION_NG "%s\n", argm->name);
}

void set_resolution(request *req, COMMAND_ARGUMENT *argm)
{
	__u8 value = atoi(argm->value);

	do {

		ControlSystemData(SFIELD_SET_RESOLUTION, (void *)&value, sizeof(value));
		req->buffer_end += sprintf(req_bufptr(req), OPTION_OK "%s\n", argm->name);
		return;
	} while (0);
	req->buffer_end += sprintf(req_bufptr(req), OPTION_NG "%s\n", argm->name);
}

void set_mpeg4resolution(request *req, COMMAND_ARGUMENT *argm)
{
	__u8 value = atoi(argm->value);

	do {
		ControlSystemData(SFIELD_SET_MPEG4_RES, (void *)&value, sizeof(value));
		req->buffer_end += sprintf(req_bufptr(req), OPTION_OK "%s\n", argm->name);
		return;
	} while (0);
	req->buffer_end += sprintf(req_bufptr(req), OPTION_NG "%s\n", argm->name);
}

void set_schedule(request *req, COMMAND_ARGUMENT *argm)
{
	do {
		if(ControlSystemData(SFIELD_SET_SCHEDULE, argm->value, strlen(argm->value)) < 0)
			break;
		req->buffer_end += sprintf(req_bufptr(req), OPTION_OK "%s\n", argm->name);
		return;
	} while (0);
	req->buffer_end += sprintf(req_bufptr(req), OPTION_NG "%s\n", argm->name);
}

void del_schedule(request *req, COMMAND_ARGUMENT *argm)
{
	int bEnable = atoi(argm->value);
	do {
		if(ControlSystemData(SFIELD_DEL_SCHEDULE, &bEnable, sizeof(bEnable)) < 0)
			break;
		req->buffer_end += sprintf(req_bufptr(req), OPTION_OK "%s\n", argm->name);
		return;
	} while (0);
	req->buffer_end += sprintf(req_bufptr(req), OPTION_NG "%s\n", argm->name);
}

void set_mpeg42resolution(request *req, COMMAND_ARGUMENT *argm)
{
	__u8 value = atoi(argm->value);
	do {

		ControlSystemData(SFIELD_SET_MPEG42_RES, (void *)&value, sizeof(value));
		req->buffer_end += sprintf(req_bufptr(req), OPTION_OK "%s\n", argm->name);
		return;
	} while (0);
	req->buffer_end += sprintf(req_bufptr(req), OPTION_NG "%s\n", argm->name);
}

void set_lostalarm(request *req, COMMAND_ARGUMENT *argm)
{
	unsigned char bEnable = atoi(argm->value);
	do {
		if(ControlSystemData(SFIELD_SET_LOSTALARM, &bEnable, sizeof(bEnable)) < 0)
			break;
		req->buffer_end += sprintf(req_bufptr(req), OPTION_OK "%s\n", argm->name);
		return;
	} while (0);
	req->buffer_end += sprintf(req_bufptr(req), OPTION_NG "%s\n", argm->name);
}
void set_sdaenable(request *req, COMMAND_ARGUMENT *argm)
{
	unsigned char bEnable = atoi(argm->value);
	do {
		if(ControlSystemData(SFIELD_SET_SDAENABLE, &bEnable, sizeof(bEnable)) < 0)
			break;
		req->buffer_end += sprintf(req_bufptr(req), OPTION_OK "%s\n", argm->name);
		return;
	} while (0);
	req->buffer_end += sprintf(req_bufptr(req), OPTION_NG "%s\n", argm->name);
}

void set_aftpenable(request *req, COMMAND_ARGUMENT *argm)
{
	unsigned char bEnable = atoi(argm->value);
	do {
		if(ControlSystemData(SFIELD_SET_AFTPENABLE, &bEnable, sizeof(bEnable)) < 0)
			break;
		req->buffer_end += sprintf(req_bufptr(req), OPTION_OK "%s\n", argm->name);
		return;
	} while (0);
	req->buffer_end += sprintf(req_bufptr(req), OPTION_NG "%s\n", argm->name);
}

void set_asmtpenable(request *req, COMMAND_ARGUMENT *argm)
{
	unsigned char bEnable = atoi(argm->value);
	do {
		if(ControlSystemData(SFIELD_SET_ASMTPENABLE, &bEnable, sizeof(bEnable)) < 0)
			break;
		req->buffer_end += sprintf(req_bufptr(req), OPTION_OK "%s\n", argm->name);
		return;
	} while (0);
	req->buffer_end += sprintf(req_bufptr(req), OPTION_NG "%s\n", argm->name);
}

void set_alarmduration(request *req, COMMAND_ARGUMENT *argm)
{
	__u8 value = atoi(argm->value);
	do {
		if(ControlSystemData(SFIELD_SET_ALARMDURATION, &value, sizeof(value)) < 0)
			break;
		req->buffer_end += sprintf(req_bufptr(req), OPTION_OK "%s\n", argm->name);
		return;
	} while (0);
	req->buffer_end += sprintf(req_bufptr(req), OPTION_NG "%s\n", argm->name);
}

void set_motionenable(request *req, COMMAND_ARGUMENT *argm)
{
#if 0
	fprintf(stderr, "\n motionenable= %s\n", argm->value);
	req->buffer_end += sprintf(req_bufptr(req), OPTION_OK "%s\n", argm->name);
	return;
#else
	do {
		if(ControlSystemData(SFIELD_SET_MOTIONENABLE, (void *)argm->value, strlen(argm->value)) < 0)
			break;
		req->buffer_end += sprintf(req_bufptr(req), OPTION_OK "%s\n", argm->name);
		return;
	} while (0);
	req->buffer_end += sprintf(req_bufptr(req), OPTION_NG "%s\n", argm->name);
#endif
}

void set_motioncenable(request *req, COMMAND_ARGUMENT *argm)
{
	__u8 value = atoi(argm->value);
	do {
		if(ControlSystemData(SFIELD_SET_MOTIONCENABLE, &value, sizeof(value)) < 0)
			break;
		req->buffer_end += sprintf(req_bufptr(req), OPTION_OK "%s\n", argm->name);
		dbg("\n motioncenable= %d\n",value);
		return;
	} while (0);
	req->buffer_end += sprintf(req_bufptr(req), OPTION_NG "%s\n", argm->name);
}

void set_motionlevel(request *req, COMMAND_ARGUMENT *argm)
{
	__u8 value = atoi(argm->value);
	do {
		if(ControlSystemData(SFIELD_SET_MOTIONLEVEL, &value, sizeof(value)) < 0)
			break;
		req->buffer_end += sprintf(req_bufptr(req), OPTION_OK "%s\n", argm->name);
		return;
	} while (0);
	req->buffer_end += sprintf(req_bufptr(req), OPTION_NG "%s\n", argm->name);
}


void set_motioncvalue(request *req, COMMAND_ARGUMENT *argm)
{
	__u8 value = atoi(argm->value);
	do {
		if(ControlSystemData(SFIELD_SET_MOTIONCVALUE, &value, sizeof(value)) < 0)
			break;
		req->buffer_end += sprintf(req_bufptr(req), OPTION_OK "%s\n", argm->name);
		return;
	} while (0);
	req->buffer_end += sprintf(req_bufptr(req), OPTION_NG "%s\n", argm->name);
}

void set_motionblock(request *req, COMMAND_ARGUMENT *argm)
{
	extern int para_motion_blk(char *data, char *arg);
	static char tempBuff[MOTION_BLK_LEN];


	if(strlen(argm->value) == 3)
	{
		if(  (strlen(argm->value)+1) > MOTION_BLK_LEN )
		{
			req->buffer_end += sprintf(req_bufptr(req), OPTION_NG "%s\n", argm->name);
			return;
		}

		if(ControlSystemData(SFIELD_SET_MOTIONBLOCK, argm->value, (strlen(argm->value)+1)) < 0)
		{
			req->buffer_end += sprintf(req_bufptr(req), OPTION_NG "%s\n", argm->name);
			return;
		}
		req->buffer_end += sprintf(req_bufptr(req), OPTION_OK "%s=%s\n", argm->name, argm->value);
		//strcpy(tempBuff, argm->value);
		return;


	}else{
		para_motion_blk(tempBuff,NULL);
		dbg(OPTION_OK "%s=%s\n",argm->name, tempBuff);
		req->buffer_end += sprintf(req_bufptr(req), OPTION_OK "%s=%s\n", argm->name, tempBuff);

	}


}

void get_motionxblock(request *req, COMMAND_ARGUMENT *argm)
{
	req->buffer_end += sprintf(req_bufptr(req), OPTION_OK "%s=%d\n", argm->name, 4);
	return;
	__u8 value = atoi(argm->value);
	do {
		if(ControlSystemData(SFIELD_SET_ALARMDURATION, &value, sizeof(value)) < 0)
			break;
		req->buffer_end += sprintf(req_bufptr(req), OPTION_OK "%s\n", argm->name);
		return;
	} while (0);
	req->buffer_end += sprintf(req_bufptr(req), OPTION_NG "%s\n", argm->name);
}

void get_motionyblock(request *req, COMMAND_ARGUMENT *argm)
{
	req->buffer_end += sprintf(req_bufptr(req), OPTION_OK "%s=%d\n", argm->name, 3);
	return;
	__u8 value = atoi(argm->value);
	do {
		if(ControlSystemData(SFIELD_SET_ALARMDURATION, &value, sizeof(value)) < 0)
			break;
		req->buffer_end += sprintf(req_bufptr(req), OPTION_OK "%s\n", argm->name);
		return;
	} while (0);
	req->buffer_end += sprintf(req_bufptr(req), OPTION_NG "%s\n", argm->name);
}

void get_supportmotion(request *req, COMMAND_ARGUMENT *argm)
{
	req->buffer_end += sprintf(req_bufptr(req), OPTION_OK "%s=%d\n", argm->name, 2);
	return;
}



void paratest(request *req, COMMAND_ARGUMENT *argm)
{
	int ret;
	char buf[255];
	char *spridx = NULL;
	do {
		if ((spridx = index(argm->value, '.')) == NULL) {
			ret = TranslateWebPara(req->authority, buf, argm->value, "");
		} else {
			*spridx++ = '\0';
			ret = TranslateWebPara(req->authority, buf, argm->value, spridx);
			*--spridx = '.';
		}
		if (ret < 0)
			break;
		req->buffer_end += sprintf(req_bufptr(req), OPTION_OK "%s=%s\n", argm->value, buf);
		return;
	} while (0);
	req->buffer_end += sprintf(req_bufptr(req), OPTION_NG "%s\n", argm->value);
}
/*-----------------------------------------------------------------*/
void set_ratecontrol(request *req, COMMAND_ARGUMENT *argm)
{

	__u8 value = atoi(argm->value);
#ifdef ISLAB_DEBUG
	printf("??????????????????????????set_ratecontrol????????????????????????\n");
#endif
	do {

		int ret = ControlSystemData(SFIELD_SET_RATE_CONTROL, (void *)&value, sizeof(value));
		req->buffer_end += sprintf(req_bufptr(req), OPTION_OK "%s\n", argm->name);
		dbg("\n++++ret = %d++++\n\n", ret);
		if (ret == 0xFF && gf_Restarting != 1) {
			gf_Restarting = 1;
		}
		return;
	} while (0);
	req->buffer_end += sprintf(req_bufptr(req), OPTION_NG "%s\n", argm->name);
}
void set_jpegframerate(request *req, COMMAND_ARGUMENT *argm)
{
	__u8 value = atoi(argm->value);

	do {
		ControlSystemData(SFIELD_SET_JPEG_FRAMERATE, (void *)&value, sizeof(value));
		req->buffer_end += sprintf(req_bufptr(req), OPTION_OK "%s\n", argm->name);
		return;
	} while (0);
	req->buffer_end += sprintf(req_bufptr(req), OPTION_NG "%s\n", argm->name);
}
void set_mpeg41framerate(request *req, COMMAND_ARGUMENT *argm)
{
	__u8 value = atoi(argm->value);

	do {
		ControlSystemData(SFIELD_SET_MPEG41_FRAMERATE, (void *)&value, sizeof(value));
		req->buffer_end += sprintf(req_bufptr(req), OPTION_OK "%s\n", argm->name);
		return;
	} while (0);
	req->buffer_end += sprintf(req_bufptr(req), OPTION_NG "%s\n", argm->name);
}
void set_mpeg42framerate(request *req, COMMAND_ARGUMENT *argm)
{
	__u8 value = atoi(argm->value);

	do {
		ControlSystemData(SFIELD_SET_MPEG42_FRAMERATE, (void *)&value, sizeof(value));
		req->buffer_end += sprintf(req_bufptr(req), OPTION_OK "%s\n", argm->name);
		return;
	} while (0);
	req->buffer_end += sprintf(req_bufptr(req), OPTION_NG "%s\n", argm->name);
}
/*-----------------------------------------------------------------*/
void set_videocodecNumber(request *req, COMMAND_ARGUMENT *argm)
{
	__u8 value ;
	codecNumberValue = atoi(argm->value);
	codecSetCount++;
#ifdef ISLAB_DEBUG
	printf("????????????????????????????set_videocodecNumber?????????????????????????\n");
#endif
	do {
		if (codecSetCount==2)
		{
			if(codecNumberValue==0)
				value = codecMenuValue;
			if (codecNumberValue==1)
				value = codecMenuValue+3;
			if (codecNumberValue==2)
				value = codecMenuValue+8;
			int ret = ControlSystemData(SFIELD_SET_VIDEOCODECMODE, (void *)&value, sizeof(value));
			req->buffer_end += sprintf(req_bufptr(req), OPTION_OK "%s\n", argm->name);
			dbg("\n++++ret = %d++++\n\n", ret);
			if (ret == 0xFF && gf_Restarting != 1) {
				gf_Restarting = 1;
			}
			codecSetCount=0;
		}
		return;
	} while (0);
	req->buffer_end += sprintf(req_bufptr(req), OPTION_NG "%s\n", argm->name);
}
void set_videocodecMenu(request *req, COMMAND_ARGUMENT *argm)
{
    int case_num;
	UNIX_CMD_STRUCT cmd;
	videoencmodetype modetype;
	
    case_num = atoi(argm -> value);
    printf("\n\r###########argm->value = %s,case_num= %d#############\r\n",argm->value,case_num);
    switch(case_num)
    {
        case 0:
			video_setting.video_enc_mode = VIDEO_ENC_H264;
			break;
		case 1:
			video_setting.video_enc_mode = VIDEO_ENC_JPEG;
			break;
		default:
			SYS_INFO("\n\r###########VIDEO_SETTING ERROR!############\r\n");
    }
	printf("\n\r!!!!!!!!!!!!!!!!!!!GBSHM_PARA_GET argm->value = %s,video_ENC = %d !!!!!!!!!!!!!!!!!!!!!!!!\r\n",argm->value,video_setting.video_enc_mode);
	modetype = (int)video_setting.video_enc_mode;
	cmd.cmd = PARACMD_BOA_BOATOCORE_PARA_SETVIDEOENCMODE;
	cmd.cmdlen = sizeof(modetype);
	memcpy(cmd.cmdinfo, &modetype, sizeof(modetype));
	cmd.ret_ack = 0;
	Unix_send(global_boa_private_handle.unix_fd, &cmd);	
}
/*
   void set_videocodecmode(request *req, COMMAND_ARGUMENT *argm)
   {
   __u8 value = atoi(argm->value);
   do {
   int ret = ControlSystemData(SFIELD_SET_VIDEOCODECMODE, (void *)&value, sizeof(value));
   req->buffer_end += sprintf(req_bufptr(req), OPTION_OK "%s\n", argm->name);
   dbg("\n++++ret = %d++++\n\n", ret);
   if (ret == 0xFF && gf_Restarting != 1) {
   gf_Restarting = 1;
   }
   return;
   } while (0);
   req->buffer_end += sprintf(req_bufptr(req), OPTION_NG "%s\n", argm->name);
   }*/
void set_videoresmenu(request *req, COMMAND_ARGUMENT *argm)
{
    int case_num;
	UNIX_CMD_STRUCT cmd;
	videoencresolutiontype reslutiontype;
	
    case_num = atoi(argm -> value);
    printf("\n\r###########argm->value = %s,case_num= %d#############\r\n",argm->value,case_num);
    switch(case_num)
    {
        case 0:
			video_setting.video_enc_resolution = VIDEO_ENC_D1;
			break;
		case 1:
			video_setting.video_enc_resolution = VIDEO_ENC_CIF;
			break;
		default:
			SYS_INFO("\n\r###########VIDEO_SETTING ERROR!############\r\n");
    }
	printf("\n\r!!!!!!!!!!!!!!!!!!!GBSHM_PARA_GET argm->value = %s,video_res = %d !!!!!!!!!!!!!!!!!!!!!!!!\r\n",argm->value,video_setting.video_enc_resolution);
	reslutiontype = (int)video_setting.video_enc_resolution;
	cmd.cmd = PARACMD_BOA_BOATOCORE_PARA_SETVIDEOENCRESOLUTION;
	cmd.cmdlen = sizeof(reslutiontype);
	memcpy(cmd.cmdinfo, &reslutiontype, sizeof(reslutiontype));
	cmd.ret_ack = 0;
	Unix_send(global_boa_private_handle.unix_fd, &cmd);	
}
void set_videoframemenu(request *req, COMMAND_ARGUMENT *argm)
{
    int case_num;
	UNIX_CMD_STRUCT cmd;
	frameratetype frametype;
	
    case_num = atoi(argm -> value);
    printf("\n\r###########argm->value = %s,case_num= %d#############\r\n",argm->value,case_num);
    switch(case_num)
    {
        case 0:
			if (video_setting.video_input_mode == VIDEO_INPUT_PAL)
				video_setting.video_enc_framerate = VIDEO_ENC_30FPS;
			else
				video_setting.video_enc_framerate = VIDEO_ENC_25FPS;
			break;
		case 1:
			video_setting.video_enc_framerate = VIDEO_ENC_15FPS;
			break;
		case 2:
			video_setting.video_enc_framerate = VIDEO_ENC_7FPS;
			break;
		default:
			SYS_INFO("\n\r###########VIDEO_SETTING ERROR!############\r\n");
    }
	printf("\n\r!!!!!!!!!!!!!!!!!!!GBSHM_PARA_GET argm->value = %s,video_frame= %d !!!!!!!!!!!!!!!!!!!!!!!!\r\n",argm->value,video_setting.video_enc_framerate);
	frametype = (int)video_setting.video_enc_framerate;
	cmd.cmd = PARACMD_BOA_BOATOCORE_PARA_SETFRAMERATE;
	cmd.cmdlen = sizeof(frametype);
	memcpy(cmd.cmdinfo, &frametype, sizeof(frametype));
	cmd.ret_ack = 0;
	Unix_send(global_boa_private_handle.unix_fd, &cmd);	

}
void set_videokeyframe(request *req, COMMAND_ARGUMENT *argm)
{
	UNIX_CMD_STRUCT cmd;
	keyframeintervaltype keyframe;
    video_setting.keyFrameInterval = atoi(argm -> value);
	printf("\n\r!!!!!!!!!!!!!!!!!!!GBSHM_PARA_GET argm->value = %s,video_frame= %d !!!!!!!!!!!!!!!!!!!!!!!!\r\n",argm->value,video_setting.keyFrameInterval);
	keyframe = video_setting.keyFrameInterval;
	cmd.cmd = BOA_BOATOCORE_PARA_SETKEYFRAMEINTERVAL_SENDTO_DEST;
	cmd.cmdlen = sizeof(keyframe);
	memcpy(cmd.cmdinfo, &keyframe, sizeof(keyframe));
	cmd.ret_ack = 0;
	Unix_send(global_boa_private_handle.unix_fd, &cmd);	

}
void set_imagesource(request *req, COMMAND_ARGUMENT *argm)
{
    int case_num;
	UNIX_CMD_STRUCT cmd;
	videoinputmodetype inputmodetype;
	case_num = atoi(argm -> value);
    printf("\n\r###########argm->value = %s,case_num= %d#############\r\n",argm->value,case_num);
    switch(case_num)
    {
        case 0:
			video_setting.video_input_mode = VIDEO_INPUT_PAL;
			break;
		case 1:
			video_setting.video_input_mode = VIDEO_INPUT_NTSC;
			break;
		default:
			SYS_INFO("\n\r###########VIDEO_SETTING ERROR!############\r\n");
    }
	printf("\n\r!!!!!!!!!!!!!!!!!!!GBSHM_PARA_GET argm->value = %s,video_input = %d !!!!!!!!!!!!!!!!!!!!!!!!\r\n",argm->value,video_setting.video_input_mode);
	inputmodetype = (int)video_setting.video_input_mode;
	cmd.cmd = PARACMD_BOA_BOATOCORE_PARA_SETVIDEOINPUTMODE;
	cmd.cmdlen = sizeof(inputmodetype);
	memcpy(cmd.cmdinfo, &inputmodetype, sizeof(inputmodetype));
	cmd.ret_ack = 0;
	Unix_send(global_boa_private_handle.unix_fd, &cmd);	


/*
	__u8 value = atoi(argm->value);

	do {
		ControlSystemData(SFIELD_SET_IMAGESOURCE, (void *)&value, sizeof(value));
		req->buffer_end += sprintf(req_bufptr(req), OPTION_OK "%s\n", argm->name);
		return;
	} while (0);
	req->buffer_end += sprintf(req_bufptr(req), OPTION_NG "%s\n", argm->name);
	*/
}
//unsigned char gl_TIflag = 0;
void set_img2a(request *req, COMMAND_ARGUMENT *argm)
{
	__u8 value = atoi(argm->value);
#ifdef ISLAB_DEBUG
	printf("?????????????????????????set_img2a?????????????????????????\n");
#endif
	/*if (value == 3 && gl_TIflag == 0) {
	  gl_TIflag = 1;
	  req->buffer_end += sprintf(req_bufptr(req), OPTION_OK "%s\n", argm->name);
	  return;
	  }
	  if (gl_TIflag == 1)
	  value += 4;*/
	do {
		int ret = ControlSystemData(SFIELD_SET_IMAGE2A, (void *)&value, sizeof(value));
		req->buffer_end += sprintf(req_bufptr(req), OPTION_OK "%s\n", argm->name);
		dbg("\n++++ret = %d++++\n\n", ret);
		if (ret == 0xFF && gf_Restarting != 1) {
			gf_Restarting = 1;
		}
		return;
	} while (0);
	req->buffer_end += sprintf(req_bufptr(req), OPTION_NG "%s\n", argm->name);
}
/*-----------------------------*/
void set_tvcable(request *req, COMMAND_ARGUMENT *argm)
{
	__u8 value = atoi(argm->value);

	do {
		ControlSystemData(SFIELD_SET_TVCABLE, (void *)&value, sizeof(value));
		req->buffer_end += sprintf(req_bufptr(req), OPTION_OK "%s\n", argm->name);
		return;
	} while (0);
	req->buffer_end += sprintf(req_bufptr(req), OPTION_NG "%s\n", argm->name);
}
void set_binning(request *req, COMMAND_ARGUMENT *argm)
{
	__u8 value = atoi(argm->value);
#ifdef ISLAB_DEBUG
	printf("?????????????????????????????set_binning??????????????????????????\n");
#endif
	do {
		int ret = ControlSystemData(SFIELD_SET_BINNING, (void *)&value, sizeof(value));
		req->buffer_end += sprintf(req_bufptr(req), OPTION_OK "%s\n", argm->name);
		dbg("\n++++ret = %d++++\n\n", ret);
		if (ret == 0xFF && gf_Restarting != 1) {
			gf_Restarting = 1;
		}
		return;
	} while (0);
	req->buffer_end += sprintf(req_bufptr(req), OPTION_NG "%s\n", argm->name);
}
void set_blc(request *req, COMMAND_ARGUMENT *argm)
{
	__u8 value = atoi(argm->value);

	do {
		ControlSystemData(SFIELD_SET_BLC, (void *)&value, sizeof(value));
		req->buffer_end += sprintf(req_bufptr(req), OPTION_OK "%s\n", argm->name);
		return;
	} while (0);
	req->buffer_end += sprintf(req_bufptr(req), OPTION_NG "%s\n", argm->name);
}
void set_img2atype(request *req, COMMAND_ARGUMENT *argm)
{
	__u8 value = atoi(argm->value);

	do {
		ControlSystemData(SFIELD_SET_IMAGE2ATYPE, (void *)&value, sizeof(value));
		req->buffer_end += sprintf(req_bufptr(req), OPTION_OK "%s\n", argm->name);
		return;
	} while (0);
	req->buffer_end += sprintf(req_bufptr(req), OPTION_NG "%s\n", argm->name);
}
void set_mpeg41bitrate(request *req, COMMAND_ARGUMENT *argm)
{
    int rate_num = 0;
	UNIX_CMD_STRUCT cmd,cmd1;
	boatoflash_para_save para_save;
	bitratetype bitratesetting;
	
	para_save.testflag = 0xFF;
	cmd.cmd = PARACMD_BOA_BOATOFLASH_PARA_SAVE;
	cmd.cmdlen = sizeof(para_save);
	memcpy(cmd.cmdinfo, &para_save, sizeof(para_save));
	cmd.ret_ack = 0;
    rate_num = atoi(argm -> value);
	rate_num = rate_num * 1000;
    printf("\n\r###########argm->value = %s,rate_num= %d#############\r\n",argm->value,rate_num);
    video_setting.video_enc_bitrate = rate_num;
	printf("\n\r!!!!!!!!!!!!!!!!!!!GBSHM_PARA_GET argm->value = %s,video_rate = %d !!!!!!!!!!!!!!!!!!!!!!!!\r\n",argm->value,video_setting.video_enc_bitrate);
	Gbshm_paramSet(global_boa_private_handle.gbshm_handle, PARA_CMD_SYSTEM_VIDEO_ENC_PARAM, &video_setting, sizeof(video_setting));
	bitratesetting = rate_num;
	cmd1.cmd = PARACMD_BOA_BOATOCORE_PARA_SETBITRATE;
	cmd1.cmdlen = sizeof(bitratesetting);
	memcpy(cmd1.cmdinfo, &bitratesetting, sizeof(bitratesetting));
	cmd1.ret_ack = 0;
	Unix_send(global_boa_private_handle.unix_fd, &cmd);	
	Unix_send(global_boa_private_handle.unix_fd, &cmd1);	
}

void set_mpeg42bitrate(request *req, COMMAND_ARGUMENT *argm)
{
    int rate_num = 0;
	UNIX_CMD_STRUCT cmd,cmd1;
	boatoflash_para_save para_save;
	bitratetype bitratesetting;
	
	para_save.testflag = 0xFF;
	cmd.cmd = PARACMD_BOA_BOATOFLASH_PARA_SAVE;
	cmd.cmdlen = sizeof(para_save);
	memcpy(cmd.cmdinfo, &para_save, sizeof(para_save));
	cmd.ret_ack = 0;
    rate_num = atoi(argm -> value);
	rate_num = rate_num * 1000;
    printf("\n\r###########argm->value = %s,rate_num= %d#############\r\n",argm->value,rate_num);
    video_setting.video_enc_bitrate = rate_num;
	printf("\n\r!!!!!!!!!!!!!!!!!!!GBSHM_PARA_GET argm->value = %s,video_rate = %d !!!!!!!!!!!!!!!!!!!!!!!!\r\n",argm->value,video_setting.video_enc_bitrate);
	Gbshm_paramSet(global_boa_private_handle.gbshm_handle, PARA_CMD_SYSTEM_VIDEO_ENC_PARAM, &video_setting, sizeof(video_setting));
	bitratesetting = rate_num;
	cmd1.cmd = PARACMD_BOA_BOATOCORE_PARA_SETBITRATE;
	cmd1.cmdlen = sizeof(bitratesetting);
	memcpy(cmd1.cmdinfo, &bitratesetting, sizeof(bitratesetting));
	cmd1.ret_ack = 0;
	Unix_send(global_boa_private_handle.unix_fd, &cmd);	
	Unix_send(global_boa_private_handle.unix_fd, &cmd1);	
}
void set_mpeg43bitrate(request *req, COMMAND_ARGUMENT *argm)
{
    int rate_num;
	UNIX_CMD_STRUCT cmd,cmd1;
	boatoflash_para_save para_save;
	bitratetype bitratesetting;
	
	para_save.testflag = 0xFF;
	cmd.cmd = PARACMD_BOA_BOATOFLASH_PARA_SAVE;
	cmd.cmdlen = sizeof(para_save);
	memcpy(cmd.cmdinfo, &para_save, sizeof(para_save));
	cmd.ret_ack = 0;
    rate_num = atoi(argm -> value);
    printf("\n\r###########argm->value = %s,rate_num= %d#############\r\n",argm->value,rate_num);
    video_setting.video_enc_bitrate = rate_num;
	printf("\n\r!!!!!!!!!!!!!!!!!!!GBSHM_PARA_GET argm->value = %s,video_rate = %d !!!!!!!!!!!!!!!!!!!!!!!!\r\n",argm->value,video_setting.video_enc_bitrate);
	Gbshm_paramSet(global_boa_private_handle.gbshm_handle, PARA_CMD_SYSTEM_VIDEO_ENC_PARAM, &video_setting, sizeof(video_setting));
	bitratesetting = rate_num;
	cmd1.cmd = PARACMD_BOA_BOATOCORE_PARA_SETBITRATE;
	cmd1.cmdlen = sizeof(bitratesetting);
	memcpy(cmd1.cmdinfo, &bitratesetting, sizeof(bitratesetting));
	cmd1.ret_ack = 0;
	Unix_send(global_boa_private_handle.unix_fd, &cmd);	
	Unix_send(global_boa_private_handle.unix_fd, &cmd1);	
}
void set_mpeg44bitrate(request *req, COMMAND_ARGUMENT *argm)
{
    int rate_num;
	UNIX_CMD_STRUCT cmd,cmd1;
	boatoflash_para_save para_save;
	bitratetype bitratesetting;
	
	para_save.testflag = 0xFF;
	cmd.cmd = PARACMD_BOA_BOATOFLASH_PARA_SAVE;
	cmd.cmdlen = sizeof(para_save);
	memcpy(cmd.cmdinfo, &para_save, sizeof(para_save));
	cmd.ret_ack = 0;
    rate_num = atoi(argm -> value);
    printf("\n\r###########argm->value = %s,rate_num= %d#############\r\n",argm->value,rate_num);
    video_setting.video_enc_bitrate = rate_num;
	printf("\n\r!!!!!!!!!!!!!!!!!!!GBSHM_PARA_GET argm->value = %s,video_rate = %d !!!!!!!!!!!!!!!!!!!!!!!!\r\n",argm->value,video_setting.video_enc_bitrate);
	Gbshm_paramSet(global_boa_private_handle.gbshm_handle, PARA_CMD_SYSTEM_VIDEO_ENC_PARAM, &video_setting, sizeof(video_setting));
	bitratesetting = rate_num;
	cmd1.cmd = PARACMD_BOA_BOATOCORE_PARA_SETBITRATE;
	cmd1.cmdlen = sizeof(bitratesetting);
	memcpy(cmd1.cmdinfo, &bitratesetting, sizeof(bitratesetting));
	cmd1.ret_ack = 0;
	Unix_send(global_boa_private_handle.unix_fd, &cmd);	
	Unix_send(global_boa_private_handle.unix_fd, &cmd1);	
}
void set_imagedefault(request *req, COMMAND_ARGUMENT *argm)
{
	__u8 value = atoi(argm->value);

	do {
		ControlSystemData(SFIELD_SET_IMAGEDEFAULT, (void *)&value, sizeof(value));
		req->buffer_end += sprintf(req_bufptr(req), OPTION_OK "%s\n", argm->name);
		return;
	} while (0);
	req->buffer_end += sprintf(req_bufptr(req), OPTION_NG "%s\n", argm->name);
}
/***************************************************************************
 *                                                                         *
 ***************************************************************************/
void set_ftppassword(request *req, COMMAND_ARGUMENT *argm)
{
	char sys_value[33];

	do {
		if (ControlSystemData(SFIELD_GET_FTP_PASSWORD, (void *)sys_value, sizeof(sys_value)) < 0)
			break;

		if (strcmp(sys_value, argm->value)) {
			ControlSystemData(SFIELD_SET_FTP_PASSWORD, (void *)argm->value, strlen(argm->value));
		}
		req->buffer_end += sprintf(req_bufptr(req), OPTION_OK "%s\n", argm->name);
		return;
	} while (0);
	req->buffer_end += sprintf(req_bufptr(req), OPTION_NG "%s\n", argm->name);

}
/***************************************************************************
 *                                                                         *
 ***************************************************************************/
void set_ftpuser(request *req, COMMAND_ARGUMENT *argm)
{
	char sys_value[33];

	do {
		if (ControlSystemData(SFIELD_GET_FTP_USERNAME, (void *)sys_value, sizeof(sys_value)) < 0)
			break;

		if (strcmp(sys_value, argm->value)) {
			ControlSystemData(SFIELD_SET_FTP_USERNAME, (void *)argm->value, strlen(argm->value));
		}
		req->buffer_end += sprintf(req_bufptr(req), OPTION_OK "%s\n", argm->name);
		return;
	} while (0);
	req->buffer_end += sprintf(req_bufptr(req), OPTION_NG "%s\n", argm->name);

}
/***************************************************************************
 *                                                                         *
 ***************************************************************************/
void set_ftppath(request *req, COMMAND_ARGUMENT *argm)
{
	char sys_value[256];

	do {
		if (ControlSystemData(SFIELD_GET_FTP_FOLDNAME, (void *)sys_value, sizeof(sys_value)) < 0)
			break;

		if (strcmp(sys_value, argm->value)) {
			ControlSystemData(SFIELD_SET_FTP_FOLDNAME, (void *)argm->value, strlen(argm->value));
		}
		req->buffer_end += sprintf(req_bufptr(req), OPTION_OK "%s\n", argm->name);
		return;
	} while (0);
	req->buffer_end += sprintf(req_bufptr(req), OPTION_NG "%s\n", argm->name);

}
/***************************************************************************
 *                                                                         *
 ***************************************************************************/
void set_smtppwd(request *req, COMMAND_ARGUMENT *argm)
{
	char sys_value[256];

	do {
		if (ControlSystemData(SFIELD_GET_SMTP_PASSWORD, (void *)sys_value, sizeof(sys_value)) < 0)
			break;

		if (strcmp(sys_value, argm->value)) {
			ControlSystemData(SFIELD_SET_SMTP_PASSWORD, (void *)argm->value, strlen(argm->value));
		}
		req->buffer_end += sprintf(req_bufptr(req), OPTION_OK "%s\n", argm->name);
		return;
	} while (0);
	req->buffer_end += sprintf(req_bufptr(req), OPTION_NG "%s\n", argm->name);

}
/***************************************************************************
 *                                                                         *
 ***************************************************************************/
void set_smtpuser(request *req, COMMAND_ARGUMENT *argm)
{
	char sys_value[256];

	do {
		if (ControlSystemData(SFIELD_GET_SMTP_USERNAME, (void *)sys_value, sizeof(sys_value)) < 0)
			break;

		if (strcmp(sys_value, argm->value)) {
			ControlSystemData(SFIELD_SET_SMTP_USERNAME, (void *)argm->value, strlen(argm->value));
		}
		req->buffer_end += sprintf(req_bufptr(req), OPTION_OK "%s\n", argm->name);
		return;
	} while (0);
	req->buffer_end += sprintf(req_bufptr(req), OPTION_NG "%s\n", argm->name);

}
/***************************************************************************
 *                                                                         *
 ***************************************************************************/
void set_smtpto(request *req, COMMAND_ARGUMENT *argm)
{
	char sys_value[256];

	do {
		if (ControlSystemData(SFIELD_GET_SMTP_RECEIVER_EMAIL_ADDRESS, (void *)sys_value, sizeof(sys_value)) < 0)
			break;

		if (strcmp(sys_value, argm->value)) {
			ControlSystemData(SFIELD_SET_SMTP_RECEIVER_EMAIL_ADDRESS, (void *)argm->value, strlen(argm->value));
		}
		req->buffer_end += sprintf(req_bufptr(req), OPTION_OK "%s\n", argm->name);
		return;
	} while (0);
	req->buffer_end += sprintf(req_bufptr(req), OPTION_NG "%s\n", argm->name);

}
/***************************************************************************
 *                                                                         *
 ***************************************************************************/
void set_smtpsender(request *req, COMMAND_ARGUMENT *argm)
{
	char sys_value[256];

	do {
		if (ControlSystemData(SFIELD_GET_SMTP_SENDER_EMAIL_ADDRESS, (void *)sys_value, sizeof(sys_value)) < 0)
			break;

		if (strcmp(sys_value, argm->value)) {
			ControlSystemData(SFIELD_SET_SMTP_SENDER_EMAIL_ADDRESS, (void *)argm->value, strlen(argm->value));
		}
		req->buffer_end += sprintf(req_bufptr(req), OPTION_OK "%s\n", argm->name);
		return;
	} while (0);
	req->buffer_end += sprintf(req_bufptr(req), OPTION_NG "%s\n", argm->name);

}
/***************************************************************************
 *                                                                         *
 ***************************************************************************/
void set_smtpfqdn(request *req, COMMAND_ARGUMENT *argm)
{
	char sys_value[256];

	do {
		if (ControlSystemData(SFIELD_GET_SMTP_SERVER_IP, (void *)sys_value, sizeof(sys_value)) < 0)
			break;

		if (strcmp(sys_value, argm->value)) {
			ControlSystemData(SFIELD_SET_SMTP_SERVER_IP, (void *)argm->value, strlen(argm->value));
		}
		req->buffer_end += sprintf(req_bufptr(req), OPTION_OK "%s\n", argm->name);
		return;
	} while (0);
	req->buffer_end += sprintf(req_bufptr(req), OPTION_NG "%s\n", argm->name);

}
/***************************************************************************
 *设置ftp开关，2010-09-20                                                  *
 ***************************************************************************/
/*
#ifdef ISLAB
void set_ftpswitch()
{
__u8 value = atoi(argm->value); 
do {
if(ControlSystemData(SFIELD_SET_FTP_SWITCH, &value, sizeof(value)) < 0)
break;
req->buffer_end += sprintf(req_bufptr(req), OPTION_OK "%s\n", argm->name);
return;
} while (0);
req->buffer_end += sprintf(req_bufptr(req), OPTION_NG "%s\n", argm->name);
}	
#endif
 */
/***************************************************************************
 *                                                                          *
 ***************************************************************************/
void set_ftpfqdn(request *req, COMMAND_ARGUMENT *argm)
{
	char sys_value[256];

	do {
		if (ControlSystemData(SFIELD_GET_FTP_SERVER_IP, (void *)sys_value, sizeof(sys_value)) < 0)
			break;

		if (strcmp(sys_value, argm->value)) {
			ControlSystemData(SFIELD_SET_FTP_SERVER_IP, (void *)argm->value, strlen(argm->value));
		}
		req->buffer_end += sprintf(req_bufptr(req), OPTION_OK "%s\n", argm->name);
		return;
	} while (0);
	req->buffer_end += sprintf(req_bufptr(req), OPTION_NG "%s\n", argm->name);

}
/***************************************************************************
 *                                                                         *
 ***************************************************************************/
void set_smtpauth(request *req, COMMAND_ARGUMENT *argm)
{
	unsigned char sys_value, value = atoi(argm->value);

	do {
		if (ControlSystemData(SFIELD_GET_SMTP_AUTHENTICATION, (void *)&sys_value, sizeof(sys_value)) < 0)
			break;

		if (sys_value != value) {
			ControlSystemData(SFIELD_SET_SMTP_AUTHENTICATION, (void *)&value, sizeof(value));
		}
		req->buffer_end += sprintf(req_bufptr(req), OPTION_OK "%s\n", argm->name);
		return;
	} while (0);
	req->buffer_end += sprintf(req_bufptr(req), OPTION_NG "%s\n", argm->name);

}
/***************************************************************************
 *                                                                         *
 ***************************************************************************/
void set_ftpport(request *req, COMMAND_ARGUMENT *argm)
{
	unsigned short sys_port, port = atoi(argm->value);

	do {
		if (ControlSystemData(SFIELD_GET_FTP_PORT, (void *)&sys_port, sizeof(sys_port)) < 0)
			break;

		if (sys_port != port) {
			ControlSystemData(SFIELD_SET_FTP_PORT, (void *)&port, sizeof(port));
		}
		req->buffer_end += sprintf(req_bufptr(req), OPTION_OK "%s\n", argm->name);
		return;
	} while (0);
	req->buffer_end += sprintf(req_bufptr(req), OPTION_NG "%s\n", argm->name);

}
/***************************************************************************
 *                                                                         *
 ***************************************************************************/
void stream1xsize(request *req, COMMAND_ARGUMENT *argm)
{
	do {
		SysInfo* pSysInfo = GetSysInfo();
		if(pSysInfo == NULL)
			break;
		req->buffer_end += sprintf(req_bufptr(req), OPTION_OK "%s=%d\n", argm->name, pSysInfo->lan_config.JpegXsize);
		return;
	} while (0);
	req->buffer_end += sprintf(req_bufptr(req), OPTION_NG "%s\n", argm->name);
}

/***************************************************************************
 *                                                                         *
 ***************************************************************************/
void stream1ysize(request *req, COMMAND_ARGUMENT *argm)
{
	do {
		SysInfo* pSysInfo = GetSysInfo();
		if(pSysInfo == NULL)
			break;
		req->buffer_end += sprintf(req_bufptr(req), OPTION_OK "%s=%d\n", argm->name, pSysInfo->lan_config.JpegYsize);
		return;
	} while (0);
	req->buffer_end += sprintf(req_bufptr(req), OPTION_NG "%s\n", argm->name);
}

/***************************************************************************
 *                                                                         *
 ***************************************************************************/
void stream2xsize(request *req, COMMAND_ARGUMENT *argm)
{
	do {
		SysInfo* pSysInfo = GetSysInfo();
		if(pSysInfo == NULL)
			break;
		req->buffer_end += sprintf(req_bufptr(req), OPTION_OK "%s=%d\n", argm->name, pSysInfo->lan_config.Mpeg41Xsize);
		return;
	} while (0);
	req->buffer_end += sprintf(req_bufptr(req), OPTION_NG "%s\n", argm->name);
}

/***************************************************************************
 *                                                                         *
 ***************************************************************************/
void stream2ysize(request *req, COMMAND_ARGUMENT *argm)
{
	do {
		SysInfo* pSysInfo = GetSysInfo();
		if(pSysInfo == NULL)
			break;
		req->buffer_end += sprintf(req_bufptr(req), OPTION_OK "%s=%d\n", argm->name, pSysInfo->lan_config.Mpeg41Ysize);
		return;
	} while (0);
	req->buffer_end += sprintf(req_bufptr(req), OPTION_NG "%s\n", argm->name);
}

/***************************************************************************
 *                                                                         *
 ***************************************************************************/
void stream3xsize(request *req, COMMAND_ARGUMENT *argm)
{
	do {
		SysInfo* pSysInfo = GetSysInfo();
		if(pSysInfo == NULL)
			break;
		req->buffer_end += sprintf(req_bufptr(req), OPTION_OK "%s=%d\n", argm->name, pSysInfo->lan_config.Mpeg42Xsize);
		return;
	} while (0);
	req->buffer_end += sprintf(req_bufptr(req), OPTION_NG "%s\n", argm->name);
}

/***************************************************************************
 *                                                                         *
 ***************************************************************************/
void stream3ysize(request *req, COMMAND_ARGUMENT *argm)
{
	do {
		SysInfo* pSysInfo = GetSysInfo();
		if(pSysInfo == NULL)
			break;
		req->buffer_end += sprintf(req_bufptr(req), OPTION_OK "%s=%d\n", argm->name, pSysInfo->lan_config.Mpeg42Ysize);
		return;
	} while (0);
	req->buffer_end += sprintf(req_bufptr(req), OPTION_NG "%s\n", argm->name);
}
/***************************************************************************
 *                                                                         *
 ***************************************************************************/
void set_ipncptz(request *req, COMMAND_ARGUMENT *argm)
{
	do {
		if(ControlSystemData(SFIELD_IPNCPTZ, argm->value, strlen(argm->value)) < 0)
			break;
		req->buffer_end += sprintf(req_bufptr(req), OPTION_OK "%s\n", argm->name);
		return;
	} while (0);
	req->buffer_end += sprintf(req_bufptr(req), OPTION_NG "%s\n", argm->name);

}
/***************************************************************************
 *                                                                         *
 ***************************************************************************/
void init_ipncptz(request *req, COMMAND_ARGUMENT *argm)
{
	do {
		if(ControlSystemData(SFIELD_INIT_IPNCPTZ, argm->value, strlen(argm->value)) < 0)
			break;
		req->buffer_end += sprintf(req_bufptr(req), OPTION_OK "%s\n", argm->name);
		return;
	} while (0);
	req->buffer_end += sprintf(req_bufptr(req), OPTION_NG "%s\n", argm->name);

}
/***************************************************************************
 *                                                                         *
 ***************************************************************************/
void set_timeformat(request *req, COMMAND_ARGUMENT *argm)
{
	__u8 value = atoi(argm->value);

	do {
		ControlSystemData(SFIELD_SET_TIMEFORMAT, (void *)&value, sizeof(value));
		req->buffer_end += sprintf(req_bufptr(req), OPTION_OK "%s\n", argm->name);
		return;
	} while (0);
	req->buffer_end += sprintf(req_bufptr(req), OPTION_NG "%s\n", argm->name);
}
/***************************************************************************
 *                                                                         *
 ***************************************************************************/
void set_daylight(request *req, COMMAND_ARGUMENT *argm)
{
	__u8 value = atoi(argm->value);

	do {
		ControlSystemData(SFIELD_SET_DAYLIGHT, (void *)&value, sizeof(value));
		req->buffer_end += sprintf(req_bufptr(req), OPTION_OK "%s\n", argm->name);
		return;
	} while (0);
	req->buffer_end += sprintf(req_bufptr(req), OPTION_NG "%s\n", argm->name);
}
/***************************************************************************
 *                                                                         *
 ***************************************************************************/
void set_timezone(request *req, COMMAND_ARGUMENT *argm)
{
	__u8 value = atoi(argm->value);

	do {
		if (value > 24)
			break;
		ControlSystemData(SFIELD_SET_TIMEZONE, (void *)&value, sizeof(value));
		req->buffer_end += sprintf(req_bufptr(req), OPTION_OK "%s\n", argm->name);
		return;
	} while (0);
	req->buffer_end += sprintf(req_bufptr(req), OPTION_NG "%s\n", argm->name);
}
/***************************************************************************
 *                                                                         *
 ***************************************************************************/
void set_gioinenable(request *req, COMMAND_ARGUMENT *argm)
{
	__u8 value = atoi(argm->value); 
	do {
		if(ControlSystemData(SFIELD_SET_GIOINENABLE, &value, sizeof(value)) < 0)
			break;
		req->buffer_end += sprintf(req_bufptr(req), OPTION_OK "%s\n", argm->name);
		return;
	} while (0);
	req->buffer_end += sprintf(req_bufptr(req), OPTION_NG "%s\n", argm->name);
}
/***************************************************************************
 *                                                                         *
 ***************************************************************************/
void set_giointype(request *req, COMMAND_ARGUMENT *argm)
{
	__u8 value = atoi(argm->value);
	do {
		if(ControlSystemData(SFIELD_SET_GIOINTYPE, &value, sizeof(value)) < 0)
			break;
		req->buffer_end += sprintf(req_bufptr(req), OPTION_OK "%s\n", argm->name);
		return;
	} while (0);
	req->buffer_end += sprintf(req_bufptr(req), OPTION_NG "%s\n", argm->name);
}
/***************************************************************************
 *                                                                         *
 ***************************************************************************/
void set_giooutenable(request *req, COMMAND_ARGUMENT *argm)
{
	__u8 value = atoi(argm->value);
	do {
		if(ControlSystemData(SFIELD_SET_GIOOUTENABLE, &value, sizeof(value)) < 0)
			break;
		req->buffer_end += sprintf(req_bufptr(req), OPTION_OK "%s\n", argm->name);
		return;
	} while (0);
	req->buffer_end += sprintf(req_bufptr(req), OPTION_NG "%s\n", argm->name);
}
/***************************************************************************
 *                                                                         *
 ***************************************************************************/
void set_gioouttype(request *req, COMMAND_ARGUMENT *argm)
{
	__u8 value = atoi(argm->value);
	do {
		if(ControlSystemData(SFIELD_SET_GIOOUTTYPE, &value, sizeof(value)) < 0)
			break;
		req->buffer_end += sprintf(req_bufptr(req), OPTION_OK "%s\n", argm->name);
		return;
	} while (0);
	req->buffer_end += sprintf(req_bufptr(req), OPTION_NG "%s\n", argm->name);
}
/***************************************************************************
 *                                                                         *
 ***************************************************************************/
void set_tstampenable(request *req, COMMAND_ARGUMENT *argm)
{
	__u8 value = atoi(argm->value);
	do {
		if(ControlSystemData(SFIELD_SET_TSTAMPENABLE, &value, sizeof(value)) < 0)
			break;
		req->buffer_end += sprintf(req_bufptr(req), OPTION_OK "%s\n", argm->name);
		return;
	} while (0);
	req->buffer_end += sprintf(req_bufptr(req), OPTION_NG "%s\n", argm->name);
}
/***************************************************************************
 *                                                                         *
 ***************************************************************************/
void set_tstampformat(request *req, COMMAND_ARGUMENT *argm)
{
	__u8 value = atoi(argm->value);
	do {
		if(ControlSystemData(SFIELD_SET_TSTAMPFORMAT, &value, sizeof(value)) < 0)
			break;
		req->buffer_end += sprintf(req_bufptr(req), OPTION_OK "%s\n", argm->name);
		return;
	} while (0);
	req->buffer_end += sprintf(req_bufptr(req), OPTION_NG "%s\n", argm->name);
}
/***************************************************************************
 *                                                                         *
 ***************************************************************************/
void set_audioinvolume(request *req, COMMAND_ARGUMENT *argm)
{
	__u8 value = atoi(argm->value);
	do {
		if(ControlSystemData(SFIELD_SET_AUDIOINVOLUME, &value, sizeof(value)) < 0)
			break;
		req->buffer_end += sprintf(req_bufptr(req), OPTION_OK "%s\n", argm->name);
		return;
	} while (0);
	req->buffer_end += sprintf(req_bufptr(req), OPTION_NG "%s\n", argm->name);
}
/***************************************************************************
 *                                                                         *
 ***************************************************************************/
void add_user(request *req, COMMAND_ARGUMENT *argm)
{
	add_user_t acount;
	int i, count = -1;

	do {
		for(i = 0;argm->value[++count] != ':';i++){
			acount.user_id[i] = argm->value[count];
		}
		acount.user_id[i] = '\0';
		for(i = 0;argm->value[++count] != ':';i++){
			acount.password[i] = argm->value[count];
		}
		acount.password[i] = '\0';
		acount.authority = atoi((argm->value) +count + 1);
		if (ControlSystemData(SFIELD_ADD_USER, (void *)&acount, sizeof(acount)) < 0)
			break;
		req->buffer_end += sprintf(req_bufptr(req), OPTION_OK "%s\n", argm->name);
		return;
	} while (0);
	req->buffer_end += sprintf(req_bufptr(req), OPTION_NG "%s\n", argm->name);
}
/***************************************************************************
 *                                                                         *
 ***************************************************************************/
void del_user(request *req, COMMAND_ARGUMENT *argm)
{
	do {
		if (req->authority > AUTHORITY_ADMIN) {
			dbg("Not enought permission!\n");
			break;
		}
		if (strcasecmp(req->user_id, argm->value) == 0) {
			dbg("Can not delete the user : %s\n", argm->value);
			break;
		}
		if (ControlSystemData(SFIELD_DEL_USER, (void *)argm->value, strlen(argm->value)) < 0)
			break;
		req->buffer_end += sprintf(req_bufptr(req), OPTION_OK "%s\n", argm->name);
		return;
	} while (0);
	req->buffer_end += sprintf(req_bufptr(req), OPTION_NG "%s\n", argm->name);
}
/***************************************************************************
 *                                                                         *
 ***************************************************************************/
void event_start(request *req, COMMAND_ARGUMENT *argm)
{
	int value = atoi(argm->value);
	do {
		if (value < 0)
			break;
		event_location = value;
		req->buffer_end += sprintf(req_bufptr(req), OPTION_OK "%s\n", argm->name);
		return;
	} while (0);
	req->buffer_end += sprintf(req_bufptr(req), OPTION_NG "%s\n", argm->name);
}
/***************************************************************************
 *                                                                         *
 ***************************************************************************/
void sdformat(request *req, COMMAND_ARGUMENT *argm)
{
	int value = atoi(argm->value);
	do {
		if (ControlSystemData(SFIELD_SD_FORMAT, &value, sizeof(value)) < 0)
			break;
		req->buffer_end += sprintf(req_bufptr(req), OPTION_OK "%s\n", argm->name);
		return;
	} while (0);
	req->buffer_end += sprintf(req_bufptr(req), OPTION_NG "%s\n", argm->name);
}
/***************************************************************************
 *                                                                         *
 ***************************************************************************/
void language(request *req, COMMAND_ARGUMENT *argm)
{
	req->buffer_end += sprintf(req_bufptr(req), OPTION_OK "%s\n", argm->name);
	return;
}
/***************************************************************************
 *                                                                         *
 ***************************************************************************/
void restart_ipcam(request *req, COMMAND_ARGUMENT *argm)
{
	char cmd[256];

	int dev_fp;
	void *pMem_map;
	unsigned long phyAddr= 0x01C41000;
	unsigned long length = 0xAE0;
	unsigned int *pPSCCtrl = NULL;
	unsigned int *pPTCMD = NULL;

	req->buffer_end += sprintf(req_bufptr(req), OPTION_OK "%s\n", argm->name);



	if ((dev_fp=open("/dev/mem",O_RDWR|O_SYNC))==-1)
	{
		printf("dev_fp Fail!! \n");
		return ;
	}
	pMem_map=mmap(	(void	*)phyAddr,length/*0x10000000*/,
			PROT_READ|PROT_WRITE|PROT_EXEC,MAP_SHARED,
			dev_fp,phyAddr);

	if (pMem_map!=(void *)phyAddr)
	{
		printf("pMem_map Fail!! \n");
		return ;
	}
#ifdef ISLAB_DEBUG
	printf("--------------------------------------------DEBUG islab restart_ipcam called--------------------------\n");	//islab 2010-06-22
#endif
	sprintf(cmd, "killall -KILL av_server.out \n");
	system(cmd);

	pPTCMD 		= (unsigned int *)( (char *)pMem_map+0x120);
	pPSCCtrl 	= (unsigned int *)( (char *)pMem_map+0xA00);

	/*EDMA */
	*(pPSCCtrl+0) 	= 2;
	*pPTCMD 		= 1;
	usleep(5000);
	/*EDMA */
	*(pPSCCtrl+1) 	= 2;
	*pPTCMD 		= 1;
	usleep(5000);
	/*EDMA */
	*(pPSCCtrl+2) 	= 2;
	*pPTCMD 		= 1;
	usleep(5000);
	/*EDMA */
	*(pPSCCtrl+3) 	= 2;
	*pPTCMD 		= 1;
	usleep(5000);
	/*EDMA */
	*(pPSCCtrl+4) 	= 2;
	*pPTCMD 		= 1;
	usleep(5000);
	/*VPSS */
	*(pPSCCtrl+47) 	= 2;
	*pPTCMD 		= 1;
	usleep(5000);
	/*MJCP */
	*(pPSCCtrl+50) 	= 2;
	*pPTCMD 		= 1;
	usleep(5000);
	/*HDVICP */
	*(pPSCCtrl+51) 	= 2;
	*pPTCMD 		= 1;
	usleep(5000);

	if( pMem_map )
		munmap(pMem_map, length);

	if( dev_fp >= 0)
		close(dev_fp);

	sprintf(cmd, "shutdown -r now\n");
	system(cmd);



	return;
}
/***************************************************************************
 *                                                                         *
 ***************************************************************************/
void getalarmstatus(request *req, COMMAND_ARGUMENT *argm)
{
	char cmd[80];
	do{
		SysInfo* pSysInfo = GetSysInfo();
		if(pSysInfo == NULL)
			break;
		req->buffer_end += sprintf(req_bufptr(req), OPTION_OK "%s=%04x\n",
				argm->name, pSysInfo->lan_config.alarmstatus);

		if(pSysInfo->lan_config.alarmstatus == 1 || pSysInfo->lan_config.alarmstatus == 2 || pSysInfo->lan_config.alarmstatus == 3)
		{
			alarm_status = 1;
		}else{
			if(alarm_status == 1)
			{
				sprintf(cmd, "killall -9 aplay");
				system(cmd);
				alarm_status = 0;
			}else{
				alarm_status = 0;
			}
		}
		return;
	}while(0);
	req->buffer_end += sprintf(req_bufptr(req), OPTION_NG "%s\n", argm->name);
}
/***************************************************************************
 *                                                                         *
 ***************************************************************************/
void sd_unmount(request *req, COMMAND_ARGUMENT *argm)
{
	__u8 value = atoi(argm->value);
	do {
		if (ControlSystemData(SFIELD_SD_UNMOUNT, (void *)&value, sizeof(value)) < 0)
			break;
		req->buffer_end += sprintf(req_bufptr(req), OPTION_OK "%s\n", argm->name);
		return;
	} while (0);
	req->buffer_end += sprintf(req_bufptr(req), OPTION_NG "%s\n", argm->name);
}
/***************************************************************************
 * 090727 for old-password check                                           *
 ***************************************************************************/
void checkpassword(request *req, COMMAND_ARGUMENT *argm)
{
	int user_no;
	char *user_name, *user_passwd;

	do {
		if ((user_name = index(argm->value, ':')) == NULL)
			break;
		*user_name++ = '\0';
		user_no = atoi(argm->value);
		//*--user_name = '.';
		if ((user_passwd = index(user_name, ':')) == NULL)
			break;
		*user_passwd++ = '\0';
		//fprintf(stderr,"\nNo:%2d Name:%s Password:%s\n", user_no, user_name, user_passwd);
		SysInfo* pSysInfo = GetSysInfo();
		if(pSysInfo == NULL) break; //pSysInfo->acounts[user_no].password
		if (strcmp(user_name, pSysInfo->acounts[user_no].user) != 0) break;
		if (strcmp(user_passwd, pSysInfo->acounts[user_no].password) != 0) break;
		//fprintf(stderr,"\nPassword OK!\n");
		req->buffer_end += sprintf(req_bufptr(req), OPTION_OK "%s\n", argm->name);
		return;
	} while (0);
	//fprintf(stderr,"\nPassword WRONG!\n");
	req->buffer_end += sprintf(req_bufptr(req), OPTION_NG "%s\n", argm->name);
}
/***************************************************************************
 *                                                                         *
 ***************************************************************************/
void set_multicast(request *req, COMMAND_ARGUMENT *argm)
{
	__u8 value = atoi(argm->value);

	do {
		ControlSystemData(SFIELD_SET_MULTICAST, (void *)&value, sizeof(value));
		req->buffer_end += sprintf(req_bufptr(req), OPTION_OK "%s\n", argm->name);
		return;
	} while (0);
	req->buffer_end += sprintf(req_bufptr(req), OPTION_NG "%s\n", argm->name);
}
/***************************************************************************
 *                                                                         *
 ***************************************************************************/
//2010-09-20

void set_ftpswitch(request *req, COMMAND_ARGUMENT *argm)
{
	__u8 value = atoi(argm->value); 
	//printf("%s:%d\n",argm->name,value);
}
void set_smtpswitch(request *req, COMMAND_ARGUMENT *argm)
{
	__u8 value = atoi(argm->value); 
	//printf("%s:%d\n",argm->name,value);
}
void set_ntpswitch(request *req, COMMAND_ARGUMENT *argm)
{
	__u8 value = atoi(argm->value); 
	//printf("%s:%d\n",argm->name,value);
}
void set_rtspswitch(request *req, COMMAND_ARGUMENT *argm)
{
	__u8 value = atoi(argm->value); 
	//printf("%s:%d\n",argm->name,value);
}
void set_h2641(request *req, COMMAND_ARGUMENT *argm)
{
	__u8 value = atoi(argm->value); 
	//printf("%d\n",value);
}
void set_h2642(request *req, COMMAND_ARGUMENT *argm)
{
	__u8 value = atoi(argm->value); 
	//printf("%d\n",value);
}
void set_mpeg41(request *req, COMMAND_ARGUMENT *argm)
{
	__u8 value = atoi(argm->value); 
	//printf("%d\n",value);
}
void set_mpeg42(request *req, COMMAND_ARGUMENT *argm)
{
	__u8 value = atoi(argm->value); 
	//printf("%d\n",value);
}
void set_jpeg(request *req, COMMAND_ARGUMENT *argm)
{
	__u8 value = atoi(argm->value); 
	//printf("%d\n",value);
}
void set_input(request *req, COMMAND_ARGUMENT *argm)
{
	__u8 value = atoi(argm->value); 
	//printf("%d\n",value);
}
void set_yellow_line(request *req, COMMAND_ARGUMENT *argm)
{
	__u8 value = atoi(argm->value); 
	//printf("%d\n",value);
}
void set_red(request *req, COMMAND_ARGUMENT *argm)
{
	__u8 value = atoi(argm->value); 
	//printf("%d\n",value);
}
void set_superspeed(request *req, COMMAND_ARGUMENT *argm)
{
	__u8 value = atoi(argm->value); 
	//printf("%d\n",value);
}
void set_di_do_action(request *req, COMMAND_ARGUMENT *argm)
{
	__u8 value = atoi(argm->value); 
	//printf("%d\n",value);
}
void set_yellow_do_action(request *req, COMMAND_ARGUMENT *argm)
{
	__u8 value = atoi(argm->value); 
	//printf("%d\n",value);
}
void set_red_do_action(request *req, COMMAND_ARGUMENT *argm)
{
	__u8 value = atoi(argm->value); 
	//printf("%d\n",value);
}
void set_sup_do_action(request *req, COMMAND_ARGUMENT *argm)
{
	__u8 value = atoi(argm->value); 
	//printf("%d\n",value);
}
void set_di_smtp_action(request *req, COMMAND_ARGUMENT *argm)
{
	__u8 value = atoi(argm->value); 
	//printf("%d\n",value);
}
void set_yellow_smtp_action(request *req, COMMAND_ARGUMENT *argm)
{
	__u8 value = atoi(argm->value); 
	//printf("%d\n",value);
}
void set_red_smtp_action(request *req, COMMAND_ARGUMENT *argm)
{
	__u8 value = atoi(argm->value); 
	//printf("%d\n",value);
}
void set_sup_smtp_action(request *req, COMMAND_ARGUMENT *argm)
{
	__u8 value = atoi(argm->value); 
	//printf("%d\n",value);
}

void set_di_record_action(request *req, COMMAND_ARGUMENT *argm)
{
	__u8 value = atoi(argm->value); 
	//printf("%d\n",value);
}
void set_yellow_record_action(request *req, COMMAND_ARGUMENT *argm)
{
	__u8 value = atoi(argm->value); 
	//printf("%d\n",value);
}
void set_red_record_action(request *req, COMMAND_ARGUMENT *argm)
{
	__u8 value = atoi(argm->value); 
	//printf("%d\n",value);
}
void set_sup_record_action(request *req, COMMAND_ARGUMENT *argm)
{
	__u8 value = atoi(argm->value); 
	//printf("%d\n",value);
}
void set_storageswitch(request *req, COMMAND_ARGUMENT *argm)
{
	__u8 value = atoi(argm->value); 
	//printf("%d\n",value);
}
void set_time_storage_switch(request *req, COMMAND_ARGUMENT *argm)
{
	__u8 value = atoi(argm->value); 
	//printf("%d\n",value);
}
void set_perstorage(request *req, COMMAND_ARGUMENT *argm)
{
	__u8 value = atoi(argm->value); 
	//printf("%d\n",value);
}
void set_storage_time_set(request *req, COMMAND_ARGUMENT *argm)
{
	int case_num;
	UNIX_CMD_STRUCT cmd,cmd1;
	boatoflash_para_save para_save;
	alert_event alert_event_cmd;
	alert_event_cmd.event_type = ALERT_EVENT_TYPE_TIME;
	para_save.testflag = 0xFF;
	cmd.cmd = PARACMD_BOA_BOATOFLASH_PARA_SAVE;
	cmd1.cmd = PARACMD_ALERT_ALERT_EVENT;
	cmd.cmdlen = sizeof(para_save);
	cmd1.cmdlen = sizeof(alert_event_cmd);
	memcpy(cmd1.cmdinfo, &alert_event_cmd, sizeof(alert_event_cmd));	
	memcpy(cmd.cmdinfo, &para_save, sizeof(para_save));
	cmd.ret_ack = 0;
    cmd1.ret_ack = 0;
	case_num = atoi(argm -> value);
    printf("\n\r###########argm->value = %s,storage_time_case_num= %d#############\r\n",argm->value,case_num);
    switch(case_num)
    {
        case 0:
			storage_setting.storage_pre_record_time = (5 * 60);
			break;
		case 1:
			storage_setting.storage_pre_record_time = (10 * 60);
			break;
		case 2:
			storage_setting.storage_pre_record_time = (30 * 60);
			break;
		case 3:
			storage_setting.storage_pre_record_time = (60 * 60);
			break;
		default:
			SYS_INFO("\n\r###########VIDEO_SETTING ERROR!############\r\n");
    }
	storage_setting.storage_method = STORAGE_METHOD_ALWAYS;
	printf("\n\r!!!!!!!!!!!!!!!!!!!GBSHM_PARA_GET argm->value = %s,storage_time_input = %d !!!!!!!!!!!!!!!!!!!!!!!!\r\n",argm->value,storage_setting.storage_pre_record_time);
	Unix_send(global_boa_private_handle.unix_fd, &cmd);	
	Gbshm_paramSet(global_boa_private_handle.gbshm_handle, PARA_CMD_SYSTEM_STORAGE_SETTING_PARAM, &storage_setting, sizeof(storage_setting));
    Unix_send(global_boa_private_handle.unix_fd, &cmd1);
	printf("############storage set!+++++++++++++++++");
}
void set_event_storage_switch(request *req, COMMAND_ARGUMENT *argm)
{
	int case_num;
	UNIX_CMD_STRUCT cmd;
	boatoflash_para_save para_save;
	para_save.testflag = 0xFF;
	cmd.cmd = PARACMD_BOA_BOATOFLASH_PARA_SAVE;
	cmd.cmdlen = sizeof(para_save);
	memcpy(cmd.cmdinfo, &para_save, sizeof(para_save));
	cmd.ret_ack = 0;

	case_num = atoi(argm -> value);
    printf("\n\r###########argm->value = %s,storage_case_num= %d#############\r\n",argm->value,case_num);
    switch(case_num)
    {
        case 0:
			storage_setting.storage_switch = STORAGE_SWITCH_OFF;
			break;
		case 1:
			storage_setting.storage_switch = STORAGE_SWITCH_ON;
			break;
		default:
			SYS_INFO("\n\r###########VIDEO_SETTING ERROR!############\r\n");
    }
	printf("\n\r!!!!!!!!!!!!!!!!!!!GBSHM_PARA_GET argm->value = %s,storage_input = %d !!!!!!!!!!!!!!!!!!!!!!!!\r\n",argm->value,storage_setting.storage_switch);
	Unix_send(global_boa_private_handle.unix_fd, &cmd);	
}
void set_sch_day(request *req, COMMAND_ARGUMENT *argm)
{
	__u8 value = atoi(argm->value); 
	//printf("%d\n",value);
}

void set_audiosource(request *req, COMMAND_ARGUMENT *argm)
{
    int case_num;
	UNIX_CMD_STRUCT cmd;
	boatoflash_para_save para_save;
	para_save.testflag = 0xFF;
	cmd.cmd = PARACMD_BOA_BOATOFLASH_PARA_SAVE;
	cmd.cmdlen = sizeof(para_save);
	memcpy(cmd.cmdinfo, &para_save, sizeof(para_save));
	cmd.ret_ack = 0;
//	videoinputmodetype inputmodetype;
	case_num = atoi(argm -> value);
    printf("\n\r###########argm->value = %s,case_num= %d#############\r\n",argm->value,case_num);
    switch(case_num)
    {
        case 0:
			audio_setting.audio_mode = AUDIO_MODE_AMR_G711;
			break;
		case 1:
			audio_setting.audio_mode = AUDIO_MODE_AAC_G711;
			break;
		default:
			SYS_INFO("\n\r###########VIDEO_SETTING ERROR!############\r\n");
    }
	printf("\n\r!!!!!!!!!!!!!!!!!!!GBSHM_PARA_GET argm->value = %s,audio_input = %d !!!!!!!!!!!!!!!!!!!!!!!!\r\n",argm->value,audio_setting.audio_mode);
//	inputmodetype = (int)video_setting.video_input_mode;
//	cmd.cmd = PARACMD_BOA_BOATOCORE_PARA_SETVIDEOINPUTMODE;
//	cmd.cmdlen = sizeof(inputmodetype);
//	memcpy(cmd.cmdinfo, &inputmodetype, sizeof(inputmodetype));
//	cmd.ret_ack = 0;
//	UNIX_SEND(global_boa_private_handle.unix_fd, &cmd);	
	Unix_send(global_boa_private_handle.unix_fd, &cmd);	
}

void set_center_ip(request *req, COMMAND_ARGUMENT *argm)
{
	UNIX_CMD_STRUCT cmd;
	boatoflash_para_save para_save;
	NET_IP_STR ipaddr;
	
	para_save.testflag = 0xFF;
	cmd.cmd = PARACMD_BOA_BOATOFLASH_PARA_SAVE;
	cmd.cmdlen = sizeof(para_save);
	memcpy(cmd.cmdinfo, &para_save, sizeof(para_save));
	cmd.ret_ack = 0;
    if ( strchr(argm -> value, '.') == NULL )
    {
		NET_IPV4 localnet;
		memset(&localnet, 0x00, sizeof(localnet));
		if (netsplit(argm->value, &localnet) == 0)
		{
    	//if(ipstrsplit(argm->value,&ipaddr) == 0)
              SYS_INFO("\n\r#########ERROR##########\r\n");
		}
		//sprintf(net_setting.center_ipaddr,"%s",ipaddr.str);
		sprintf(net_setting.center_ipaddr,"%d.%d.%d.%d",localnet.str[0], localnet.str[1],localnet.str[2],localnet.str[3]);
		SYS_INFO("\n\r!!!!!!!!!!!!!!!!!!!GBSHM_PARA_GET argm->value = %s,center.ip = %s !!!!!!!!!!!!!!!!!!!!!!!!\r\n",argm->value,net_setting.center_ipaddr);
	}
	else
		sprintf(net_setting.center_ipaddr,argm -> value,strlen(argm -> value));
//	UNIX_SEND(global_boa_private_handle.unix_fd, &cmd);	
}

void set_center_port(request *req, COMMAND_ARGUMENT *argm)
{
	UNIX_CMD_STRUCT cmd;
	boatoflash_para_save para_save;
	
	para_save.testflag = 0xFF;
	cmd.cmd = PARACMD_BOA_BOATOFLASH_PARA_SAVE;
	cmd.cmdlen = sizeof(para_save);
	memcpy(cmd.cmdinfo, &para_save, sizeof(para_save));
	cmd.ret_ack = 0;

    SYS_INFO("\n\r###########argm->value = %s#############\r\n",argm->value);

    net_setting.center_port = atoi(argm -> value);
	SYS_INFO("\n\r!!!!!!!!!!!!!!!!!!!GBSHM_PARA_GET argm->value = %s,centerport = %d !!!!!!!!!!!!!!!!!!!!!!!!\r\n",argm->value,net_setting.center_port);
//	UNIX_SEND(global_boa_private_handle.unix_fd, &cmd);
}


void set_center_id(request *req, COMMAND_ARGUMENT *argm)
{
	UNIX_CMD_STRUCT cmd;
	boatoflash_para_save para_save;
	
	para_save.testflag = 0xFF;
	cmd.cmd = PARACMD_BOA_BOATOFLASH_PARA_SAVE;
	cmd.cmdlen = sizeof(para_save);
	memcpy(cmd.cmdinfo, &para_save, sizeof(para_save));
	cmd.ret_ack = 0;

    SYS_INFO("\n\r###########argm->value = %s#############\r\n",argm->value);

    sprintf(net_setting.center_loginid,argm -> value,strlen(argm -> value));
	SYS_INFO("\n\r!!!!!!!!!!!!!!!!!!!GBSHM_PARA_GET argm->value = %s,centerport = %d !!!!!!!!!!!!!!!!!!!!!!!!\r\n",argm->value,net_setting.center_port);
	Gbshm_paramSet(global_boa_private_handle.gbshm_handle, PARA_CMD_SYSTEM_NET_PARAM, &net_setting, sizeof(net_setting));
	Unix_send(global_boa_private_handle.unix_fd, &cmd);
	printf("\n\r\###################UNIX_SEND!!####################r\n");

}



#define HASH_TABLE_SIZE	(sizeof(HttpOptionTable)/sizeof(HTTP_OPTION))
HTTP_OPTION HttpOptionTable [] =
{
	{ "dhcp"				, set_dhcpclient			, AUTHORITY_OPERATOR, FALSE,  TRUE, NULL },
	{ "internetip"			, set_netip					, AUTHORITY_OPERATOR, FALSE,  TRUE, NULL },
	{ "subnetmask"			, set_netmask				, AUTHORITY_OPERATOR, FALSE,  TRUE, NULL },
	{ "dnsip"				, set_dnsip					, AUTHORITY_OPERATOR, FALSE,  TRUE, NULL },
	{ "gateway"				, set_gateway				, AUTHORITY_OPERATOR, FALSE,  TRUE, NULL },
	{ "httpport"			, set_httpport				, AUTHORITY_OPERATOR, FALSE,  TRUE, NULL },
	{ "sntpfqdn"			, set_sntpfqdn				, AUTHORITY_OPERATOR, FALSE,  TRUE, NULL },
	{ "newdate"				, set_system_date			, AUTHORITY_OPERATOR, FALSE,  TRUE, NULL },
	{ "newtime"				, set_system_time			, AUTHORITY_OPERATOR, FALSE,  TRUE, NULL },
	{ "setdaylight"			, set_daylight				, AUTHORITY_OPERATOR, FALSE,  TRUE, NULL },
	{ "timezone"			, set_timezone				, AUTHORITY_OPERATOR, FALSE,  TRUE, NULL },
	{ "timefrequency"		, set_time_frequency		, AUTHORITY_OPERATOR, FALSE,  TRUE, NULL },
	{ "timeformat"			, set_timeformat			, AUTHORITY_OPERATOR, FALSE,  TRUE, NULL },
	{ "colorkiller"			, set_day_night				, AUTHORITY_OPERATOR, FALSE,  TRUE, NULL },
	//	{ "awb"					, set_white_balance			, AUTHORITY_OPERATOR, FALSE,  TRUE, NULL },
	{ "tvcable"				, set_tvcable				, AUTHORITY_OPERATOR, FALSE,  TRUE, NULL },
	{ "binning"				, set_binning				, AUTHORITY_OPERATOR, FALSE,  TRUE, NULL },
	{ "setblc"				, set_blc					, AUTHORITY_OPERATOR, FALSE,  TRUE, NULL },
	{ "setbacklight"		, set_backlight				, AUTHORITY_OPERATOR, FALSE,  TRUE, NULL },
	{ "brightness"			, set_brightness			, AUTHORITY_OPERATOR, FALSE,  TRUE, NULL },
	{ "contrast"			, set_contrast				, AUTHORITY_OPERATOR, FALSE,  TRUE, NULL },
	{ "saturation"			, set_saturation			, AUTHORITY_OPERATOR, FALSE,  TRUE, NULL },
	{ "sharpness"			, set_sharpness				, AUTHORITY_OPERATOR, FALSE,  TRUE, NULL },
	{ "quality"				, set_jpgquality			, AUTHORITY_OPERATOR, FALSE,  TRUE, NULL },
	{ "cameratitle"			, set_cameratitle			, AUTHORITY_OPERATOR, FALSE,  TRUE, NULL },
	//	{ "videomode"			, videomode					, AUTHORITY_VIEWER, FALSE,  TRUE, NULL },
	//	{ "getvideomode"		, get_videomode				, AUTHORITY_VIEWER, FALSE,  TRUE, NULL },
	{ "imageformat"			, set_imageformat			, AUTHORITY_OPERATOR, FALSE,  TRUE, NULL },
	{ "resolution"			, set_resolution			, AUTHORITY_OPERATOR, FALSE,  TRUE, NULL },
	{ "mpeg4resolution"		, set_mpeg4resolution		, AUTHORITY_OPERATOR, FALSE,  TRUE, NULL },
	{ "mpeg42resolution"	, set_mpeg42resolution		, AUTHORITY_OPERATOR, FALSE,  TRUE, NULL },
	{ "schedule"			, set_schedule				, AUTHORITY_OPERATOR, FALSE,  TRUE, NULL },
	{ "setsdaenable"		, set_sdaenable				, AUTHORITY_OPERATOR, FALSE,  TRUE, NULL },
	{ "aviduration"		     , set_aviduration			, AUTHORITY_OPERATOR, FALSE,  TRUE, NULL },
	{ "aviformat"		     , set_aviformat			, AUTHORITY_OPERATOR, FALSE,  TRUE, NULL },
	{ "ftpfileformat"		 , set_ftpfileformat			, AUTHORITY_OPERATOR, FALSE,  TRUE, NULL },
	{ "sdfileformat"	     , set_sdfileformat		    	, AUTHORITY_OPERATOR, FALSE,  TRUE, NULL },
	{ "attfileformat"	     , set_attfileformat		, AUTHORITY_OPERATOR, FALSE,  TRUE, NULL },

	{ "audioenable"		     , set_audioenable			, AUTHORITY_OPERATOR, FALSE,  TRUE, NULL },
	{ "getaudioenable"       , get_audioenable          , AUTHORITY_VIEWER, FALSE,  TRUE, NULL },

	{ "ratecontrol"			, set_ratecontrol			, AUTHORITY_OPERATOR, FALSE,  TRUE, NULL },
	{ "jpegframerate"		, set_jpegframerate			, AUTHORITY_OPERATOR, FALSE,  TRUE, NULL },
	{ "mpeg4framerate"		, set_mpeg41framerate		, AUTHORITY_OPERATOR, FALSE,  TRUE, NULL },
	{ "mpeg42framerate"		, set_mpeg42framerate		, AUTHORITY_OPERATOR, FALSE,  TRUE, NULL },
	//{ "videocodecmode"		, set_videocodecmode		, AUTHORITY_OPERATOR, FALSE,  TRUE, NULL },
	//{ "videocodecres"		, set_videocodecres			, AUTHORITY_OPERATOR, FALSE,  TRUE, NULL },
	{ "codecnumber"		    , set_videocodecNumber		, AUTHORITY_OPERATOR, FALSE,  TRUE, NULL },
	{ "codecmenu"		    , set_videocodecMenu		, AUTHORITY_OPERATOR, FALSE,  TRUE, NULL },
	{ "resmenu"		        , set_videoresmenu			, AUTHORITY_OPERATOR, FALSE,  TRUE, NULL },
	{ "framemenu"		    , set_videoframemenu			, AUTHORITY_OPERATOR, FALSE,  TRUE, NULL },
	{ "videokeyframe"		, set_videokeyframe			, AUTHORITY_OPERATOR, FALSE,  TRUE, NULL },
	{ "setimagesource"		, set_imagesource			, AUTHORITY_OPERATOR, FALSE,  TRUE, NULL },
	{ "img2a"				, set_img2a					, AUTHORITY_OPERATOR, FALSE,  TRUE, NULL },
	{ "img2atype"			, set_img2atype				, AUTHORITY_OPERATOR, FALSE,  TRUE, NULL },
	{ "rotctrl"				, set_rotctrl				, AUTHORITY_OPERATOR, FALSE,  TRUE, NULL },
	{ "mirctrl"				, set_mirctrl				, AUTHORITY_OPERATOR, FALSE,  TRUE, NULL },
	{ "advmode"				, set_advmode				, AUTHORITY_OPERATOR, FALSE,  TRUE, NULL },
	{ "preprocess"			, set_advmodeX				, AUTHORITY_OPERATOR, FALSE,  TRUE, NULL },
	{ "noisefilt"			, set_advmodeX				, AUTHORITY_OPERATOR, FALSE,  TRUE, NULL },
	{ "m41sfeature"			, set_m41sfeature			, AUTHORITY_OPERATOR, FALSE,  TRUE, NULL },
	{ "m42sfeature"			, set_m42sfeature			, AUTHORITY_OPERATOR, FALSE,  TRUE, NULL },
	{ "jpgsfeature"			, set_jpgsfeature			, AUTHORITY_OPERATOR, FALSE,  TRUE, NULL },
	{ "fdetect"				, set_fdetect				, AUTHORITY_OPERATOR, FALSE,  TRUE, NULL },
	{ "democfg"				, set_democfg				, AUTHORITY_OPERATOR, FALSE,  TRUE, NULL },
	{ "histogram"			, set_histogram				, AUTHORITY_OPERATOR, FALSE,  TRUE, NULL },
	{ "gbce"     			, set_gbce          		, AUTHORITY_VIEWER   , NULL },
	{ "osdwin"				, set_osdwin				, AUTHORITY_OPERATOR, FALSE,  TRUE, NULL },
	{ "osdwinnum"			, set_osdwinnum				, AUTHORITY_OPERATOR, FALSE,  TRUE, NULL },
	{ "osdstream"			, set_osdstream				, AUTHORITY_OPERATOR, FALSE,  TRUE, NULL },
	{ "osdtextinfo"			, set_osdtextname			, AUTHORITY_OPERATOR, FALSE,  TRUE, NULL },
	{ "mpeg4cvalue"			, set_mpeg41bitrate			, AUTHORITY_OPERATOR, FALSE,  TRUE, NULL },
	{ "mpeg5cvalue"		    , set_mpeg42bitrate			, AUTHORITY_OPERATOR, FALSE,  TRUE, NULL },
	{ "mpeg6cvalue"		    , set_mpeg43bitrate			, AUTHORITY_OPERATOR, FALSE,  TRUE, NULL },
	{ "mpeg7cvalue"		    , set_mpeg44bitrate			, AUTHORITY_OPERATOR, FALSE,  TRUE, NULL },
	{ "imagedefault"		, set_imagedefault			, AUTHORITY_OPERATOR, FALSE,  TRUE, NULL },
	{ "setmulticast"		, set_multicast				, AUTHORITY_OPERATOR, FALSE,  TRUE, NULL },

	{ "asmtpattach"			, set_asmtpattach			, AUTHORITY_OPERATOR, FALSE,  TRUE, NULL },

	{ "rftpenable"			, set_rftpenable			, AUTHORITY_OPERATOR, FALSE,  TRUE, NULL },
	{ "setsdrenable"		, set_sdrenable				, AUTHORITY_OPERATOR, FALSE,  TRUE, NULL },
	{ "lostalarm"           , set_lostalarm 			, AUTHORITY_OPERATOR, FALSE,  TRUE, NULL },
	{ "aftpenable"          , set_aftpenable			, AUTHORITY_OPERATOR, FALSE,  TRUE, NULL },
	{ "asmtpenable"			, set_asmtpenable			, AUTHORITY_OPERATOR, FALSE,  TRUE, NULL },
	{ "alarmduration"		, set_alarmduration			, AUTHORITY_OPERATOR, FALSE,  TRUE, NULL },
	{ "motionenable"        , set_motionenable          , AUTHORITY_OPERATOR, FALSE,  TRUE, NULL },
	{ "motioncenable"       , set_motioncenable         , AUTHORITY_OPERATOR, FALSE,  TRUE, NULL },
	{ "motionlevel"         , set_motionlevel           , AUTHORITY_OPERATOR, FALSE,  TRUE, NULL },
	{ "motioncvalue"        , set_motioncvalue          , AUTHORITY_OPERATOR, FALSE,  TRUE, NULL },
	{ "motionblock"         , set_motionblock           , AUTHORITY_OPERATOR, FALSE,  TRUE, NULL },
	{ "motionxblock"		, get_motionxblock			, AUTHORITY_OPERATOR, FALSE,  TRUE, NULL },
	{ "motionyblock"		, get_motionyblock			, AUTHORITY_OPERATOR, FALSE,  TRUE, NULL },
	{ "supportmotion"		, get_supportmotion			, AUTHORITY_OPERATOR, FALSE,  TRUE, NULL },

	{ "delschedule"		    , del_schedule				, AUTHORITY_OPERATOR, FALSE,  TRUE, NULL },
	{ "paratest"            , paratest                  , AUTHORITY_OPERATOR, FALSE,  TRUE, NULL },
	{ "ipncptz"          , set_ipncptz                 , AUTHORITY_OPERATOR, FALSE,  TRUE, NULL },
	{ "ptzpopup"          , init_ipncptz                 , AUTHORITY_OPERATOR, FALSE,  TRUE, NULL },

	/// {"/ptz.htm"                 ,NULL			,AUTHORITY_OPERATOR ,URI_FLAG_NEED_PARSE   ,NULL },

	{ "getdate"			    , get_date					, AUTHORITY_OPERATOR, FALSE,  TRUE, NULL },
	{ "gettime"			    , get_time					, AUTHORITY_OPERATOR, FALSE,  TRUE, NULL },
	{ "getdhcpenable"	    , get_dhcpclient			, AUTHORITY_OPERATOR, FALSE,  TRUE, NULL },
	{ "getnetip"		    , get_netip					, AUTHORITY_OPERATOR, FALSE,  TRUE, NULL },
	{ "getnetmask"		    , get_netmask				, AUTHORITY_OPERATOR, FALSE,  TRUE, NULL },
	{ "getgateway"		    , get_gateway				, AUTHORITY_OPERATOR, FALSE,  TRUE, NULL },
	{ "getdnsip"		    , get_dnsip					, AUTHORITY_OPERATOR, FALSE,  TRUE, NULL },
	{ "gethttpport"		    , get_httpport				, AUTHORITY_OPERATOR, FALSE,  TRUE, NULL },
	{ "getsntpip"		    , get_sntpip				, AUTHORITY_OPERATOR, FALSE,  TRUE, NULL },

#ifdef ISLAB_DEBUG   //2010-09-20 23:00  ftp开关
	{ "ftpSwitch"           , set_ftpswitch             , AUTHORITY_OPERATOR, FALSE,  TRUE, NULL },
#endif
	{ "ftpfqdn"	    	    , set_ftpfqdn				, AUTHORITY_OPERATOR, FALSE,  TRUE, NULL },
	{ "ftpport"		        , set_ftpport				, AUTHORITY_OPERATOR, FALSE,  TRUE, NULL },
	{ "ftpuser"		        , set_ftpuser				, AUTHORITY_OPERATOR, FALSE,  TRUE, NULL },
	{ "ftppath"		        , set_ftppath   	        , AUTHORITY_OPERATOR, FALSE,  TRUE, NULL },
	{ "ftppassword"		    , set_ftppassword			, AUTHORITY_OPERATOR, FALSE,  TRUE, NULL },

	{ "smtpfqdn"	    	, set_smtpfqdn				, AUTHORITY_OPERATOR, FALSE,  TRUE, NULL },
	{ "smtpauth"		    , set_smtpauth				, AUTHORITY_OPERATOR, FALSE,  TRUE, NULL },
	{ "smtpsender"		    , set_smtpsender			, AUTHORITY_OPERATOR, FALSE,  TRUE, NULL },
	{ "smtpto"		        , set_smtpto				, AUTHORITY_OPERATOR, FALSE,  TRUE, NULL },
	{ "smtpuser"		    , set_smtpuser		    	, AUTHORITY_OPERATOR, FALSE,  TRUE, NULL },
	{ "smtppwd"		        , set_smtppwd			    , AUTHORITY_OPERATOR, FALSE,  TRUE, NULL },

	{ "stream1xsize"          , stream1xsize                 , AUTHORITY_VIEWER, FALSE,  TRUE, NULL },
	{ "stream1ysize"          , stream1ysize                 , AUTHORITY_VIEWER, FALSE,  TRUE, NULL },
	{ "stream2xsize"          , stream2xsize                 , AUTHORITY_VIEWER, FALSE,  TRUE, NULL },
	{ "stream2ysize"          , stream2ysize                 , AUTHORITY_VIEWER, FALSE,  TRUE, NULL },
	{ "stream3xsize"          , stream3xsize                 , AUTHORITY_VIEWER, FALSE,  TRUE, NULL },
	{ "stream3ysize"          , stream3ysize                 , AUTHORITY_VIEWER, FALSE,  TRUE, NULL },

	{ "adduser"		        , add_user		        	, AUTHORITY_ADMIN, FALSE,  TRUE, NULL },
	{ "deluser"		        , del_user	    	    	, AUTHORITY_ADMIN, FALSE,  TRUE, NULL },
	{ "checkpassword"       , checkpassword             , AUTHORITY_ADMIN, FALSE,  TRUE, NULL },
	{ "eventstart"	    	, event_start			    , AUTHORITY_OPERATOR, FALSE,  TRUE, NULL },

	{ "setgioinenable"		, set_gioinenable	     	, AUTHORITY_OPERATOR, FALSE,  TRUE, NULL },
	{ "setgiointype"		, set_giointype	        	, AUTHORITY_OPERATOR, FALSE,  TRUE, NULL },
	{ "setgiooutenable"		, set_giooutenable		  	, AUTHORITY_OPERATOR, FALSE,  TRUE, NULL },
	{ "setgioouttype"		, set_gioouttype	     	, AUTHORITY_OPERATOR, FALSE,  TRUE, NULL },

	{ "tstampenable"		, set_tstampenable	     	, AUTHORITY_OPERATOR, FALSE,  TRUE, NULL },
	{ "tstampformat"		, set_tstampformat	     	, AUTHORITY_OPERATOR, FALSE,  TRUE, NULL },
	{ "audioinvolume"		, set_audioinvolume	     	, AUTHORITY_OPERATOR, FALSE,  TRUE, NULL },
	{ "sdformat"	    	, sdformat	            	, AUTHORITY_OPERATOR, FALSE,  TRUE, NULL },
	{ "sdunmount"	    	, sd_unmount	            	, AUTHORITY_OPERATOR, FALSE,  TRUE, NULL },
	{ "language"		    , language	            	, AUTHORITY_VIEWER, FALSE,  TRUE, NULL },

	{ "ipcamrestartcmd"		, restart_ipcam				, AUTHORITY_OPERATOR, FALSE,  TRUE, NULL },
	{ "getalarmstatus"		, getalarmstatus				, AUTHORITY_VIEWER, FALSE,  TRUE, NULL },

	//2010-09-20 23:45
	{ "smtpSwitch"	    	, set_smtpswitch	     	, AUTHORITY_OPERATOR, FALSE,  TRUE, NULL }, 
	{ "ntpSwitch"	    	, set_ntpswitch	    		, AUTHORITY_OPERATOR, FALSE,  TRUE, NULL },
	{ "rtspSwitch"		    , set_rtspswitch			, AUTHORITY_OPERATOR, FALSE,  TRUE, NULL },
	{ "h2641"		        , set_h2641	     	, AUTHORITY_OPERATOR, FALSE,  TRUE, NULL },
	{ "h2642"		        , set_h2642			, AUTHORITY_OPERATOR, FALSE,  TRUE, NULL },
	{ "mpeg41"		        , set_mpeg41			, AUTHORITY_OPERATOR, FALSE,  TRUE, NULL },
	{ "mpeg42"		        , set_mpeg42			, AUTHORITY_OPERATOR, FALSE,  TRUE, NULL },
	{ "jpeg"		        , set_jpeg			, AUTHORITY_OPERATOR, FALSE,  TRUE, NULL },
	{ "input"		        , set_input			, AUTHORITY_OPERATOR, FALSE,  TRUE, NULL },
	{ "yellow_line"		    , set_yellow_line			, AUTHORITY_OPERATOR, FALSE,  TRUE, NULL },
	{ "red"		            , set_red			, AUTHORITY_OPERATOR, FALSE,  TRUE, NULL },
	{ "superspeed"		    , set_superspeed			, AUTHORITY_OPERATOR, FALSE,  TRUE, NULL },
	{ "di_do_action"		, set_di_do_action			, AUTHORITY_OPERATOR, FALSE,  TRUE, NULL },
	{ "yellow_do_action"		, set_yellow_do_action			, AUTHORITY_OPERATOR, FALSE,  TRUE, NULL },
	{ "red_do_action"		, set_red_do_action			, AUTHORITY_OPERATOR, FALSE,  TRUE, NULL },
	{ "sup_do_action"		, set_sup_do_action			, AUTHORITY_OPERATOR, FALSE,  TRUE, NULL },
	//	{ "di_smtp_action"		, set_smtp_action			, AUTHORITY_OPERATOR, FALSE,  TRUE, NULL },
	{ "yellow_smtp_action"		, set_yellow_smtp_action			, AUTHORITY_OPERATOR, FALSE,  TRUE, NULL },
	{ "red_smtp_action"		, set_red_smtp_action			, AUTHORITY_OPERATOR, FALSE,  TRUE, NULL },
	{ "sup_smtp_action"		, set_sup_smtp_action			, AUTHORITY_OPERATOR, FALSE,  TRUE, NULL },
	{ "di_record_action"	, set_di_record_action			, AUTHORITY_OPERATOR, FALSE,  TRUE, NULL },
	{ "yellow_record_action", set_yellow_record_action			, AUTHORITY_OPERATOR, FALSE,  TRUE, NULL },
	{ "red_record_action"	, set_red_record_action			, AUTHORITY_OPERATOR, FALSE,  TRUE, NULL },
	{ "sup_record_action"	, set_sup_record_action			, AUTHORITY_OPERATOR, FALSE,  TRUE, NULL },
	{ "storageswitch"		, set_storageswitch			, AUTHORITY_OPERATOR, FALSE,  TRUE, NULL },
	{ "time_storage_switch"	, set_time_storage_switch			, AUTHORITY_OPERATOR, FALSE,  TRUE, NULL },
	{ "perstorage"		    , set_perstorage			, AUTHORITY_OPERATOR, FALSE,  TRUE, NULL },
	{ "StorageTimeSet"		    , set_storage_time_set			, AUTHORITY_OPERATOR, FALSE,  TRUE, NULL },
	{ "SetEventStorageSwitch", set_event_storage_switch			, AUTHORITY_OPERATOR, FALSE,  TRUE, NULL },
	//	{ "sch_day"		        , set_schday			, AUTHORITY_OPERATOR, FALSE,  TRUE, NULL },
	{ "setaudiosource"		, set_audiosource			, AUTHORITY_OPERATOR, FALSE,  TRUE, NULL },
    { "center_ip_add"		, set_center_ip					, AUTHORITY_OPERATOR, FALSE,  TRUE, NULL },
    { "center_port"			, set_center_port					, AUTHORITY_OPERATOR, FALSE,  TRUE, NULL },
    { "center_id"			, set_center_id					, AUTHORITY_OPERATOR, FALSE,  TRUE, NULL },
};


unsigned int hash_cal_value(char *name)
{
	unsigned int value = 0;

	while (*name)
		value = value * 37 + (unsigned int)(*name++);
	return value;
}


void hash_insert_entry(CMD_HASH_TABLE *table, HTTP_OPTION *op)
{
	if (table->entry) {
		op->next = table->entry;
	}
	table->entry = op;
}

CMD_HASH_TABLE *cmd_hash;
int hash_table_init(void)
{
	int i;

	if ( (cmd_hash = (CMD_HASH_TABLE *)calloc(sizeof(CMD_HASH_TABLE), MAX_CMD_HASH_SIZE)) == NULL) {
		return -1;
	}
	for (i=0; i<HASH_TABLE_SIZE; i++) {
		hash_insert_entry(cmd_hash+(hash_cal_value(HttpOptionTable[i].name)%MAX_CMD_HASH_SIZE), HttpOptionTable+i);
	}
	dbg("HASH_TABLE_SIZE = %d\n", HASH_TABLE_SIZE);
	return 0;
}

HTTP_OPTION *http_option_search(char *arg)
{
#ifdef FY_ 
	printf("enter http_option_search ......\n");
#endif
	HTTP_OPTION *opt;

	opt = cmd_hash[hash_cal_value(arg)%MAX_CMD_HASH_SIZE].entry;

	while (opt) {
		if ( strcmp(opt->name, arg) == 0 )
			return opt;
		opt = opt->next;
	}
	return NULL;
}

void http_run_command(request *req, COMMAND_ARGUMENT *arg, int num)
{
#ifdef FY_
	printf("entering http_run_command......\n");
#endif
	AUTHORITY authority = req->authority;
	HTTP_OPTION *option;
	int i;

	send_request_ok_api(req);     /* All's well */
	dbg("argument count = %d\n", num);


	for (i=0; i<num; i++) {
		strtolower(arg[i].name);  // convert the command argument to lowcase
#ifdef FY
		printf("*********************************%s,%s\n",arg[i].name,arg[i].value);
#endif

#ifdef ISLAB_DEBUG
		dbg("arg[%d].name=%s", i, arg[i].name);
		if (arg[i].value[0])
			dbg(", value=%s\n", arg[i].value);
		else
			dbg("\n");
#endif
		option = http_option_search(arg[i].name);
		if (option) {
			if (authority <= option->authority && gf_CmdBlocking == 0) {
				arg[i].flags = 0;
				(*option->handler) (req, &arg[i]);    //islab 2010-09-19
			}
			else {
				req_write(req, OPTION_UA);
				req_write(req, arg[i].name);
				dbg("http_run_command: Permission denied!!!\n");
			}
		}
		else {
			req_write(req, OPTION_UW);
			req_write(req, arg[i].name);
		}
	}


	if ((gf_CmdBlocking = gf_Restarting) > 0)
		dbg("+++ Command Block +++++++++++\n");
}

extern int ShowAllWebValue(char *data, int max_size, AUTHORITY authority);
extern int ShowAllPara(char *data, int max_size, AUTHORITY authority);
int html_ini_cmd(AUTHORITY authority, char *addr, int max_size)
{
	int ret;

	ret = ShowAllWebValue(addr, max_size, authority);
	return ret;
}

int html_keyword_cmd(AUTHORITY authority, char *addr, int max_size)
{
	int ret;

	ret = ShowAllPara(addr, max_size, authority);
	return ret;
}

int html_cmdlist_cmd(AUTHORITY authority, char *addr, int max_size)
{
	int ret = 0,i,count = 0,size;
	HTTP_OPTION *option;
	char buf[80];
	for (i=0; i<HASH_TABLE_SIZE; i++) {
		option = &HttpOptionTable[i];
		if (authority > option->authority)
			continue;
		size = sprintf(buf, "%03d.%-25s%d\n", ++count, option->name, option->authority);
		if(ret + size + 1 > max_size){
			ret = sprintf(addr, "Not enogh size to show");
			break;
		}
		ret += sprintf(addr + ret, "%s", buf);
	}
	return ret;
}
#define MMC_NODE "/dev/mmcblk0p1"
#define MMC_PATH "/mnt/mmc/ipnc/"

int http_sdget_cmd(request *req, COMMAND_ARGUMENT *argm, char *addr, int max_size, char *sdpath)
{
	int ret = 0;

	//	dbg("name : %s \n", argm->name);
	//	dbg("value : %s \n", argm->value);

	if (argm->value != "\0")
	{
		if( CheckFileExist(MMC_PATH,argm->value) == 0 )
		{
			strcpy(sdpath, MMC_PATH);
			return 0;
		}
	}
	/*
	   if( MEM_List_files_To_html( MMC_PATH, MMC_NODE, addr, max_size) > 0 )
	   {
	   ret = strlen(addr);
	   }
	 */
	MEM_List_files_To_html( MMC_PATH, MMC_PATH, addr, max_size);
	ret = strlen(addr);

	return ret;
}

void http_sddel_cmd(request *req, COMMAND_ARGUMENT *argm)
{
	char http_ip_addr[100];

	GetIP_Addr(http_ip_addr);

	send_request_ok_api(req);

	//	dbg("http_sddel_cmd: parameter=%s, value=%s\n", argm->name, argm->value);

	if (strcmp(argm->name, "FILE") == 0)
	{
		if ( DeleteFile(MMC_PATH,argm->value) == 0 ) {
			req->buffer_end += sprintf(req_bufptr(req), OPTION_OK "%s\n", argm->name);
		}
		else {
			req->buffer_end += sprintf(req_bufptr(req), OPTION_NG "%s\n", argm->name);
		}
		req->buffer_end += sprintf(req_bufptr(req), "<HR>HTTP Server at <A HREF=\"http://%s\">ipnc</A><BR></PRE></BODY></HTML>", http_ip_addr);
	}
	else {
		req_write(req, OPTION_UW);
		req_write(req, argm->name);
	}
}



