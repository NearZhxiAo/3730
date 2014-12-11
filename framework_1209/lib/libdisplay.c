#include "libdisplay.h"
#define MODULE_NAME     "Display"
#if 0
static int cleanup(Display_Handle hDisplay)
{
    int                   ret     = Dmai_EOK;
    BufTab_Handle         hBufTab = hDisplay->hBufTab;
    enum v4l2_buf_type    type;
    int                   bufIdx;
    Buffer_Handle         hDispBuf;
    struct v4l2_requestbuffers  req;

    if (hDisplay->fd != -1) {
        if (hDisplay->started) {
            /* Shut off the video display */
            type = V4L2_BUF_TYPE_VIDEO_OUTPUT;

            if (ioctl(hDisplay->fd, VIDIOC_STREAMOFF, &type) == -1) {
                SYS_ERROR("VIDIOC_STREAMOFF failed (%s)\n", strerror(errno));
                ret = Dmai_EFAIL;
            }
        }

        if (close(hDisplay->fd) == -1) {
            SYS_ERROR("Failed to close display device (%s)\n", strerror(errno));
            ret = Dmai_EIO;
        }

        if (hDisplay->userAlloc == FALSE) {
            if (hBufTab) {
                for (bufIdx = 0;
                     bufIdx < BufTab_getNumBufs(hBufTab);
                     bufIdx++) {

                    hDispBuf = BufTab_getBuf(hBufTab, bufIdx);

                    if (munmap(Buffer_getUserPtr(hDispBuf),
                               (hDisplay->bufDescs[bufIdx].v4l2buf.length))
                                == -1) {
                        SYS_ERROR("Failed to unmap display buffer%d\n", bufIdx);
                        ret = Dmai_EFAIL;
                    }
                }
            }
        }
        else {
            Dmai_clear(req);
            req.type   = V4L2_BUF_TYPE_VIDEO_OUTPUT;
            req.count  = 0;
            req.memory = V4L2_MEMORY_USERPTR;
            if (ioctl(hDisplay->fd, VIDIOC_REQBUFS, &req) == -1) {
                SYS_ERROR("Could not allocate video display buffers\n");
                return Dmai_ENOMEM;
            }
        } 
    }

    if (hDisplay->bufDescs) {
        free(hDisplay->bufDescs);
    }

    free(hDisplay);

    return ret;
}

#endif



static int writeSysFs(char *fileName, char *val)
{
    FILE *fp;
    char *valString;

    valString = malloc(strlen(val) + 1);

    if (valString == NULL) {
        SYS_ERROR("Failed to allocate memory for temporary string\n");
        return -1;
    }

    fp = fopen(fileName, "w");

    if (fp == NULL) {
        SYS_ERROR("Failed to open %s for writing\n", fileName);
        free(valString);
        return -1;
    }

    if (fwrite(val, strlen(val) + 1, 1, fp) != 1) {
        SYS_ERROR("Failed to write sysfs variable %s to %s\n",
                  fileName, val);
        fclose(fp);
        free(valString);
        return -1;
    }

    fclose(fp);

    free(valString);

    return 0;
}

static int readSysFs(char *fileName, char *val, int length)
{
    FILE *fp;
    int ret;
    int len;
    char *tok;

    fp = fopen(fileName, "r");

    if (fp == NULL) {
        SYS_ERROR("Failed to open %s for reading\n", fileName);
        return -1;
    }

    memset(val, '\0', length);

    ret = fread(val, 1, length, fp);

    if (ret < 1) {
        SYS_ERROR("Failed to read sysfs variable from %s\n", fileName);
        return -1;
    }

    tok = strtok(val, "\n");
    len = tok ? strlen(tok) : strlen(val);
    val[len] = '\0';

    fclose(fp);

    return 0;
}

static int getFbResolution(char *str, unsigned int *w, unsigned int *h)
{
    int ret;
    char fileName[100], val[100], *ptr;

    sprintf(fileName, "/sys/devices/platform/omapdss/%s/timings", str);

    ret = readSysFs(fileName, val, 100);
    if (ret < 0) {
        SYS_ERROR("Failed to read from %s\n", fileName);
        return -1;
    }

    ptr = val;
    while (*ptr != ',')
        ptr++;
    *w = strtoul(ptr+1, &ptr, 0);

    while (*ptr != ',')
        ptr++;
    *h = strtoul(ptr+1, &ptr, 0);

    return 0;
}

