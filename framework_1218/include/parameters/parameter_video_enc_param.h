#ifndef __PARAMETER_VIDEO_ENC_PARAM_H__
#define __PARAMETER_VIDEO_ENC_PARAM_H__

#ifdef __cplusplus
extern "C"{
#endif

typedef enum _video_enc_mode
{
    VIDEO_ENC_MODE_START,
    VIDEO_ENC_H264,
    VIDEO_ENC_JPEG,
    VIDEO_ENC_MODE_END
}VIDEO_ENC_MODE;


typedef enum _video_input_mode
{
    VIDEO_INPUT_MODE_START,
    VIDEO_INPUT_PAL,
    VIDEO_INPUT_NTSC,
    VIDEO_INPUT_MODE_END
}VIDEO_INPUT_MODE;

typedef enum _video_enc_resolution
{
    VIDEO_ENC_RESOLUTION_START,
    VIDEO_ENC_D1,
    VIDEO_ENC_CIF,
    VIDEO_ENC_RESOLUTION_END
}VIDEO_ENC_RESOLUTION;


typedef enum _video_enc_framerate
{
    VIDEO_ENC_FRAMERATE_START,
    VIDEO_ENC_30FPS,
    VIDEO_ENC_25FPS,
    VIDEO_ENC_15FPS,
    VIDEO_ENC_7FPS,
    VIDEO_ENC_FRAMERATE_END
}VIDEO_ENC_FRAMERATE;


typedef struct video_enc_param
{
    VIDEO_ENC_MODE video_enc_mode;
    VIDEO_INPUT_MODE video_input_mode;
    VIDEO_ENC_RESOLUTION video_enc_resolution;
    VIDEO_ENC_FRAMERATE video_enc_framerate;
    unsigned int video_enc_bitrate;
    unsigned int keyFrameInterval;
}video_enc_param;

#ifdef __cplusplus
}
#endif



#endif
