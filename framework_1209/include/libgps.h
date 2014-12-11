#ifndef	_LIB_GPS_H 
#define	_LIB_GPS_H

#ifdef __cplusplus
extern "C"{
#endif
#define CMD_LENGTH 128

typedef struct _LIBGPS_OBJ
{
    int gps_fd;
    int reserved;
}LIBGPS_OBJ;

LIBGPS_OBJ *GPS_INIT();
succeed_type GPS_DESTROY(LIBGPS_OBJ *phandle);

#ifdef __cplusplus
}
#endif
#endif  
