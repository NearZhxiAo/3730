#ifndef __PARAMETER_RUNTIME_DEVICE_VERSION_H__
#define __PARAMETER_RUNTIME_DEVICE_VERSION_H__

#ifdef __cplusplus
extern "C"{
#endif

#define VERSION_LENGTH	64
typedef struct runtime_device_version
{
    unsigned char device_version_hardware[VERSION_LENGTH];
    unsigned char device_version_software[VERSION_LENGTH];
}runtime_device_version;


#ifdef __cplusplus
}
#endif



#endif
