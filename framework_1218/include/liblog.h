#ifndef	_LIB_LOG_H
#define	_LIB_LOG_H

#ifdef __cplusplus
extern "C"{
#endif

#define LOG_UINT_LEN 56

#define LOG_MAGIC_NUM 0x16800861
#define LOG_MAGIC_CLR 0x00000000

typedef struct _LIBLOG_UINT_STRUCT
{
	int log_cmd;//»’÷æ√¸¡Ó
	time_t log_time_t;
	unsigned char log_description[LOG_UINT_LEN];
}LIBLOG_UINT_STRUCT;

typedef struct _LIBLOG_OBJ
{
	int log_fd;
	unsigned int log_mmap;
	unsigned int log_phyaddr;
	int log_numsize;
	int reserved;
}LIBLOG_OBJ;


LIBLOG_OBJ *LOG_INIT();

succeed_type LOG_DESTROY(LIBLOG_OBJ *phandle);

succeed_type LOG_READ(LIBLOG_OBJ *phandle, int log_seq, void *ptr, int length);

succeed_type LOG_WRITE(LIBLOG_OBJ *phandle, void *ptr, int length);

#ifdef __cplusplus
}
#endif
#endif
