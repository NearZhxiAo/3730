/*
 * video.h
 *
 * ============================================================================
 * Copyright (c) Texas Instruments Incorporated 2010
 *
 * Use of this software is controlled by the terms and conditions found in the
 * license agreement under which this software has been supplied or provided.
 * ============================================================================
 */

#ifndef _VIDEO_H
#define _VIDEO_H


/* Thread function prototype */
extern void H264_INIT(char *inbuf,char *outbuf, int _width, int _height, int _framerate, int _bitrate, int _intra);
extern void H264_ENCODE(int *len);
extern void H264_DESTROY();

#endif /* _VIDEO_H */
