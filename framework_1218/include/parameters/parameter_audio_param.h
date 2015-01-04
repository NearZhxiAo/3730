#ifndef __PARAMETER_AUDIO_PARAM_H__
#define __PARAMETER_AUDIO_PARAM_H__

#ifdef __cplusplus
extern "C"{
#endif

typedef enum _audio_mode
{
    AUDIO_MODE_START,
    AUDIO_MODE_AMR_G711,
    AUDIO_MODE_AAC_G711,
    AUDIO_MODE_A711,
    AUDIO_MODE_U711,
    AUDIO_MODE_END
}AUDIO_MODE;


typedef struct audio_param
{
    AUDIO_MODE audio_mode;
}audio_param;

#ifdef __cplusplus
}
#endif



#endif
