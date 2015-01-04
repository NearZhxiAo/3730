#ifndef	_AV_ENCODER_PRIVATE_H__ 
#define _AV_ENCODER_PRIVATE_H__
#ifdef __cplusplus
extern "C"{
#endif

#include "system_includes.h"
#include "system_debug.h"
#include <linux/ioctl.h>
//#include <linux/videodev2.h>
#include <sys/time.h>
#include <asm/types.h>
#include <assert.h>
#include "libad.h"
#include "libgbshm.h"
#include "libunix.h"
#include "libvsshm.h"
#include "libresize.h"
#include "libasshm.h"
#include "libsignal.h"
#include "libenc.h" 
#include "libvidctl.h"
#include "libi2c.h"
#include "libaudio.h"
#include "libcpuinfo.h"
#include "libcapture.h"
#include "libdisplay.h"
//#include "libalarm_alg.h"

#define AV_ENCODER_POLL_COUNT_MAX       	2
#define I2CADDR                            0xB8
#define BRITNESSCTRLREGADDR                0x09
#define COLORSATURATIONCTRLREGADDR         0x0A
#define HUECTRLREGADDR                     0x0B
#define CONTRASTCTRLREGADDR                0x0C


#define CMEM_SIZE(x, y) (x * y * 2)


typedef unsigned short Uint16;

typedef enum {
		VideoStd_AUTO = 0,      /**< Automatically select standard (if supported) */
		VideoStd_CIF,           /**< CIF @ 30 frames per second */
		VideoStd_SIF_NTSC,      /**< SIF @ 30 frames per second */
		VideoStd_SIF_PAL,       /**< SIF @ 25 frames per second */
		VideoStd_VGA,           /**< VGA (640x480) @ 60 frames per second */
		VideoStd_D1_NTSC,       /**< D1 NTSC @ 30 frames per second */
		VideoStd_D1_PAL,        /**< D1 PAL @ 25 frames per second */
		VideoStd_480P,          /**< D1 Progressive NTSC @ 60 frames per second */
		VideoStd_576P,          /**< D1 Progressive PAL @ 50 frames per second */
		VideoStd_720P_60,       /**< 720P @ 60 frames per second */
		VideoStd_720P_50,       /**< 720P @ 50 frames per second */
		VideoStd_720P_30,       /**< 720P @ 30 frames per second */
		VideoStd_1080I_30,      /**< 1080I @ 30 frames per second */
		VideoStd_1080I_25,      /**< 1080I @ 25 frames per second */
		VideoStd_1080P_30,      /**< 1080P @ 30 frames per second */
		VideoStd_1080P_25,      /**< 1080P @ 25 frames per second */
		VideoStd_1080P_24,      /**< 1080P @ 24 frames per second */
		VideoStd_COUNT
} VideoStd_Type;

typedef enum __FRAME_TYPE
{
	IVIDEO_I_FRAME = 3,
	IVIDEO_P_FRAME = 1
}VIDEO_FRAME_TYPE;

typedef enum _AVENC_STATE
{
		AVENC_START,
		AVENC_IDLE,    
		AVENC_ENC,
		AVENC_DEC,
		AVENC_END
}AVENC_STATE;

typedef enum _CAMERA_INPUT_TYPE
{
	INPUT_START = 0,
	INPUT_CCD,
	INPUT_INFRARED,
	INPUT_END
}CAMERA_INPUT_TYPE;

#if 0
typedef enum _ALARM_STATE
{
		ALARM_YES=0,
		ALARM_NO=1
}ALARM_STATE;
#endif

typedef struct __videoStatus
{
	unsigned int bytesGenerated;
	unsigned int isKeyFrame;
}videoStatus;


typedef struct _AV_ENCODER_PRIVATE_HANDLE
{

		/*************SHM_OBJ define******************/
		LIBGBSHM_OBJ *pgbshm_obj;
		LIBVSSHM_OBJ *pvsshm_obj;
		LIBASSHM_OBJ *passhm_obj;
		LIBVIDCTL_OBJ *pvidctl_obj;
		LIBI2C_OBJ *pi2c_obj;

		/*************Handle define******************/
		Capture_Handle 		capHandle;      
		Resize_Handle 		rszHandle;
		Display_Handle 		dispHandle;
		//buffer define
		CaptureBuffer 		capBuf;
		DisplayBuffer 		disBuf;
		//attr define
		CaptureAttr 		capAttr;
		DisplayAttr			disAttr;
		ResizeAttr			rszAttr;

		/***************CMEM define***********************/
		CMEM_AllocParams    cmem_params;
		char                *jpeginbuffer;
		char 				*jpegoutbuffer;

		char                *videoinbuffer;
		char 				*videooutbuffer;


		/*************fd define******************/
		int unix_fd;
		int capture_fd;
		int display_fd;
		int resize_fd;
		/*************video parameters define******************/
		unsigned int width; //picture width
		unsigned int height;//picture height
		unsigned int framerate;
		unsigned int bitrate;
		unsigned int intra;  //key frame
		unsigned int qvalue; // 80~100 

		unsigned int resize_enable;   //0--disable, 1---enable
		unsigned int cam_input_switch;
		CAMERA_INPUT_TYPE cam_input_type;
		unsigned int Is_jpeg_pic; //is jpeg pic

	
		struct pollfd polltimeout[AV_ENCODER_POLL_COUNT_MAX];
		int active_fdcount;
		video_enc_param videoencparam; //???

		AVENC_STATE current_state;

		

		unsigned int frame_count; //采集总数
		unsigned int start_timestamp; //第一采集时间

		pthread_mutex_t flip_mutex;
		char *sync_buffer;
		int sync_count;

}AV_ENCODER_PRIVATE_HANDLE;

extern pthread_mutex_t mtx ;
extern pthread_cond_t  cont ;


extern AV_ENCODER_PRIVATE_HANDLE global_av_encoder_private_handle;

void *pthread_rezRun(void *data);
void av_encoder_poll_init(AV_ENCODER_PRIVATE_HANDLE *phandle);
succeed_type av_encoder_privatehandle_init(AV_ENCODER_PRIVATE_HANDLE *phandle);
succeed_type av_encoder_vidEncRunMain(AV_ENCODER_PRIVATE_HANDLE  *phandle,FILE *fp);
succeed_type av_encoder_init(AV_ENCODER_PRIVATE_HANDLE *phandle);
succeed_type av_encoder_unix(AV_ENCODER_PRIVATE_HANDLE *phandle);
succeed_type pthread_audio(AV_ENCODER_PRIVATE_HANDLE  *phandle);

void *video_enc(void *args);


struct buffer_private {
	void *start;
	size_t length;
	int index;
	int offset;
};
//#include <asm/arch/imp_previewer.h>
//#include <asm/arch/imp_resizer.h>
//#include <asm/arch/dm365_ipipe.h>
#if 0
extern struct imp_buffer rsz_buf_in[];
extern struct imp_buffer rsz_buf_out1[];
extern struct imp_buffer rsz_buf_out2[];
#endif
#define WDQ_VERSION
#define round_32(width)		((((width) + 31) / 32) * 32 )
/* D1 screen dimensions */
#define VID0_WIDTH		800 //720
#define VID0_HEIGHT		480
#define VID0_BPP		16
#define VID0_FRAME_SIZE		(VID0_WIDTH*VID0_HEIGHT)
#define VID0_VMODE		FB_VMODE_INTERLACED

#define VID1_WIDTH 		720
#define VID1_HEIGHT		480
#define VID1_BPP		16
#define VID1_FRAME_SIZE		(VID1_WIDTH*VID1_HEIGHT)
#define VID1_VMODE		FB_VMODE_INTERLACED

#define OSD0_BPP		4
#define	OSD0_WIDTH		(round_32(240*OSD0_BPP/8) * 8/OSD0_BPP)
#define	OSD0_HEIGHT		120
#define OSD0_FRAME_SIZE		(OSD0_WIDTH*OSD0_HEIGHT)
#define OSD0_VMODE		FB_VMODE_INTERLACED

#define OSD1_BPP		8
#define	OSD1_WIDTH		(round_32(240*OSD1_BPP/8) * 8/OSD1_BPP)
#define	OSD1_HEIGHT		120
#define OSD1_FRAME_SIZE		(OSD1_WIDTH*OSD1_HEIGHT)
#define OSD1_VMODE		FB_VMODE_INTERLACED

/* position */
#define	OSD0_XPOS		0
#define	OSD0_YPOS		0
#define	OSD1_XPOS		300
#define	OSD1_YPOS		250
#define	VID0_XPOS		0
#define	VID0_YPOS		0
#define	VID1_XPOS		0
#define	VID1_YPOS		0

/* Zoom Params */
#define	OSD0_HZOOM		0
#define	OSD0_VZOOM		0
#define	OSD1_HZOOM		0
#define	OSD1_VZOOM		0
#define	VID0_HZOOM		0
#define	VID0_VZOOM		0
#define	VID1_HZOOM		0
#define	VID1_VZOOM		0

/* OSD window blend factor */
#define OSD1_WINDOW_BF		0
#define OSD1_WINDOW_CK		0
#define OSD1_CK			0
#define OSD0_WINDOW_BF		3
#define OSD0_WINDOW_CK		0
#define OSD0_CK			0

#define VIDEO_NUM_BUFS		3
#define OSD_NUM_BUFS		2
#define RED_COLOR 		249
#define BLUE_COLOR 		140	//blue color
#define RAM_CLUT_IDX 		0xFF
#define BITMAP_COLOR		0x11

#define CURSOR_XPOS		100
#define CURSOR_YPOS		100
#define CURSOR_XRES		50
#define CURSOR_YRES		50
#define CURSOR_THICKNESS	1
#define CURSOR_COLOR		0xF9

#define ATTR_BLINK_INTERVAL	1
#define ATTR_BLEND_VAL 		0xaa

#define ATTRIB_MODE		"mode"
#define ATTRIB_OUTPUT		"output"

#define DISPLAY_INTERFACE	"COMPOSITE"
#define DISPLAY_MODE		"NTSC"
struct vpbe_test_info {
	int vid0_width;
	int vid0_height;
	int vid0_bpp;
	int vid0_frame_size;
	int vid0_vmode;

	int vid1_width;
	int vid1_height;
	int vid1_bpp;
	int vid1_frame_size;
	int vid1_vmode;

	int osd0_bpp;
	int osd0_width;
	int osd0_height;
	int osd0_frame_size;
	int osd0_vmode;

	int osd1_bpp;
	int osd1_width;
	int osd1_height;
	int osd1_frame_size;
	int osd1_vmode;

	int osd0_xpos;
	int osd0_ypos;
	int osd1_xpos;
	int osd1_ypos;
	int vid0_xpos;
	int vid0_ypos;
	int vid1_xpos;
	int vid1_ypos;

	int osd0_hzoom;
	int osd0_vzoom;
	int osd1_hzoom;
	int osd1_vzoom;
	int vid0_hzoom;
	int vid0_vzoom;
	int vid1_hzoom;
	int vid1_vzoom;

	int osd1_window_bf;
	int osd1_window_ck;
	int osd1_ck;
	int osd0_window_bf;
	int osd0_window_ck;
	int osd0_ck;
	char display_interface[32];
	char display_mode[32];

	int osd0_coloridx;
	int osd1_coloridx;
	int ram_clut_idx;
	int bitmap_color;

	int cursor_xpos;
	int cursor_ypos;
	int cursor_xres;
	int cursor_yres;
	int cursor_thickness;
	int cursor_color;

	int attr_blink_interval;
	int attr_blend_val;
};
#ifdef __cplusplus
}
#endif
#endif  
