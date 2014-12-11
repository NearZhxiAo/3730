#include "system_includes.h"
#include "system_debug.h"
#include "parameter_offset.default"

int main(int argc, char **argv)
{
    //int cmd = PARA_CMD_SYSTEM_GBSHM_MUTEX; 
    //int cmd = PARA_CMD_SYSTEM_TEST_VAR; 
    //int cmd = PARA_CMD_SYSTEM_VIDEO_ENC_PARAM; 
    int cmd = PARA_CMD_SYSTEM_NET_PARAM; 
    

    SYS_INFO("cmd index = %d.\r\n", parameter_indexfind(cmd));
    SYS_INFO("cmd argument count = %d.\r\n", parameter_argument_count(cmd));
    SYS_INFO("cmd offset = %d.\r\n", parameter_offset(cmd));
    SYS_INFO("cmd size = %d.\r\n", parameter_size(cmd));
    return 0;
}