static int configure_fb (unsigned int width, unsigned int height) 
{
   	int fd;
    struct fb_var_screeninfo varInfo;

    fd = open("/dev/fb0", O_RDWR);

    if (fd == -1) {
	    SYS_ERROR("Failed to open /dev/fb0");
	    return -1;
    }

    if (ioctl(fd, FBIOGET_VSCREENINFO, &varInfo) == -1) {
        SYS_ERROR("Failed FBIOGET_VSCREENINFO on /dev/fb0");
        close(fd);
	    return -1;
    }

    if (varInfo.rotate == 1 || varInfo.rotate == 3) {
        varInfo.xres = height;
        varInfo.yres = width;
    } else {
        varInfo.xres = width;
        varInfo.yres = height;
    }

    varInfo.xres_virtual    = width;
    varInfo.yres_virtual    = height ;

    SYS_INFO("Configuring /dev/fb0, width=%d, height=%d\n", varInfo.xres,
        varInfo.yres);

    if (ioctl(fd, FBIOPUT_VSCREENINFO, &varInfo) < 0) {
        SYS_ERROR("Failed FBIOPUT_VSCREENINFO on /dev/fb0");
        return -1;
    }

    close(fd);

    return 0;        
}


static inline int config_dss_sysfs(const char *file, char *val)
{
    char f_name[100];

    sprintf(f_name, "/sys/devices/platform/omapdss/%s", file);
    return writeSysFs(f_name, val);
}


void  _SysFs_change()
{
    char str[6];
    unsigned int width, height;

     //SYS_INFO("Setting display to LCD\n");

#if 1
        /* Disable overlays and displays and break the link */
        config_dss_sysfs("overlay0/enabled", "0");
        config_dss_sysfs("overlay1/enabled", "0");
		config_dss_sysfs("overlay2/enabled", "0");
		config_dss_sysfs("overlay0/manager", "\n");
		config_dss_sysfs("overlay1/manager", "\n");
		config_dss_sysfs("overlay2/manager", "\n");
		config_dss_sysfs("display0/enabled", "0");
		config_dss_sysfs("display1/enabled", "0");
		config_dss_sysfs("display2/enabled", "0");



		/* Set the links and enable overlays: Currentl only GFX is enabled */
#if 0
		getFbResolution("display0", &width, &height);
		configure_fb(width, height);
		config_dss_sysfs("overlay0/manager", "lcd");
		config_dss_sysfs("overlay1/manager", "lcd");
		config_dss_sysfs("overlay2/manager", "lcd");
		config_dss_sysfs("manager0/display", "lcd");
		config_dss_sysfs("display0/enabled", "1");
		config_dss_sysfs("overlay0/enabled", "1");
#endif
#if 1
		getFbResolution("display2", &width, &height);

		printf("\r\n\r\n before config display2 width %d height %d\r\n",width, height);
		configure_fb(800, 600);
		
		getFbResolution("display2", &width, &height);
		printf("\r\n\r\n after config display2 width %d height %d\r\n",width, height);
		//configure_fb(800, 600);
		config_dss_sysfs("manager0/display", "dvi");
		config_dss_sysfs("overlay0/manager", "lcd");
		config_dss_sysfs("overlay1/manager", "lcd");
		config_dss_sysfs("overlay2/manager", "lcd");
		config_dss_sysfs("overlay0/enabled", "1");
		config_dss_sysfs("display2/enabled", "1");
#endif

	//	configure_fb(800, 600);
		//configure_fb(width, height);
#endif
		return 0;
}

