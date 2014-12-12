#include "system_includes.h"
#include "system_debug.h"
#include "libcapture.h"
#include "libosd.h"


#define WIDTH 720
#define HEIGHT 576


Capture_Handle phandle = NULL;

void cleanup(int sig)
{
	Capture_destroy(phandle);
}

int main(int argc, char **argv)
{
	CaptureBuffer buffer;
	CaptureAttr  attr;
	struct v4l2_buffer v4l2buf;
	FILE *fp = NULL;
	int count = 0;
	int ret;
	DrawType drawtype;
	int x, y;
	
	drawtype = TIME;
	x=360;y = 240;
	
	memset(&attr, 0x00, sizeof(struct CaptureAttr));
	attr.bufNum    = 4;
	attr.userAlloc = 0;
	phandle = Capture_init(&attr);
	if(phandle == NULL)
	{
		SYS_ERROR("Capture_init failed ..\r\n");
		return -1;
	}

	signal(SIGINT, cleanup);
	while(1)
	{
		SYS_INFO("count %d.\r\n",count++);
		//poll wait
		ret = Capture_get(phandle, &buffer);	
		if(ret < 0)
		{
			SYS_ERROR("Capture_get failed ..\r\n");
			return -1;
		}
	//	set_osd(buffer.virtaddr, TIME, NULL, x, y, 255);

		fp = fopen("capture.yuv","ab+");
		char *buf = (char*)buffer.virtaddr;
		fwrite(buf, WIDTH * HEIGHT * 2, 1, fp);
		fclose(fp);

		ret = Capture_put(phandle, &buffer);
		if(ret < 0)
		{
			SYS_ERROR("Capture_get failed ..\r\n");
			return -1;
		}
	}

	return 0;
}

