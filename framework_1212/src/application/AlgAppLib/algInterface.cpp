#include "algInterface.h"
#include "time.h"

#pragma comment(lib, "APPSV.lib")



/*算法内部使用结构体*/
struct InParamAPP_AlarmAreaLine inputParamAlarmAreaLine ;		//绊线禁区算法
struct InParamAPP_AlarmArea inputParamAlarmArea;				//夜间起床检测
struct InParamAPP_AlarmLine inputParamAlarmLine;				//警戒区（摸高）检测
struct InParamAPP_XIAN_Window inputParamWindow ;				//翻窗算法
struct InParamAPP_FindBag inputParamFindBag;					//遗留物检测
struct InParamAPP_FightDetection inputParamFightDetection;		//打架检测
struct InParamAPP_ClusterDetection inputParamClusterDetection;	//聚集检测
struct InParamAPP_PeopleDense_2 inputParamPeopleDense;			//人群密度 
struct InParamAPP_RunningDetect inputParamRunningDetect;		//奔跑物体检测
																//离岗检测
struct InParamAPP_AbnormalFall inputParamAbnormalFall;			//跌倒检测
struct InParamAPP_HoverDetect inputParamHoverDetect;			//徘徊检测

								

/*算法句柄*/
static APPSVInterface * pAppsvInterfaceList[32];


AreaParamExt inputAreaParam ;
AlarmLineParamExt inputLineParam;
BackGlyphAreaExtList backGlyphList;

static CRITICAL_SECTION g_stCritical_Section;

GLOBAL_FLASH *_g_stGlobal_Flash = NULL;

PICINFO g_stGlobalPicInfo ;
static unsigned int g_PointFixedRate = 1;

void ALG_InitAreaLineAreaInfo();		//禁区绊线防区设置
void ALG_InitAreaAlarmArea();			//夜间起床算法防区设置
void ALG_InitAreaAlarmLine();			//摸高检测防区设置
void ALG_InitAreaBackGlyphAreaInfo();	//回字形防区设置
void ALG_InitLeaveOverBag();			//遗留物防区设置
void Alg_InitAreaClusterDetection();	//聚集算法防区设置
void ALG_InitAreaPeopleDense();			//人群检测算法防区设置
void ALG_InitAreaRunningObj();			//奔跑检测算法
void ALG_InitPeopleLeave();				//离岗检测算法
void Alg_InitAreaHoverDetect();			//徘徊算法防区设置



bool isEnableAlgArea(SCHEDULE_UNIT scheduleUnit[][MAX_SEGMENT_ONEDAY] , UINT isValid);

void defaultParamAlarmAeraLine();
void defaultParamAlarmArea();
void defaultParamAlarmLine();
void defaultParamWindowSet();
void defaultParamFindBag();
void defaultParamFightDetection();
void defaultParamClusterDetection();
void defaultParamPeopleDense();
void defaultParamRunningObj();
void defaultParamPeopleLeave();
void defaultParamAbnormalFall();
void defaultParamHoverDetect();

/************************************************************************/
/*	name:unsigned int ALG_INIT()                                        */
/*	fun:创建算法句柄                                                    */
/*	return: 1 正确                                                      */
/*			0 错误                                                      */
/************************************************************************/
EXPORT_API	unsigned int ALG_INIT()
{
	
	memset(pAppsvInterfaceList , 0x00, sizeof(pAppsvInterfaceList));
	InitializeCriticalSection(&g_stCritical_Section);

	APPSVCreateObject( &pAppsvInterfaceList[ALG_ALARM_AREA_LINE] , ALG_ALARM_AREA_LINE ); //禁区绊线
	APPSVCreateObject(&pAppsvInterfaceList[ALG_ALARM_AREA] , ALG_ALARM_AREA  );    //警戒区(夜晚起床)
	APPSVCreateObject(&pAppsvInterfaceList[ALG_ALARM_LINE] , ALG_ALARM_LINE  );    //警戒线(摸高)
	APPSVCreateObject( &pAppsvInterfaceList[ALG_WINDOW_THIEF] , ALG_WINDOW_THIEF ); //翻窗检测
	APPSVCreateObject( &pAppsvInterfaceList[ALG_LEAVE_OVER_BAG] , ALG_LEAVE_OVER_BAG ); //遗留物检测
	APPSVCreateObject( &pAppsvInterfaceList[ALG_PEOPLE_FIGHT] , ALG_PEOPLE_FIGHT); //打架
	APPSVCreateObject( &pAppsvInterfaceList[ALG_PEOPLE_CLUSTER] , ALG_PEOPLE_CLUSTER ); //聚集检测
	APPSVCreateObject( &pAppsvInterfaceList[ALG_PEOPLE_DENSE] , ALG_PEOPLE_DENSE );  //人群密度
	APPSVCreateObject( &pAppsvInterfaceList[ALG_PEOPLE_RUNNING] , ALG_PEOPLE_RUNNING ); //奔跑检测

	APPSVCreateObject( &pAppsvInterfaceList[ALG_PEOPLE_LEAVE] , ALG_PEOPLE_LEAVE );  //离岗检测
	APPSVCreateObject( &pAppsvInterfaceList[ALG_PEOPLE_FALL] , ALG_PEOPLE_FALL );  //跌倒
	APPSVCreateObject( &pAppsvInterfaceList[ALG_PEOPLE_HOVER] , ALG_PEOPLE_HOVER ); //徘徊
	
//	int i = 0;
//	for ( i = ALG_ALARM_AREA_LINE ;  i<= ALG_PEOPLE_HOVER ; i++)
//	{
//		if(pAppsvInterfaceList[i] == NULL)
//		{
//			ALG_RELEASE();
//			return 0;
//		}
//	}
	return 1;
	
}

