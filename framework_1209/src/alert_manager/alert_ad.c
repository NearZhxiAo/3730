#include "alert_private.h"

static succeed_type alert_ad_deal( ALERT_PRIVATE_HANDLE *phandle, AD_CMD_STRUCT *pcmd)
{
   switch(pcmd->cmd)	//根据不同的交互命令进行不同的处理
   {
       case 1:
	   break;
       default:
	   break;
   }
   return succeed_type_succeed;
}

succeed_type alert_ad(ALERT_PRIVATE_HANDLE *phandle)
{
	int ad_value = -1;
#if 0
    unsigned char remotename[128];
    AD_CMD_STRUCT ad_rcv_cmd;

    memset(remotename, 0x00, 128);
    memset(&ad_rcv_cmd, 0x00, sizeof(AD_CMD_STRUCT));

    //1. recv the unix event
    if ( UNIX_RECV( phandle->unix_fd, remotename, &ad_rcv_cmd) == succeed_type_failed )
    {
	SYS_ERROR("UNIX RECV failed.\r\n");
	return succeed_type_failed;
    }
    SYS_INFO("Rcv Event from %s in alert.\r\n", remotename); 
    //2. deal the receiving event.
#endif 
	if(NULL == phandle)
	{
		SYS_ERROR("phandle = NULL!!\r\n");
		return succeed_type_failed;
	}
#if 0
	AD_READ(phandle->pad_obj,&ad_value);
	if(ad_value <= 0x300) //0x000-0x3FF
	{
		GPIO_SETOUTPUT(phandle->pgpio_obj, GPIO_OUTPUT_PWR_LED_RED);
		SYS_WARNING("Power Low!! Please Charge!!\r\n");
	}
	else 
	{
		GPIO_SETOUTPUT(phandle->pgpio_obj, GPIO_OUTPUT_PWR_LED_GREEN);
		SYS_WARNING("Fully Charging!! Please unplug the power to save energy!!\r\n");
	}
#endif
    return succeed_type_succeed;
}
