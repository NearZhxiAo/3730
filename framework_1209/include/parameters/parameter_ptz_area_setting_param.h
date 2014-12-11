#ifndef __PARAMETER_PTZ_AREA_SETTING_PARAM_H__
#define __PARAMETER_PTZ_AREA_SETTING_PARAM_H__

#ifdef __cplusplus
extern "C"{
#endif


#define AREA_NUM	72
typedef struct ptz_setting_param
{
	unsigned int valid;		//0, not valid, 1, valid
	unsigned int out_startx;	
	unsigned int out_starty;
	unsigned int out_endx;
	unsigned int out_endy;
	unsigned int inter_startx;
	unsigned int inter_starty;
	unsigned int inter_endx;
	unsigned int inter_endy;
	unsigned int timeelapsms;
}ptz_setting_param;
typedef struct ptz_area_setting_param
{
	ptz_setting_param ptz_setting[AREA_NUM];
}ptz_area_setting_param;


#ifdef __cplusplus
}
#endif



#endif
