#ifndef __PARAMETER_STORAGE_SETTING_PARAM_H__
#define __PARAMETER_STORAGE_SETTING_PARAM_H__

#ifdef __cplusplus
extern "C"{
#endif

typedef enum _STORAGE_SWITCH
{
    STORAGE_SWITCH_START,
    STORAGE_SWITCH_ON,
    STORAGE_SWITCH_OFF,
    STORAGE_SWITCH_END
}STORAGE_SWITCH;

/*存储策略，一直存储？事件存储？还是定时存储？*/
typedef enum _STORAGE_METHOD
{
    STORAGE_METHOD_START,
    STORAGE_METHOD_ALWAYS,	//一直存储
    STORAGE_METHOD_EVENT,	//有事件才存储
    STORAGE_METHOD_SCHEDULE,	//使用时间设置进行存储
    STORAGE_METHOD_END
}STORAGE_METHOD;

typedef struct storage_setting_param
{
    unsigned int storage_pre_record_time;	//预录时间参数，实际使用中表示的是以秒为单位
    STORAGE_SWITCH storage_switch;	
    STORAGE_METHOD storage_method;
    unsigned int storage_schedule_setting[7];  //当存储策略为SCHEDULE的时候的设置  
}storage_setting_param;


#ifdef __cplusplus
}
#endif



#endif
