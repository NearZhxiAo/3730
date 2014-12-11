#ifndef _LIBASSHM_H_
#define _LIBASSHM_H_

#ifdef __cplusplus
extern "C"{
#endif

#include <semaphore.h>

#define ASSHM_UINT_LEN 1*1024

#define ASSHM_SEM_MAX 2

#define ASSHM_MAGIC_NUM 0x16888861
#define ASSHM_MAGIC_CLR 0x00000000

typedef enum _LIBASSHM_FRAME_TYPE 
{
	FRAME_TYPE_AUDIO_START,
	FRAME_TYPE_AUDIO_A711,
	FRAME_TYPE_AUDIO_U711,
	FRAME_TYPE_AUDIO_END,
}LIBASSHM_FRAME_TYPE;

typedef struct _LIBASSHM_UINT_HEADER
{
	LIBASSHM_FRAME_TYPE frame_type;
	long long timestamp;
	long data_len;
	int reserved;

}LIBASSHM_UINT_HEADER;

typedef struct _LIBASSHM_UINT_DATA
{	
	LIBASSHM_UINT_HEADER asshm_header;
	unsigned char asshm_context[ASSHM_UINT_LEN];
}LIBASSHM_UINT_DATA;

typedef struct _LIBASSHM_UINT_STRUCT
{
	sem_t asshm_sem_t;
	LIBASSHM_UINT_DATA asshm_data;
}LIBASSHM_UINT_STRUCT;

typedef struct _LIBASSHM_OBJ
{
	int asshm_fd;
	unsigned int asshm_mmap;
	unsigned int asshm_phyaddr;
	int asshm_size;
	int reserved;
}LIBASSHM_OBJ;

//FUNC init for asshm
LIBASSHM_OBJ *ASSHM_INIT();

//FUNC destroy asshm
succeed_type ASSHM_DESTROY(LIBASSHM_OBJ *phandle);

//IN  phandle ASSHM_INIT return
//IN  asshm_seq ASSHM_INIT buffer number
//IN  i_p_flag i-0 p-1
//IN  video_type h264
//IN  ptr    put original data ( vedio data/audio data)
//IN  length put original data len
//FUNC PUT the ptr data in the buffer[asshm_seq]
succeed_type ASSHM_PUT(LIBASSHM_OBJ *phandle, int asshm_seq, LIBASSHM_FRAME_TYPE frame_type, void *ptr, int length);

//IN  phandle ASSHM_INIT return
//IN  asshm_seq ASSHM_INIT buffer number
//OUT ptr_sem the ASSHM buffer sem_t 0/1/2...
//FUNC get the sem_t in the buffer[asshm_seq]
succeed_type ASSHM_GET_SEM(LIBASSHM_OBJ *phandle, int asshm_seq, int* ptr_sem);

//IN  phandle ASSHM_INIT return
//IN  asshm_seq ASSHM_INIT buffer number
//OUT ptr_data the ASSHM buffer data 
//FUNC get the data in the buffer[asshm_seq]
succeed_type ASSHM_GET_DATA(LIBASSHM_OBJ *phandle, int asshm_seq, void *ptr_data);

//IN  phandle ASSHM_INIT return
//IN  asshm_seq ASSHM_INIT buffer number
//FUNC waittry the sem_t in the buffer[asshm_seq]
succeed_type ASSHM_GET_COMPLETE(LIBASSHM_OBJ *phandle, int asshm_seq);

#ifdef __cplusplus
}
#endif

#endif