/************************************************************************/
/*	name:unsigned int ALG_SETPARA(...)                                  */
/*	fun:设置算法参数                                                    */
/*	return: 1 正确                                                      */
/*			0 错误                                                      */
/************************************************************************/
EXPORT_API  unsigned int ALG_SETPARAM(GLOBAL_FLASH *g_stGlobal_Flash_out , ALG_XIANWINDOW_PARAM_ENV *algParamWindowEnv ,ALG_AREALINE_PARAM_ENV *algParamAreaLineEnv ,
									  ALG_PEOPLEDENSE_ENV *algParamPeopleDenseEnv , ALG_FIGHTDETECTION_ENV *algParamFighteDetectionEnv,
									  ALG_CLUSTERDETECTION_ENV *algParamClusterDetectionEnv , _ALG_ALARMAREA_ENV *algParamAlarmAreaEnv, 
									  ALG_ABNORMALFALL_ENV *algParamAbnormalFallEnv, ALG_HOVERDETECTION_ENV *algParamHoverDetectionEnv,
									  AlG_ALRAMLINE_ENV *algParamAlarmLineEnv, ALG_RUNNINGDETECT_ENV *algParamRunningObjEnv, ALG_FINDBAG_ENV *algParamFindBagEnv )
{

	if((g_stGlobal_Flash_out)==NULL)
	{
		return 0;
	}
	
	
	/*初始化数据默认结构*/ 
	memset(&inputParamWindow,0x00,sizeof(struct InParamAPP_XIAN_Window));
	memset(&inputParamAlarmAreaLine , 0x00, sizeof(struct InParamAPP_AlarmAreaLine));
	memset(&inputParamPeopleDense , 0x00, sizeof(struct InParamAPP_PeopleDense_2));
	memset(&inputParamFightDetection, 0x00 , sizeof(struct InParamAPP_FightDetection));
	memset(&inputParamClusterDetection , 0x00, sizeof(struct InParamAPP_ClusterDetection));
	memset(&inputParamAlarmArea , 0x00 , sizeof(struct InParamAPP_AlarmArea));
	memset(&inputParamAbnormalFall , 0x00 , sizeof(struct InParamAPP_AbnormalFall));
	memset(&inputParamHoverDetect , 0x00 , sizeof(struct InParamAPP_HoverDetect));
	memset(&inputParamAlarmLine , 0x00, sizeof(struct InParamAPP_AlarmLine));
	memset(&inputParamRunningDetect , 0x00, sizeof(struct InParamAPP_RunningDetect));
	memset(&inputParamFindBag , 0x00 , sizeof(struct InParamAPP_FindBag));

	if (g_stGlobal_Flash_out->videopara.src == VIDEO_SRC_ANALOG)
	{
		switch(g_stGlobal_Flash_out->videopara.analoginfo.vframetype)
		{
		case ANALOG_VFRAME_TYPE_QCIF:
			{
				g_stGlobalPicInfo.width = QCIF_WIDTH;
				g_stGlobalPicInfo.height = QCIF_HEIGHT;
			}
			break;
		case ANALOG_VFRAME_TYPE_CIF:
			{
				g_stGlobalPicInfo.width = CIF_WIDTH;
				g_stGlobalPicInfo.height = CIF_HEIGHT;
			}
			break;
		case ANALOG_VFRAME_TYPE_D1:
			{
				g_stGlobalPicInfo.width = D1_WIDTH;
				g_stGlobalPicInfo.height = D1_HEIGHT;
			}
			break;
		default:
			{
				g_stGlobalPicInfo.width = D1_WIDTH;
				g_stGlobalPicInfo.height = D1_HEIGHT;				
			}
			break;
		}
	}
	else
	{
		g_stGlobalPicInfo.width = D1_WIDTH;
		g_stGlobalPicInfo.height = D1_HEIGHT;
	}
	//gmaui 修正防区的坐标
	g_PointFixedRate = D1_WIDTH / g_stGlobalPicInfo.width;
	
	SVPARAM m_vaildPicParam ;
	memset((void *)&m_vaildPicParam , 0x00, sizeof( SVPARAM));
	{
		m_vaildPicParam.m_poly_point_num = 4;

		m_vaildPicParam.m_pv[0] = cvPoint(0,0);
		m_vaildPicParam.m_pv[1] = cvPoint(g_stGlobalPicInfo.width-1,0);
		m_vaildPicParam.m_pv[2] = cvPoint(g_stGlobalPicInfo.width-1,g_stGlobalPicInfo.height-1);
		m_vaildPicParam.m_pv[3] = cvPoint(0,g_stGlobalPicInfo.height-1);

	}	


	//gmaui 修正回型高宽度
	algParamWindowEnv->imgw = g_stGlobalPicInfo.width;
	algParamWindowEnv->imgh = g_stGlobalPicInfo.height;
	if(algParamAreaLineEnv == NULL){	
		defaultParamWindowSet();	
	}else{
		inputParamWindow._areaROI = m_vaildPicParam;

		inputParamWindow._imgw = algParamWindowEnv->imgw;
		inputParamWindow._imgh = algParamWindowEnv->imgh;
		inputParamWindow._iniframe = algParamWindowEnv->iniframe;
		inputParamWindow._grayThresh = algParamWindowEnv->grayThresh;
		inputParamWindow._light_area_ratio = algParamWindowEnv->light_area_ratio;
		inputParamWindow._disthresh = algParamWindowEnv->disthresh;
		inputParamWindow._max_tracksize = algParamWindowEnv->max_tracksize;
		inputParamWindow._lostcount = algParamWindowEnv->lostcount;	

		inputParamWindow._minfind = algParamWindowEnv->minfind;
		inputParamWindow._minmove = algParamWindowEnv->minmove;
		inputParamWindow._track_pos_diff_thresh = algParamWindowEnv->track_pos_diff_thresh;
		inputParamWindow._track_rect_diff_thresh = algParamWindowEnv->track_rect_diff_thresh;

		inputParamWindow._cont_minw = algParamWindowEnv->cont_minw;
		inputParamWindow._cont_maxw = algParamWindowEnv->cont_maxw;
		inputParamWindow._cont_minh = algParamWindowEnv->cont_minh;
		inputParamWindow._cont_maxh = algParamWindowEnv->cont_maxh;
		inputParamWindow._cont_area_rate = algParamWindowEnv->cont_area_rate;

		inputParamWindow._rect_merge_flag = algParamWindowEnv->rect_merge_flag;
		inputParamWindow._rect_minw = algParamWindowEnv->rect_minw;
		inputParamWindow._rect_minh = algParamWindowEnv->rect_minh;
		inputParamWindow._rect_area_rate = algParamWindowEnv->rect_area_rate;
	

	}
	
	//gmaui 修正拌线高宽度
	algParamAreaLineEnv->imgw = g_stGlobalPicInfo.width;
	algParamAreaLineEnv->imgh = g_stGlobalPicInfo.height;
	if( algParamAreaLineEnv == NULL){
		defaultParamAlarmAeraLine();
	}else{
		inputParamAlarmAreaLine._areaROI = m_vaildPicParam;

		inputParamAlarmAreaLine._imgw = algParamAreaLineEnv->imgw;
		inputParamAlarmAreaLine._imgh = algParamAreaLineEnv->imgh;
		inputParamAlarmAreaLine._iniframe = algParamAreaLineEnv->iniframe;
		inputParamAlarmAreaLine._thresh = algParamAreaLineEnv->thresh;
		inputParamAlarmAreaLine._disthresh = algParamAreaLineEnv->disthresh;
		inputParamAlarmAreaLine._max_tracksize = algParamAreaLineEnv->max_tracksize;
		inputParamAlarmAreaLine._lostcount = algParamAreaLineEnv->lostcount;	

		inputParamAlarmAreaLine._minfind = algParamAreaLineEnv->minfind;
		inputParamAlarmAreaLine._minmove = algParamAreaLineEnv->minmove;
		inputParamAlarmAreaLine._track_pos_diff_thresh = algParamAreaLineEnv->track_pos_diff_thresh;
		inputParamAlarmAreaLine._track_rect_diff_thresh = algParamAreaLineEnv->track_rect_diff_thresh;

		inputParamAlarmAreaLine._cont_minw = algParamAreaLineEnv->cont_minw;
		inputParamAlarmAreaLine._cont_maxw = algParamAreaLineEnv->cont_maxw;
		inputParamAlarmAreaLine._cont_minh = algParamAreaLineEnv->cont_minh;
		inputParamAlarmAreaLine._cont_maxh = algParamAreaLineEnv->cont_maxh;
		inputParamAlarmAreaLine._cont_area_rate = algParamAreaLineEnv->cont_area_rate;

		inputParamAlarmAreaLine._rect_merge_flag = algParamAreaLineEnv->rect_merge_flag;
		inputParamAlarmAreaLine._rect_minw = algParamAreaLineEnv->rect_minw;
		inputParamAlarmAreaLine._rect_minh = algParamAreaLineEnv->rect_minh;
		inputParamAlarmAreaLine._rect_area_rate = algParamAreaLineEnv->rect_area_rate;
	}
	

	//gmaui 修正密度高宽度
	algParamPeopleDenseEnv->_imgw = g_stGlobalPicInfo.width;
	algParamPeopleDenseEnv->_imgh = g_stGlobalPicInfo.height;
	if(	algParamPeopleDenseEnv == NULL){
		defaultParamPeopleDense();
	}else{
		inputParamPeopleDense._areaROI = m_vaildPicParam;

		inputParamPeopleDense._bVanishPt = algParamPeopleDenseEnv->_bVanishPt;
		inputParamPeopleDense._fSigma0 = algParamPeopleDenseEnv->_fSigma0;
		inputParamPeopleDense._fSigma1 = algParamPeopleDenseEnv->_fSigma1;
		inputParamPeopleDense._imgh = algParamPeopleDenseEnv->_imgh;
		inputParamPeopleDense._imgw = algParamPeopleDenseEnv->_imgw;
		inputParamPeopleDense._nLg = algParamPeopleDenseEnv->_nLg;
		inputParamPeopleDense._nLm = algParamPeopleDenseEnv->_nLm;
		inputParamPeopleDense._nNm = algParamPeopleDenseEnv->_nNm;
		inputParamPeopleDense._nNs = algParamPeopleDenseEnv->_nNs;
		inputParamPeopleDense._nOccHighestWarningThr = algParamPeopleDenseEnv->_nOccHighestWarningThr;
		inputParamPeopleDense._nSPNF = algParamPeopleDenseEnv->_nSPNF;
		inputParamPeopleDense._nT0 = algParamPeopleDenseEnv->_nT0;
		
	}
	

	//gmaui 修正打架高宽度
	algParamFighteDetectionEnv->_ImageWidth = g_stGlobalPicInfo.width;
	algParamFighteDetectionEnv->_ImageHeight = g_stGlobalPicInfo.height;
	if(algParamFighteDetectionEnv == NULL){
		defaultParamFightDetection();
	}else{
		inputParamFightDetection._areaROI = m_vaildPicParam;

		inputParamFightDetection._HypotenuseMax = algParamFighteDetectionEnv->_HypotenuseMax;
		inputParamFightDetection._HypotenuseMin = algParamFighteDetectionEnv->_HypotenuseMin;
		inputParamFightDetection._ImageHeight = algParamFighteDetectionEnv->_ImageHeight;
		inputParamFightDetection._ImageWidth = algParamFighteDetectionEnv->_ImageWidth;
		inputParamFightDetection._IsMovingCountThresh = algParamFighteDetectionEnv->_IsMovingCountThresh;
		inputParamFightDetection._IsMovingThresh = algParamFighteDetectionEnv->_IsMovingThresh;
		inputParamFightDetection._ROIBlockCountMax = algParamFighteDetectionEnv->_ROIBlockCountMax;
		inputParamFightDetection._ROIBlockCountMin = algParamFighteDetectionEnv->_ROIBlockCountMin;
		inputParamFightDetection._ROIBlockThresh = algParamFighteDetectionEnv->_ROIBlockThresh;
		inputParamFightDetection._UniDirectionCountThresh = algParamFighteDetectionEnv->_UniDirectionCountThresh;
		inputParamFightDetection._UnidirectionRatio = algParamFighteDetectionEnv->_UnidirectionRatio;
	}
	

	//gmaui 修正聚集高宽度
	algParamClusterDetectionEnv->_ImageWidth = g_stGlobalPicInfo.width;
	algParamClusterDetectionEnv->_ImageHeight = g_stGlobalPicInfo.height;
	if(algParamClusterDetectionEnv == NULL){
		defaultParamClusterDetection();
	}else{
		inputParamClusterDetection._areaROI = m_vaildPicParam;

		inputParamClusterDetection._clustermovingCountThresh = algParamClusterDetectionEnv->_clustermovingCountThresh;
		inputParamClusterDetection._HypotenuseMax = algParamClusterDetectionEnv->_HypotenuseMax;
		inputParamClusterDetection._HypotenuseMin = algParamClusterDetectionEnv->_HypotenuseMin;
		inputParamClusterDetection._ImageHeight = algParamClusterDetectionEnv->_ImageHeight;
		inputParamClusterDetection._ImageWidth = algParamClusterDetectionEnv->_ImageWidth;
		inputParamClusterDetection._ROIBlockCountMinHeight = algParamClusterDetectionEnv->_ROIBlockCountMinHeight;
		inputParamClusterDetection._ROIBlockCountMinWidth = algParamClusterDetectionEnv->_ROIBlockCountMinWidth;
		inputParamClusterDetection._ROIBlockThresh = algParamClusterDetectionEnv->_ROIBlockThresh;
	}


	//gmaui 修正起床高宽度
	algParamAlarmAreaEnv->_Imgw = g_stGlobalPicInfo.width;
	algParamAlarmAreaEnv->_Imgh = g_stGlobalPicInfo.height;
	if(algParamAlarmAreaEnv == NULL){
		defaultParamAlarmArea();
	}else{
		inputParamAlarmArea._areaROI = m_vaildPicParam;

		inputParamAlarmArea._Diff_Threshold = algParamAlarmAreaEnv->_Diff_Threshold;
		inputParamAlarmArea._Imgh = algParamAlarmAreaEnv->_Imgh;
		inputParamAlarmArea._Imgw = algParamAlarmAreaEnv->_Imgw;
		inputParamAlarmArea._MHI_Duration = algParamAlarmAreaEnv->_MHI_Duration;
		inputParamAlarmArea._RatioDetect = algParamAlarmAreaEnv->_RatioDetect;
		inputParamAlarmArea._RatioNoisze = algParamAlarmAreaEnv->_RatioNoisze;
	}


	//gmaui 修正跌倒高宽度
	algParamAbnormalFallEnv->_ImageWidth = g_stGlobalPicInfo.width;
	algParamAbnormalFallEnv->_ImageHeight = g_stGlobalPicInfo.height;
	if(algParamAbnormalFallEnv == NULL){
		defaultParamAbnormalFall();
	}else{
		//inputParamAbnormalFall.
		
		inputParamAbnormalFall._IsMovingCountThresh = algParamAbnormalFallEnv->_IsMovingCountThresh;
		inputParamAbnormalFall._UniDirectionCountThresh = algParamAbnormalFallEnv->_UniDirectionCountThresh;
		inputParamAbnormalFall._UnidirectionRatio = algParamAbnormalFallEnv->_UnidirectionRatio;
		inputParamAbnormalFall._ImageWidth = algParamAbnormalFallEnv->_ImageWidth;
		inputParamAbnormalFall._ImageHeight = algParamAbnormalFallEnv->_ImageHeight;
		inputParamAbnormalFall._HypotenuseMin = algParamAbnormalFallEnv->_HypotenuseMin;
		inputParamAbnormalFall._HypotenuseMax = algParamAbnormalFallEnv->_HypotenuseMax;
		inputParamAbnormalFall._IsMovingThresh = algParamAbnormalFallEnv->_IsMovingThresh;
		inputParamAbnormalFall._ROIBlockThresh = algParamAbnormalFallEnv->_ROIBlockThresh;
		inputParamAbnormalFall._ROIBlockCountMax = algParamAbnormalFallEnv->_ROIBlockCountMax;//感兴趣区域宽度最大占的格数
		inputParamAbnormalFall._ROIBlockCountMin = algParamAbnormalFallEnv->_ROIBlockCountMin;//感兴趣区域宽度最小占的格数

	}

	//gmaui 修正徘徊高宽度
	algParamHoverDetectionEnv->_imgw = g_stGlobalPicInfo.width;
	algParamHoverDetectionEnv->_imgh = g_stGlobalPicInfo.height;
	if(algParamHoverDetectionEnv == NULL){
		defaultParamHoverDetect();
	}else{
		inputParamHoverDetect._cont_area_rate = algParamHoverDetectionEnv->_cont_area_rate;
		inputParamHoverDetect._cont_maxh = algParamHoverDetectionEnv->_cont_maxh ;
		inputParamHoverDetect._cont_maxw = algParamHoverDetectionEnv->_cont_maxw ;
		inputParamHoverDetect._cont_minh = algParamHoverDetectionEnv->_cont_minh ;
		inputParamHoverDetect._cont_minw = algParamHoverDetectionEnv->_cont_minw ;
		inputParamHoverDetect._disthresh = algParamHoverDetectionEnv->_disthresh ;
		inputParamHoverDetect._imgh = algParamHoverDetectionEnv->_imgh ;
		inputParamHoverDetect._imgw = algParamHoverDetectionEnv->_imgw ;
		inputParamHoverDetect._iniframe = algParamHoverDetectionEnv->_iniframe ;
		inputParamHoverDetect._lostcount = algParamHoverDetectionEnv->_lostcount;
		inputParamHoverDetect._max_tracksize = algParamHoverDetectionEnv->_max_tracksize ;
		inputParamHoverDetect._minfind = algParamHoverDetectionEnv->_minfind ;
		inputParamHoverDetect._minmove = algParamHoverDetectionEnv->_minmove ;
		inputParamHoverDetect._thresh = algParamHoverDetectionEnv->_thresh ;
		inputParamHoverDetect._timeThresh = algParamHoverDetectionEnv->_timeThresh ;
		inputParamHoverDetect._track_pos_diff_thresh = algParamHoverDetectionEnv->_track_pos_diff_thresh ;
		inputParamHoverDetect._track_rect_diff_thresh = algParamHoverDetectionEnv->_track_rect_diff_thresh ;
	}
	
	//警戒线（摸高算法初始化）
	algParamAlarmLineEnv->_Imgh = g_stGlobalPicInfo.height;
	algParamAlarmLineEnv->_Imgw = g_stGlobalPicInfo.width;
	if(algParamAlarmLineEnv == NULL){
		defaultParamAlarmLine();		
	}else{
		(SVPARAM)(inputParamAlarmLine._areaROI) = m_vaildPicParam;

		inputParamAlarmLine._Diff_Threshold = algParamAlarmLineEnv->_Diff_Threshold;
		inputParamAlarmLine._Imgh = algParamAlarmLineEnv->_Imgh;
		inputParamAlarmLine._Imgw = algParamAlarmLineEnv->_Imgw;
		inputParamAlarmLine._MHI_Duration = algParamAlarmLineEnv->_MHI_Duration;
		inputParamAlarmLine._RatioNoisze = algParamAlarmLineEnv->_RatioNoisze;
		inputParamAlarmLine._ObjWidth = algParamAlarmLineEnv->_ObjWidth;
	}
	//奔跑检测
	algParamRunningObjEnv->_imgh = g_stGlobalPicInfo.height;
	algParamRunningObjEnv->_imgw = g_stGlobalPicInfo.width;
	if(algParamRunningObjEnv == NULL){
		defaultParamRunningObj();
	}else{	
		inputParamRunningDetect._areaROI = m_vaildPicParam;

		inputParamRunningDetect._imgw = algParamRunningObjEnv->_imgw;
		inputParamRunningDetect._imgh = algParamRunningObjEnv->_imgh;
		inputParamRunningDetect._iniframe = algParamRunningObjEnv->_iniframe;
		inputParamRunningDetect._thresh = algParamRunningObjEnv->_thresh;
		inputParamRunningDetect._disthresh = algParamRunningObjEnv->_disthresh;
		inputParamRunningDetect._max_tracksize = algParamRunningObjEnv->_max_tracksize ;
		inputParamRunningDetect._lostcount = algParamRunningObjEnv->_lostcount;
		inputParamRunningDetect._minfind = algParamRunningObjEnv->_minfind;
		inputParamRunningDetect._minmove = algParamRunningObjEnv->_minmove;
		inputParamRunningDetect._track_pos_diff_thresh = algParamRunningObjEnv->_track_pos_diff_thresh;
		inputParamRunningDetect._track_rect_diff_thresh = algParamRunningObjEnv->_track_rect_diff_thresh;

		inputParamRunningDetect._cont_minw = algParamRunningObjEnv->_cont_minw;
		inputParamRunningDetect._cont_maxw = algParamRunningObjEnv->_cont_maxw;
		inputParamRunningDetect._cont_minh =algParamRunningObjEnv->_cont_minh;
		inputParamRunningDetect._cont_maxh = algParamRunningObjEnv->_cont_maxh;
		inputParamRunningDetect._cont_area_rate = algParamRunningObjEnv->_cont_area_rate;

		inputParamRunningDetect._rect_merge_flag = algParamRunningObjEnv->_rect_merge_flag;
		inputParamRunningDetect._rect_minw = algParamRunningObjEnv->_rect_minw;
		inputParamRunningDetect._rect_minh = algParamRunningObjEnv->_rect_minh;
		inputParamRunningDetect._rect_area_rate = algParamRunningObjEnv->_rect_area_rate;

	}

	//遗留物检测
	algParamFindBagEnv->_imgh = g_stGlobalPicInfo.height;
	algParamFindBagEnv->_imgw = g_stGlobalPicInfo.width;
	if(algParamFindBagEnv == NULL){
		defaultParamFindBag();
	}else{
		(SVPARAM)(inputParamFindBag._areaROI) = m_vaildPicParam;

		inputParamFindBag._imgw = algParamFindBagEnv->_imgw;
		inputParamFindBag._imgh = algParamFindBagEnv->_imgh;
		inputParamFindBag._thresh = algParamFindBagEnv->_thresh;
		inputParamFindBag._iniframe = algParamFindBagEnv->_iniframe;
		inputParamFindBag._cont_minw = algParamFindBagEnv->_cont_minw;
		inputParamFindBag._cont_maxw = algParamFindBagEnv->_cont_maxw;
		inputParamFindBag._cont_minh = algParamFindBagEnv->_cont_minh;
		inputParamFindBag._cont_maxh = algParamFindBagEnv->_cont_maxh;

		
		
	}
	//离岗检测算法设置
	

	inputParamAlarmAreaLine._al_alarm.AreaNum = 0;
	inputParamAlarmAreaLine._al_alarm.LineNum = 0;

	inputAreaParam.AreaNum = 0;
	inputParamWindow._alarm_area.BackGlyphNum = 0;
	

	_g_stGlobal_Flash = g_stGlobal_Flash_out;
	
	
	

	ALG_InitAreaLineAreaInfo(); //禁区绊线防区设置
	ALG_InitAreaAlarmArea();//夜间起床算法防区设置
	ALG_InitAreaAlarmLine();//摸高检测防区设置
	ALG_InitAreaBackGlyphAreaInfo();//回字形防区设置
	ALG_InitLeaveOverBag();//遗留物检测
	Alg_InitAreaClusterDetection();//聚集算法防区设置
	ALG_InitAreaPeopleDense();//人群检测算法防区设置
	ALG_InitAreaRunningObj(); //奔跑检测算法
	ALG_InitPeopleLeave(); //离岗检测算法
	Alg_InitAreaHoverDetect();//徘徊算法防区设置
	
	
	//gmaui 修改 增加算法运行标志位判断
	EnterCriticalSection(&g_stCritical_Section);
	if(pAppsvInterfaceList[ALG_ALARM_AREA_LINE ] != NULL
		&&_g_stGlobal_Flash->algpara.AlgParaConfUint[_g_stGlobal_Flash->algpara.AlgParaConfUsedIndex].AlgParaChooseGroup.bAlgAlarmAreaLine)
	{
		pAppsvInterfaceList[ALG_ALARM_AREA_LINE ]->SetParameter((const unsigned char *)&inputParamAlarmAreaLine , sizeof(inputParamAlarmAreaLine));
	}//禁区绊线算法
	if(pAppsvInterfaceList[ALG_ALARM_AREA]  != NULL
		&&_g_stGlobal_Flash->algpara.AlgParaConfUint[_g_stGlobal_Flash->algpara.AlgParaConfUsedIndex].AlgParaChooseGroup.bAlgAlarmArea)
	{
		pAppsvInterfaceList[ALG_ALARM_AREA] ->SetParameter((const unsigned char *)&inputParamAlarmArea , sizeof(inputParamAlarmArea));
	}//夜间起夜算法
	if(pAppsvInterfaceList[ALG_ALARM_LINE]  != NULL
		&&_g_stGlobal_Flash->algpara.AlgParaConfUint[_g_stGlobal_Flash->algpara.AlgParaConfUsedIndex].AlgParaChooseGroup.bAlgAlarmLine)
	{
		pAppsvInterfaceList[ALG_ALARM_LINE] ->SetParameter((const unsigned char *)&inputParamAlarmLine , sizeof(inputParamAlarmLine));
	}//警戒线（摸高）算法
	if(pAppsvInterfaceList[ALG_WINDOW_THIEF] != NULL
		&&_g_stGlobal_Flash->algpara.AlgParaConfUint[_g_stGlobal_Flash->algpara.AlgParaConfUsedIndex].AlgParaChooseGroup.bAlgWindowThief)
	{
		pAppsvInterfaceList[ALG_WINDOW_THIEF]->SetParameter((const unsigned  char *)&inputParamWindow , sizeof(inputParamWindow));
	}//回字形算法
	if(pAppsvInterfaceList[ALG_LEAVE_OVER_BAG]  != NULL
		&&_g_stGlobal_Flash->algpara.AlgParaConfUint[_g_stGlobal_Flash->algpara.AlgParaConfUsedIndex].AlgParaChooseGroup.bAlgLeaveOverBag)
	{
		pAppsvInterfaceList[ALG_LEAVE_OVER_BAG]->SetParameter((const unsigned char *)&inputParamFindBag , sizeof(inputParamFindBag));
	}//遗留物算法
	if(pAppsvInterfaceList[ALG_PEOPLE_FIGHT] != NULL
		&&_g_stGlobal_Flash->algpara.AlgParaConfUint[_g_stGlobal_Flash->algpara.AlgParaConfUsedIndex].AlgParaChooseGroup.bAlgPeopleFight)
	{
		pAppsvInterfaceList[ALG_PEOPLE_FIGHT]->SetParameter((const unsigned char *)&inputParamFightDetection , sizeof(inputParamFightDetection));
	}//打架算法
	if(pAppsvInterfaceList[ALG_PEOPLE_CLUSTER] != NULL
		&&_g_stGlobal_Flash->algpara.AlgParaConfUint[_g_stGlobal_Flash->algpara.AlgParaConfUsedIndex].AlgParaChooseGroup.bAlgPeopleCluster)
	{
		pAppsvInterfaceList[ALG_PEOPLE_CLUSTER]->SetParameter((const unsigned char  *)&inputParamClusterDetection , sizeof(inputParamClusterDetection));
	}//聚集算法
	if(pAppsvInterfaceList[ALG_PEOPLE_DENSE] != NULL
		&&_g_stGlobal_Flash->algpara.AlgParaConfUint[_g_stGlobal_Flash->algpara.AlgParaConfUsedIndex].AlgParaChooseGroup.bAlgPeopleDense)
	{
		pAppsvInterfaceList[ALG_PEOPLE_DENSE]->SetParameter((const unsigned char *)&inputParamPeopleDense , sizeof(InParamAPP_PeopleDense_2));
	}//人群密度
	if(pAppsvInterfaceList[ALG_PEOPLE_RUNNING] != NULL
		&&_g_stGlobal_Flash->algpara.AlgParaConfUint[_g_stGlobal_Flash->algpara.AlgParaConfUsedIndex].AlgParaChooseGroup.bAlgPeopleRunning)
	{
		pAppsvInterfaceList[ALG_PEOPLE_RUNNING]->SetParameter((const unsigned char *)&inputParamRunningDetect , sizeof(inputParamRunningDetect));
	}//奔跑算法
	if(pAppsvInterfaceList[ALG_PEOPLE_LEAVE] != NULL
		&&_g_stGlobal_Flash->algpara.AlgParaConfUint[_g_stGlobal_Flash->algpara.AlgParaConfUsedIndex].AlgParaChooseGroup.bAlgPeopleLeave)
	{
		//pAppsvInterfaceList[ALG_PEOPLE_LEAVE]->SetParameter((const unsigned char *)&inputParam , sizeof(inputParamHoverDetect));
	}//离岗算法
	if(pAppsvInterfaceList[ALG_PEOPLE_FALL]  != NULL
		&&_g_stGlobal_Flash->algpara.AlgParaConfUint[_g_stGlobal_Flash->algpara.AlgParaConfUsedIndex].AlgParaChooseGroup.bAlgPeopleFall)
	{
		pAppsvInterfaceList[ALG_PEOPLE_FALL]->SetParameter((const unsigned char *)&inputParamAbnormalFall , sizeof(inputParamAbnormalFall));
	}//跌倒算法
	if(pAppsvInterfaceList[ALG_PEOPLE_HOVER] != NULL
		&&_g_stGlobal_Flash->algpara.AlgParaConfUint[_g_stGlobal_Flash->algpara.AlgParaConfUsedIndex].AlgParaChooseGroup.bAlgPeopleHover)
	{
		pAppsvInterfaceList[ALG_PEOPLE_HOVER]->SetParameter((const unsigned char *)&inputParamHoverDetect , sizeof(inputParamHoverDetect));
	}//徘徊算法
	LeaveCriticalSection(&g_stCritical_Section);
	return 1;
}

