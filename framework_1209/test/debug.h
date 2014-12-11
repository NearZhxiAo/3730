/*
   c* =====================================================================================
 *       Copyright (C), 2008, islab@ZJU
 *       文件名     :  dvs_debug.h
 *       文件描述   :  DVS File Header
 *       版本信息   :  1.0
 *       创建日期   :  2007年12月21日 22时18分05秒 CST
 *       版本记录   :  none
 *       编译器     :  arm_v5t_le-gcc/g++
 *       作者信息   :  Dr Islab (Islab), islab@zju.edu.cn.
 *       公司机构   :  ZJU Digital ISLab.
 *       功能说明   :
 * =====================================================================================
 */
#ifndef	_SYSTEM_DEBUG_H 
#define	_SYSTEM_DEBUG_H

#ifdef __cplusplus
extern "C"{
#endif

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/poll.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <unistd.h>
#include <errno.h>

typedef enum succeed_type_type
{
    succeed_type_failed  = -1,
    succeed_type_succeed, 
}succeed_type;

/*调试开关*/
#define SYS_INFO_ON         1
#define SYS_INFO_OFF        0

#define SYS_ERROR_FLAG      SYS_INFO_OFF
#define SYS_ERROR_FLAG1      0
#define SYS_WARING_FLAG     SYS_INFO_OFF
#define SYS_ALERT_FLAG     	SYS_INFO_OFF
#define SYS_DEBUG_FLAG      SYS_INFO_OFF
#define SYS_LOG_FLAG        SYS_INFO_OFF
/*标准输入输出，方便重定向*/
#define SYS_STDIN       stdin

#define SYS_STDOUT      stdout

#define SYS_STDERR      stderr

#define SYS_INFO( format, args... ) \
{ \
    if( 1 == SYS_ERROR_FLAG1 ) \
    { \
	fprintf( SYS_STDERR, "[%s : %u]  ", __FILE__, __LINE__ ); \
	fprintf( SYS_STDERR, format, ##args ); \
	fprintf( SYS_STDERR, "\n" ); \
    } \
}

#if 0
/*断言错误的输出，文件名，行号，函数名 */
#define SYS_ASSERT_ERROR( file_name, line_no, func_name ) do \
{ \
    fprintf( SYS_STDERR, "\nAssert failed: %s, line %u, func: %s\n", \
		file_name, line_no, func_name ); \
} while( 0 )

/*条件判断的断言*/
#define SYS_ASSERT( condition ) do \
{ \
    if( !(condition) ) \
    { \
	SYS_ASSERT_ERROR( __FILE__, __LINE__, __PRETTY_FUNCTION__); \
    } \
} while( 0 )

/*调试定位当前位置*/
#define SYS_POS  fprintf( SYS_STDOUT, "\nPos: %s, line %u\n", __FILE__, __LINE__ )


#define SYS_INFO( format, args... ) \
{ \
    if( 1 == SYS_ERROR_FLAG1 ) \
    { \
	fprintf( SYS_STDERR, "[%s : %u]  ", __FILE__, __LINE__ ); \
	fprintf( SYS_STDERR, format, ##args ); \
	fprintf( SYS_STDERR, "\n" ); \
    } \
}

/*Error信息输出*/
#define SYS_ERROR( format, args... ) do \
{ \
    if( 1 == SYS_ERROR_FLAG ) \
    { \
	fprintf( SYS_STDERR, "\n[Error]: " ); \
	SYS_INFO( format, ##args ); 
    } \
} while( 0 )

/*Warning 信息输出*/
#define SYS_WARNING( format, args... ) do \
{ \
    if( 1 == SYS_WARING_FLAG ) \
    { \
	fprintf( SYS_STDERR, "\n[Waring]: " ); \
	SYS_INFO( format, ##args ); \
    } \
} while( 0 )

/*Alert 信息输出*/
#define SYS_ALERT( format, args... ) do \
{ \
    if( 1 == SYS_ALERT_FLAG ) \
    { \
	fprintf( SYS_STDERR, "\n[Alert]: " ); \
	SYS_INFO( format, ##args ); \
    } \
} while( 0 )

#define SYS_DEBUG( format, args... ) do \
{ \
    if( 1 == SYS_DEBUG_FLAG ) \
    { \
	fprintf( SYS_STDERR, "\n[Debug]: " ); \
	SYS_INFO( format, ##args ); \
    } \
} while( 0 )
#endif
#ifdef __cplusplus
}
#endif
#endif   /* ----- #ifndef DVS_DEBUG_INC  ----- */


