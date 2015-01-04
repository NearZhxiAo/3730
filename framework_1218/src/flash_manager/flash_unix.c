#include "flash_private.h"
#include "flash_unix.h"

static int FILE_VALID 	= FILEFLAG_VALID;
static int FILE_INVALID = FILEFLAG_INVALID;

static succeed_type get_param_mount(char *param_first,char *param_second)
{
	int ret = -1;
	//SD_MOUNT_POINT sd_mount;
	memset(param_first,0x00,FILENAME_LENGTH);
	memset(param_second,0x00,FILENAME_LENGTH);

	//暂时先从文件中读取
	//ret = libmisc_getsdmountpoint(&sd_mount);
	if (ret == 0)
	{
		//sprintf(param_first,"%s%s%s",sd_mount.sd_param,param_path,PARA_FIRST_FILE_NAME);
		//sprintf(param_second,"%s%s%s",sd_mount.sd_param,param_path,PARA_SEC_FILE_NAME);
		return succeed_type_succeed;
	}
	else if(ret == -1)
	{
		//no vaild sd
		//try NAND flash
		sprintf(param_first,"%s%s",FLASH_MOUNTPOINT_1,PARA_FIRST_FILE_NAME);
		sprintf(param_second,"%s%s",FLASH_MOUNTPOINT_2,PARA_SEC_FILE_NAME);

		return succeed_type_succeed;
	}
	return succeed_type_failed;
}

succeed_type flash_para_save(void *arg)
{
	FLASH_PRIVATE_HANDLE * phandle = (FLASH_PRIVATE_HANDLE *)arg;
	FILE	*fp1,*fp2;
	int   	file1_valid = FILEFLAG_INVALID;
	int   	file2_valid = FILEFLAG_INVALID;
	int   	readstat;
	int 	iret;
	struct 	stat file1_stat;
	struct 	stat file2_stat;
	char 	param_first[FILENAME_LENGTH];
	char 	param_second[FILENAME_LENGTH];
	succeed_type ret = succeed_type_failed;

	ret = get_param_mount(param_first,param_second);
	if(ret != succeed_type_succeed)
	{
		SYS_ERROR("get param_mountpoint error\n\r");
		return succeed_type_failed;
	}

	SYS_INFO("the flash's update thread save param to file.\r\n");
	fp1 = fopen(param_first,"rb+");
	if(fp1 ==NULL)
	{
		SYS_INFO("file1 open error.\r\n");
	}
	fp2 = fopen(param_second,"rb+");
	if(fp2 == NULL)
	{
		SYS_INFO("file2 open error.\r\n");
	}

	iret = fread(&readstat,4,1,fp1);
	if(iret != 1)
	{
		SYS_INFO("read file1's stat error.\r\n");
		//return succeed_type_failed;
	}
	if(readstat == FILEFLAG_VALID)
	{
		file1_valid = FILEFLAG_VALID;
	}
	iret = fread(&readstat,4,1,fp2);
	if(iret != 1)
	{
		SYS_INFO("read file2's stat error.\r\n");
		//return succeed_type_failed;
	}
	if(readstat == FILEFLAG_VALID)
	{
		file2_valid = FILEFLAG_VALID;
	}
	if((file1_valid == FILEFLAG_VALID) && (file2_valid == FILEFLAG_VALID))
	{
		stat(param_first,&file1_stat);
		stat(param_second,&file2_stat);
		if(difftime(file1_stat.st_mtime,file2_stat.st_mtime)<=0)
		{
			SYS_WARNING("save param to file1.....\r\n");
			rewind(fp1);
			fwrite(&FILE_INVALID,4,1,fp1);
            phandle->gbshm_handle->gbshm_mmap;
			fwrite((void*)(phandle->gbshm_handle)->gbshm_mmap,phandle->savesize,1,fp1);
			rewind(fp1);
			fwrite(&FILE_VALID,4,1,fp1);

			//2011-05-14
			SYS_WARNING("save param to file12.....\r\n");
			rewind(fp2);
			fwrite(&FILE_INVALID,4,1,fp2);
			fwrite((void*)phandle->gbshm_handle->gbshm_mmap,phandle->savesize,1,fp2);
			rewind(fp2);
			fwrite(&FILE_VALID,4,1,fp2);

			fclose(fp1);
			fclose(fp2);
			clear_saveparam_flag(phandle);
		}
		else
		{
			SYS_WARNING("save param to file2.....\r\n");
			rewind(fp2);
			fwrite(&FILE_INVALID,4,1,fp2);
			fwrite((void*)phandle->gbshm_handle->gbshm_mmap,phandle->savesize,1,fp2);
			rewind(fp2);
			fwrite(&FILE_VALID,4,1,fp2);

			//2011-05-14
			SYS_WARNING("save param to file1.....\r\n");
			rewind(fp1);
			fwrite(&FILE_INVALID,4,1,fp1);
			fwrite((void*)phandle->gbshm_handle->gbshm_mmap,phandle->savesize,1,fp1);
			rewind(fp1);
			fwrite(&FILE_VALID,4,1,fp1);

			fclose(fp1);
			fclose(fp2);
			clear_saveparam_flag(phandle);

		}
	}
	if((file1_valid == FILEFLAG_INVALID) && (file2_valid == FILEFLAG_INVALID))
	{
		SYS_INFO("save param to file1.....\r\n");
		rewind(fp1);
		fwrite(&FILE_INVALID,4,1,fp1);
		fwrite((void*)phandle->gbshm_handle->gbshm_mmap,phandle->savesize,1,fp1);
		rewind(fp1);
		fwrite(&FILE_VALID,4,1,fp1);

		//2011-05-14
		rewind(fp2);
		fwrite(&FILE_INVALID,4,1,fp2);
		fwrite((void*)phandle->gbshm_handle->gbshm_mmap,phandle->savesize,1,fp2);
		rewind(fp2);
		fwrite(&FILE_VALID,4,1,fp2);

		fclose(fp1);
		fclose(fp2);
		clear_saveparam_flag(phandle);
	}
	if(file1_valid == FILEFLAG_INVALID)
	{
		SYS_INFO("save param to file1.....\r\n");
		rewind(fp1);
		fwrite(&FILE_INVALID,4,1,fp1);
		fwrite((void*)phandle->gbshm_handle->gbshm_mmap,phandle->savesize,1,fp1);
		rewind(fp1);
		fwrite(&FILE_VALID,4,1,fp1);

		//2011-05-14
		rewind(fp2);
		fwrite(&FILE_INVALID,4,1,fp2);
		fwrite((void*)phandle->gbshm_handle->gbshm_mmap,phandle->savesize,1,fp2);
		rewind(fp2);
		fwrite(&FILE_VALID,4,1,fp2);

		fclose(fp1);
		fclose(fp2);
		clear_saveparam_flag(phandle);
	}
	if(file2_valid == FILEFLAG_INVALID)
	{
		SYS_INFO("save param to file2......\r\n");
		rewind(fp2);
		fwrite(&FILE_INVALID,4,1,fp2);
		fwrite((void*)phandle->gbshm_handle->gbshm_mmap,phandle->savesize,1,fp2);
		rewind(fp2);
		fwrite(&FILE_VALID,4,1,fp2);

		//2011-05-14
		rewind(fp1);
		fwrite(&FILE_INVALID,4,1,fp1);
		fwrite((void*)phandle->gbshm_handle->gbshm_mmap,phandle->savesize,1,fp1);
		rewind(fp1);
		fwrite(&FILE_VALID,4,1,fp1);

		fclose(fp1);
		fclose(fp2);
		clear_saveparam_flag(phandle);
	}
	//return succeed_type_failed;
	return succeed_type_succeed;
}


