#ifndef _ALG_INTERFACE_H_
#define _ALG_INTERFACE_H_

/*
#ifdef __cplusplus
extern "C"{
#endif
*/

//#ifndef DLL_EXPORTS
//#define EXPORT_CLASS __declspec(dllexport)
//#define EXPORT_API extern "C" __declspec(dllexport)
//#define EXPORT_API __declspec(dllexport)

//#define DLL_EXPORTS
//#endif 
#include "algConf.h"

typedef struct _PICINFO{
	int width;
	int height;
	_PICINFO(){
		width = 0;
		height = 0;
	}
}PICINFO;

EXPORT_API	unsigned int ALG_INIT();
EXPORT_API  unsigned int ALG_SETPARAM(GLOBAL_FLASH *g_stGlobal_Flash_out , ALG_XIANWINDOW_PARAM_ENV *algParamWindowEnv ,ALG_AREALINE_PARAM_ENV *algParamAreaLineEnv ,
									  ALG_PEOPLEDENSE_ENV *algParamPeopleDenseEnv , ALG_FIGHTDETECTION_ENV *algParamFighteDetectionEnv,
									  ALG_CLUSTERDETECTION_ENV *algParamClusterDetectionEnv , _ALG_ALARMAREA_ENV *algParamAlarmAreaEnv, 
									  ALG_ABNORMALFALL_ENV *algParamAbnormalFallEnv, ALG_HOVERDETECTION_ENV *algParamHoverDetectionEnv,
									  AlG_ALRAMLINE_ENV *algParamAlarmLineEnv, ALG_RUNNINGDETECT_ENV *algParamRunningObj, ALG_FINDBAG_ENV *algParamFindBagEnv);
EXPORT_API	unsigned int ALG_PROCESS(void *PData , bool isShow);

EXPORT_API	unsigned int ALG_GETRESULT(void *Result,ALG_EXTEND_TYPE algType);
EXPORT_API	unsigned int ALG_RELEASE();

/*
#ifdef __cplusplus
}
#endif
*/

#endif