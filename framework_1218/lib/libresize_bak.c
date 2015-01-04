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





Resize_Handle Resize_create(Resize_Attrs *attrs)
{
    Resize_Handle phandle;
	struct rsz_params           params;
	struct v4l2_requestbuffers  reqbuf;
	int                         hrsz, vrsz;
	int                         width, height;
	int                         rszRate,i;


	phandle = (Resize_Handle)calloc(1, sizeof(Resize_Object));

	if (phandle == NULL) {
		SYS_ERROR("Failed to allocate space for Resize Object\n");
		return NULL;
	}

	/* Open resizer device */
	phandle->fd = open(RESIZER_DEVICE, O_RDWR);

	if (phandle->fd == -1) {
		SYS_ERROR("Failed to open %s\n", RESIZER_DEVICE);
		Resize_delete(phandle);
		return NULL;
	}

	phandle->hWindowType = attrs->hWindowType;
	phandle->vWindowType = attrs->vWindowType;
	phandle->hFilterType = attrs->hFilterType;
	phandle->vFilterType = attrs->vFilterType;

	//Dmai_dbg4("Configuring resizer job resizing %dx%d to %dx%d\n",
	//		srcDim.width, srcDim.height, dstDim.width, dstDim.height);

	memset (&params, 0, sizeof(struct rsz_params));

	/* Set up the resizer job */
	params.in_hsize            = 720;
	params.in_vsize            = 480;
	params.in_pitch            = 720 * 2;
	params.cbilin              = 0;
	params.pix_fmt             = RSZ_PIX_FMT_UYVY;
	params.out_hsize           = 720;
	params.out_vsize           = 480;
	params.out_pitch           = 720 * 2;
	params.vert_starting_pixel = 0;
	params.horz_starting_pixel = 0;
	params.inptyp              = RSZ_INTYPE_YCBCR422_16BIT;
	params.hstph               = 0;
	params.vstph               = 0;
	params.yenh_params.type    = 0;
	params.yenh_params.gain    = 0;
	params.yenh_params.slop    = 0;
	params.yenh_params.core    = 0;

	/* As we are downsizing, we put */
	for (i = 0; i < 32; i++)
		params.tap4filt_coeffs[i] =
			gRDRV_reszFilter4TapHighQuality[i];

	for (i = 0; i < 32; i++)
		params.tap7filt_coeffs[i] =
			gRDRV_reszFilter7TapHighQuality[i];

	if (ioctl(phandle->fd, RSZ_S_PARAM, &params) == -1) {
		SYS_ERROR("Resize setting parameters failed.\n");
		goto error;
	}

	rszRate = 0;
	if (ioctl(phandle->fd, RSZ_S_EXP, &rszRate) == -1) {
		SYS_ERROR("Resize setting read cycle failed.\n");
		goto error;
	}

	phandle->inSize = 720 * 480 *2;
	phandle->outSize = 720 * 480 * 2;

	/* Request the resizer buffers */
	reqbuf.type     = V4L2_BUF_TYPE_VIDEO_CAPTURE;
	reqbuf.memory   = V4L2_MEMORY_USERPTR;
	reqbuf.count    = 2;

	if (ioctl(hResize->fd, RSZ_REQBUF, &reqbuf) == -1) {
		SYS_ERROR("Request buffer failed.\n");
		goto error;
	}

	return phandle;

error:
	if(phandle->fd > 0)
	{
		close(phandle->fd);
	}
	return NULL;
}
int Resize_execute(Resize_Handle hResize, unsigned long hSrcBuf, unsigned long hDstBuf);
{
	int                         i;
	struct v4l2_buffer          qbuf[2];



	/* Queue the resizer buffers */
	for (i=0; i < 2; i++) 
	{ 
		qbuf[i].type         = V4L2_BUF_TYPE_VIDEO_CAPTURE;
		qbuf[i].memory       = V4L2_MEMORY_USERPTR;
		qbuf[i].index        = i;

		if (ioctl (hResize->fd, RSZ_QUERYBUF, &qbuf[i]) == -1) {
			SYS_ERROR("Failed to query buffer index %d\n", i);
			return -1;
		}

		if (i == 0) {
			qbuf[i].m.userptr = hSrcBuf;
		}
		else {
			qbuf[i].m.userptr = hDstBuf;
		}

		if (ioctl (hResize->fd, RSZ_QUEUEBUF, &qbuf[i]) == -1) {
			SYS_ERROR("Failed to query buffer index %d\n", i);
			return -1;
		}
	}

	if (ioctl(hResize->fd, RSZ_RESIZE, NULL) == -1) {
		SYS_ERROR("Failed to execute resize job\n");
		return -1;
	}

	return 0;
}

/******************************************************************************
 * Resize_delete
 ******************************************************************************/
int Resize_destroy(Resize_Handle hResize)
{
	if (hResize) {
		if (hResize->fd != -1) {
			close(hResize->fd);
		}
	}

	free(hResize);

	return 0;
}

