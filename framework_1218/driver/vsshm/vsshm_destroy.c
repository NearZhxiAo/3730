#include "system_includes.h"
#include "system_debug.h"
#include "parameter_offset.h"
#include "libvsshm.h"

int main(int argc, char **argv)
{
	LIBVSSHM_OBJ *p = VSSHM_INIT();
	if(NULL == p)
	{
		SYS_INFO("VSSHM_INIT\r\n");
	}
#if 0
	LIBVSSHM_UINT_STRUCT stvsshm_write;
	LIBVSSHM_UINT_STRUCT stvsshm_read;

	if (p == NULL)
	{
		SYS_ERROR("libvsshm_init failed.\r\n");
		return 0;
	}
	SYS_INFO("libvsshm_init ok.\r\n");

	memset(&stvsshm_write, 0x00, sizeof(stvsshm_write));
	memset(&stvsshm_read, 0x00, sizeof(stvsshm_read));

	stvsshm_write.vsshm_cmd = 0x00; 
	time(&stvsshm_write.vsshm_time_t);
	strcpy(stvsshm_write.vsshm_description, "This is vsshm0 test!!");
	VSSHM_WRITE(p, &stvsshm_write, sizeof(stvsshm_write));

	stvsshm_write.vsshm_cmd = 0x01; 
	time(&stvsshm_write.vsshm_time_t);
	strcpy(stvsshm_write.vsshm_description, "This is vsshm1 test!!");
	VSSHM_WRITE(p, &stvsshm_write, sizeof(stvsshm_write));

	stvsshm_write.vsshm_cmd = 0x02; 
	time(&stvsshm_write.vsshm_time_t);
	strcpy(stvsshm_write.vsshm_description, "This is vsshm2 test!!");
	VSSHM_WRITE(p, &stvsshm_write, sizeof(stvsshm_write));

	stvsshm_write.vsshm_cmd = 0x03; 
	time(&stvsshm_write.vsshm_time_t);
	strcpy(stvsshm_write.vsshm_description, "This is vsshm3 test!!");
	VSSHM_WRITE(p, &stvsshm_write, sizeof(stvsshm_write));

	stvsshm_write.vsshm_cmd = 0x04; 
	time(&stvsshm_write.vsshm_time_t);
	strcpy(stvsshm_write.vsshm_description, "This is vsshm4 test!!");
	VSSHM_WRITE(p, &stvsshm_write, sizeof(stvsshm_write));

	if(VSSHM_READ(p, 0, &stvsshm_read, sizeof(stvsshm_read)) == succeed_type_succeed){
		SYS_INFO("stvsshm[0].vsshm_cmd = %d ", stvsshm_read.vsshm_cmd);
		SYS_INFO("stvsshm[0].vsshm_time_t = %s ", ctime(&stvsshm_read.vsshm_time_t));
		SYS_INFO("stvsshm[0].vsshm_description = %s", stvsshm_read.vsshm_description);
	}
	if(VSSHM_READ(p, 1, &stvsshm_read, sizeof(stvsshm_read)) == succeed_type_succeed){
		SYS_INFO("stvsshm[1].vsshm_cmd = %d ", stvsshm_read.vsshm_cmd);
		SYS_INFO("stvsshm[1].vsshm_time_t = %s ", ctime(&stvsshm_read.vsshm_time_t));
		SYS_INFO("stvsshm[1].vsshm_description = %s", stvsshm_read.vsshm_description);
	}
	if(VSSHM_READ(p, 2, &stvsshm_read, sizeof(stvsshm_read)) == succeed_type_succeed){
		SYS_INFO("stvsshm[2].vsshm_cmd = %d ", stvsshm_read.vsshm_cmd);
		SYS_INFO("stvsshm[2].vsshm_time_t = %s ", ctime(&stvsshm_read.vsshm_time_t));
		SYS_INFO("stvsshm[2].vsshm_description = %s", stvsshm_read.vsshm_description);
	}
	if(VSSHM_READ(p, 3, &stvsshm_read, sizeof(stvsshm_read)) == succeed_type_succeed){
		SYS_INFO("stvsshm[3].vsshm_cmd = %d ", stvsshm_read.vsshm_cmd);
		SYS_INFO("stvsshm[3].vsshm_time_t = %s ", ctime(&stvsshm_read.vsshm_time_t));
		SYS_INFO("stvsshm[3].vsshm_description = %s", stvsshm_read.vsshm_description);
	}
	if(VSSHM_READ(p, 4, &stvsshm_read, sizeof(stvsshm_read)) == succeed_type_succeed){
		SYS_INFO("stvsshm[4].vsshm_cmd = %d ", stvsshm_read.vsshm_cmd);
		SYS_INFO("stvsshm[4].vsshm_time_t = %s ", ctime(&stvsshm_read.vsshm_time_t));
		SYS_INFO("stvsshm[4].vsshm_description = %s", stvsshm_read.vsshm_description);
	}
#endif
	VSSHM_MAGIC_DESTROY(p);
	//VSSHM_DESTROY(p);
	return 0;
}
