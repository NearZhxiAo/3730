#include "system_includes.h"
#include "system_debug.h"
#include "libgbshm.h"

int main(int argc, char **argv)
{
	succeed_type iret = succeed_type_failed;
	net_param net_param; 
	LIBGBSHM_OBJ *pgbshm_obj = GBSHM_INIT();
        if ( NULL == pgbshm_obj )
        {
                SYS_ERROR("GBSHM_INIT failed.\r\n");
                return -1;
        }
	int cmd = PARA_CMD_SYSTEM_NET_PARAM; 
	iret = GBSHM_PARA_GET(pgbshm_obj, cmd, &net_param, sizeof(net_param));
#if 1
	printf("net_param.ipmode=0x%x ntpaddr=%s ipaddr=%s ipmask=%s ipgateway=%s wireless_ipaddr=%s center_ipaddr=%s center_loginid=%s center_port=%d port=%d \r\n"
			,net_param.ipmode
			,net_param.ntpaddr
			,net_param.ipaddr
			,net_param.ipmask
			,net_param.ipgateway
			,net_param.wireless_ipaddr
			,net_param.center_ipaddr
			,net_param.center_loginid
			,net_param.center_port
			,net_param.port
			);
#endif

	SYS_INFO("\n\r!!!!!!!!!!net_param.ipgateway = %s!!!!!!!!!!!\r\n",net_param.ipgateway );

	return 0;
}
