#ifndef	_LIB_AUDIO_H 
#define	_LIB_AUDIO_H

#ifdef __cplusplus
extern "C"{
#endif

typedef enum _LIBAUDIO_MODE
{
	AUDIO_MODE_READ,
	AUDIO_MODE_WRITE
}LIBAUDIO_MODE;
typedef enum _LIBAUDIO_TYPE
{
	AUDIO_START,
	AUDIO_A711,
	AUDIO_U711,
	AUDIO_END
}AUDIO_TYPE;

typedef struct _LIBAUDIO_OBJ
{
    int audio_rfd;
    int audio_wfd;
    int audio_volfd;
    int reserved;
}LIBAUDIO_OBJ;

//retlen is the 711 length, 160
succeed_type AUDIO_READ(LIBAUDIO_OBJ *phandle, unsigned char *outbuf, AUDIO_TYPE audiotype, int *retlen);
succeed_type AUDIO_READ_FILE(LIBAUDIO_OBJ *phandle, unsigned char *outbuf, AUDIO_TYPE audiotype, int *retlen, FILE *fp);

//len 
succeed_type AUDIO_WRITE(LIBAUDIO_OBJ *phandle, unsigned char *outbuf, AUDIO_TYPE audiotype, int len );
succeed_type AUDIO_WRITE_FILE(LIBAUDIO_OBJ *phandle, unsigned char *outbuf, AUDIO_TYPE audiotype, int len, FILE *fp );


LIBAUDIO_OBJ *AUDIO_INIT(int mode);
succeed_type AUDIO_DESTROY(LIBAUDIO_OBJ *phandle, int mode);


#ifdef __cplusplus
}
#endif
#endif  
