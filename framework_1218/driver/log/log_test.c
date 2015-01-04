#include "system_includes.h"
#include "system_debug.h"
#include "parameter_offset.h"
#include "liblog.h"

#define SYS_INFO printf
int main(int argc, char **argv)
{
	LIBLOG_OBJ *p = LOG_INIT();
	LIBLOG_UINT_STRUCT stlog_write;
	LIBLOG_UINT_STRUCT stlog_read;

	SYS_INFO("log_test start.\r\n");
	if (p == NULL)
	{
		SYS_ERROR("liblog_init failed.\r\n");
		return 0;
	}
	SYS_INFO("liblog_init ok.\r\n");

	memset(&stlog_write, 0x00, sizeof(stlog_write));
	memset(&stlog_read, 0x00, sizeof(stlog_read));

	stlog_write.log_cmd = 0x00; 
	time(&stlog_write.log_time_t);
	strcpy(stlog_write.log_description, "\r\nThis is log0 test!!\r\n");
	LOG_WRITE(p, &stlog_write, sizeof(stlog_write));

	stlog_write.log_cmd = 0x01; 
	time(&stlog_write.log_time_t);
	strcpy(stlog_write.log_description, "\r\nThis is log1 test!!\r\n");
	LOG_WRITE(p, &stlog_write, sizeof(stlog_write));

	stlog_write.log_cmd = 0x02; 
	time(&stlog_write.log_time_t);
	strcpy(stlog_write.log_description, "\r\nThis is log2 test!!\r\n");
	LOG_WRITE(p, &stlog_write, sizeof(stlog_write));

	stlog_write.log_cmd = 0x03; 
	time(&stlog_write.log_time_t);
	strcpy(stlog_write.log_description, "\r\nThis is log3 test!!\r\n");
	LOG_WRITE(p, &stlog_write, sizeof(stlog_write));

	stlog_write.log_cmd = 0x04; 
	time(&stlog_write.log_time_t);
	strcpy(stlog_write.log_description, "\r\nThis is log4 test!!\r\n");
	LOG_WRITE(p, &stlog_write, sizeof(stlog_write));

	if(LOG_READ(p, 0, &stlog_read, sizeof(stlog_read)) == succeed_type_succeed){
		SYS_INFO("stlog[0].log_cmd = %d \r\n", stlog_read.log_cmd);
		SYS_INFO("stlog[0].log_time_t = %s \r\n", ctime(&stlog_read.log_time_t));
		SYS_INFO("stlog[0].log_description = %s\r\n", stlog_read.log_description);
	}
	if(LOG_READ(p, 1, &stlog_read, sizeof(stlog_read)) == succeed_type_succeed){
		SYS_INFO("stlog[1].log_cmd = %d \r\n", stlog_read.log_cmd);
		SYS_INFO("stlog[1].log_time_t = %s \r\n", ctime(&stlog_read.log_time_t));
		SYS_INFO("stlog[1].log_description = %s\r\n", stlog_read.log_description);
	}
	if(LOG_READ(p, 2, &stlog_read, sizeof(stlog_read)) == succeed_type_succeed){
		SYS_INFO("stlog[2].log_cmd = %d \r\n", stlog_read.log_cmd);
		SYS_INFO("stlog[2].log_time_t = %s \r\n", ctime(&stlog_read.log_time_t));
		SYS_INFO("stlog[2].log_description = %s\r\n", stlog_read.log_description);
	}
	if(LOG_READ(p, 3, &stlog_read, sizeof(stlog_read)) == succeed_type_succeed){
		SYS_INFO("stlog[3].log_cmd = %d \r\n", stlog_read.log_cmd);
		SYS_INFO("stlog[3].log_time_t = %s \r\n", ctime(&stlog_read.log_time_t));
		SYS_INFO("stlog[3].log_description = %s\r\n", stlog_read.log_description);
	}
	if(LOG_READ(p, 4, &stlog_read, sizeof(stlog_read)) == succeed_type_succeed){
		SYS_INFO("stlog[4].log_cmd = %d \r\n", stlog_read.log_cmd);
		SYS_INFO("stlog[4].log_time_t = %s \r\n", ctime(&stlog_read.log_time_t));
		SYS_INFO("stlog[4].log_description = %s\r\n", stlog_read.log_description);
	}
	SYS_INFO("DESTROY");
	LOG_DESTROY(p);
	return 0;
}
