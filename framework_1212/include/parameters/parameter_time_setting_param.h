#ifndef __PARAMETER_TIME_SETTING_PARAM_H__
#define __PARAMETER_TIME_SETTING_PARAM_H__

#ifdef __cplusplus
extern "C"{
#endif

typedef enum _TIME_SETTING_MODE
{
    TIME_SETTING_MODE_START,
    TIME_SETTING_MODE_NORMAL,
    TIME_SETTING_MODE_NTP,
    TIME_SETTING_MODE_END
}TIME_SETTING_MODE;

typedef struct time_setting_param
{
    TIME_SETTING_MODE time_setting_mode;
    unsigned int time_setting_year;
    unsigned int time_setting_mon;
    unsigned int time_setting_day;
    unsigned int time_setting_hour;
    unsigned int time_setting_min;
    unsigned int time_setting_sec;
   

}time_setting_param;


#ifdef __cplusplus
}
#endif



#endif