EXPORT_API	unsigned int ALG_PROCESS(void *PData , bool isShow)
{
#if 1
	int i = ALG_ALARM_AREA_LINE;
	int j = 0,k = 0;
	unsigned char * YUV_Image = NULL;// YUV420 bit stream
	YUV_Image = (unsigned char*)malloc(g_stGlobalPicInfo.width*g_stGlobalPicInfo.height);
	memset(YUV_Image, 0x00, g_stGlobalPicInfo.width*g_stGlobalPicInfo.height);
	//gmaui 添加算法运行标志位
	if (_g_stGlobal_Flash->algpara.AlgParaConfUint[_g_stGlobal_Flash->algpara.AlgParaConfUsedIndex].AlgParaChooseGroup.bAlgEnableFlag)
	{
		unsigned int flag = *(unsigned int*)&_g_stGlobal_Flash->algpara.AlgParaConfUint[_g_stGlobal_Flash->algpara.AlgParaConfUsedIndex].AlgParaChooseGroup;
		EnterCriticalSection(&g_stCritical_Section);		
		for(i= ALG_ALARM_AREA_LINE ; i <= ALG_PEOPLE_HOVER ; i++)
		{
			if (( i == ALG_PEOPLE_DENSE) && pAppsvInterfaceList[i] && 
				((flag&(1<<i)) == (1<<i)))
			{
				pAppsvInterfaceList[ALG_PEOPLE_DENSE]->Process((const unsigned char*)PData);
				//将密度算法放到最后去分析
			}
			else if( pAppsvInterfaceList[i] && ((flag&(1<<i)) == (1<<i)) )
			{
				/*将数据转化成灰度图像*/			
				for(j=0 ; j<g_stGlobalPicInfo.height ; j++)
				{
					for(k=0 ; k<g_stGlobalPicInfo.width ; k++)
					{
						int pos = g_stGlobalPicInfo.width * j + k; // pixel position
						unsigned char B = *((unsigned char *)((unsigned char *)PData+pos*3));
						unsigned char G = *((unsigned char *)((unsigned char *)PData+pos*3+1));
						unsigned char R = *((unsigned char *)((unsigned char *)PData+pos*3+2));
						
						unsigned char  Y= (unsigned char)(0.3*R + 0.59*G + 0.11*B);
						//unsigned char  U= (unsigned char)((B-Y) * 0.493);
						//unsigned char  V= (unsigned char)((R-Y) * 0.877);
						
						*((unsigned char *)(YUV_Image+pos)) = Y;			   
						pos ++;
					}
				}
				pAppsvInterfaceList[i]->Process((const unsigned char*)YUV_Image);			
			}
		}
		for(i= ALG_ALARM_AREA_LINE ; i <= ALG_PEOPLE_HOVER ; i++)
		{
			if( pAppsvInterfaceList[i]&& ((flag&(1<<i)) == (1<<i)) && isShow)
			{
				pAppsvInterfaceList[i]->Draw((const unsigned char*)PData);
			}
		}
		
		LeaveCriticalSection(&g_stCritical_Section);
	}
	free(YUV_Image);
	return 1;

#endif
}



