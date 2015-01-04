#ifndef _INTERCMD_AUDIO_START_PRESET_H__
#define _INTERCMD_AUDIO_START_PRESET_H__

#ifdef __cplusplus
extern "C"{
#endif

typedef struct audio_start
{
	unsigned char audio_sendipaddr[32];
	unsigned int audio_port;
}audio_start;

#ifdef __cplusplus
}
#endif



#endif
