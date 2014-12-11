#include "libdisplay.h"

LIBDISP_OBJ *libdisplay_init(void)
{
	LIBDISP_OBJ *phandle = NULL;
	phandle = malloc(sizeof(LIBDISP_OBJ));
    struct fb_var_screeninfo varInfo;
    struct fb_fix_screeninfo fixInfo;
    unsigned int            *buf;
    int                      displaySize;
    int                      bufIdx;
    int                      fd;
    int                      i;

    /* Open video display device */
    phandle->display_fd = open("/dev/fb/3", O_RDWR);

    if ( phandle->display_fd < 0 ) 
	{
		SYS_ERROR("open /dev/fb/3 failed\r\n");
		return phandle;
    }

    if (ioctl(phandle->display_fd, FBIOGET_FSCREENINFO, &fixInfo) == -1) 
	{
		SYS_ERROR("IOCTL FBIOGET_FSCREENINFO failed.\r\n");
        return phandle;
    }

    if (ioctl(phandle->display_fd, FBIOGET_VSCREENINFO, &varInfo) == -1) 
	{
		SYS_ERROR("IOCTL FBIOGET_VSCREENINFO failed.\r\n");
        return phandle;
    }

    varInfo.xres = 720;
    varInfo.yres = 576;
    varInfo.bits_per_pixel = 16;

    /* Set video display format */
    if (ioctl(phandle->display_fd, FBIOPUT_VSCREENINFO, &varInfo) == -1) 
	{
		SYS_ERROR("FBIOPUT_VSCREENINFO failed.\r\n");
		return phandle;
    }

    if (varInfo.xres != 720 ||
        varInfo.yres != 576 ||
        varInfo.bits_per_pixel != 16) 
	{
		SYS_ERROR("failed.\r\n");
		return phandle;
    }

    /* Map the video buffers to user space */
    phandle->virtDisplays[0] = (char *) mmap (NULL,
                                     fixInfo.line_length * varInfo.yres_virtual,
                                     PROT_READ | PROT_WRITE,
                                     MAP_SHARED,
                                     phandle->display_fd, 0);

    if (phandle->virtDisplays[0] == MAP_FAILED) 
	{
		SYS_ERROR("mmap failed.\r\n");
        return phandle;
    }

    /* Determine the size of the display buffers inside the device driver */
    displaySize = fixInfo.line_length * varInfo.yres;

    for (bufIdx=0; bufIdx < NUM_DISPLAY_BUFS-1; bufIdx++) 
	{
        phandle->virtDisplays[bufIdx+1] = phandle->virtDisplays[bufIdx] + displaySize;
    }

    for (bufIdx=0; bufIdx < NUM_DISPLAY_BUFS; bufIdx++) 
	{
        /* Clear the video buffer */
        buf = (unsigned int *) (phandle->virtDisplays[bufIdx]);

        for (i=0; i < displaySize / sizeof(unsigned int); i++) 
		{
            buf[i] = UYVY_BLACK;
        }

        /* Translate the virtual address to physical as resizer needs this */
        //physDisplays[bufIdx] = Memory_getPhysicalAddress(virtDisplays[bufIdx]);
        //SYS_ERROR("Display buffer %d mapped to %#lx has physical address %#lx\n",
          //  bufIdx, (unsigned long) virtDisplays[bufIdx], physDisplays[bufIdx]);
    }

    //*dpp = fixInfo.line_length;
	
	phandle->resize_fd = open("/dev/davinci_resizer", O_RDWR);
	if (phandle->resize_fd < 0 )
	{
		SYS_ERROR("open davinci_resizer failed.\r\n");
		return phandle;
	}
	
	{
		unsigned int resizespeed = 0x0e;
		if (ioctl(phandle->resize_fd, RSZ_S_EXP, &resizespeed) < 0 )
		{
			SYS_ERROR("resize set exp failed.\r\n");
			return phandle;
		}
	}
	phandle->displaySize = displaySize;
	return phandle;
}
static succeed_type libdisplay_flip(LIBDISP_OBJ *phandle, unsigned int *psrc, unsigned int index)
{
	struct fb_var_screeninfo varinfo;
	memset(&varinfo, 0x00, sizeof(varinfo));
	if (ioctl(phandle->display_fd, FBIOGET_VSCREENINFO, &varinfo) < 0 )
	{
		SYS_ERROR("VSCREENINFO failed.\r\n");
		return succeed_type_failed;
	}
	//1. copy or resize from psrc to virtDisplays[index]
	// memcpy(phandle->virtDisplays[index], psrc, phandle->displaySize);
	varinfo.yoffset = varinfo.yres * index;
	if (ioctl(phandle->display_fd, FBIOPAN_DISPLAY, &varinfo) < 0 )
	{
		SYS_ERROR("FBIOPAN_DISPLAY failed.\r\n");
		return succeed_type_failed;
	}
	return succeed_type_succeed;
}
static succeed_type libdisplay_destroy(LIBDISP_OBJ *phandle)
{
	close(phandle->display_fd);
	close(phandle->resize_fd);
	return succeed_type_succeed;
}

LIBDISP_OBJ *DISP_INIT()
{
	return libdisplay_init();
}
succeed_type DISP_FLIP(LIBDISP_OBJ *phandle, unsigned int *psrc, unsigned int index)
{
	return libdisplay_flip(phandle, psrc, index);
}
succeed_type DISP_DESTROY(LIBDISP_OBJ *phandle)
{
	return libdisplay_destroy(phandle);
}
