#ifndef __PARAMETER_ALERT_MOTION_PARAM_H__
#define __PARAMETER_ALERT_MOTION_PARAM_H__

#ifdef __cplusplus
extern "C"{
#endif

typedef enum _MOTION_SWITCH
{
    MOTION_SWITCH_START,

    MOTION_SWITCH_ON,
    MOTION_SWITCH_OFF,
    MOTION_SWITCH_END
}MOTION_SWITCH;

typedef enum _MOTION_ACTION_DEF
{
    MOTION_ACTION_START,
    MOTION_ACTION_STORAGE,
    MOTION_ACTION_END
}MOTION_ACTION_DEFINE;

typedef struct alert_motion_param
{
    MOTION_SWITCH motion_switch;
    unsigned int motion_delay_sec;
    unsigned int motion_action;	//bit define
}alert_motion_param;


#ifdef __cplusplus
}
#endif



#endif
