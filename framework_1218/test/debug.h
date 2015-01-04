/*
   c* =====================================================================================
 *       Copyright (C), 2008, islab@ZJU
 *       �ļ���     :  dvs_debug.h
 *       �ļ�����   :  DVS File Header
 *       �汾��Ϣ   :  1.0
 *       ��������   :  2007��12��21�� 22ʱ18��05�� CST
 *       �汾��¼   :  none
 *       ������     :  arm_v5t_le-gcc/g++
 *       ������Ϣ   :  Dr Islab (Islab), islab@zju.edu.cn.
 *       ��˾����   :  ZJU Digital ISLab.
 *       ����˵��   :
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

/*���Կ���*/
#define SYS_INFO_ON         1
#define SYS_INFO_OFF        0

#define SYS_ERROR_FLAG      SYS_INFO_OFF
#define SYS_ERROR_FLAG1      0
#define SYS_WARING_FLAG     SYS_INFO_OFF
#define SYS_ALERT_FLAG     	SYS_INFO_OFF
#define SYS_DEBUG_FLAG      SYS_INFO_OFF
#define SYS_LOG_FLAG        SYS_INFO_OFF
/*��׼��������������ض���*/
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
/*���Դ����������ļ������кţ������� */
#define SYS_ASSERT_ERROR( file_name, line_no, func_name ) do \
{ \
    fprintf( SYS_STDERR, "\nAssert failed: %s, line %u, func: %s\n", \
		file_name, line_no, func_name ); \
} while( 0 )

/*�����жϵĶ���*/
#define SYS_ASSERT( condition ) do \
{ \
    if( !(condition) ) \
    { \
	SYS_ASSERT_ERROR( __FILE__, __LINE__, __PRETTY_FUNCTION__); \
    } \
} while( 0 )

/*���Զ�λ��ǰλ��*/
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

/*Error��Ϣ���*/
#define SYS_ERROR( format, args... ) do \
{ \
    if( 1 == SYS_ERROR_FLAG ) \
    { \
	fprintf( SYS_STDERR, "\n[Error]: " ); \
	SYS_INFO( format, ##args ); 
    } \
} while( 0 )

/*Warning ��Ϣ���*/
#define SYS_WARNING( format, args... ) do \
{ \
    if( 1 == SYS_WARING_FLAG ) \
    { \
	fprintf( SYS_STDERR, "\n[Waring]: " ); \
	SYS_INFO( format, ##args ); \
    } \
} while( 0 )

/*Alert ��Ϣ���*/
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