EXPORT_API	unsigned int ALG_GETRESULT(void *Result,ALG_EXTEND_TYPE algType )
{

	struct OutPutAPP_XIAN_Window outPutAppXIANWindow;    
	struct OutPutAPP_AlarmAreaLine outPutAppAlarmAreaLine; 
	struct OutPutAPP_PeopleDense_2 outPutAppPeopleDense; 
	struct OutPutAPP_FightDetection outPutAppFightDetection;
	struct OutPutAPP_ClusterDetection outPutAppClusterDetection;
	struct OutPutAPP_AbnormalFall outPutAppAbnormalFall;
	struct OutPutAPP_HoverDetect outPutAppHoverDetect;
	struct OutPutAPP_RunningDetect outPutAppRunningDetect; //奔跑检测
	struct OutPutAPP_FindBag outPutAppFindBag; //遗留物检测
	struct OutPutAPP_AlarmArea outPutAppAlarmArea; //起床算法
	struct OutPutAPP_AlarmLine outPutAppAlarmLine; //摸高算法
	//离岗结果输出

	memset((void *)&outPutAppXIANWindow , 0x00, sizeof(outPutAppXIANWindow));
	memset((void *)&outPutAppAlarmAreaLine , 0x00, sizeof(outPutAppAlarmAreaLine));
	memset((void *)&outPutAppPeopleDense , 0x00, sizeof(outPutAppPeopleDense));
	memset((void *)&outPutAppFightDetection , 0x00, sizeof(outPutAppFightDetection));
	memset((void *)&outPutAppClusterDetection , 0x00, sizeof(outPutAppClusterDetection));
	memset((void *)&outPutAppAlarmArea , 0x00, sizeof(outPutAppAlarmArea));
	memset((void *)&outPutAppAbnormalFall , 0x00, sizeof(outPutAppAbnormalFall));
	memset((void *)&outPutAppHoverDetect , 0x00, sizeof(outPutAppHoverDetect));
	memset((void *)&outPutAppRunningDetect , 0x00, sizeof(outPutAppRunningDetect));
	memset((void *)&outPutAppFindBag , 0x00, sizeof(outPutAppFindBag));


	switch(algType)
	{
	case ALG_REC:  //回型区算法
		if(pAppsvInterfaceList[ALG_WINDOW_THIEF] == NULL
			||!_g_stGlobal_Flash->algpara.AlgParaConfUint[_g_stGlobal_Flash->algpara.AlgParaConfUsedIndex].AlgParaChooseGroup.bAlgWindowThief){
			((OutPutAPP_XIAN_WindowExt *)Result)->_objnum = 0;
			return 0 ;
		}else{

			pAppsvInterfaceList[ALG_WINDOW_THIEF]->GetResult(&outPutAppXIANWindow);
			((OutPutAPP_XIAN_WindowExt *)Result)->_objnum = 0;
			for (int i=0; i<outPutAppXIANWindow._objnum ;i++)
			{
				if(outPutAppXIANWindow._obj_list[i]._alarm != 0)
				{
					int index_one = backGlyphList.nodeList[outPutAppXIANWindow._obj_list[i]._alarm-1].backGlyphID/UNIT_REC_EVERY_PACKET;
					int index_two = backGlyphList.nodeList[outPutAppXIANWindow._obj_list[i]._alarm-1].backGlyphID%UNIT_REC_EVERY_PACKET;

					bool isEnable = isEnableAlgArea(_g_stGlobal_Flash->algquadpara[index_one].REC[index_two].scheduleunit,_g_stGlobal_Flash->algquadpara[index_one].REC[index_two].valid);
					if(isEnable)
					{
						((OutPutAPP_XIAN_WindowExt *)Result)->_obj_list[((OutPutAPP_XIAN_WindowExt *)Result)->_objnum]._alarm =  outPutAppXIANWindow._obj_list[i]._alarm-1;
						((OutPutAPP_XIAN_WindowExt *)Result)->_obj_list[((OutPutAPP_XIAN_WindowExt *)Result)->_objnum]._object = outPutAppXIANWindow._obj_list[i]._objrect;
						((OutPutAPP_XIAN_WindowExt *)Result)->_objnum++;
					}	
				}
			}
		}
		break;
	case ALG_AREA:  //禁区
		if (pAppsvInterfaceList[ALG_ALARM_AREA_LINE] == NULL
			||!_g_stGlobal_Flash->algpara.AlgParaConfUint[_g_stGlobal_Flash->algpara.AlgParaConfUsedIndex].AlgParaChooseGroup.bAlgAlarmAreaLine){
			((OutPutAPP_XIAN_WindowExt *)Result)->_objnum = 0;
			return 0 ;
		}else{
			pAppsvInterfaceList[ALG_ALARM_AREA_LINE]->GetResult(&outPutAppAlarmAreaLine);

			((OutPutAPP_XIAN_WindowExt *)Result)->_objnum = 0;
			for (int i=0; (i < outPutAppAlarmAreaLine._objnum);i++)
			{
				if(outPutAppAlarmAreaLine._obj_list[i]._alarm_area != 0)
				{
					if(inputAreaParam.areaParamList[outPutAppAlarmAreaLine._obj_list[i]._alarm_area-1].algType == ALG_AREA)
					{
						int index_one = inputAreaParam.areaParamList[outPutAppAlarmAreaLine._obj_list[i]._alarm_area-1].areaID / CONFIG_UNIT_EVERY_PACKET;
						int index_two = inputAreaParam.areaParamList[outPutAppAlarmAreaLine._obj_list[i]._alarm_area-1].areaID % CONFIG_UNIT_EVERY_PACKET;
						bool isEnable = isEnableAlgArea(_g_stGlobal_Flash->algareapara[index_one].configunit[index_two].scheduleunit , _g_stGlobal_Flash->algareapara[index_one].configunit[index_two].validflag);
						if(isEnable)
						{
							((OutPutAPP_XIAN_WindowExt *)Result)->_obj_list[((OutPutAPP_XIAN_WindowExt *)Result)->_objnum]._alarm =  inputAreaParam.areaParamList[outPutAppAlarmAreaLine._obj_list[i]._alarm_area-1].areaID;
							((OutPutAPP_XIAN_WindowExt *)Result)->_obj_list[((OutPutAPP_XIAN_WindowExt *)Result)->_objnum]._object = outPutAppAlarmAreaLine._obj_list[i]._objrect;
							((OutPutAPP_XIAN_WindowExt *)Result)->_objnum++;
						}
					}
				}
			}
		}
		
		break;
	
	case ALG_LINE: //绊线
		if (pAppsvInterfaceList[ALG_ALARM_AREA_LINE] == NULL
			||!_g_stGlobal_Flash->algpara.AlgParaConfUint[_g_stGlobal_Flash->algpara.AlgParaConfUsedIndex].AlgParaChooseGroup.bAlgAlarmAreaLine){
			((OutPutAPP_XIAN_WindowExt *)Result)->_objnum = 0;
			return 0 ;
		}else{
			pAppsvInterfaceList[ALG_ALARM_AREA_LINE]->GetResult(&outPutAppAlarmAreaLine);

			((OutPutAPP_XIAN_WindowExt *)Result)->_objnum = 0;
			for (int i=0; (i < outPutAppAlarmAreaLine._objnum);i++)
			{
				if(outPutAppAlarmAreaLine._obj_list[i]._alarm_line != 0)
				{
					if(inputLineParam.lineList[outPutAppAlarmAreaLine._obj_list[i]._alarm_line-1].algType == ALG_LINE)
					{
						int index_one = inputLineParam.lineList[outPutAppAlarmAreaLine._obj_list[i]._alarm_line-1].lineID / UNIT_COUNT_EVERY_PACKET;
						int index_two = inputLineParam.lineList[outPutAppAlarmAreaLine._obj_list[i]._alarm_line-1].lineID % UNIT_COUNT_EVERY_PACKET;
						bool isEnable = isEnableAlgArea(_g_stGlobal_Flash->alglinepara[index_one].lineunit[index_two].scheduleunit , _g_stGlobal_Flash->alglinepara[index_one].lineunit[index_two].validflag);
						if (isEnable)
						{
							((OutPutAPP_XIAN_WindowExt *)Result)->_obj_list[((OutPutAPP_XIAN_WindowExt *)Result)->_objnum]._alarm =  inputAreaParam.areaParamList[outPutAppAlarmAreaLine._obj_list[i]._alarm_line-1].areaID;
							((OutPutAPP_XIAN_WindowExt *)Result)->_obj_list[((OutPutAPP_XIAN_WindowExt *)Result)->_objnum]._object = outPutAppAlarmAreaLine._obj_list[i]._objrect;
							((OutPutAPP_XIAN_WindowExt *)Result)->_objnum++;
						}
					}
				}
			}
		}
		break;
	case ALG_LINE_COUNT:
		if (pAppsvInterfaceList[ALG_ALARM_AREA_LINE] == NULL
			||!_g_stGlobal_Flash->algpara.AlgParaConfUint[_g_stGlobal_Flash->algpara.AlgParaConfUsedIndex].AlgParaChooseGroup.bAlgAlarmAreaLine){
			((OutPutAPP_XIAN_WindowExt *)Result)->_objnum = 0;
			return 0 ;
		}else{
			pAppsvInterfaceList[ALG_ALARM_AREA_LINE]->GetResult(&outPutAppAlarmAreaLine);

			((OutPutAPP_XIAN_WindowExt *)Result)->_objnum = 0;
			for (int i=0; (i < outPutAppAlarmAreaLine._objnum);i++)
			{
				if(outPutAppAlarmAreaLine._obj_list[i]._alarm_line != 0)
				{
					if(inputLineParam.lineList[outPutAppAlarmAreaLine._obj_list[i]._alarm_line-1].algType == ALG_OBJ_ENTER)
					{
						int index_one = inputLineParam.lineList[outPutAppAlarmAreaLine._obj_list[i]._alarm_line-1].lineID / UNIT_COUNT_EVERY_PACKET;
						int index_two = inputLineParam.lineList[outPutAppAlarmAreaLine._obj_list[i]._alarm_line-1].lineID % UNIT_COUNT_EVERY_PACKET;
						bool isEnable = isEnableAlgArea(_g_stGlobal_Flash->algcountpara[index_one].countunit[index_two].scheduleunit , _g_stGlobal_Flash->algcountpara[index_one].countunit[index_two].validflag);
						if (isEnable)
						{
							_g_stGlobal_Flash->algcountpara[index_one].countunit[index_two].count++;
							((OutPutAPP_XIAN_WindowExt *)Result)->_obj_list[((OutPutAPP_XIAN_WindowExt *)Result)->_objnum]._alarm =  inputAreaParam.areaParamList[outPutAppAlarmAreaLine._obj_list[i]._alarm_line-1].areaID;
							((OutPutAPP_XIAN_WindowExt *)Result)->_obj_list[((OutPutAPP_XIAN_WindowExt *)Result)->_objnum]._object = outPutAppAlarmAreaLine._obj_list[i]._objrect;
							((OutPutAPP_XIAN_WindowExt *)Result)->_objnum++;
						}

					}
				}
			}
		}
		break;
	case ALG_OBJ_ENTER:
		if (pAppsvInterfaceList[ALG_ALARM_AREA_LINE] == NULL
			||!_g_stGlobal_Flash->algpara.AlgParaConfUint[_g_stGlobal_Flash->algpara.AlgParaConfUsedIndex].AlgParaChooseGroup.bAlgAlarmAreaLine){
			((OutPutAPP_XIAN_WindowExt *)Result)->_objnum = 0;
			return 0 ;
		}else{
			pAppsvInterfaceList[ALG_ALARM_AREA_LINE]->GetResult(&outPutAppAlarmAreaLine);

			((OutPutAPP_XIAN_WindowExt *)Result)->_objnum = 0;
			for (int i=0; (i < outPutAppAlarmAreaLine._objnum);i++)
			{
				if(outPutAppAlarmAreaLine._obj_list[i]._alarm_line != 0)
				{
					if(inputLineParam.lineList[outPutAppAlarmAreaLine._obj_list[i]._alarm_line-1].algType == ALG_OBJ_ENTER)
					{
						int index_one = inputLineParam.lineList[outPutAppAlarmAreaLine._obj_list[i]._alarm_line-1].lineID / UNIT_ENTER_EVERY_PACKET;
						int index_two = inputLineParam.lineList[outPutAppAlarmAreaLine._obj_list[i]._alarm_line-1].lineID % UNIT_ENTER_EVERY_PACKET;
						bool isEnable = isEnableAlgArea(_g_stGlobal_Flash->algenterpara[index_one].enterunit[index_two].scheduleunit , _g_stGlobal_Flash->algenterpara[index_one].enterunit[index_two].validflag);
						if (isEnable)
						{
							((OutPutAPP_XIAN_WindowExt *)Result)->_obj_list[((OutPutAPP_XIAN_WindowExt *)Result)->_objnum]._alarm =  inputAreaParam.areaParamList[outPutAppAlarmAreaLine._obj_list[i]._alarm_line-1].areaID;
							((OutPutAPP_XIAN_WindowExt *)Result)->_obj_list[((OutPutAPP_XIAN_WindowExt *)Result)->_objnum]._object = outPutAppAlarmAreaLine._obj_list[i]._objrect;
							((OutPutAPP_XIAN_WindowExt *)Result)->_objnum++;
						}
						
					}
				}
			}
		}
		break;
	case ALG_OBJ_OUT:
		if (pAppsvInterfaceList[ALG_ALARM_AREA_LINE] == NULL
			||!_g_stGlobal_Flash->algpara.AlgParaConfUint[_g_stGlobal_Flash->algpara.AlgParaConfUsedIndex].AlgParaChooseGroup.bAlgAlarmAreaLine){
			((OutPutAPP_XIAN_WindowExt *)Result)->_objnum = 0;
			return 0 ;
		}else{
			pAppsvInterfaceList[ALG_ALARM_AREA_LINE]->GetResult(&outPutAppAlarmAreaLine);

			((OutPutAPP_XIAN_WindowExt *)Result)->_objnum = 0;
			for (int i=0; (i < outPutAppAlarmAreaLine._objnum);i++)
			{
				if(outPutAppAlarmAreaLine._obj_list[i]._alarm_line != 0)
				{
					if(inputLineParam.lineList[outPutAppAlarmAreaLine._obj_list[i]._alarm_line-1].algType == ALG_OBJ_OUT)
					{
						int index_one = inputLineParam.lineList[outPutAppAlarmAreaLine._obj_list[i]._alarm_line-1].lineID / UNIT_OUT_EVERY_PACKET;
						int index_two = inputLineParam.lineList[outPutAppAlarmAreaLine._obj_list[i]._alarm_line-1].lineID % UNIT_OUT_EVERY_PACKET;
						bool isEnable = isEnableAlgArea( _g_stGlobal_Flash->algoutpara[index_one].outunit[index_two].scheduleunit , _g_stGlobal_Flash->algoutpara[index_one].outunit[index_two].validflag);
						if (isEnable)
						{
							((OutPutAPP_XIAN_WindowExt *)Result)->_obj_list[((OutPutAPP_XIAN_WindowExt *)Result)->_objnum]._alarm =  inputAreaParam.areaParamList[outPutAppAlarmAreaLine._obj_list[i]._alarm_line-1].areaID;
							((OutPutAPP_XIAN_WindowExt *)Result)->_obj_list[((OutPutAPP_XIAN_WindowExt *)Result)->_objnum]._object = outPutAppAlarmAreaLine._obj_list[i]._objrect;
							((OutPutAPP_XIAN_WindowExt *)Result)->_objnum++;
						}
						
					}
				}
			}
		}
		break;
// 	case ALG_OBJ_STOP:  //目标遗留
// 		
// 		break;
	case ALG_OBJ_LOST: //目标消失

		break;
	case ALG_DIRECTION:
		if (pAppsvInterfaceList[ALG_ALARM_AREA_LINE] == NULL
			||!_g_stGlobal_Flash->algpara.AlgParaConfUint[_g_stGlobal_Flash->algpara.AlgParaConfUsedIndex].AlgParaChooseGroup.bAlgAlarmAreaLine){
			((OutPutAPP_XIAN_WindowExt *)Result)->_objnum = 0;
			return 0 ;
		}else{
			pAppsvInterfaceList[ALG_ALARM_AREA_LINE]->GetResult(&outPutAppAlarmAreaLine);

			((OutPutAPP_XIAN_WindowExt *)Result)->_objnum = 0;
			for (int i=0; (i < outPutAppAlarmAreaLine._objnum);i++)
			{
				if(outPutAppAlarmAreaLine._obj_list[i]._alarm_line != 0)
				{
					if(inputLineParam.lineList[outPutAppAlarmAreaLine._obj_list[i]._alarm_line-1].algType == ALG_DIRECTION)
					{ 
						int index_one = inputLineParam.lineList[outPutAppAlarmAreaLine._obj_list[i]._alarm_line-1].lineID / UNIT_DIRECTION_EVERY_PACKET;
						int index_two = inputLineParam.lineList[outPutAppAlarmAreaLine._obj_list[i]._alarm_line-1].lineID % UNIT_DIRECTION_EVERY_PACKET;
						bool isEnable = isEnableAlgArea(_g_stGlobal_Flash->algdirectpara[index_one].directunit[index_two].scheduleunit , _g_stGlobal_Flash->algdirectpara[index_one].directunit[index_two].validflag);
						if (isEnable)
						{
							((OutPutAPP_XIAN_WindowExt *)Result)->_obj_list[((OutPutAPP_XIAN_WindowExt *)Result)->_objnum]._alarm =  inputAreaParam.areaParamList[outPutAppAlarmAreaLine._obj_list[i]._alarm_line-1].areaID;
							((OutPutAPP_XIAN_WindowExt *)Result)->_obj_list[((OutPutAPP_XIAN_WindowExt *)Result)->_objnum]._object = outPutAppAlarmAreaLine._obj_list[i]._objrect;
							((OutPutAPP_XIAN_WindowExt *)Result)->_objnum++;
						}
					}
				}
			}
		}
		break;
	case ALG_DETECTION:
		if (pAppsvInterfaceList[ALG_ALARM_AREA_LINE] == NULL
			||!_g_stGlobal_Flash->algpara.AlgParaConfUint[_g_stGlobal_Flash->algpara.AlgParaConfUsedIndex].AlgParaChooseGroup.bAlgAlarmAreaLine){
			((OutPutAPP_XIAN_WindowExt *)Result)->_objnum = 0;
			return 0 ;
		}else{
			pAppsvInterfaceList[ALG_ALARM_AREA_LINE]->GetResult(&outPutAppAlarmAreaLine);

			((OutPutAPP_XIAN_WindowExt *)Result)->_objnum = 0;
			for (int i=0; i < outPutAppAlarmAreaLine._objnum;i++)
			{
				if(outPutAppAlarmAreaLine._obj_list[i]._alarm_line != 0)
				{
					if(inputLineParam.lineList[outPutAppAlarmAreaLine._obj_list[i]._alarm_line-1].algType == ALG_DETECTION)
					{
						int index_one = inputLineParam.lineList[outPutAppAlarmAreaLine._obj_list[i]._alarm_line-1].lineID / UNIT_DETECTION_EVERY_PACKET;
						int index_two = inputLineParam.lineList[outPutAppAlarmAreaLine._obj_list[i]._alarm_line-1].lineID % UNIT_DETECTION_EVERY_PACKET;
						bool isEnable = isEnableAlgArea( _g_stGlobal_Flash->algdetectpara[index_one].dectunit[index_two].scheduleunit , _g_stGlobal_Flash->algdetectpara[index_one].dectunit[index_two].validflag);
						if(isEnable)
						{
							((OutPutAPP_XIAN_WindowExt *)Result)->_obj_list[((OutPutAPP_XIAN_WindowExt *)Result)->_objnum]._alarm =  inputAreaParam.areaParamList[outPutAppAlarmAreaLine._obj_list[i]._alarm_line-1].areaID;
							((OutPutAPP_XIAN_WindowExt *)Result)->_obj_list[((OutPutAPP_XIAN_WindowExt *)Result)->_objnum]._object = outPutAppAlarmAreaLine._obj_list[i]._objrect;
							((OutPutAPP_XIAN_WindowExt *)Result)->_objnum++;
						}
					}
				}
			}
		}
		break;
	case ALG_PEOPLEDENSE:
		if (pAppsvInterfaceList[ALG_PEOPLE_DENSE] == NULL
			||!_g_stGlobal_Flash->algpara.AlgParaConfUint[_g_stGlobal_Flash->algpara.AlgParaConfUsedIndex].AlgParaChooseGroup.bAlgPeopleDense){
			((OutPutAPP_PeopleDense_Ext *)Result)->_dense = 0;
			return 0 ;
		}else{
			pAppsvInterfaceList[ALG_PEOPLE_DENSE]->GetResult(&outPutAppPeopleDense);
			((OutPutAPP_PeopleDense_Ext *)Result)->_dense = outPutAppPeopleDense._dense;
		}
		break;
	case ALG_FIGHTDETECTION:
		if (pAppsvInterfaceList[ALG_PEOPLE_FIGHT] == NULL
			||!_g_stGlobal_Flash->algpara.AlgParaConfUint[_g_stGlobal_Flash->algpara.AlgParaConfUsedIndex].AlgParaChooseGroup.bAlgPeopleFight){
			((OutPutAPP_FightDetection_Ext *)Result)->_bFighting = 0;
			return 0 ;
		}else{
			pAppsvInterfaceList[ALG_PEOPLE_FIGHT]->GetResult(&outPutAppFightDetection);
			((OutPutAPP_FightDetection_Ext *)Result)->_bFighting = outPutAppFightDetection._IsFighting;
		}
		break;
	case ALG_CLUSTERDETECTION:
		if (pAppsvInterfaceList[ALG_PEOPLE_CLUSTER] == NULL
			||!_g_stGlobal_Flash->algpara.AlgParaConfUint[_g_stGlobal_Flash->algpara.AlgParaConfUsedIndex].AlgParaChooseGroup.bAlgPeopleCluster){
			((OutPutAPP_ClusterDetection_Ext *)Result)->_bClustered = 0;
			return 0 ;
		}else{
			pAppsvInterfaceList[ALG_PEOPLE_CLUSTER]->GetResult(&outPutAppClusterDetection);
			((OutPutAPP_ClusterDetection_Ext *)Result)->_bClustered = outPutAppClusterDetection._bClustered;
		}
		break;
	case ALG_ALARMAREA:
		if (pAppsvInterfaceList[ALG_ALARM_AREA] == NULL
			||!_g_stGlobal_Flash->algpara.AlgParaConfUint[_g_stGlobal_Flash->algpara.AlgParaConfUsedIndex].AlgParaChooseGroup.bAlgAlarmArea){
			((OutPutAPP_AlarmArea_Ext *)Result)->_ALG_AlarmArea_Alarm = 0;
			return 0 ;
		}else{
			pAppsvInterfaceList[ALG_ALARM_AREA]->GetResult(&outPutAppAlarmArea);
			((OutPutAPP_AlarmArea_Ext *)Result)->_ALG_AlarmArea_Alarm = outPutAppAlarmArea._ALG_AlarmArea_Alarm;
		}
		break;
	case ALG_ABNORMALFALL:
		if (pAppsvInterfaceList[ALG_PEOPLE_FALL] == NULL
			||!_g_stGlobal_Flash->algpara.AlgParaConfUint[_g_stGlobal_Flash->algpara.AlgParaConfUsedIndex].AlgParaChooseGroup.bAlgPeopleFall){
			((OutPutAPP_AbnormalFall_Ext *)Result)->_bFall = 0;
			return 0 ;
		}else{
			pAppsvInterfaceList[ALG_PEOPLE_FALL]->GetResult(&outPutAppAbnormalFall);
			((OutPutAPP_AbnormalFall_Ext *)Result)->_bFall = outPutAppAbnormalFall._bFall;
			((OutPutAPP_AbnormalFall_Ext *)Result)->_objrect = outPutAppAbnormalFall._objrect;
		}
		break;
	case ALG_HOVERDETECT:
		if (pAppsvInterfaceList[ALG_PEOPLE_HOVER] == NULL
			||!_g_stGlobal_Flash->algpara.AlgParaConfUint[_g_stGlobal_Flash->algpara.AlgParaConfUsedIndex].AlgParaChooseGroup.bAlgPeopleHover){
			((OutPutAPP_HoverDetect_Ext *)Result)->_objnum = 0;
			return 0 ;
		}else{
			pAppsvInterfaceList[ALG_PEOPLE_HOVER]->GetResult(&outPutAppHoverDetect);
			int i = 0;
			((OutPutAPP_HoverDetect_Ext *)Result)->_objnum = 0;
			for(i = 0; i< outPutAppHoverDetect._objnum ; i++)
			{
				if(outPutAppHoverDetect._obj_list[i]._objID != 0)
				{
					int index_one = outPutAppHoverDetect._obj_list[i]._alarm_area-1 / UNIT_HOVERDETE_PACKET;
					int index_two = outPutAppHoverDetect._obj_list[i]._alarm_area-1 % UNIT_HOVERDETE_PACKET;
					bool isEnable = isEnableAlgArea(_g_stGlobal_Flash->alghoverdectpara[index_one].hoverdeteUnit[index_two].scheduleunit ,_g_stGlobal_Flash->alghoverdectpara[index_one].hoverdeteUnit[index_two].validflag);
					if (isEnable)
					{
						((OutPutAPP_HoverDetect_Ext *)Result)->_obj_list[((OutPutAPP_HoverDetect_Ext *)Result)->_objnum]._alarm_area = outPutAppHoverDetect._obj_list[i]._alarm_area;
						((OutPutAPP_HoverDetect_Ext *)Result)->_obj_list[((OutPutAPP_HoverDetect_Ext *)Result)->_objnum]._objrect = outPutAppHoverDetect._obj_list[i]._objrect;				
						((OutPutAPP_HoverDetect_Ext *)Result)->_objnum ++;
					}
				}
			}
		}
		break;
	case ALG_FINDBAG:
		if (pAppsvInterfaceList[ALG_LEAVE_OVER_BAG] == NULL
			||!_g_stGlobal_Flash->algpara.AlgParaConfUint[_g_stGlobal_Flash->algpara.AlgParaConfUsedIndex].AlgParaChooseGroup.bAlgLeaveOverBag){
				((OutPutAPP_FindBag_Ext *)Result)->_objnum = 0;
				return 0 ;
		}else{
			pAppsvInterfaceList[ALG_LEAVE_OVER_BAG]->GetResult(&outPutAppFindBag);
			int i = 0;
			((OutPutAPP_FindBag_Ext *)Result)->_objnum = 0;
			for(i = 0; i< outPutAppFindBag._objnum ; i++)
			{
				((OutPutAPP_FindBag_Ext *)Result)->_objrect[i] = outPutAppFindBag._objrect[i];
				((OutPutAPP_FindBag_Ext *)Result)->_objnum++;
		
			}
		}
		break;
	case ALG_RUNNINGOBJ: //奔跑检测算法
		if (pAppsvInterfaceList[ALG_PEOPLE_RUNNING] == NULL
			||!_g_stGlobal_Flash->algpara.AlgParaConfUint[_g_stGlobal_Flash->algpara.AlgParaConfUsedIndex].AlgParaChooseGroup.bAlgPeopleRunning){
				((OutPutAPP_RunningDetect_Ext *)Result)->_objnum = 0;
				return 0 ;
		}else{
			pAppsvInterfaceList[ALG_PEOPLE_RUNNING]->GetResult(&outPutAppRunningDetect);
			int i = 0;
			((OutPutAPP_RunningDetect_Ext *)Result)->_objnum = 0;
			for(i = 0; i< outPutAppRunningDetect._objnum ; i++)
			{
				if(outPutAppRunningDetect._obj_list[i]._objID != 0)
				{
					int index_one = outPutAppRunningDetect._obj_list[i]._alarm-1 / UNIT_RUNNINGOBJ_PACKET;
					int index_two = outPutAppRunningDetect._obj_list[i]._alarm-1 % UNIT_RUNNINGOBJ_PACKET;
					bool isEnable = isEnableAlgArea(_g_stGlobal_Flash->algrunningpara[index_one].runningObjUnit[index_two].scheduleunit ,_g_stGlobal_Flash->algrunningpara[index_one].runningObjUnit[index_two].validflag);
					if (isEnable)
					{
						((OutPutAPP_RunningDetect_Ext *)Result)->_obj_list[((OutPutAPP_RunningDetect_Ext *)Result)->_objnum]._alarm_area = outPutAppRunningDetect._obj_list[i]._alarm;
						((OutPutAPP_RunningDetect_Ext *)Result)->_obj_list[((OutPutAPP_RunningDetect_Ext *)Result)->_objnum]._objrect = outPutAppRunningDetect._obj_list[i]._objrect;				
						((OutPutAPP_RunningDetect_Ext *)Result)->_objnum ++;
					}
				}
			}
		}
		break;
	case ALG_ALARMLINE:
		if (pAppsvInterfaceList[ALG_ALARM_LINE] == NULL
			||!_g_stGlobal_Flash->algpara.AlgParaConfUint[_g_stGlobal_Flash->algpara.AlgParaConfUsedIndex].AlgParaChooseGroup.bAlgAlarmLine){
				((OutPutAPP_AlarmLine_Ext *)Result)->_ALG_AlarmLine_Alarm = 0;
				return 0 ;
		}else{
			pAppsvInterfaceList[ALG_ALARM_LINE]->GetResult(&outPutAppAlarmLine);
			((OutPutAPP_AlarmLine_Ext *) Result)->_ALG_AlarmLine_Alarm = outPutAppAlarmLine._ALG_AlarmLine_Alarm;
		}
		break;
	case ALG_PEOPLELEAVE:
		return 0;
		break;
	default:
		return 0;
		break;
	}

	return 1;
}