int libdisplay_set( int enable, int fd)
{
	SYS_INFO("in libdisplay_set %d.....................\r\n",enable);
	struct v4l2_framebuffer framebuffer;

	/* Enable/Disable the alpha blending */
	if (ioctl(fd, VIDIOC_G_FBUF, &framebuffer) < 0){
		SYS_ERROR("Failed getting Buffer Information\r\n");
		exit(1);
		return -1;
	}
	//printf("framebuffer.flags = 0x%x.\r\n", framebuffer.flags);
	if (enable){
		//framebuffer.flags |= V4L2_FBUF_FLAG_LOCAL_ALPHA  ;
		//framebuffer.flags |= V4L2_FBUF_FLAG_LOCAL_ALPHA;
		//framebuffer.flags &= ~(V4L2_FBUF_FLAG_CHROMAKEY | V4L2_FBUF_FLAG_SRC_CHROMAKEY);
		framebuffer.flags |= V4L2_FBUF_FLAG_GLOBAL_ALPHA  ;
		framebuffer.flags &= ~ (V4L2_FBUF_FLAG_CHROMAKEY | 0x0040);
#if 0
		framebuffer.flags |= V4L2_FBUF_FLAG_GLOBAL_ALPHA  ;
		framebuffer.flags &= ~ (V4L2_FBUF_FLAG_CHROMAKEY | 0x0040);
#endif
		//V4L2_FBUF_FLAG_SRC_CHROMAKEY);	
	}
	else 
	{
		//framebuffer.flags &= ~V4L2_FBUF_FLAG_LOCAL_ALPHA;
		//framebuffer.flags |= V4L2_FBUF_FLAG_OVERLAY;
		

		//framebuffer.flags &= ~V4L2_FBUF_FLAG_GLOBAL_ALPHA;

		framebuffer.flags &= ~V4L2_FBUF_FLAG_GLOBAL_ALPHA;
#if 0
		if(framebuffer.capability & V4L2_FBUF_CAP_SRC_CHROMAKEY) {
			SYS_INFO("Support SRC CHROMAKEY...\r\n");
#if 1
			framebuffer.flags |= V4L2_FBUF_FLAG_SRC_CHROMAKEY;
			framebuffer.flags &= ~V4L2_FBUF_FLAG_CHROMAKEY;
#endif
#if 0
			framebuffer.flags &= ~V4L2_FBUF_FLAG_SRC_CHROMAKEY;
#endif
		}
		else
		{
			SYS_INFO("do not Support SRC CHROMAKEY...\r\n");
		}
#endif

#if 0
		if(framebuffer.capability & V4L2_FBUF_CAP_CHROMAKEY) {
			SYS_INFO("Support CAP CHROMAKEY...\r\n");
			//framebuffer.flags |= V4L2_FBUF_FLAG_SRC_CHROMAKEY;
			//framebuffer.flags &= ~V4L2_FBUF_FLAG_CHROMAKEY;
//			framebuffer.flags &= ~V4L2_FBUF_FLAG_SRC_CHROMAKEY;
				framebuffer.flags &= ~V4L2_FBUF_FLAG_CHROMAKEY;
		}
		else
		{
			SYS_INFO("do not Support CAP CHROMAKEY...\r\n");
		}
#endif

#if 0
		struct v4l2_format fmt;
		unsigned  global_alpha = 128; /* Red color RGB565 format */
		int ret;

		fmt.type = V4L2_BUF_TYPE_VIDEO_OVERLAY;
		ret = ioctl(fd, VIDIOC_G_FMT, &fmt);
		if (ret < 0) {
			    perror("VIDIOC_G_FMT\n");
				    close(fd);
					    exit(0);
		}
		fmt.fmt.win.global_alpha = global_alpha;
		ret = ioctl(fd, VIDIOC_S_FMT, &fmt);
		if (ret < 0) {
			    perror("VIDIOC_G_FMT\n");
				    close(fd);
					    exit(0);
		}
#endif
	}
	if (ioctl(fd, VIDIOC_S_FBUF, &framebuffer) < 0){
		SYS_ERROR("Failed setting Buffer Information\r\n");
		exit(1);
		return -1;
	}
	if (ioctl(fd, VIDIOC_G_FBUF, &framebuffer) < 0){
		exit(1);
		SYS_ERROR("Failed getting Buffer Information\r\n");
		return -1;
	}
	//printf("After framebuffer.flags = 0x%x.\r\n", framebuffer.flags);
	return 0;
}

