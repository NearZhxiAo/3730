#include "sys_env_type.h"
#include "Appro_interface.h"
static SysInfo globalinfo;
SysInfo* GetSysInfo()
{
    memset(&globalinfo, 0x00, sizeof(globalinfo));
    return &globalinfo;
}
int GetAVData( unsigned int field, int serial, AV_DATA * ptr )
{
    return 0;
}
int ControlSystemData(unsigned int field, void *data, unsigned int len)
{
    return 0;
}
int GetSysLog(int nPageNum, int nItemNum, LogEntry_t * pBuf)
{
    return 0;
}