EXPORT_API	unsigned int ALG_RELEASE()
{

	int i = ALG_ALARM_AREA_LINE;
	for(i= ALG_ALARM_AREA_LINE ; i <= ALG_PEOPLE_HOVER ; i++)
	{
		
		if((pAppsvInterfaceList[i]!=NULL))
		{
			pAppsvInterfaceList[i]->Release();
		}
	}
	return 1;
}

void ALG_InitAreaBackGlyphAreaInfo()
{
	backGlyphList.BackGlyphNum = 0;
	for(int i=0; i<32; i++)
	{
		for(int j=0; j<UNIT_REC_EVERY_PACKET; j++)
		{
		//	bool isEnable = isEnableAlgArea(_g_stGlobal_Flash->algquadpara[i].REC[j].scheduleunit,_g_stGlobal_Flash->algquadpara[i].REC[j].valid);
			
			if(_g_stGlobal_Flash->algquadpara[i].REC[j].valid == 1)
			{
				CvPoint ins1 = cvPoint(_g_stGlobal_Flash->algquadpara[i].REC[j].x1_start/g_PointFixedRate,
					g_stGlobalPicInfo.height - _g_stGlobal_Flash->algquadpara[i].REC[j].y1_start/g_PointFixedRate);
				CvPoint out1 = cvPoint(_g_stGlobal_Flash->algquadpara[i].REC[j].x2_start/g_PointFixedRate,
					g_stGlobalPicInfo.height - _g_stGlobal_Flash->algquadpara[i].REC[j].y2_start/g_PointFixedRate);
				CvPoint ins2 = cvPoint(_g_stGlobal_Flash->algquadpara[i].REC[j].x3_start/g_PointFixedRate,
					g_stGlobalPicInfo.height - _g_stGlobal_Flash->algquadpara[i].REC[j].y3_start/g_PointFixedRate);
				CvPoint out2 = cvPoint(_g_stGlobal_Flash->algquadpara[i].REC[j].x4_start/g_PointFixedRate,
					g_stGlobalPicInfo.height - _g_stGlobal_Flash->algquadpara[i].REC[j].y4_start/g_PointFixedRate);
				CvPoint ins3 = cvPoint(_g_stGlobal_Flash->algquadpara[i].REC[j].x5_start/g_PointFixedRate,
					g_stGlobalPicInfo.height - _g_stGlobal_Flash->algquadpara[i].REC[j].y5_start/g_PointFixedRate);
				CvPoint out3 = cvPoint(_g_stGlobal_Flash->algquadpara[i].REC[j].x6_start/g_PointFixedRate,
					g_stGlobalPicInfo.height - _g_stGlobal_Flash->algquadpara[i].REC[j].y6_start/g_PointFixedRate);
				CvPoint ins4 = cvPoint(_g_stGlobal_Flash->algquadpara[i].REC[j].x7_start/g_PointFixedRate,
					g_stGlobalPicInfo.height - _g_stGlobal_Flash->algquadpara[i].REC[j].y7_start/g_PointFixedRate);
				CvPoint out4 = cvPoint(_g_stGlobal_Flash->algquadpara[i].REC[j].x8_start/g_PointFixedRate,
					g_stGlobalPicInfo.height - _g_stGlobal_Flash->algquadpara[i].REC[j].y8_start/g_PointFixedRate);
				
// 				CvPoint OutsideBorder[4] = { out1 , out2 , out3 , out4 };
// 				CvPoint InsiderBorder[4] = { ins1 , ins2 , ins3 , ins4 };
				CvPoint OutsideBorder[4];
				OutsideBorder[0] = out1;
				OutsideBorder[1] = out2;
				OutsideBorder[2] = out3;
				OutsideBorder[3] = out4;
				CvPoint InsiderBorder[4];
				InsiderBorder[0] = ins1;
				InsiderBorder[1] = ins2;
				InsiderBorder[2] = ins3;
				InsiderBorder[3] = ins4;

				CvPoint ins5 = cvPoint(_g_stGlobal_Flash->algquadpara[i].REC[j].x9_start/g_PointFixedRate,
					g_stGlobalPicInfo.height - _g_stGlobal_Flash->algquadpara[i].REC[j].y9_start/g_PointFixedRate);
				CvPoint out5 = cvPoint(_g_stGlobal_Flash->algquadpara[i].REC[j].x10_start/g_PointFixedRate,
					g_stGlobalPicInfo.height - _g_stGlobal_Flash->algquadpara[i].REC[j].y10_start/g_PointFixedRate);
				CvPoint ins6 = cvPoint(_g_stGlobal_Flash->algquadpara[i].REC[j].x11_start/g_PointFixedRate,
					g_stGlobalPicInfo.height - _g_stGlobal_Flash->algquadpara[i].REC[j].y11_start/g_PointFixedRate);
				CvPoint out6 = cvPoint(_g_stGlobal_Flash->algquadpara[i].REC[j].x12_start/g_PointFixedRate,
					g_stGlobalPicInfo.height - _g_stGlobal_Flash->algquadpara[i].REC[j].y12_start/g_PointFixedRate);
				CvPoint ins7 = cvPoint(_g_stGlobal_Flash->algquadpara[i].REC[j].x13_start/g_PointFixedRate,
					g_stGlobalPicInfo.height - _g_stGlobal_Flash->algquadpara[i].REC[j].y13_start/g_PointFixedRate);
				CvPoint out7 = cvPoint(_g_stGlobal_Flash->algquadpara[i].REC[j].x14_start/g_PointFixedRate,
					g_stGlobalPicInfo.height - _g_stGlobal_Flash->algquadpara[i].REC[j].y14_start/g_PointFixedRate);
				CvPoint ins8 = cvPoint(_g_stGlobal_Flash->algquadpara[i].REC[j].x15_start/g_PointFixedRate,
					g_stGlobalPicInfo.height - _g_stGlobal_Flash->algquadpara[i].REC[j].y15_start/g_PointFixedRate);
				CvPoint out8 = cvPoint(_g_stGlobal_Flash->algquadpara[i].REC[j].x16_start/g_PointFixedRate,
					g_stGlobalPicInfo.height - _g_stGlobal_Flash->algquadpara[i].REC[j].y16_start/g_PointFixedRate);	
			
				switch(_g_stGlobal_Flash->algquadpara[i].REC[j].looptype)
				{
					case LOOPTYPE_THREE:
						break;
					case LOOPTYPE_FOUR:
						BackGlyphNodeExt backGlyNode ;
						backGlyNode.backGlyphID = i*UNIT_REC_EVERY_PACKET+j;
						backGlyNode.InsideBorder[0] = InsiderBorder[0] ;
						backGlyNode.InsideBorder[1] = InsiderBorder[1] ;
						backGlyNode.InsideBorder[2] = InsiderBorder[2] ;
						backGlyNode.InsideBorder[3] = InsiderBorder[3] ;
						backGlyNode.OutsideBorder[0] = OutsideBorder[0] ;
						backGlyNode.OutsideBorder[1] = OutsideBorder[1] ;
						backGlyNode.OutsideBorder[2] = OutsideBorder[2] ;
						backGlyNode.OutsideBorder[3] = OutsideBorder[3] ;
						backGlyphList.nodeList[backGlyphList.BackGlyphNum] = backGlyNode;
						backGlyphList.BackGlyphNum ++;
					break;
					case LOOPTYPE_FIVE:
						break;
					case LOOPTYPE_SIX:
						break;
					case LOOPTYPE_SEVEN:
						break;
					case LOOPTYPE_EIGHT:
						break;
					default:
						break;
				}
			}
			
		}
	}
	
	inputParamWindow._alarm_area.BackGlyphNum = backGlyphList.BackGlyphNum;
	for(int k=0;k<backGlyphList.BackGlyphNum;k++)
	{	
		inputParamWindow._alarm_area.nodeList[k].InsideBorder[0] =  backGlyphList.nodeList[k].InsideBorder[0];
		inputParamWindow._alarm_area.nodeList[k].InsideBorder[1] =  backGlyphList.nodeList[k].InsideBorder[1];
		inputParamWindow._alarm_area.nodeList[k].InsideBorder[2] =  backGlyphList.nodeList[k].InsideBorder[2];
		inputParamWindow._alarm_area.nodeList[k].InsideBorder[3] =  backGlyphList.nodeList[k].InsideBorder[3];
		inputParamWindow._alarm_area.nodeList[k].OutsideBorder[0] =  backGlyphList.nodeList[k].OutsideBorder[0];
		inputParamWindow._alarm_area.nodeList[k].OutsideBorder[1] =  backGlyphList.nodeList[k].OutsideBorder[1];
		inputParamWindow._alarm_area.nodeList[k].OutsideBorder[2] =  backGlyphList.nodeList[k].OutsideBorder[2];
		inputParamWindow._alarm_area.nodeList[k].OutsideBorder[3] =  backGlyphList.nodeList[k].OutsideBorder[3];
	}

	
	
}