succeed_type flash_para_load(void *arg)
{
	FLASH_PRIVATE_HANDLE * phandle = (FLASH_PRIVATE_HANDLE *)arg;
	FILE    *fp1 = NULL;
	FILE    *fp2 = NULL;
	char    param_first[FILENAME_LENGTH];
	char    param_second[FILENAME_LENGTH];
	int     file1_valid = FILEFLAG_INVALID;
	int     file2_valid = FILEFLAG_INVALID;
	struct  stat file1_stat;
	struct  stat file2_stat;
	int     readstat;
	succeed_type ret = succeed_type_failed;

	//获取文件1,2的文件名
	ret = get_param_mount(param_first,param_second);
	if(ret != succeed_type_succeed)
	{
		SYS_ERROR("get param_mountpoint error\n\r");
		return succeed_type_failed;
	}

	//当文件1，2都不存在的处理
	fp1 = fopen(param_first,"rb");
	fp2 = fopen(param_second,"rb");
	if((fp1 == NULL) && (fp2 == NULL))
	{
		SYS_INFO("fp1 NULL fp2 NULL........the first start.\r\n");
		flash_set_default(arg);
	}
	close(fp1);
	close(fp2);

	printf("first = %s, second = %s\r\n", param_first, param_second);
	fp1 = fopen(param_first,"rb+");
	fp2 = fopen(param_second,"rb+");
	fread(&readstat,4,1,fp1);
	if(readstat == FILEFLAG_VALID)
	{
		file1_valid = FILEFLAG_VALID;
	}
	fread(&readstat,4,1,fp2);
	if(readstat == FILEFLAG_VALID)
	{
		file2_valid = FILEFLAG_VALID;
	}
	SYS_INFO("the file1's stat is %x....the file2's stat is %x.\r\n",file1_valid,file2_valid);

	//当文件都为合法文件时 判断文件的时间 以最新的为准
	if((file1_valid == FILEFLAG_VALID) && (file2_valid == FILEFLAG_VALID))
	{
		stat(param_first,&file1_stat);
		stat(param_second,&file2_stat);
		if(difftime(file1_stat.st_mtime,file2_stat.st_mtime)>=0)
		{
			SYS_WARNING("load param from file1....\r\n");
			fread((void*)phandle->gbshm_handle->gbshm_mmap,phandle->savesize,1,fp1);
		}
		else
		{
			SYS_WARNING("load param from file2....\r\n");
			fread((void*)phandle->gbshm_handle->gbshm_mmap,phandle->savesize,1,fp2);
		}
		fclose(fp1);
		fclose(fp2);
		return succeed_type_succeed;
	}
	//当文件都为不合法文件时 从flash默认值读取
	if((file1_valid != FILEFLAG_VALID) && (file2_valid != FILEFLAG_VALID))
	{
		SYS_INFO("load default parameter....\r\n");
		fclose(fp1);
		fclose(fp2);
		do_flashpara_default(phandle->gbshm_handle);
		return succeed_type_succeed;
	}

	if(file1_valid == FILEFLAG_VALID)
	{
		SYS_INFO("load param from file1....\r\n");
		fread((void*)phandle->gbshm_handle->gbshm_mmap,phandle->savesize,1,fp1);
		fclose(fp1);
		fclose(fp2);
		return succeed_type_succeed;
	}

	if(file2_valid == FILEFLAG_VALID)
	{
		SYS_INFO("load param from file2....\r\n");
		fread((void*)phandle->gbshm_handle->gbshm_mmap,phandle->savesize,1,fp2);
		fclose(fp1);
		fclose(fp2);
		return succeed_type_succeed;
	}
	return succeed_type_failed;
}

