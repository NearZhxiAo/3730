#ifndef ALGCONFIG_H_INCLUDED
#define ALGCONFIG_H_INCLUDED

//#include "../cvApi/cv.h"
//#include "../ParamMgr.h"


#ifdef _cplusplus
extern "C" {
#endif

typedef enum _ALG_EXTEND_TYPE
{
	ALG_TYPE_START,

	ALG_REC,			//������
	ALG_AREA,			//����
	ALG_LINE,			//����
	ALG_LINE_COUNT,		//����
	ALG_OBJ_ENTER,		//Ŀ�����
	ALG_OBJ_OUT,		//Ŀ���뿪
//	ALG_OBJ_STOP,		//��������
	ALG_OBJ_LOST,		//������ʧ
	ALG_DIRECTION,		//���򱨾�
	ALG_DETECTION,		//�������
	
	ALG_PEOPLEDENSE, //�����ܶ�
	ALG_FIGHTDETECTION, //��ܼ��
	ALG_CLUSTERDETECTION, //�ۼ����
	ALG_ALARMAREA, //ҹ���𴲼��   modify(modify) by hzc 2012-05-20
	ALG_ABNORMALFALL, //�쳣����
	ALG_HOVERDETECT, //�ǻ����

	ALG_FINDBAG, //��������   modify(add) by hzc 2012-05-20
	ALG_RUNNINGOBJ, //���屼�ܼ�� modify(add) by hzc 2012-05-20
	ALG_PEOPLELEAVE, //��ڼ��  modify(add) by hzc 2012-05-20
	ALG_ALARMLINE, //���߼�� modify (add) by hzc 2012-05-20
	ALG_AUDIO,
  	ALG_TYPE_END
}ALG_EXTEND_TYPE;
	
typedef enum _ALG_WORK_MODE
{
	ALG_WORK_MODE_START,
    ALG_BACKGLY_MODE,
    ALG_AREALINE_MODE,
    ALG_BOTH_MODE,
    ALG_WORK_MODE_END
}ALG_WORK_MODE;


typedef struct _BackGlyphNodeExt
{
	USHORT backGlyphID;
	CvPoint OutsideBorder[4];
	CvPoint InsideBorder[4];
}BackGlyphNodeExt;

#define MAX_BACKGLYPH_NUM_EXT 30
typedef struct _BackGlyphAreaExtList
{
	USHORT BackGlyphNum;
	BackGlyphNodeExt nodeList[MAX_BACKGLYPH_NUM_EXT];
	_BackGlyphAreaExtList()
	{
		BackGlyphNum = 0;
	}
}BackGlyphAreaExtList;

struct XIAN_ObjNodeExt
{
	USHORT _alarm;
	CvRect _object;
};

#define  MAX_OBJNODEEXT 128
typedef struct _OutPutAPP_XIAN_WindowExt
{
	USHORT _objnum;
	XIAN_ObjNodeExt _obj_list[MAX_OBJNODEEXT];
// 	_OutPutAPP_XIAN_WindowExt()
// 	{
// 		_objnum =0 ;
// 	}
}OutPutAPP_XIAN_WindowExt;

#define MAX_ALARM_AL_NUM_EXT 70
typedef struct _LineExt{
	ALG_EXTEND_TYPE algType;
	USHORT lineID;
//	USHORT objID;
	USHORT countNum;
	CvPoint lineStart;
	CvPoint lineEnd;
	LINE_DIR direction;
}LineExt;

typedef struct _AlarmLineParamExt
{	
	
	USHORT LineNum;
	LineExt lineList[MAX_ALARM_AL_NUM_EXT];
	_AlarmLineParamExt()
	{
		LineNum = 0;
	}
}AlarmLineParamExt;

typedef struct _Area_Node_Rel{
	//USHORT objID;
	USHORT areaID;
	ALG_EXTEND_TYPE algType;
	USHORT areaPointNum;
	CvPoint Area_p[50];
}Area_node_rea;

typedef struct _AreaParamExt{
	USHORT AreaNum;
	Area_node_rea areaParamList[MAX_ALARM_AL_NUM_EXT];
	_AreaParamExt()
	{
		AreaNum = 0;
	}
}AreaParamExt;

	 
#define MAX_DETECT_AREA_NUM 16  //��󱨾��������
#define MAX_DETECT_AERA_POINT_NUM 20 //ÿ�������������Ŀ
typedef struct _DetectAreaParam_Ext 
{
	int AreaNum;
	int AreaPointNum[MAX_DETECT_AREA_NUM];
	CvPoint Area_P[MAX_DETECT_AREA_NUM*MAX_DETECT_AERA_POINT_NUM];
	_DetectAreaParam_Ext()
	{
		AreaNum = 0;
	}
}DetectAreaParam_Ext;

typedef struct _OutPutAPP_AlarmLine_Ext{
	bool _ALG_AlarmLine_Alarm;
}OutPutAPP_AlarmLine_Ext;

typedef struct _OutPutAPP_AlarmArea_Ext{
	bool _ALG_AlarmArea_Alarm;
}OutPutAPP_AlarmArea_Ext;

typedef struct _OutPutAPP_PeopleDense_Ext
{
	float _dense;
}OutPutAPP_PeopleDense_Ext;

/*��ܼ�������ṹ*/
typedef struct _OutPutAPP_FightDetection_Ext
{
	bool _bFighting;
}OutPutAPP_FightDetection_Ext;

/*�ۼ�����㷨����ṹ*/
typedef struct _OutPutAPP_ClusterDetection_Ext
{
	bool _bClustered;
}OutPutAPP_ClusterDetection_Ext;

//modify (delete) by hzc 2012-05-20	
///*ҹ���𴲼���㷨����ṹ*/
//typedef struct _OutPutAPP_MotionHistory_Ext
//{
//	bool _Alarm;
//}OutPutAPP_MotionHistory_Ext;

typedef struct _OutPutAPP_AbnormalFall_Ext
{
	bool _bFall;
	CvRect _objrect;
}OutPutAPP_AbnormalFall_Ext;

typedef struct _HOVER_ObjNode_Ext
{
	int _alarm_area;
	CvRect _objrect;
}HOVER_ObjNode_Ext;

typedef struct _OutPutAPP_HoverDetect_Ext
{
	int _objnum;
	HOVER_ObjNode_Ext _obj_list[128];
// 	_OutPutAPP_HoverDetect_Ext()
// 	{
// 		_objnum = 0;
// 	}
}OutPutAPP_HoverDetect_Ext;

typedef struct _Running_ObjNode_Ext
{
	int _alarm_area;
	CvRect _objrect;
}Running_ObjNode_Ext;

typedef struct _OutPutAPP_RunningDetect_Ext
{
	int _objnum;
	Running_ObjNode_Ext _obj_list[128];
// 	_OutPutAPP_RunningDetect_Ext()
// 	{
// 		_objnum = 0;
// 	}
}OutPutAPP_RunningDetect_Ext;

typedef struct _OutPutAPP_FindBag_Ext
{
	int _objnum;
	CvRect _objrect[64];
}OutPutAPP_FindBag_Ext;


typedef union _OutPutApp_Result_Ext
{
	OutPutAPP_XIAN_WindowExt outputwindow;
	OutPutAPP_PeopleDense_Ext peopledense;
	OutPutAPP_FightDetection_Ext fightdetection;
	OutPutAPP_ClusterDetection_Ext clusterdetection;
	OutPutAPP_AlarmArea_Ext  alarmarea;
	OutPutAPP_AlarmLine_Ext  alarmline;
	OutPutAPP_AbnormalFall_Ext abnormalfall;
	OutPutAPP_HoverDetect_Ext hoverdetect;
	OutPutAPP_RunningDetect_Ext running;
	OutPutAPP_FindBag_Ext findbag;
}OutPutApp_Result_Ext;

#ifdef __cplusplus
}
#endif


#endif // ALGCONFIG_H_INCLUDED