void ALG_InitAreaLineAreaInfo()
{
	/*禁区算法*/
	
	inputAreaParam.AreaNum = 0;
	int i,j,k;
	bool isInsertFlag = false;
	for (i = 0; i<8; i++)
	{
		for(j = 0; j < CONFIG_UNIT_EVERY_PACKET; j++)
		{
		//	bool isEnable = isEnableAlgArea(_g_stGlobal_Flash->algareapara[i].configunit[j].scheduleunit , _g_stGlobal_Flash->algareapara[i].configunit[j].validflag);
			isInsertFlag = false;
			if(_g_stGlobal_Flash->algareapara[i].configunit[j].validflag  == 1)
			{
				inputAreaParam.areaParamList[inputAreaParam.AreaNum].areaPointNum = 0;
				for(int k = 0; k<MAX_UNIT_AREANUM ; k++)
				{
					if(_g_stGlobal_Flash->algareapara[i].configunit[j].config_unit[k].valid)
					{
						CvPoint point = cvPoint(_g_stGlobal_Flash->algareapara[i].configunit[j].config_unit[k].x/g_PointFixedRate,
							g_stGlobalPicInfo.height - _g_stGlobal_Flash->algareapara[i].configunit[j].config_unit[k].y/g_PointFixedRate);
						inputAreaParam.areaParamList[inputAreaParam.AreaNum].algType = ALG_AREA;
						
						inputAreaParam.areaParamList[inputAreaParam.AreaNum].areaID = i*CONFIG_UNIT_EVERY_PACKET+j;
						inputAreaParam.areaParamList[inputAreaParam.AreaNum].Area_p[inputAreaParam.areaParamList[inputAreaParam.AreaNum].areaPointNum] = point;
						inputAreaParam.areaParamList[inputAreaParam.AreaNum].areaPointNum ++ ;
						isInsertFlag = true;
					}
				}
				if(isInsertFlag)
				{
					inputAreaParam.AreaNum++;
				}
				
			}
		}
	}
	

	/*物体遗留*/

	/*物体消失*/

	/*防区配置*/
	inputLineParam.LineNum = 0;

	for(i = 0; i < 4 ; i++)
	{
		for(j = 0; j < UNIT_LINE_EVERY_PACKET ; j++)
		{
		//	bool isEnable = isEnableAlgArea(_g_stGlobal_Flash->alglinepara[i].lineunit[j].scheduleunit , _g_stGlobal_Flash->alglinepara[i].lineunit[j].validflag);
			if (_g_stGlobal_Flash->alglinepara[i].lineunit[j].validflag == 1)
			{
				CvPoint startPoint = cvPoint( _g_stGlobal_Flash->alglinepara[i].lineunit[j].x_start/g_PointFixedRate,
					g_stGlobalPicInfo.height - _g_stGlobal_Flash->alglinepara[i].lineunit[j].y_start/g_PointFixedRate);
				CvPoint endPoint = cvPoint( _g_stGlobal_Flash->alglinepara[i].lineunit[j].x_end/g_PointFixedRate,
					g_stGlobalPicInfo.height - _g_stGlobal_Flash->alglinepara[i].lineunit[j].y_end/g_PointFixedRate);
				LineExt lineParam;
				lineParam.algType = ALG_LINE;
				lineParam.countNum = 0;
				lineParam.direction = _g_stGlobal_Flash->alglinepara[i].lineunit[j].direction;
				lineParam.lineStart = startPoint;
				lineParam.lineEnd = endPoint;
				lineParam.lineID = i*UNIT_LINE_EVERY_PACKET+j;
				inputLineParam.lineList[inputLineParam.LineNum] = lineParam;
				inputLineParam.LineNum ++;
			}
			
		}
	}
	/*计数算法*/
	for(i = 0; i < 4 ; i++)
	{
		for(j = 0; j < UNIT_COUNT_EVERY_PACKET ; j++)
		{
		//	bool isEnable = isEnableAlgArea(_g_stGlobal_Flash->algcountpara[i].countunit[j].scheduleunit , _g_stGlobal_Flash->algcountpara[i].countunit[j].validflag);
			if (_g_stGlobal_Flash->algcountpara[i].countunit[j].validflag == 1)
			{
				CvPoint startPoint = cvPoint( _g_stGlobal_Flash->algcountpara[i].countunit[j].x_start/g_PointFixedRate,
					g_stGlobalPicInfo.height - _g_stGlobal_Flash->algcountpara[i].countunit[j].y_start/g_PointFixedRate);
				CvPoint endPoint = cvPoint( _g_stGlobal_Flash->algcountpara[i].countunit[j].x_end/g_PointFixedRate,
					g_stGlobalPicInfo.height - _g_stGlobal_Flash->algcountpara[i].countunit[j].y_end/g_PointFixedRate);
				LineExt lineParam;
				lineParam.algType = ALG_LINE_COUNT;
				lineParam.countNum = 0;
				lineParam.direction = _g_stGlobal_Flash->algcountpara[i].countunit[j].direction;
				lineParam.lineStart = startPoint;
				lineParam.lineEnd = endPoint;
				lineParam.lineID = i*UNIT_COUNT_EVERY_PACKET+j;
				inputLineParam.lineList[inputLineParam.LineNum] = lineParam;
				inputLineParam.LineNum ++;
			}

		}
	}
	/*目标进入*/
	for(i = 0; i < 4 ; i++)
	{
		for(j = 0; j < UNIT_ENTER_EVERY_PACKET ; j++)
		{
		//	bool isEnable = isEnableAlgArea( _g_stGlobal_Flash->algenterpara[i].enterunit[j].scheduleunit , _g_stGlobal_Flash->algenterpara[i].enterunit[j].validflag);
			if (_g_stGlobal_Flash->algenterpara[i].enterunit[j].validflag == 1)
			{
				CvPoint startPoint = cvPoint( _g_stGlobal_Flash->algenterpara[i].enterunit[j].x_start/g_PointFixedRate,
					g_stGlobalPicInfo.height - _g_stGlobal_Flash->algenterpara[i].enterunit[j].y_start/g_PointFixedRate);
				CvPoint endPoint = cvPoint( _g_stGlobal_Flash->algenterpara[i].enterunit[j].x_end/g_PointFixedRate,
					g_stGlobalPicInfo.height - _g_stGlobal_Flash->algenterpara[i].enterunit[j].y_end/g_PointFixedRate);
				LineExt lineParam;
				lineParam.algType = ALG_OBJ_ENTER;
				lineParam.countNum = 0;
				lineParam.direction = _g_stGlobal_Flash->algenterpara[i].enterunit[j].direction;
				lineParam.lineStart = startPoint;
				lineParam.lineEnd = endPoint;
				lineParam.lineID = i*UNIT_ENTER_EVERY_PACKET+j;
				inputLineParam.lineList[inputLineParam.LineNum] = lineParam;
				inputLineParam.LineNum ++;
			}

		}
	}
	/*目标离开*/
	for(i = 0; i < 4 ; i++)
	{
		for(j = 0; j < UNIT_OUT_EVERY_PACKET ; j++)
		{
		//	bool isEnable = isEnableAlgArea( _g_stGlobal_Flash->algoutpara[i].outunit[j].scheduleunit , _g_stGlobal_Flash->algoutpara[i].outunit[j].validflag);
			if (_g_stGlobal_Flash->algoutpara[i].outunit[j].validflag == 1)
			{
				CvPoint startPoint = cvPoint( _g_stGlobal_Flash->algoutpara[i].outunit[j].x_start/g_PointFixedRate,
					g_stGlobalPicInfo.height - _g_stGlobal_Flash->algoutpara[i].outunit[j].y_start/g_PointFixedRate);
				CvPoint endPoint = cvPoint( _g_stGlobal_Flash->algoutpara[i].outunit[j].x_end/g_PointFixedRate,
					g_stGlobalPicInfo.height - _g_stGlobal_Flash->algoutpara[i].outunit[j].y_end/g_PointFixedRate);
				LineExt lineParam;
				lineParam.algType = ALG_OBJ_OUT;
				lineParam.countNum = 0;
				lineParam.direction = _g_stGlobal_Flash->algoutpara[i].outunit[j].direction;
				lineParam.lineStart = startPoint;
				lineParam.lineEnd = endPoint;
				lineParam.lineID = i*UNIT_OUT_EVERY_PACKET+j;
				inputLineParam.lineList[inputLineParam.LineNum] = lineParam;
				inputLineParam.LineNum ++;
			}

		}
	}
	/*方向报警*/
	for(i = 0; i < 4 ; i++)
	{
		for(j = 0; j < UNIT_DIRECTION_EVERY_PACKET ; j++)
		{
		//	bool isEnable = isEnableAlgArea( _g_stGlobal_Flash->algdirectpara[i].directunit[j].scheduleunit , _g_stGlobal_Flash->algdirectpara[i].directunit[j].validflag);
			if (_g_stGlobal_Flash->algdirectpara[i].directunit[j].validflag == 1)
			{
				CvPoint startPoint = cvPoint( _g_stGlobal_Flash->algdirectpara[i].directunit[j].x_start/g_PointFixedRate, 
					g_stGlobalPicInfo.height - _g_stGlobal_Flash->algdirectpara[i].directunit[j].y_start/g_PointFixedRate);
				CvPoint endPoint = cvPoint( _g_stGlobal_Flash->algdirectpara[i].directunit[j].x_end/g_PointFixedRate,
					g_stGlobalPicInfo.height - _g_stGlobal_Flash->algdirectpara[i].directunit[j].y_end/g_PointFixedRate);
				LineExt lineParam;
				lineParam.algType = ALG_DIRECTION;
				lineParam.countNum = 0;
				lineParam.direction = _g_stGlobal_Flash->algdirectpara[i].directunit[j].direction;
				lineParam.lineStart = startPoint;
				lineParam.lineEnd = endPoint;
				lineParam.lineID = i*UNIT_DIRECTION_EVERY_PACKET+j;
				inputLineParam.lineList[inputLineParam.LineNum] = lineParam;
				inputLineParam.LineNum ++;
			}

		}
	}
	/*入侵检测*/
	for(i = 0; i < 4 ; i++)
	{
		for(j = 0; j < UNIT_DETECTION_EVERY_PACKET ; j++)
		{
		//	bool isEnable = isEnableAlgArea( _g_stGlobal_Flash->algdetectpara[i].dectunit[j].scheduleunit , _g_stGlobal_Flash->algdetectpara[i].dectunit[j].validflag);
			if (_g_stGlobal_Flash->algdetectpara[i].dectunit[j].validflag == 1)
			{
				CvPoint startPoint = cvPoint( _g_stGlobal_Flash->algdetectpara[i].dectunit[j].x_start/g_PointFixedRate,
					g_stGlobalPicInfo.height - _g_stGlobal_Flash->algdetectpara[i].dectunit[j].y_start/g_PointFixedRate);
				CvPoint endPoint = cvPoint( _g_stGlobal_Flash->algdetectpara[i].dectunit[j].x_end/g_PointFixedRate,
					g_stGlobalPicInfo.height - _g_stGlobal_Flash->algdetectpara[i].dectunit[j].y_end/g_PointFixedRate);
				LineExt lineParam;
				lineParam.algType = ALG_DETECTION;
				lineParam.countNum = 0;
				lineParam.direction = _g_stGlobal_Flash->algdetectpara[i].dectunit[j].direction;
				lineParam.lineStart = startPoint;
				lineParam.lineEnd = endPoint;
				lineParam.lineID = i*UNIT_DETECTION_EVERY_PACKET+j;
				inputLineParam.lineList[inputLineParam.LineNum] = lineParam;
				inputLineParam.LineNum ++;
			}

		}
	}


	/*配置禁区信息*/
	int totalPointNum = 0;
	inputParamAlarmAreaLine._al_alarm.AreaNum = inputAreaParam.AreaNum;
	for(k=0;k<inputAreaParam.AreaNum;k++)
	{
		inputParamAlarmAreaLine._al_alarm.AreaPointNum[k] = inputAreaParam.areaParamList[k].areaPointNum;
		for(j=0 ;j<inputAreaParam.areaParamList[k].areaPointNum;j++)
		{
			inputParamAlarmAreaLine._al_alarm.Area_P[totalPointNum] = inputAreaParam.areaParamList[k].Area_p[j];
			totalPointNum ++;
		}
	}
	/*配置防区信息*/
	
	inputParamAlarmAreaLine._al_alarm.LineNum = inputLineParam.LineNum;
	for(k=0; k<inputLineParam.LineNum ; k++)
	{
		switch(inputLineParam.lineList[k].direction)
		{
		case LINE_LEFT2RIGHT: //由左向右
			inputParamAlarmAreaLine._al_alarm.Line_Direct[k] = 1;
			break;
		case LINE_RIGHT2LEFT: //由右向左
			inputParamAlarmAreaLine._al_alarm.Line_Direct[k] = 2;
			break;
		case LINE_BOTH: //双向
			inputParamAlarmAreaLine._al_alarm.Line_Direct[k] = 0;
			break;
		default :
			inputParamAlarmAreaLine._al_alarm.Line_Direct[k] = 0;
			break;
		}
		inputParamAlarmAreaLine._al_alarm.Line_P[2*k] = inputLineParam.lineList[k].lineStart;
		inputParamAlarmAreaLine._al_alarm.Line_P[2*k+1] = inputLineParam.lineList[k].lineEnd;
	}
	

}