Display_Handle Display_init(DisplayAttr *attr)
{
	struct v4l2_format         fmt;
	Display_Handle             phandle = NULL;
	DisplayBuffer			   *disBufs = NULL;
	int                      width, height;
	struct v4l2_buffer		   *v4l2buf = NULL;
	struct v4l2_requestbuffers  req;
#if defined(V4L2_CID_ROTATE)
	//    struct v4l2_control        control;
#endif
	int BufNum;
	int userAlloc;
	int                         bufindex;
	int rotation = 0;
	struct v4l2_control			control;

	if(attr == NULL)
	{
		SYS_ERROR("Thre DisplayAttr is null, use default attr.\r\n");
		BufNum = NUM_DISPLAY_BUFS;
		userAlloc = 0;
	}
	else
	{
		if (attr->bufNum <= 0 || attr->bufNum >= 10)
		{
			SYS_ERROR("attr->bufNum %d is not legal, use default attr\r\n", attr->bufNum);
			BufNum = NUM_DISPLAY_BUFS;
			userAlloc = attr->userAlloc;
		}
		else
		{
			BufNum = attr->bufNum;
			userAlloc = attr->userAlloc;
		}
	}

	/* Allocate space for state object */
	phandle = calloc(1, sizeof(Display_Object));

	if (phandle == NULL) {
		SYS_ERROR("Failed to allocate space for Display Object\n");
		goto error;
	}

	/* Setup sysfs variable */
	//_SysFs_change();

	// phandle->userAlloc = TRUE;
	// phandle->attrs     = *attrs;

	/* Open video capture device */
	phandle->display_fd = open("/dev/video2", O_RDWR, 0);

	if (phandle->display_fd == -1) {
		SYS_ERROR("Cannot open /dev/video1\n");
		goto error;
	}



	control.value = 0;

	/* Enable hardware rotation */
//	control.id    = V4L2_CID_ROTATE;
#if 0
	if (ioctl(phandle->display_fd, VIDIOC_S_CTRL, &control) == -1) {
		SYS_ERROR("Failed to set rotation angle\n");
		goto error;
	}
#endif
	width = attr->width;
	height = attr->height;

	memset(&fmt, 0x00, sizeof(struct v4l2_format));
	fmt.fmt.pix.width = width;
	fmt.fmt.pix.height = height;  
	fmt.fmt.pix.pixelformat = V4L2_PIX_FMT_UYVY;
	fmt.type = V4L2_BUF_TYPE_VIDEO_OUTPUT;

	if (ioctl(phandle->display_fd, VIDIOC_S_FMT, &fmt) == -1) {
		SYS_ERROR("Failed to Set video display format\n");
		goto error;
	}

	if (ioctl(phandle->display_fd, VIDIOC_G_FMT, &fmt) == -1) {
		SYS_ERROR("Failed to determine video display format\n");
		goto error;
	}

	printf("Video output set to size %dx%d pitch %d, Format %d\n",
			fmt.fmt.pix.width, fmt.fmt.pix.height,
			fmt.fmt.pix.bytesperline, fmt.fmt.pix.pixelformat);

	if ( userAlloc == 0) 
	{
		phandle->userAlloc = 0;

		memset(&req, 0x00, sizeof(struct v4l2_requestbuffers));
		req.count  = NUM_DISPLAY_BUFS;
		req.type   = V4L2_BUF_TYPE_VIDEO_OUTPUT;
		req.memory = V4L2_MEMORY_MMAP;

		/* Allocate buffers in the capture device driver */
		if (ioctl(phandle->display_fd, VIDIOC_REQBUFS, &req) == -1) {
			SYS_ERROR("VIDIOC_REQBUFS failed (%s)\n", strerror(errno));
			goto error;
		}

		if (req.count < NUM_DISPLAY_BUFS || !req.count) {
			SYS_ERROR("Insufficient device driver buffer memory\n");
			goto error;
		}

		/* Allocate space for buffer descriptors */
		//	*bufDescsPtr = calloc(numBufs, sizeof(_VideoBufDesc));
		disBufs = calloc(req.count, sizeof(DisplayBuffer));
		v4l2buf = calloc(req.count, sizeof(struct v4l2_buffer));

		if(disBufs == NULL || v4l2buf == NULL)
		{
			SYS_ERROR("malloc DisplayBuffer or v4l2_buffer failed.\r\n");
			goto error;
		}

		for (bufindex = 0; bufindex < req.count; bufindex++) 
		{
			memset(&v4l2buf[bufindex], 0x00, sizeof(struct v4l2_buffer));
			v4l2buf[bufindex].type = V4L2_BUF_TYPE_VIDEO_OUTPUT;
			v4l2buf[bufindex].memory = V4L2_MEMORY_MMAP;
			v4l2buf[bufindex].index = bufindex;

			if (ioctl(phandle->display_fd, VIDIOC_QUERYBUF, &v4l2buf[bufindex]) == -1) {
				SYS_ERROR("Failed VIDIOC_QUERYBUF (%s)\n", strerror(errno));
				goto error;
			}

			disBufs[bufindex].length = v4l2buf[bufindex].length;
			disBufs[bufindex].physaddr = v4l2buf[bufindex].m.offset;
			disBufs[bufindex].index = bufindex;
			/* Map the driver buffer to user space */
			disBufs[bufindex].virtaddr = mmap(NULL, v4l2buf[bufindex].length, PROT_READ|PROT_WRITE, MAP_SHARED, phandle->display_fd, v4l2buf[bufindex].m.offset);

			SYS_INFO("index %d length %d physaddr %x virtaddr %x.\r\n",
					bufindex,v4l2buf[bufindex].length, v4l2buf[bufindex].m.offset, disBufs[bufindex].virtaddr);

			if (disBufs[bufindex].virtaddr == MAP_FAILED)
			{
				SYS_ERROR("mmap failed. index = %d.\r\n", bufindex);
				goto error;
			}


			/* Queue buffer in device driver */
			if (ioctl(phandle->display_fd, VIDIOC_QBUF, &v4l2buf[bufindex])  < 0) {
				SYS_ERROR("VIODIOC_QBUF failed (%s)\n", strerror(errno));
				goto error;
			}
		}
	}
	else 
	{
		SYS_INFO("User alloc the display buffer.\r\n");
		phandle->userAlloc = 1;
		if(CMEM_init() < 0)
		{
			SYS_ERROR("CMEM_init failed.\r\n");
			goto error;
		}

		memset(&req, 0x00, sizeof(struct v4l2_requestbuffers));
		req.type   = V4L2_BUF_TYPE_VIDEO_OUTPUT;
		req.count  = BufNum; 
		req.memory = V4L2_MEMORY_USERPTR;

		if (ioctl(phandle->display_fd, VIDIOC_REQBUFS, &req) == -1) {
			SYS_ERROR("Could not allocate video display buffers\n");
			goto error;
		}

		/* The driver may return less buffers than requested */
		if (req.count < BufNum || !req.count) {
			SYS_ERROR("Insufficient device driver buffer memory\n");
			goto error;
		}

		disBufs = calloc(req.count, sizeof(DisplayBuffer));
		v4l2buf = calloc(req.count, sizeof(struct v4l2_buffer));

		if(disBufs == NULL || v4l2buf == NULL)
		{
			SYS_ERROR("malloc DisplayBuffer or v4l2_buffer failed.\r\n");
			goto error;
		}
		for (bufindex = 0; bufindex < req.count; bufindex++) 
		{
			memset(&v4l2buf[bufindex], 0x00, sizeof(struct v4l2_buffer));
			char *data = CMEM_alloc((720 * 480 * 2 + 4 * 1024) & (~0xFFF), &dis_cmem_param);
			if (data == NULL)
			{
				SYS_ERROR("CMEM_alloc failed.\r\n");
				goto error;
			}
			v4l2buf[bufindex].type = V4L2_BUF_TYPE_VIDEO_OUTPUT;
			v4l2buf[bufindex].memory = V4L2_MEMORY_USERPTR;
			v4l2buf[bufindex].index = bufindex;
			v4l2buf[bufindex].m.userptr = (int)data;
			v4l2buf[bufindex].length	=  (720*480*2 + 4*1024) & (~0xFFF);

			disBufs[bufindex].length	= v4l2buf[bufindex].length;
			disBufs[bufindex].physaddr	= CMEM_getPhys(data);
			disBufs[bufindex].index		= v4l2buf[bufindex].index;
			disBufs[bufindex].virtaddr	= (void*)v4l2buf[bufindex].m.userptr;

			SYS_INFO("index %d length %d physaddr %x virtaddr %x.\r\n",
					bufindex,v4l2buf[bufindex].length, v4l2buf[bufindex].m.offset, disBufs[bufindex].virtaddr);


			/* Queue buffer in device driver */
			if (ioctl(phandle->display_fd, VIDIOC_QBUF, &v4l2buf[bufindex])  < 0) 
			{
				SYS_ERROR("VIODIOC_QBUF failed (%s)\n", strerror(errno));
				goto error;
			}
		}

	}

	enum v4l2_buf_type type = V4L2_BUF_TYPE_VIDEO_OUTPUT;
	if(ioctl(phandle->display_fd, VIDIOC_STREAMON, &type) == -1)
	{
		goto error;
	}
	phandle->disBufsPtr = disBufs;
	phandle->v4l2buf = v4l2buf;
	phandle->disBufsNum = req.count;



	libdisplay_set(0, phandle->display_fd);  //设置透明叠加视频显示在video层,qt显示在framebuffer层, 设置framebuffer显示在video上面，然后将qt界面背景设置成透明的

	return phandle;

error:
	if(phandle != NULL)
	{
		if(phandle->display_fd > 0)
		{
			close(phandle->display_fd);
		}
		free(phandle);
	}
	if (disBufs != NULL)
	{
		free(disBufs);
	}
	return NULL;

}

