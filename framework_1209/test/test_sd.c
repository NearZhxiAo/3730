#include "system_includes.h"
#include "system_debug.h"

#define SD_PATH "/mnt/mmc/"
#define SD_WRITTEN_NUM 100

int main(int argc ,char **argv)
{
	FILE * fpin = NULL;	
	FILE * fpout = NULL;	
	int iRet = -1;	
	char ain[SD_WRITTEN_NUM] = "";

	memset(ain, 0x01, SD_WRITTEN_NUM);
	system("echo 'Hello' ");

	fpin = fopen("/mnt/mmc/sd_test","wb+");
	if(NULL == fpin)
	{
		SYS_ERROR("fpin fopen failed !!filename = %s\r\n fpin = 0x%x\r\n","/mnt/mmc/sd_test",fpin);
		return -1;
	}
	if((iRet = fwrite(ain, 1, SD_WRITTEN_NUM, fpin)) <0)	
	{
		SYS_ERROR("fpin fwrite failed !! iRet = 0x%x\r\n",iRet);
		return -1;
	}
	SYS_WARNING("fwrite %d b\r\n",iRet);
	fclose(fpin);
	return 0;
}

