#ifndef _INTERCMD_UPDATE_H__
#define _INTERCMD_UPDATE_H__

#ifdef __cplusplus
extern "C"{
#endif

typedef enum _UPDATE_SECTION
{
	UPDATE_SECTION_START,
	UPDATE_KERNEL,
	UPDATE_FILESYS,
	UPDATE_OTHERS,
	UPDATE_SECTION_END
}UPDATE_SECTION;
typedef struct update
{
	unsigned char filename[64];
	UPDATE_SECTION updatesection;
}update;

#ifdef __cplusplus
}
#endif



#endif