succeed_type flash_set_default(void *arg)
{
	FLASH_PRIVATE_HANDLE * phandle = (FLASH_PRIVATE_HANDLE *)arg;
	FILE *fp1 = NULL;
	FILE *fp2 = NULL;
	char param_first[FILENAME_LENGTH];
	char param_second[FILENAME_LENGTH];

	do_flashpara_default(phandle->gbshm_handle);
	get_param_mount(param_first,param_second);

	fp1 = fopen(param_first,"wb+");
	if(fp1 == NULL)
	{
		SYS_INFO("the first parameter's file %s open error.\r\n",param_first);
		return succeed_type_failed;
	}
	rewind(fp1);
	fwrite(&FILE_INVALID,4,1,fp1);
	fwrite((void*)phandle->gbshm_handle->gbshm_mmap,phandle->savesize,1,fp1);
	rewind(fp1);
	fwrite(&FILE_VALID,4,1,fp1);
	fclose(fp1);

	fp2 = fopen(param_second,"wb+");
	if(fp2 == NULL)
	{
		SYS_INFO("the sec parameter's file %s open error.\r\n",param_second);
		return succeed_type_failed;
	}
	rewind(fp2);
	fwrite(&FILE_INVALID,4,1,fp2);
	fwrite((void*)phandle->gbshm_handle->gbshm_mmap,phandle->savesize,1,fp2);
	rewind(fp2);
	fwrite(&FILE_VALID,4,1,fp2);
	fclose(fp2);

	return succeed_type_succeed;

}

static succeed_type flash_unix_deal( FLASH_PRIVATE_HANDLE *phandle, UNIX_CMD_STRUCT *pcmd)
{
	switch(pcmd->cmd)	//根据不同的交互命令进行不同的处理
	{
#if 0
		case PARACMD_CLIENTPC_CLIENTPCTOFLASH_PARA_SETDEFAULT:
		case PARACMD_BOA_BOATOFLASH_PARA_SETDEFAULT:
			SYS_INFO("Flash recv the setdefault cmd.\r\n");
			flash_set_default(phandle);
			break;
		case PARACMD_CLIENTPC_CLIENTPCTOFLASH_PARA_SAVE:
		case PARACMD_BOA_BOATOFLASH_PARA_SAVE:
			SYS_INFO("Flash recv the save cmd.\r\n");
			set_saveparam_flag(phandle);
			break;
#endif
		default:
			SYS_INFO("FLASH RECV ERROR CMD. \r\n");
			break;
	}

	return succeed_type_succeed;
}

succeed_type flash_unix(FLASH_PRIVATE_HANDLE *phandle)
{
	unsigned char remotename[128];
	UNIX_CMD_STRUCT unix_rcv_cmd;

	memset(remotename, 0x00, 128);
	memset(&unix_rcv_cmd, 0x00, sizeof(UNIX_CMD_STRUCT));

	//1. recv the unix event
	if ( UNIX_RECV( phandle->unix_fd, remotename, &unix_rcv_cmd) == succeed_type_failed )
	{
		SYS_ERROR("UNIX RECV failed.\r\n");
		return succeed_type_failed;
	}
	SYS_INFO("Rcv Event from %s in flash.\r\n", remotename);

	//2. deal the receiving event.
	flash_unix_deal(phandle, &unix_rcv_cmd);
	return succeed_type_succeed;
}
