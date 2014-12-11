#ifndef	_LIB_SIGNAL_H 
#define	_LIB_SIGNAL_H

#ifdef __cplusplus
extern "C"{
#endif

//信号量注册最大数目
#define SIGNAL_NUM 16
//处理函数类型定义
typedef void (*sig_phandle)(int);
//信号量注册结构体
typedef struct _SIGNAL_STRUCT_T
{
	int signo[SIGNAL_NUM]; //信号量值 通过kill -l可以查看得到
	sig_phandle sig_phandle[SIGNAL_NUM]; //相应处理函数定义
}SIGNAL_STRUCT_T;

#ifdef __cplusplus
}
#endif
#endif  