//人群检测算法防区设置
void ALG_InitAreaPeopleDense()
{
	int i = 0 , j = 0 , k =0;
	int totalPoint = 0;
	inputParamPeopleDense._detectArea.AreaNum = 0;
	for (i=0 ; i<4 ; i++)
	{
		for (j=0 ; j< UNIT_PEOPLE_DENSE_PACKET ; j++)
		{
			
			if(_g_stGlobal_Flash->algpeopledectpara[i].peopledenseunit[j].validflag == 1)
			{
				
				inputParamPeopleDense._detectArea.AreaPointNum[inputParamPeopleDense._detectArea.AreaNum]=0;
				for(k = 0; k<MAX_UNIT_AREANUM ; k++)  
				{
					if(_g_stGlobal_Flash->algpeopledectpara[i].peopledenseunit[j].config_unit[k].valid)
					{
						CvPoint point = cvPoint(_g_stGlobal_Flash->algpeopledectpara[i].peopledenseunit[j].config_unit[k].x/g_PointFixedRate,
							g_stGlobalPicInfo.height - _g_stGlobal_Flash->algpeopledectpara[i].peopledenseunit[j].config_unit[k].y/g_PointFixedRate);
						
						inputParamPeopleDense._detectArea.Area_P[totalPoint]=point;
						inputParamPeopleDense._detectArea.AreaPointNum[inputParamPeopleDense._detectArea.AreaNum]++;
						totalPoint++;
					}
				}
				
				inputParamPeopleDense._detectArea.AreaNum ++;
			}
		}
	}
	
}


//聚集算法防区设置
void Alg_InitAreaClusterDetection()
{
	int i = 0 , j = 0 , k =0;
	int totalPoint = 0;
	inputParamClusterDetection._detectArea.AreaNum = 0;
	for (i=0 ; i<4 ; i++)
	{
		for (j=0 ; j< UNIT_CLUSTER_DETE_PACKET ; j++)
		{

			if(_g_stGlobal_Flash->algclusterdectpara[i].clusterdeteunit[j].validflag == 1)
			{

				inputParamClusterDetection._detectArea.AreaPointNum[inputParamClusterDetection._detectArea.AreaNum]=0;
				for(k = 0; k<MAX_UNIT_AREANUM ; k++)  
				{
					if(_g_stGlobal_Flash->algclusterdectpara[i].clusterdeteunit[j].config_unit[k].valid)
					{
						CvPoint point = cvPoint(_g_stGlobal_Flash->algclusterdectpara[i].clusterdeteunit[j].config_unit[k].x/g_PointFixedRate,
							g_stGlobalPicInfo.height - _g_stGlobal_Flash->algclusterdectpara[i].clusterdeteunit[j].config_unit[k].y/g_PointFixedRate);

						inputParamClusterDetection._detectArea.Area_P[totalPoint]=point;
						inputParamClusterDetection._detectArea.AreaPointNum[inputParamClusterDetection._detectArea.AreaNum]++;
						totalPoint++;
					}
				}

				inputParamClusterDetection._detectArea.AreaNum ++;
			}
		}
	}
}
//夜间起床算法防区设置
void ALG_InitAreaAlarmArea()
{
	int i = 0 , j = 0 , k =0;
	int totalPoint = 0;
	inputParamAlarmArea._detectArea.AreaNum = 0;
	for (i=0 ; i<4 ; i++)
	{
		for (j=0 ; j< UNIT_MOTION_HISTORY_PACKET ; j++)
		{

			if(_g_stGlobal_Flash->algmotionhistorypara[i].motionhistoryunit[j].validflag == 1)
			{

				inputParamAlarmArea._detectArea.AreaPointNum[inputParamAlarmArea._detectArea.AreaNum]=0;
				for(k = 0; k<MAX_UNIT_AREANUM ; k++)  
				{
					if(_g_stGlobal_Flash->algmotionhistorypara[i].motionhistoryunit[j].config_unit[k].valid)
					{
						CvPoint point = cvPoint(_g_stGlobal_Flash->algmotionhistorypara[i].motionhistoryunit[j].config_unit[k].x/g_PointFixedRate,
							g_stGlobalPicInfo.height - _g_stGlobal_Flash->algmotionhistorypara[i].motionhistoryunit[j].config_unit[k].y/g_PointFixedRate);

						inputParamAlarmArea._detectArea.Area_P[totalPoint]=point;
						inputParamAlarmArea._detectArea.AreaPointNum[inputParamAlarmArea._detectArea.AreaNum]++;
						totalPoint++;
					}
				}

				inputParamAlarmArea._detectArea.AreaNum ++;
			}
		}
	}
}
//徘徊算法防区设置
void Alg_InitAreaHoverDetect()
{
	int i = 0 , j = 0 , k =0;
	int totalPoint = 0;
	inputParamHoverDetect._detectArea.AreaNum = 0;
	for (i=0 ; i<4 ; i++)
	{
		for (j=0 ; j< UNIT_HOVERDETE_PACKET ; j++)
		{

			if(_g_stGlobal_Flash->alghoverdectpara[i].hoverdeteUnit[j].validflag == 1)
			{

				inputParamHoverDetect._detectArea.AreaPointNum[inputParamHoverDetect._detectArea.AreaNum]=0;
				for(k = 0; k<MAX_UNIT_AREANUM ; k++)  
				{
					if(_g_stGlobal_Flash->alghoverdectpara[i].hoverdeteUnit[j].config_unit[k].valid)
					{
						CvPoint point = cvPoint(_g_stGlobal_Flash->alghoverdectpara[i].hoverdeteUnit[j].config_unit[k].x/g_PointFixedRate,
							g_stGlobalPicInfo.height - _g_stGlobal_Flash->alghoverdectpara[i].hoverdeteUnit[j].config_unit[k].y/g_PointFixedRate);

						inputParamHoverDetect._detectArea.Area_P[totalPoint]=point;
						inputParamHoverDetect._detectArea.AreaPointNum[inputParamHoverDetect._detectArea.AreaNum]++;
						totalPoint++;
					}
				}

				inputParamHoverDetect._detectArea.AreaNum ++;
			}
		}
	}
}


//摸高检测防区设置
void ALG_InitAreaAlarmLine()
{
	int i = 0 , j = 0 , k =0;
	
	inputParamAlarmLine._Alarm_Line_Num = 0;
	for (i=0 ; i<4 ; i++)
	{
		for (j=0 ; j< UNIT_ALARMLINE_PACKET ; j++)
		{

			if (_g_stGlobal_Flash->algalarmlinepara[i].alarmLine[j].validflag == 1)
			{
				CvPoint startPoint = cvPoint( _g_stGlobal_Flash->algalarmlinepara[i].alarmLine[j].x_start/g_PointFixedRate,
					g_stGlobalPicInfo.height - _g_stGlobal_Flash->algalarmlinepara[i].alarmLine[j].y_start/g_PointFixedRate);
				CvPoint endPoint = cvPoint( _g_stGlobal_Flash->algalarmlinepara[i].alarmLine[j].x_end/g_PointFixedRate,
					g_stGlobalPicInfo.height - _g_stGlobal_Flash->algalarmlinepara[i].alarmLine[j].y_end/g_PointFixedRate);
				
				inputParamAlarmLine._Alarm_Line_Point[inputParamAlarmLine._Alarm_Line_Num][0] = startPoint;
				inputParamAlarmLine._Alarm_Line_Point[inputParamAlarmLine._Alarm_Line_Num][1] = endPoint;
				inputParamAlarmLine._Alarm_Line_Num ++;
				
			}
		}
	}
}

//奔跑检测算法
void ALG_InitAreaRunningObj()
{
	int i = 0 , j = 0 , k =0;
	int totalPoint = 0;
	inputParamRunningDetect._detectArea.AreaNum = 0;
	for (i=0 ; i<4 ; i++)
	{
		for (j=0 ; j< UNIT_RUNNINGOBJ_PACKET ; j++)
		{

			if(_g_stGlobal_Flash->algrunningpara[i].runningObjUnit[j].validflag == 1)
			{

				inputParamRunningDetect._detectArea.AreaPointNum[inputParamRunningDetect._detectArea.AreaNum]=0;
				for(k = 0; k<MAX_UNIT_AREANUM ; k++)  
				{
					if(_g_stGlobal_Flash->algrunningpara[i].runningObjUnit[j].config_unit[k].valid)
					{
						CvPoint point = cvPoint(_g_stGlobal_Flash->algrunningpara[i].runningObjUnit[j].config_unit[k].x/g_PointFixedRate,
							g_stGlobalPicInfo.height - _g_stGlobal_Flash->algrunningpara[i].runningObjUnit[j].config_unit[k].y/g_PointFixedRate);

						inputParamRunningDetect._detectArea.Area_P[totalPoint]=point;
						inputParamRunningDetect._detectArea.AreaPointNum[inputParamRunningDetect._detectArea.AreaNum]++;
						totalPoint++;
					}
				}

				inputParamRunningDetect._detectArea.AreaNum ++;
			}
		}
	}
}

//离岗检测算法
void ALG_InitPeopleLeave()
{
		
}

//遗留物检测算法
void ALG_InitLeaveOverBag()
{
	int i = 0 , j = 0 , k =0;
	int totalPoint = 0;
	inputParamFindBag._detectArea.AreaNum = 0;
	for (i=0 ; i<4 ; i++)
	{
		for (j=0 ; j< UNIT_FINDBAG_PACKET ; j++)
		{
			
			if(_g_stGlobal_Flash->algfindbagpara[i].findBagUnit[j].validflag == 1)
			{
				
				inputParamFindBag._detectArea.AreaPointNum[inputParamFindBag._detectArea.AreaNum]=0;
				for(k = 0; k<MAX_UNIT_STOP ; k++)  
				{
					if(_g_stGlobal_Flash->algfindbagpara[i].findBagUnit[j].config_unit[k].valid)
					{
						CvPoint point = cvPoint(_g_stGlobal_Flash->algfindbagpara[i].findBagUnit[j].config_unit[k].x/g_PointFixedRate,
							g_stGlobalPicInfo.height - _g_stGlobal_Flash->algfindbagpara[i].findBagUnit[j].config_unit[k].y/g_PointFixedRate);
						
						inputParamFindBag._detectArea.Area_P[totalPoint]=point;
						inputParamFindBag._detectArea.AreaPointNum[inputParamFindBag._detectArea.AreaNum]++;
						totalPoint++;
					}
				}
				
				inputParamFindBag._detectArea.AreaNum ++;
			}
		}
	}
}

bool isEnableAlgArea(SCHEDULE_UNIT scheduleUnit[][MAX_SEGMENT_ONEDAY] , UINT isValid)
{
	if(isValid == 0)
	{
		return false;
	}

	int hour,minute;
	char dayOfWeek[64];
	char temp[64];
	time_t t = time( 0 );
	strftime( temp, sizeof(temp), "%Y/%m/%d %X %A",localtime(&t) ); 	
	sscanf(temp,"%*s%d:%d:%*d %s",&hour,&minute,dayOfWeek);

	int index = 0;
	if(strcmp(dayOfWeek,"Monday")==0){
		index = 0;
	}
	if(strcmp(dayOfWeek,"Tuesday")==0){
		index = 1;
	}
	if(strcmp(dayOfWeek,"Wendesday")==0){
		index = 2;
	}
	if(strcmp(dayOfWeek,"Thursday")==0){
		index = 3;
	}
	if(strcmp(dayOfWeek,"Friday")==0){
		index = 4;
	}
	if(strcmp(dayOfWeek,"Saturday")==0){
		index = 5;
	}
	if(strcmp(dayOfWeek,"Sunday")==0){
		index = 6;
	}

	int minuteOfDay = hour*60+minute;
	bool flag = false;
	for(int i=0; i < MAX_SEGMENT_ONEDAY; i++)
	{
		if( (minuteOfDay >= scheduleUnit[index][i].timestart) && (minuteOfDay <= scheduleUnit[index][i].timeend))
		{
			flag = true;
			break;
		}
	}

	return flag;
}


