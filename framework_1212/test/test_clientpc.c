/***************************************
Program write for test the process conmunication

Examples: test_send program produce alert_event_one, and send it out

Exactly: test_avenc and test_storage can recv this event. 

but test_rtsp can't recv this event. This is the same with the intercmd.def definition

such as: Line 41, for alert_event_one, the dest is storage and avenc

***************************************/
#include "system_includes.h"
#include "system_debug.h"
#include "libunix.h"
#include "libgbshm.h"
#include "parameter_net_param.h"
#include "parameter_offset.h"

int main(int argc, char **argv)
{
   int sockfd = 0;
   UNIX_CMD_STRUCT cmd;
   clientpctoflash_para_setdefault eventvalue;
   net_param net_param_tmp;

   LIBGBSHM_OBJ *p = GBSHM_INIT();
   memset(&net_param_tmp,0x00, sizeof(net_param_tmp));
   GBSHM_PARA_SET(p, PARA_CMD_SYSTEM_NET_PARAM, &net_param_tmp, sizeof(net_param_tmp));
   memset(&cmd, 0x00, sizeof(cmd));
   memset(&eventvalue, 0x00, sizeof(eventvalue));

   UNIX_CREATE(UNIXFILE_SOCKET_TEST, &sockfd, 0);

   cmd.cmd = PARACMD_CLIENTPC_CLIENTPCTOFLASH_PARA_SAVE;
   cmd.cmdlen = sizeof(eventvalue);
   eventvalue.testflag = 0xFF;

   memcpy(&eventvalue, cmd.cmdinfo, sizeof(eventvalue));
   SYS_INFO("PARACMD_CLIENTPC_CLIENTPCTOFLASH_PARA_SAVE been sent");
   UNIX_SEND(sockfd, &cmd);
   UNIX_DESTROY(UNIXFILE_SOCKET_TEST, sockfd);
   
   GBSHM_DESTROY(p);
   return 0;
}
