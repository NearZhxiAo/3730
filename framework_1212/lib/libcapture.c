#include "libcapture.h"

static Capture_Handle Capture_init(CaptureAttr *attr)
{
	Capture_Handle phandle = NULL;
	CaptureBuffer *capBufs = NULL;
	struct v4l2_capability cap;
	struct v4l2_format fmt;
	struct v4l2_fmtdesc fmtdesc;
	struct v4l2_streamparm setfps;
	struct v4l2_requestbuffers req;
	struct v4l2_buffer *v4l2buf = NULL;
	struct v4l2_input  input;
	struct v4l2_standard  std;
	v4l2_std_id id;
	enum v4l2_buf_type type;
	int  BufNum;
	int userAlloc;
	int ret = 0;
	int index;
	int queryinput = 0;
	unsigned int bufindex = 0;
	unsigned int failCount = 0;

	if(attr == NULL)
	{
		SYS_WARNING("The CaptureAttr is null, use default attr.\r\n");
		SYS_WARNING("BufNum is %d, userAlloc is %s.\r\n", CAP_BUF_NUM, "false");
		BufNum = CAP_BUF_NUM;
		userAlloc = 0;
	}
	else
	{
		if(attr->bufNum <=0 || attr->bufNum >=10)
		{
			SYS_ERROR("attr->bufNum %d is not legal, use default attr\r\n",attr->bufNum);
			BufNum = CAP_BUF_NUM;
			userAlloc = attr->userAlloc;
		}
		else
		{
			BufNum = attr->bufNum;
			userAlloc = attr->userAlloc;
		}
	}

	phandle = malloc(sizeof(Capture_Object));

	if(phandle == NULL)
	{
		SYS_ERROR("malloc Capture_Object failed.\r\n");
		goto error;
	}

	phandle->capture_fd = open(VIDEO_DEVICE, O_RDWR);
	if (phandle->capture_fd <= 0)
	{
		SYS_ERROR("open device /dev/video0 failed.\r\n");
		goto error;
	}

	if (ioctl(phandle->capture_fd, VIDIOC_G_INPUT, &index) < 0)
	{
		SYS_ERROR("VIDIOC_G_INPUT failed.\r\n");
		goto error;
	}
	//SYS_INFO("VIdeoc_G_Input index = %d.\r\n", index);
	for (index=0;; index++) {

		input.index = index;
		if (ioctl(phandle->capture_fd, VIDIOC_ENUMINPUT, &input) < 0) {
			//SYS_ERROR("VIDIOC_ENUMINPUT failed\r\n");
			break;
		}

		//SYS_INFO(" name=%s, index = %d\n", input.name, index);
	}
#if 0	
	index = 0;
	if (ioctl(phandle->capture_fd, VIDIOC_S_INPUT, &index) < 0) {
		SYS_ERROR("Failed VIDIOC_S_INPUT to index %d \n", 
				index);
		goto error;
	}
#endif
	for (index=0;; index++) {

		std.frameperiod.numerator = 1;
		std.frameperiod.denominator = 0;
		std.index = index;

		if (ioctl(phandle->capture_fd, VIDIOC_ENUMSTD, &std) < 0) {
			break;
		}
		//SYS_INFO(" name=%s, fps=%d/%d\n", std.name, 
				//std.frameperiod.denominator, std.frameperiod.numerator);
	}
	/* Detect the standard in the input detected */ 
#if 0
	if (ioctl(phandle->capture_fd, VIDIOC_QUERYSTD, &id) < 0) {
		SYS_ERROR("VIDIOC_QUERYSTD failed\r\n");
		goto error;
	}
#endif
	/* Get current video standard */
	if (ioctl(phandle->capture_fd, VIDIOC_G_STD, &id) < 0) {
		SYS_ERROR("Failed VIDIOC_G_STD\n");
		goto error;
	}

	if (id & V4L2_STD_NTSC) {
		//SYS_INFO("in ntsc................\r\n");
	}
	else if (id & V4L2_STD_PAL) {
		//SYS_INFO("in pal................\r\n");
	}
	else if (id & V4L2_STD_525_60) {
		//SYS_INFO("Found 525_60 std input\n");
	}
	else if (id & V4L2_STD_625_50) {
		//SYS_INFO("Found 625_50 std input\n");
	}
	else {
		SYS_ERROR("Unknown video standard on capture device \n");
	}


	if(ioctl(phandle->capture_fd, VIDIOC_QUERYCAP, &cap) == -1)
	{
		SYS_ERROR("Error opening device %s: unable to query device.\r\n", VIDEO_DEVICE);
		goto error;
	}
	else
	{
		//SYS_INFO("driver:\t\t%s\n",cap.driver);
		//SYS_INFO("card:\t\t%s\n",cap.card);
		//SYS_INFO("bus_info:\t%s\n",cap.bus_info);
		//SYS_INFO("version:\t%d\n",cap.version);
		//SYS_INFO("capabilities:\t%x\n",cap.capabilities);

		if ((cap.capabilities & V4L2_CAP_VIDEO_CAPTURE) == V4L2_CAP_VIDEO_CAPTURE) 
		{
			//SYS_INFO("Device %s: supports capture.\n",VIDEO_DEVICE);
		}

		if ((cap.capabilities & V4L2_CAP_STREAMING) == V4L2_CAP_STREAMING) 
		{
			//SYS_INFO("Device %s: supports streaming.\n",VIDEO_DEVICE);
		}
	} 
#if 0

	//emu all support fmt
	fmtdesc.index = 0;
	fmtdesc.type = V4L2_BUF_TYPE_VIDEO_CAPTURE;
	//SYS_INFO("Support format:\n");
	while(ioctl(phandle->capture_fd,VIDIOC_ENUM_FMT,&fmtdesc)!=-1)
	{
		//SYS_INFO("\t%d.%s\n",fmtdesc.index+1,fmtdesc.description);
		fmtdesc.index++;
	}
#endif
	//set fmt
	fmt.type = V4L2_BUF_TYPE_VIDEO_CAPTURE;
	fmt.fmt.pix.pixelformat = V4L2_PIX_FMT_UYVY;
	fmt.fmt.pix.height = HEIGHT;
	fmt.fmt.pix.width = WIDTH;
	fmt.fmt.pix.field = V4L2_FIELD_INTERLACED;
	fmt.fmt.pix.bytesperline = WIDTH * 2;

	if(ioctl(phandle->capture_fd, VIDIOC_S_FMT, &fmt) == -1)
	{
		SYS_ERROR("Unable to set format.1\n");
		goto error;
	} 	
	if(ioctl(phandle->capture_fd, VIDIOC_G_FMT, &fmt) == -1)
	{
		SYS_ERROR("Unable to get format\n");
		goto error;
	} 
	{
		//SYS_INFO("fmt.type:\t\t%d\n",fmt.type);
		//SYS_INFO("pix.pixelformat:\t%c%c%c%c\n",fmt.fmt.pix.pixelformat & 0xFF, (fmt.fmt.pix.pixelformat >> 8) & 0xFF,(fmt.fmt.pix.pixelformat >> 16) & 0xFF, (fmt.fmt.pix.pixelformat >> 24) & 0xFF);
		//SYS_INFO("..........pix.height:\t\t%d\n",fmt.fmt.pix.height);
		//SYS_INFO("............pix.width:\t\t%d\n",fmt.fmt.pix.width);
		//SYS_INFO("..........pix.field:\t\t%d\n",fmt.fmt.pix.field);
	}
	//set fps
	setfps.type = V4L2_BUF_TYPE_VIDEO_CAPTURE;
	setfps.parm.capture.timeperframe.numerator = 10;
	setfps.parm.capture.timeperframe.denominator = 10;

	//SYS_INFO("init %s \t[OK]\n",VIDEO_DEVICE);


	if(!userAlloc)
	{
		//SYS_INFO("Driver alloc the capture buffer. BufNum = %d\r\n", BufNum);
		phandle->userAlloc = 0;
		memset(&req, 0x00, sizeof(req));
		req.count = BufNum;
		req.type = V4L2_BUF_TYPE_VIDEO_CAPTURE;
		req.memory = V4L2_MEMORY_MMAP;
		if (ioctl(phandle->capture_fd, VIDIOC_REQBUFS, &req) < 0 )
		{
			SYS_ERROR("VIDIOC_REQDQBUFS failed.\r\n");
			goto error;
		}

		if (req.count < BufNum)
		{
			if(!req.count)
			{
				SYS_ERROR("req.count is 0.\r\n", req.count);
				goto error;
			}
			else
			{
				SYS_WARNING("req.count = %d < %d\r\n", req.count, BufNum);
			}
		}

		capBufs = calloc(req.count, sizeof(CaptureBuffer));
		v4l2buf = calloc(req.count, sizeof(struct v4l2_buffer));

		if (capBufs == NULL || v4l2buf == NULL)
		{
			SYS_ERROR("malloc capbufs failed.\r\n");
			goto error;
		}

		for(bufindex = 0; bufindex < req.count; bufindex++)
		{
			memset(&v4l2buf[bufindex], 0x00, sizeof(struct v4l2_buffer));
			v4l2buf[bufindex].type = V4L2_BUF_TYPE_VIDEO_CAPTURE;
			v4l2buf[bufindex].memory = V4L2_MEMORY_MMAP;
			v4l2buf[bufindex].index = bufindex;

			if (ioctl(phandle->capture_fd, VIDIOC_QUERYBUF, &v4l2buf[bufindex]) < 0)
			{
				SYS_ERROR("VIDIOC_QUERYBUF failed. index = %d.\r\n", bufindex);
				goto error;
			}

			capBufs[bufindex].length = v4l2buf[bufindex].length;
			capBufs[bufindex].physaddr = v4l2buf[bufindex].m.offset;
			capBufs[bufindex].index = bufindex;
			capBufs[bufindex].virtaddr = mmap(NULL, v4l2buf[bufindex].length, PROT_READ|PROT_WRITE, MAP_SHARED, phandle->capture_fd, v4l2buf[bufindex].m.offset);

			//SYS_INFO("index %d length %d physaddr %x virtaddr %x.\r\n",
					//bufindex,v4l2buf[bufindex].length, v4l2buf[bufindex].m.offset, capBufs[bufindex].virtaddr);
			memset(capBufs[bufindex].virtaddr, 0x55, 720*480 );
			if (capBufs[bufindex].virtaddr == MAP_FAILED)
			{
				SYS_ERROR("mmap failed. index = %d.\r\n", bufindex);
				goto error;
			}

			if (ioctl(phandle->capture_fd, VIDIOC_QBUF, &v4l2buf[bufindex]) < 0 )
			{
				SYS_ERROR("VIDIOC_QBUF failed.\r\n");
				goto error;
			}
		}
	}
	else
	{

		if(ioctl(phandle->capture_fd, VIDIOC_G_FMT, &fmt) == -1)
		{
			SYS_ERROR("Unable to get format\n");
			goto error;
		} 
		//SYS_INFO("User alloc the capture buffer.\r\n");
		phandle->userAlloc = 1;
		if(CMEM_init() < 0)
		{
			SYS_ERROR("CMEM_init error.\r\n");
			goto error;
		}
		memset(&req, 0x00, sizeof(req));
		req.count = BufNum;
		req.type = V4L2_BUF_TYPE_VIDEO_CAPTURE;
		req.memory = V4L2_MEMORY_USERPTR;
		if (ioctl(phandle->capture_fd, VIDIOC_REQBUFS, &req) < 0 )
		{
			SYS_ERROR("VIDIOC_REQDQBUFS failed.\r\n");
			goto error;
		}

		if (req.count < BufNum)
		{
			if(!req.count)
			{
				SYS_ERROR("req.count is 0.\r\n", req.count);
				goto error;
			}
			else
			{
				SYS_WARNING("req.count = %d < %d\r\n", req.count, BufNum);
			}
		}

		capBufs = calloc(req.count, sizeof(CaptureBuffer));
		v4l2buf = calloc(req.count, sizeof(struct v4l2_buffer));

		if (capBufs == NULL || v4l2buf == NULL)
		{
			SYS_ERROR("malloc capbufs failed.\r\n");
			goto error;
		}

		for(bufindex = 0; bufindex < req.count; bufindex++)
		{
			memset(&v4l2buf[bufindex], 0x00, sizeof(struct v4l2_buffer));
			//char *data = CMEM_alloc((720*480*2 + 4*1024) & (~0xFFF), &cmem_param);
			char *data = CMEM_alloc(831488, &cap_cmem_param);
			if(data == NULL)
			{
				SYS_ERROR("CMEM_alloc error.");
				goto error;
			}

			v4l2buf[bufindex].index		= bufindex;
			v4l2buf[bufindex].type		= V4L2_BUF_TYPE_VIDEO_CAPTURE;
			v4l2buf[bufindex].memory	= V4L2_MEMORY_USERPTR;
			v4l2buf[bufindex].m.userptr = (int)data;
			//v4l2buf[bufindex].length	=  (720*480*2 + 4*1024) & (~0xFFF);
			v4l2buf[bufindex].length	=  831488;
			//int len	=  (720*480*2 + 4*1024) & (~0xFFF);

			capBufs[bufindex].length	= v4l2buf[bufindex].length;
			capBufs[bufindex].physaddr	= CMEM_getPhys(data);
			capBufs[bufindex].index		= v4l2buf[bufindex].index;
			capBufs[bufindex].virtaddr	= (void*)v4l2buf[bufindex].m.userptr;

			////SYS_INFO("len %d\r\n",len);
			//SYS_INFO("index %d length %d physaddr %x virtaddr %x.\r\n",
			//		bufindex,v4l2buf[bufindex].length, v4l2buf[bufindex].m.offset, capBufs[bufindex].virtaddr);
			if (ioctl(phandle->capture_fd, VIDIOC_QBUF, &v4l2buf[bufindex]) < 0 )
			{
				SYS_ERROR("VIDIOC_QBUF failed.\r\n");
				goto error;
			}
		}

	}

	type = V4L2_BUF_TYPE_VIDEO_CAPTURE;
#if 1
	if (ioctl(phandle->capture_fd, VIDIOC_STREAMON, &type) < 0 )
	{
		SYS_ERROR("stream on failed.\r\n");
		goto error;
	}
#endif

	phandle->capBufsPtr = capBufs;
	phandle->v4l2buf = v4l2buf;
	phandle->capBufsNum = req.count;
	return phandle;

error:
	SYS_ERROR("Capture_init error.\r\n");
	if(phandle != NULL)
	{
		if(phandle->capture_fd > 0)
		{
			close(phandle->capture_fd);
		}
		free(phandle);
	}
	if(capBufs != NULL)
	{
		free(capBufs);
	}
	return NULL;
}

