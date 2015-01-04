#include "libresize.h"

/* For resizing between */
short gRDRV_reszFilter4TapHighQuality[RDRV_RESZ_SPEC__MAX_FILTER_COEFF]= {
        0, 256, 0, 0, -6, 246, 16, 0, -7, 219, 44, 0, -5, 179, 83, -1, -3,
        130, 132, -3, -1, 83, 179, -5, 0, 44, 219, -7, 0, 16, 246, -6
};

short gRDRV_reszFilter7TapHighQuality[RDRV_RESZ_SPEC__MAX_FILTER_COEFF] = {
        -1, 19, 108, 112, 19, -1, 0, 0, 0, 6, 88, 126, 37, -1, 0, 0,
        0, 0, 61, 134, 61, 0, 0, 0, 0, -1, 37, 126, 88, 6, 0, 0
};



static Resize_Handle Resize_init(ResizeAttr *attr)
{
	Resize_Handle phandle;
	struct rsz_params 	params;
	struct v4l2_requestbuffers reqbuf;
	int i;
	int rszRate;

    phandle = (Resize_Handle)calloc(1, sizeof(Resize_Object));

    if (phandle == NULL) {
        SYS_ERROR("Failed to allocate space for Resize_Object\n");
		goto error;
    }

    phandle->resize_fd = open(RESIZER_DEVICE, O_RDWR);

    if (phandle->resize_fd == -1) {
        SYS_ERROR("Failed to open %s\n", RESIZER_DEVICE);
		goto error;
    }


    phandle->hWindowType = attr->hWindowType;
    phandle->vWindowType = attr->vWindowType;
    phandle->hFilterType = attr->hFilterType;
    phandle->vFilterType = attr->vFilterType;

    params.in_hsize            = 720;
    params.in_vsize            = 576;
    params.in_pitch            = 720 * 2;
    params.cbilin              = 0;
    params.pix_fmt             = RSZ_PIX_FMT_UYVY;
    params.out_hsize           = attr->out_width;
	if (params.out_hsize > 720)
	{
		params.out_pitch           = attr->out_width * 2;
		params.out_vsize           = attr->out_height;
	}
	else
	{
		params.out_pitch = 720 * 2;
		params.out_vsize = attr->out_height;
	}

	params.vert_starting_pixel = 0;
	params.horz_starting_pixel = 0;
	params.inptyp              = RSZ_INTYPE_YCBCR422_16BIT;
	params.hstph               = 0;
	params.vstph               = 0;
	params.yenh_params.type    = 0;
	params.yenh_params.gain    = 0;
	params.yenh_params.slop    = 0;
	params.yenh_params.core    = 0;

	for (i = 0; i < 32; i++)
		params.tap4filt_coeffs[i] =
			gRDRV_reszFilter4TapHighQuality[i];

	for (i = 0; i < 32; i++)
		params.tap7filt_coeffs[i] =
			gRDRV_reszFilter7TapHighQuality[i];

	if (ioctl(phandle->resize_fd, RSZ_S_PARAM, &params) == -1) {
		SYS_ERROR("Resize setting parameters failed.\n");
		goto error;
	}

	rszRate = 0;
	if (ioctl(phandle->resize_fd, RSZ_S_EXP, &rszRate) == -1) {
		SYS_ERROR("Resize setting read cycle failed.\n");
		goto error;
	}


	/* Request the resizer buffers */
	reqbuf.type     = V4L2_BUF_TYPE_VIDEO_CAPTURE;
	reqbuf.memory   = V4L2_MEMORY_USERPTR;
	//reqbuf.memory   = V4L2_MEMORY_MMAP;
	reqbuf.count    = 2;

	if (ioctl(phandle->resize_fd, RSZ_REQBUF, &reqbuf) == -1) {
		SYS_ERROR("Request buffer failed.\n");
		goto error;
	}

	return phandle;

error:
	if(phandle->resize_fd > 0)
	{
		close(phandle->resize_fd);
	}

	return NULL;
}

static int Resize_exec(Resize_Handle phandle, unsigned long physInbuf, unsigned long physOutbuf)
{
	int i;
	struct v4l2_buffer qbuf[2];

	for (i=0; i < 2; i++) { 
		qbuf[i].type         = V4L2_BUF_TYPE_VIDEO_CAPTURE;
		qbuf[i].memory       = V4L2_MEMORY_USERPTR;
		//qbuf[i].memory       = V4L2_MEMORY_MMAP;
		qbuf[i].index        = i;

		if (ioctl (phandle->resize_fd, RSZ_QUERYBUF, &qbuf[i]) == -1) {
			SYS_ERROR("Failed to query buffer index %d\n", i);
			return -1;
		}

		if (i == 0) {
			qbuf[i].m.userptr = physInbuf;
		}
		else {
			qbuf[i].m.userptr = physOutbuf;
		}

		if (ioctl (phandle->resize_fd, RSZ_QUEUEBUF, &qbuf[i]) == -1) {
			SYS_ERROR("Failed to queue buffer index %d\n",i);
			return -1;
		}
	}

	if (ioctl(phandle->resize_fd, RSZ_RESIZE, NULL) == -1) {
		SYS_ERROR("Failed to execute resize job\n");
		return -1;
	}


	return 0;
}

static int Resize_destroy(Resize_Handle phandle)
{
	if(phandle != NULL)
	{
		if(phandle->resize_fd > 0)
		{
			close(phandle->resize_fd);
		}

		free(phandle);
	}

	return 0;
}


Resize_Handle RESIZE_INIT(ResizeAttr *attr)
{
	return Resize_init(attr);
}


int RESIZE_EXEC(Resize_Handle phandle, unsigned long physInbuf, unsigned long physOutbuf)
{
	return Resize_exec(phandle, physInbuf, physOutbuf);
}



int RESIZE_DESTROY(Resize_Handle phandle)
{
	return	Resize_destroy(phandle);
}
