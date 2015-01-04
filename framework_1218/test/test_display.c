#include "system_includes.h"
#include "system_debug.h"
#include "libdisplay.h"
#include "libcmem.h"

#define WIDTH 720
#define HEIGHT 576



Display_Handle phandle = NULL;

void cleanup(int sig)
{
	Display_destroy(phandle);
}

int main(int argc, char **argv)
{
	DisplayBuffer buffer;
	DisplayAttr attr;
	FILE *fp = NULL;
	char *buf;
	int ret;
	int index;
	int count = 0;
	int n;

	buf = malloc(WIDTH * HEIGHT * 2);
	if(buf == NULL)
	{
		SYS_ERROR("malloc buffer error.\r\n");
		return -1;
	}

	memset(&attr, 0x00, sizeof(struct DisplayAttr));
	attr.bufNum    = 3;
	attr.userAlloc = 0;
	attr.width = WIDTH;
	attr.height = HEIGHT;
	phandle = Display_init(&attr);
	if(phandle == NULL)
	{
		SYS_ERROR("Display_init failed ..\r\n");
		return -1;
	}

	fp = fopen("transfer.yuv","rb");
	if(fp != NULL)
	{
		
		fseek(fp, WIDTH * HEIGHT * 2, SEEK_SET);
	//	n = ftell(fp) / (720*480*2);
	//	rewind(fp);

	}
	else
	{
		SYS_ERROR("open capture.yuv error.\r\n");
		return -1;
	}

	fread(buf,WIDTH * HEIGHT * 2, 1, fp);
	signal(SIGINT, cleanup);
	while(1)
	{
		SYS_INFO("count %d.\r\n",count++);
		ret = Display_get(phandle, &buffer);	
		if(ret < 0)
		{
			SYS_ERROR("Display_get failed ..\r\n");
			return -1;
		}

		for(index = 0; index < NUM_DISPLAY_BUFS; index++)
		{
			memcpy(buffer.virtaddr, buf, WIDTH * HEIGHT * 2);
		}

		ret = Display_put(phandle, &buffer);
		if(ret < 0)
		{
			SYS_ERROR("Display_get failed ..\r\n");
			return -1;
		}
	}

	return 0;
}
