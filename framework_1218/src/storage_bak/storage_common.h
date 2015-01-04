/*******************************************************************************
 * COPYRIGHT
 ********************************************************************************
/*
 * =====================================================================================
 *
 *       Filename:  storage_common.h
 *
 *    Description:
 *
 *        Version:
 *        Created:
 *       Revision:
 *       Compiler:
 *
 *         Author:
 *        Company:
 *
 * =====================================================================================
 */
/******************************** Souurce Code Start ******************************/
/**
 * @file storage_common.h
 */
#ifndef _STORAGE_COMM_H_
#define _STORAGE_COMM_H_

#ifdef _cplusplus
extern "C"{
#endif

#define STORAGE_FAIL    1
#define STORAGE_SUCCESS 0
#define STORAGE_TRUE    1
#define STORAGE_FALSE   0

#define STORAGE_DIR_NAME_ROOT "/mnt/mmc"

//#define DATA_DIR_NAME_ROOT "/root/data"
#define DATA_DIR_NAME_ROOT "/mnt/mmc/data"
#define DATA_DIR_NAME_INDEX "data"
#define DATA_EVENT_NUM_MAX 5
#define DATA_DIR_NAME_DI "di"
#define DATA_DIR_NAME_RED "red"
#define DATA_DIR_NAME_YELLOW "yellow"
#define DATA_DIR_NAME_SPEED "speed"
#define DATA_DIR_NAME_TIME "time"

#define DATA_DIR_NAME_H264 "h264"
#define DATA_DIR_NAME_MPEG4 "mpeg4"
#define DATA_DIR_NAME_MJPEG "mjpeg"

//#define INDEX_DIR_NAME_ROOT "/root/index"
#define INDEX_DIR_NAME_ROOT "/mnt/mmc/index"
#define INDEX_DIR_NAME_INDEX "index"
#define INDEX_EVENT_NUM_MAX 1
#define INDEX_DIR_NAME_DI "di"
#define INDEX_DIR_NAME_RED "red"
#define INDEX_DIR_NAME_YELLOW "yellow"
#define INDEX_DIR_NAME_SPEED "speed"
#define INDEX_DIR_NAME_TIME "time"

#define INDEX_DIR_NAME_H264 "h264"
#define INDEX_DIR_NAME_MPEG4 "mpeg4"
#define INDEX_DIR_NAME_MJPEG "mjpeg"


/*typedef unsigned char BOOL;*/
typedef enum bool
{
	False =0,
	True = 1,
}BOOL;

/************************************************************************/
/*Global Variables                                                      */
/************************************************************************/
typedef enum _storage_video_type_t
{
	VIDEO_TYPE_START     = -1,
	VIDEO_TYPE_MPEG4     = 0,
	VIDEO_TYPE_MPEG4_CIF    ,
	VIDEO_TYPE_H264         ,
	VIDEO_TYPE_H264_CIF     ,
	VIDEO_TYPE_MJPEG        ,
	VIDEO_TYPE_END        ,
}enStorageVideoType_T;

typedef enum _storage_code_tpye_t
{
	INDEX_CODE_START = -1,
	INDEX_CODE_H264 = 0,
	INDEX_CODE_MPEG4,
	INDEX_CODE_MJPEG,
	INDEX_CODE_END,
}enStorageCodeType_T;

typedef enum _storage_event_tpye_t
{
	INDEX_EVENT_START = -1,
	INDEX_EVENT_DI =0,
	INDEX_EVENT_RED,
	INDEX_EVENT_YELLOW,
	INDEX_EVENT_SPEED,
	INDEX_EVENT_TIME,
	INDEX_EVENT_END,
}enStorageEventType_T;

typedef struct _storage_index_data_t
{
	enStorageEventType_T event_type;
	enStorageCodeType_T code_type;
	char* ptr_index;
	char* ptr_data;
}stStorageIndexData_T;

#define __STORE_DEBUG

#ifdef __STORE_DEBUG
#define __D(fmt, args...) fprintf(stderr, "Info: " fmt, ## args)
#else
#define __D(fmt, args...)
#endif

#ifdef __STORE_DEBUG
#define __E(fmt, args...) fprintf(stderr, "Error: " fmt, ## args)
#else
#define __E(fmt, args...)
#endif

#ifdef _cplusplus
}
#endif

#endif

/******************************** Souurce Code End ******************************/