int Display_get(Display_Handle phandle, DisplayBuffer *buffer)
{
	struct v4l2_buffer  v4l2buf;
	DisplayBuffer *disBufs = NULL;



	if(buffer == NULL)
	{
		SYS_ERROR("The parameter DisplayBuffer can not be null.\r\n");
		return -1;
	}


	disBufs = phandle->disBufsPtr;
	memset(&v4l2buf, 0x00, sizeof(struct v4l2_buffer));
	v4l2buf.type   = V4L2_BUF_TYPE_VIDEO_OUTPUT;

	v4l2buf.memory = phandle->userAlloc ? V4L2_MEMORY_USERPTR : V4L2_MEMORY_MMAP;

	/* Get a frame buffer with captured data */
	if(ioctl(phandle->display_fd, VIDIOC_DQBUF, &v4l2buf) < 0)
	{
		SYS_ERROR("VIDIOC_DQBUF failed.\r\n");
		return -1;
	}

	buffer->virtaddr = disBufs[v4l2buf.index].virtaddr;
	buffer->physaddr = disBufs[v4l2buf.index].physaddr;
	buffer->length	 = disBufs[v4l2buf.index].length;
	buffer->index	 = disBufs[v4l2buf.index].index;

	return 0;


}

int Display_put(Display_Handle phandle, DisplayBuffer *buffer)
{
	struct v4l2_buffer *pbuf;

	pbuf = &phandle->v4l2buf[buffer->index];

	pbuf->type = V4L2_BUF_TYPE_VIDEO_OUTPUT;
	pbuf->m.userptr = (unsigned long)buffer->virtaddr;

	if(ioctl(phandle->display_fd, VIDIOC_QBUF, pbuf) < 0)
	{
		SYS_ERROR("display_put vidioc_qbuf failed.\r\n");
		return -1;
	}

	return 0;
}

