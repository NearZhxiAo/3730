#include "system_includes.h"
#include "system_debug.h"
#include "sys/vfs.h"

#define SD_PATH "/mnt/mmc/"
#define SD_WRITTEN_NUM 100

long long GetDiskfreeSpace(char *pDisk)
{
	long long freespace = 0;
	struct statfs disk_statfs;
	if(statfs(pDisk, &disk_statfs) >= 0)
	{
		freespace = (((long long)disk_statfs.f_bsize * (long long)disk_statfs.f_bfree)/(long long)1024);
	}
	return freespace;
}

int main(int argc ,char **argv)
{
	long long freespace = -1;
	freespace = GetDiskfreeSpace(SD_PATH);
	SYS_WARNING("GetDiskfreeSpace: %lld \r\n",freespace);
	return 0;
}

