#include "system_includes.h"
#include "system_debug.h"
#include "libaudio.h"

int main(int argc, char **argv)
{
    LIBAUDIO_OBJ *p = NULL;

    int num;
    int len;
    unsigned char *inbuf;
    int j;

    p = AUDIO_INIT(AUDIO_MODE_WRITE);

    printf("after audio-init.\r\n");
    
    inbuf =(unsigned char *)malloc(160);
    if(inbuf == NULL)
    {
	    SYS_ERROR("malloc failed.\r\n");
	    return -1;
    }

    memset(inbuf, 0x00, 160);   

    printf("after malloc.\r\n"); 
    
    FILE *fp = NULL;
    fp = fopen("b.g711", "rb+");
 
    fseek(fp, 0, SEEK_END);
    len = ftell(fp);
    fseek(fp, 0, SEEK_SET);
    num = (int)(len/160);

    printf("before for().\r\n");
    for(j = 0; j<num; j++)
    {
	    if(fread(inbuf, 160, 1, fp) != 1)
	    {
		    SYS_ERROR("fread failed.\r\n");
		    return -1;
	    }   

	    if(AUDIO_WRITE(p, inbuf, AUDIO_A711,160) !=succeed_type_succeed)
	    {
		    SYS_ERROR("AUDIO_WRITE failed.\r\n");
		    return -1;
	    }
    } 

    fclose(fp);
    AUDIO_DESTROY(p, AUDIO_MODE_WRITE);
    return 0;
}