#if 1
void defaultParamWindowSet()
{
	inputParamWindow._imgw = g_stGlobalPicInfo.width;
	inputParamWindow._imgh = g_stGlobalPicInfo.height;
	inputParamWindow._iniframe = 10;
	inputParamWindow._grayThresh = 20;
	inputParamWindow._light_area_ratio = 30;
	inputParamWindow._disthresh = 20;
	inputParamWindow._max_tracksize = 20;
	inputParamWindow._lostcount = 2;	

	inputParamWindow._minfind = 6;
	inputParamWindow._minmove = 8;
	inputParamWindow._track_pos_diff_thresh = 0.3;
	inputParamWindow._track_rect_diff_thresh = 0.3;

	inputParamWindow._cont_minw = 2;
	inputParamWindow._cont_maxw = 15;
	inputParamWindow._cont_minh = 2;
	inputParamWindow._cont_maxh = 189;
	inputParamWindow._cont_area_rate = 0.3;

	inputParamWindow._rect_merge_flag = 1;
	inputParamWindow._rect_minw = 4;
	inputParamWindow._rect_minh = 12;
	inputParamWindow._rect_area_rate = 0.1;
	
	(inputParamWindow._areaROI).m_poly_point_num = 4;
	(inputParamWindow._areaROI).m_pv[0] = cvPoint(1,1);
	(inputParamWindow._areaROI).m_pv[1] = cvPoint(g_stGlobalPicInfo.width,1);
	(inputParamWindow._areaROI).m_pv[2] = cvPoint(g_stGlobalPicInfo.width,g_stGlobalPicInfo.height);
	(inputParamWindow._areaROI).m_pv[3] = cvPoint(1,g_stGlobalPicInfo.height);

}
void defaultParamAlarmAeraLine()
{

	inputParamAlarmAreaLine._imgw = g_stGlobalPicInfo.width;
	inputParamAlarmAreaLine._imgh = g_stGlobalPicInfo.height;
	inputParamAlarmAreaLine._iniframe = 10;
	inputParamAlarmAreaLine._thresh = 20;
	inputParamAlarmAreaLine._disthresh = 20;
	inputParamAlarmAreaLine._max_tracksize = 500 ;
	inputParamAlarmAreaLine._lostcount = 2;
	inputParamAlarmAreaLine._minfind = 6;
	inputParamAlarmAreaLine._minmove = 8;
	inputParamAlarmAreaLine._track_pos_diff_thresh = 0.3;
	inputParamAlarmAreaLine._track_rect_diff_thresh = 0.3;

	inputParamAlarmAreaLine._cont_minw = 2;
	inputParamAlarmAreaLine._cont_maxw = 140;
	inputParamAlarmAreaLine._cont_minh = 2;
	inputParamAlarmAreaLine._cont_maxh = 225;
	inputParamAlarmAreaLine._cont_area_rate = 0.3;

	inputParamAlarmAreaLine._rect_merge_flag = 0;
	inputParamAlarmAreaLine._rect_minw = 4;
	inputParamAlarmAreaLine._rect_minh = 14;
	inputParamAlarmAreaLine._rect_area_rate = 0.1;

	(inputParamAlarmAreaLine._areaROI).m_poly_point_num = 4;
	(inputParamAlarmAreaLine._areaROI).m_pv[0] = cvPoint(1,1);
	(inputParamAlarmAreaLine._areaROI).m_pv[1] = cvPoint(g_stGlobalPicInfo.width,1);
	(inputParamAlarmAreaLine._areaROI).m_pv[2] = cvPoint(g_stGlobalPicInfo.width,g_stGlobalPicInfo.height);
	(inputParamAlarmAreaLine._areaROI).m_pv[3] = cvPoint(1,g_stGlobalPicInfo.height);
}
void defaultParamPeopleDense()
{
	inputParamPeopleDense._bVanishPt = 2;
	inputParamPeopleDense._fSigma0 = 3;
	inputParamPeopleDense._fSigma1 = 1;
	inputParamPeopleDense._imgh = g_stGlobalPicInfo.height;
	inputParamPeopleDense._imgw = g_stGlobalPicInfo.width;
	inputParamPeopleDense._nLg = 5;
	inputParamPeopleDense._nLm = 5;
	inputParamPeopleDense._nNm = 5;
	inputParamPeopleDense._nNs = 6;
	inputParamPeopleDense._nOccHighestWarningThr = 30;
	inputParamPeopleDense._nSPNF = 0.2;
	inputParamPeopleDense._nT0 = 0.2;

	(inputParamPeopleDense._areaROI).m_poly_point_num = 4;
	(inputParamPeopleDense._areaROI).m_pv[0] = cvPoint(1,1);
	(inputParamPeopleDense._areaROI).m_pv[1] = cvPoint(g_stGlobalPicInfo.width,1);
	(inputParamPeopleDense._areaROI).m_pv[2] = cvPoint(g_stGlobalPicInfo.width,g_stGlobalPicInfo.height);
	(inputParamPeopleDense._areaROI).m_pv[3] = cvPoint(1,g_stGlobalPicInfo.height);
}

void defaultParamFightDetection()
{
	inputParamFightDetection._HypotenuseMax = 11.3;
	inputParamFightDetection._HypotenuseMin = 1.66;
	inputParamFightDetection._ImageHeight = g_stGlobalPicInfo.height;
	inputParamFightDetection._ImageWidth = g_stGlobalPicInfo.width;
	inputParamFightDetection._IsMovingCountThresh = 12;
	inputParamFightDetection._IsMovingThresh = 50;
	inputParamFightDetection._ROIBlockCountMax = 3;
	inputParamFightDetection._ROIBlockCountMin = 2;
	inputParamFightDetection._ROIBlockThresh = 100;
	inputParamFightDetection._UniDirectionCountThresh = 10;
	inputParamFightDetection._UnidirectionRatio = 1.2;

	(inputParamFightDetection._areaROI).m_poly_point_num = 4;
	(inputParamFightDetection._areaROI).m_pv[0] = cvPoint(1,1);
	(inputParamFightDetection._areaROI).m_pv[1] = cvPoint(g_stGlobalPicInfo.width,1);
	(inputParamFightDetection._areaROI).m_pv[2] = cvPoint(g_stGlobalPicInfo.width,g_stGlobalPicInfo.height);
	(inputParamFightDetection._areaROI).m_pv[3] = cvPoint(1,g_stGlobalPicInfo.height);

}

void defaultParamClusterDetection()
{
	inputParamClusterDetection._clustermovingCountThresh = 3;
	inputParamClusterDetection._HypotenuseMax = 11.3;
	inputParamClusterDetection._HypotenuseMin =0.6;
	inputParamClusterDetection._ImageHeight = g_stGlobalPicInfo.height;
	inputParamClusterDetection._ImageWidth = g_stGlobalPicInfo.width;
	inputParamClusterDetection._ROIBlockCountMinHeight = 3;
	inputParamClusterDetection._ROIBlockCountMinWidth = 3;
	inputParamClusterDetection._ROIBlockThresh = 9;

	(inputParamClusterDetection._areaROI).m_poly_point_num = 4;
	(inputParamClusterDetection._areaROI).m_pv[0] = cvPoint(1,1);
	(inputParamClusterDetection._areaROI).m_pv[1] = cvPoint(g_stGlobalPicInfo.width,1);
	(inputParamClusterDetection._areaROI).m_pv[2] = cvPoint(g_stGlobalPicInfo.width,g_stGlobalPicInfo.height);
	(inputParamClusterDetection._areaROI).m_pv[3] = cvPoint(1,g_stGlobalPicInfo.height);
}

void defaultParamAlarmArea()
{	
	inputParamAlarmArea._Imgh = g_stGlobalPicInfo.height;
	inputParamAlarmArea._Imgw = g_stGlobalPicInfo.width;
	inputParamAlarmArea._Diff_Threshold = 20;	
	inputParamAlarmArea._MHI_Duration = 1;
	inputParamAlarmArea._RatioDetect = 0.5;
	inputParamAlarmArea._RatioNoisze = 0.4;


	(inputParamAlarmArea._areaROI).m_poly_point_num = 4;
	(inputParamAlarmArea._areaROI).m_pv[0] = cvPoint(1,1);
	(inputParamAlarmArea._areaROI).m_pv[1] = cvPoint(g_stGlobalPicInfo.width,1);
	(inputParamAlarmArea._areaROI).m_pv[2] = cvPoint(g_stGlobalPicInfo.width,g_stGlobalPicInfo.height);
	(inputParamAlarmArea._areaROI).m_pv[3] = cvPoint(1,g_stGlobalPicInfo.height);


}

void defaultParamAlarmLine()
{
	inputParamAlarmLine._Diff_Threshold = 20;
	inputParamAlarmLine._Imgh = g_stGlobalPicInfo.height;
	inputParamAlarmLine._Imgw = g_stGlobalPicInfo.width;
	inputParamAlarmLine._MHI_Duration = 1;
	inputParamAlarmLine._RatioNoisze = 0.4;
	inputParamAlarmLine._ObjWidth = 50;

	(inputParamAlarmLine._areaROI).m_poly_point_num = 4;
	(inputParamAlarmLine._areaROI).m_pv[0] = cvPoint(1,1);
	(inputParamAlarmLine._areaROI).m_pv[1] = cvPoint(g_stGlobalPicInfo.width,1);
	(inputParamAlarmLine._areaROI).m_pv[2] = cvPoint(g_stGlobalPicInfo.width,g_stGlobalPicInfo.height);
	(inputParamAlarmLine._areaROI).m_pv[3] = cvPoint(1,g_stGlobalPicInfo.height);
}

void defaultParamFindBag()
{
	
	inputParamFindBag._imgw = g_stGlobalPicInfo.width;
	inputParamFindBag._imgh = g_stGlobalPicInfo.height;
	inputParamFindBag._thresh = 20;
	inputParamFindBag._iniframe = 100;
	inputParamFindBag._cont_minw = 2;
	inputParamFindBag._cont_maxw = 140;
	inputParamFindBag._cont_minh = 2;
	inputParamFindBag._cont_maxh = 225;

	(inputParamFindBag._areaROI).m_poly_point_num = 4;
	(inputParamFindBag._areaROI).m_pv[0] = cvPoint(1,1);
	(inputParamFindBag._areaROI).m_pv[1] = cvPoint(g_stGlobalPicInfo.width,1);
	(inputParamFindBag._areaROI).m_pv[2] = cvPoint(g_stGlobalPicInfo.width,g_stGlobalPicInfo.height);
	(inputParamFindBag._areaROI).m_pv[3] = cvPoint(1,g_stGlobalPicInfo.height);
}

void defaultParamRunningObj()
{
	
	inputParamRunningDetect._imgw = g_stGlobalPicInfo.width;
	inputParamRunningDetect._imgh = g_stGlobalPicInfo.height;
	inputParamRunningDetect._iniframe = 10;
	inputParamRunningDetect._thresh = 20;
	inputParamRunningDetect._disthresh = 20;
	inputParamRunningDetect._max_tracksize = 500 ;
	inputParamRunningDetect._lostcount = 2;
	inputParamRunningDetect._minfind = 6;
	inputParamRunningDetect._minmove = 8;
	inputParamRunningDetect._track_pos_diff_thresh = 0.3;
	inputParamRunningDetect._track_rect_diff_thresh = 0.3;

	inputParamRunningDetect._cont_minw = 2;
	inputParamRunningDetect._cont_maxw = 140;
	inputParamRunningDetect._cont_minh = 2;
	inputParamRunningDetect._cont_maxh = 225;
	inputParamRunningDetect._cont_area_rate = 0.3;

	inputParamRunningDetect._rect_merge_flag = 0;
	inputParamRunningDetect._rect_minw = 4;
	inputParamRunningDetect._rect_minh = 14;
	inputParamRunningDetect._rect_area_rate = 0.1;

	(inputParamRunningDetect._areaROI).m_poly_point_num = 4;
	(inputParamRunningDetect._areaROI).m_pv[0] = cvPoint(1,1);
	(inputParamRunningDetect._areaROI).m_pv[1] = cvPoint(g_stGlobalPicInfo.width,1);
	(inputParamRunningDetect._areaROI).m_pv[2] = cvPoint(g_stGlobalPicInfo.width,g_stGlobalPicInfo.height);
	(inputParamRunningDetect._areaROI).m_pv[3] = cvPoint(1,g_stGlobalPicInfo.height);
	
}

void defaultParamAbnormalFall()
{
	//inputParamAbnormalFall._IsMovingCountThresh;
	//inputParamAbnormalFall._UniDirectionCountThresh;
	//inputParamAbnormalFall._UnidirectionRatio;
	//inputParamAbnormalFall._ImageWidth;
	//inputParamAbnormalFall._ImageHeight;
	//inputParamAbnormalFall._HypotenuseMin;
	//inputParamAbnormalFall._HypotenuseMax;
	//inputParamAbnormalFall._IsMovingThresh;
	//inputParamAbnormalFall._ROIBlockThresh;
	//inputParamAbnormalFall._ROIBlockCountMax;//感兴趣区域宽度最大占的格数
	//inputParamAbnormalFall._ROIBlockCountMin;//感兴趣区域宽度最小占的格数


}
void defaultParamHoverDetect()
{
	//inputParamHoverDetect._cont_area_rate = ;
	//inputParamHoverDetect._cont_maxh = ;
	//inputParamHoverDetect._cont_maxw =;
	//inputParamHoverDetect._cont_minh =  ;
	//inputParamHoverDetect._cont_minw = ;
	//inputParamHoverDetect._disthresh =  ;
	//inputParamHoverDetect._imgh = ;
	//inputParamHoverDetect._imgw =  ;
	//inputParamHoverDetect._iniframe =  ;
	//inputParamHoverDetect._lostcount = ;
	//inputParamHoverDetect._max_tracksize = ;
	//inputParamHoverDetect._minfind =  ;
	//inputParamHoverDetect._minmove =  ;
	//inputParamHoverDetect._thresh =  ;
	//inputParamHoverDetect._timeThresh =  ;
	//inputParamHoverDetect._track_pos_diff_thresh = ;
	//inputParamHoverDetect._track_rect_diff_thresh =  ;

	
}


void defaultParamPeopleLeave();
#endif