static int Capture_get(Capture_Handle phandle, CaptureBuffer *buffer)
{
	CaptureBuffer *capBufs = NULL;
	struct v4l2_buffer v4l2buf;

	if(buffer == NULL)
	{
		SYS_ERROR("The parameter CaptureBuffer can not be null.\r\n");
		return -1;
	}

	capBufs = phandle->capBufsPtr;

	memset(&v4l2buf, 0x00, sizeof(struct v4l2_buffer));
	v4l2buf.type = V4L2_BUF_TYPE_VIDEO_CAPTURE;
	v4l2buf.memory = phandle->userAlloc ? V4L2_MEMORY_USERPTR : V4L2_MEMORY_MMAP;

	if (ioctl(phandle->capture_fd, VIDIOC_DQBUF, &v4l2buf) < 0)
	{
		SYS_ERROR("capture_get VIDIOC_DQBUF failed.\r\n");
		return -1;
	}

	buffer->virtaddr = capBufs[v4l2buf.index].virtaddr;
	buffer->physaddr = capBufs[v4l2buf.index].physaddr;
	buffer->length	= capBufs[v4l2buf.index].length;
	buffer->index	= capBufs[v4l2buf.index].index;

	return 0;
}

static int Capture_put(Capture_Handle phandle, CaptureBuffer *buffer)
{
	struct v4l2_buffer *pbuf;

	pbuf = &phandle->v4l2buf[buffer->index];

	pbuf->type = V4L2_BUF_TYPE_VIDEO_CAPTURE;
	pbuf->m.userptr = buffer->virtaddr;

	if(ioctl(phandle->capture_fd, VIDIOC_QBUF, pbuf) < 0)
	{
		SYS_ERROR("capture_put vidioc_qbuf failed.\r\n");
		return -1;
	}

	return 0;
}

static int Capture_destroy(Capture_Handle phandle)
{
	if(phandle != NULL)
	{
		if(phandle->capture_fd > 0)
		{
			close(phandle->capture_fd);
		}

		if(phandle->v4l2buf != NULL)
		{
			free(phandle->v4l2buf);
		}

		if(phandle->capBufsPtr != NULL)
		{
			free(phandle->capBufsPtr);
		}

		free(phandle);
	}

	return 0;
}

Capture_Handle CAPTURE_INIT(CaptureAttr *attr)
{
	return Capture_init(attr);	
}


succeed_type CAPTURE_GET(Capture_Handle phandle,CaptureBuffer *buffer)
{

	return Capture_get(phandle, buffer);

}

succeed_type CAPTURE_PUT(Capture_Handle phandle,CaptureBuffer *buffer)
{

	return Capture_get(phandle,buffer);
}

succeed_type CAPTURE_DESTROY(Capture_Handle phandle)
{

	return Capture_destroy(phandle);
}

