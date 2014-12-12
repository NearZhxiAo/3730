#ifndef __PARAMETER_RUNTIME_DEVICE_STATUS_H__
#define __PARAMETER_RUNTIME_DEVICE_STATUS_H__

#ifdef __cplusplus
extern "C"{
#endif

typedef enum _DEVICE_STATUS
{
    DEVICE_STATUS_START,
    DEVICE_STATUS_ON,
    DEVICE_STATUS_OFF,
    DEVICE_STATUS_END
}DEVICE_STATUS;

typedef struct runtime_device_status
{
    DEVICE_STATUS device_status_online; //3G
    DEVICE_STATUS device_status_di;     //di
    DEVICE_STATUS device_status_video;  //采集
    DEVICE_STATUS device_status_storage;//存储
}runtime_device_status;


#ifdef __cplusplus
}
#endif



#endif
