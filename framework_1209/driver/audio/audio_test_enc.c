#include "system_includes.h"
#include "system_debug.h"
#include "libaudio.h"

int main(int argc, char **argv)
{
	LIBAUDIO_OBJ *p = NULL;

	unsigned char *inbuf;
	int j;

	FILE *fp2 = NULL;
	fp2 = fopen("audio.g711", "ab+");

	int count = 1;
	int retlen = 0;

	p = AUDIO_INIT(AUDIO_MODE_READ);
	printf("p = %d..\r\n", p);

	inbuf =(unsigned char *)malloc(160);
	if(inbuf == NULL)
	{
		SYS_ERROR("malloc failed.\r\n");
		return -1;
	}
	memset(inbuf, 0x00, 160);   


	for(j = 0; j<200; j++)
	{

		if(AUDIO_READ(p, inbuf, AUDIO_A711, &retlen) !=succeed_type_succeed)
		{
			SYS_ERROR("AUDIO_WRITE failed.\r\n");
			return -1;
		}
		fwrite(inbuf, retlen, 1, fp2);

	}

	fclose(fp2);
	AUDIO_DESTROY(p, AUDIO_MODE_READ);
	return 0;
}

