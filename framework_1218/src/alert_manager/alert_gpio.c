#include "alert_private.h"

static succeed_type alert_gpio_deal( ALERT_PRIVATE_HANDLE *phandle, GPIO_CMD_STRUCT *pcmd)
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

succeed_type alert_gpio(ALERT_PRIVATE_HANDLE *phandle)
{
    unsigned char remotename[128];
    GPIO_CMD_STRUCT gpio_rcv_cmd;

    memset(remotename, 0x00, 128);
    memset(&gpio_rcv_cmd, 0x00, sizeof(GPIO_CMD_STRUCT));

    //1. recv the unix event
    if ( UNIX_RECV( phandle->unix_fd, remotename, &gpio_rcv_cmd) == succeed_type_failed )
    {
	SYS_ERROR("UNIX RECV failed.\r\n");
	return succeed_type_failed;
    }
    SYS_INFO("Rcv Event from %s in alert.\r\n", remotename); 
    
    //2. deal the receiving event.
    alert_gpio_deal(phandle, &gpio_rcv_cmd);
    return succeed_type_succeed;
}
