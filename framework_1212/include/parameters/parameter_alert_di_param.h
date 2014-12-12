#ifndef __PARAMETER_ALERT_DI_PARAM_H__
#define __PARAMETER_ALERT_DI_PARAM_H__

#ifdef __cplusplus
extern "C"{
#endif

typedef enum _DI_SWITCH
{
    DI_SWITCH_START,

    DI_SWITCH_ON,
    DI_SWITCH_OFF,
    DI_SWITCH_END
}DI_SWITCH;

typedef enum _DI_ACTION_DEF
{
    DI_ACTION_START,
    DI_ACTION_STORAGE,
    DI_ACTION_END
}DI_ACTION_DEFINE;

typedef struct alert_di_param
{
    DI_SWITCH di_switch;
    unsigned int di_delay_sec;
    unsigned int di_action;	//bit define
}alert_di_param;


#ifdef __cplusplus
}
#endif



#endif
