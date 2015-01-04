#ifndef __PARAMETER_ALERT_VLOSS_PARAM_H__
#define __PARAMETER_ALERT_VLOSS_PARAM_H__

#ifdef __cplusplus
extern "C"{
#endif

typedef enum _VLOSS_SWITCH
{
    VLOSS_SWITCH_START,

    VLOSS_SWITCH_ON,
    VLOSS_SWITCH_OFF,
    VLOSS_SWITCH_END
}VLOSS_SWITCH;

typedef enum _VLOSS_ACTION_DEF
{
    VLOSS_ACTION_START,
    VLOSS_ACTION_STORAGE,
    VLOSS_ACTION_END
}VLOSS_ACTION_DEFINE;

typedef struct alert_vloss_param
{
    VLOSS_SWITCH vloss_switch;
    unsigned int vlosee_delay_sec;
    unsigned int vloss_action;	//bit define
}alert_vloss_param;


#ifdef __cplusplus
}
#endif



#endif
