#include "system_includes.h"
#include "system_debug.h"
#include "libaudio.h"

int main(int argc, char **argv)
{
	LIBAUDIO_OBJ *p = NULL;
	LIBAUDIO_OBJ *pwrite = NULL;

	unsigned char *inbuf;
	int j;

	int count = 1;
	int retlen = 0;

	struct timeval time_start;
	struct timeval time_end;
	unsigned int time;

	p = AUDIO_INIT(AUDIO_MODE_READ);
	printf("\r\n only read p = %d..\r\n", p);
//	AUDIO_DESTROY(p, AUDIO_MODE_READ);
	
	pwrite = AUDIO_INIT(AUDIO_MODE_WRITE);
	printf("pwrite = %d..\r\n", pwrite);

	inbuf =(unsigned char *)malloc(160);
	if(inbuf == NULL)
	{
		SYS_ERROR("malloc failed.\r\n");
		return -1;
	}
	memset(inbuf, 0x00, 160);   

	for(j = 0; j<2000; j++)
	{

		if(AUDIO_READ(p, inbuf, AUDIO_A711, &retlen) !=succeed_type_succeed)
		{
			SYS_ERROR("AUDIO_WRITE failed.\r\n");
			return -1;
		}
		gettimeofday(&time_start, NULL);
		if(AUDIO_WRITE(pwrite, inbuf, AUDIO_A711, retlen) !=succeed_type_succeed)
		{
			SYS_ERROR("AUDIO_WRITE failed.\r\n");
			return -1;
		}
		gettimeofday(&time_end, NULL);
		time =(time_end.tv_sec - time_start.tv_sec)*1000000 + (time_end.tv_usec - time_start.tv_usec);
	//	printf("write time = %d..\r\n", time);
	//	printf("count  = %d,,\r\n\r\n", count);
		count ++;
	} 

	AUDIO_DESTROY(p, AUDIO_MODE_READ);
	AUDIO_DESTROY(pwrite, AUDIO_MODE_WRITE);
	return 0;
}