#if 0
/******************************************************************************
 * Display_v4l2_control
 ******************************************************************************/
int Display_v4l2_control(Display_Handle hDisplay, 
		Display_Control_Message message,
		void * parameters,
		BufTab_Handle hBufTab,
		Display_Attrs *attrs)
{
	struct v4l2_format             *fmt;
	enum v4l2_buf_type              type;
	fmt = (struct v4l2_format *)    parameters;

	switch(message) {
		case Display_Control_V4L2_Overlay_FMT:
			/* Set the overlay parameters*/
			fmt->type = V4L2_BUF_TYPE_VIDEO_OVERLAY;
			if (ioctl(hDisplay->fd, VIDIOC_S_FMT, fmt) < 0) {
				SYS_ERROR("VIDIOC_S_FMT (OVERLAY) failed (%s)\n",
						strerror(errno));
				return Dmai_EIO;
			}
			break;

		case Display_Control_V4L2_Output_FMT:
			/* Set the output parameters*/
			fmt->type = V4L2_BUF_TYPE_VIDEO_OUTPUT;
			if (ioctl(hDisplay->fd, VIDIOC_S_FMT, fmt) < 0) {
				SYS_ERROR("VIDIOC_S_FMT (OUTPUT) failed (%s)\n",
						strerror(errno));
				return Dmai_EIO;
			}
			if (ioctl(hDisplay->fd, VIDIOC_G_FMT, fmt) == -1) {
				SYS_ERROR("Failed to determine video display format\n");
				cleanup(hDisplay);
				return NULL;
			}

			Dmai_dbg4("Video output set to size %dx%d pitch %d, Format %d\n",
					fmt->fmt.pix.width, fmt->fmt.pix.height,
					fmt->fmt.pix.bytesperline, fmt->fmt.pix.pixelformat);
			break;

		case Display_Control_V4L2_Streamoff:
			/* Stop streaming*/
			if (hDisplay->started) {
				attrs->streamonDisable = TRUE;
				type = V4L2_BUF_TYPE_VIDEO_OUTPUT;

				if (ioctl(hDisplay->fd, VIDIOC_STREAMOFF, &type) < 0) {
					SYS_ERROR("VIDIOC_STREAMOFF failed (%s)\n",
							strerror(errno));
					return Dmai_EFAIL;
				}
			} 
			break;

		case Display_Control_V4L2_Qbuf:
			/* Queue the buffers before streaming*/
			attrs->streamonDisable = FALSE;
			if (_Dmai_v4l2UserAlloc(hDisplay->fd,
						attrs->numBufs,
						V4L2_BUF_TYPE_VIDEO_OUTPUT,
						&hDisplay->bufDescs,
						hBufTab,
						0, attrs->colorSpace, TRUE) < 0) {
				SYS_ERROR("Failed to allocate display driver buffers on %s\n",
						attrs->displayDevice);
				cleanup(hDisplay);
				return NULL;
			}

			hDisplay->hBufTab = hBufTab;
			hDisplay->started = TRUE;
			hDisplay->displayStd = Display_Std_V4L2;
			break;

		case Display_Control_V4L2_Streamon:
			/* Start the video streaming */
			type = V4L2_BUF_TYPE_VIDEO_OUTPUT;

			if (ioctl(hDisplay->fd, VIDIOC_STREAMON, &type) == -1) {
				Dmai_err2("VIDIOC_STREAMON failed on %s (%s)\n",
						attrs->displayDevice, strerror(errno));
				cleanup(hDisplay);
				return NULL;
			}

			hDisplay->started = TRUE;
			hDisplay->hBufTab = hBufTab;
			hDisplay->displayStd = Display_Std_V4L2;
			break;

		default:
			Dmai_dbg1("Unsupported Message in Display_v4l2_control: %d\n", 
					message);            
	}    

	return Dmai_EOK;
}
#endif


int Display_destroy(Display_Handle phandle)
{
	int index;

	if(phandle != NULL)
	{
		if(phandle->display_fd > 0)
		{
			close(phandle->display_fd);
		}

		if(!phandle->userAlloc)
		{
			if(phandle->v4l2buf != NULL)
			{
				free(phandle->v4l2buf);
			}

			if(phandle->disBufsPtr != NULL)
			{
				free(phandle->disBufsPtr);
			}
		}
		else
		{
			for(index = 0; index < phandle->disBufsNum; index++)
			{
				if(phandle->disBufsPtr[index].virtaddr != NULL)
				{
					CMEM_free(phandle->disBufsPtr[index].virtaddr, &dis_cmem_param);
				}
			}

			if(phandle->v4l2buf != NULL)
			{
				free(phandle->v4l2buf);
			}

			if(phandle->disBufsPtr != NULL)
			{
				free(phandle->disBufsPtr);
			}
		}
		free(phandle);
	}
	return 0;
}
