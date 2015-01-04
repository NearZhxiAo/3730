#ifndef AREACONF_H_INCLUDED
#define AREACONF_H_INCLUDED

#ifdef _cplusplus
extern "C" {
#endif

#define USHORT unsigned short
#define SHORT  short
#define UINT   unsigned int

/*�������ò���*/
typedef struct _SCHEDULE_UNIT
{
    USHORT  timestart;		//��ʼʱ�䣬���շ��ӽ��д洢�����磺1:31�ִ洢Ϊ1*60+31 = 91
    USHORT  timeend; 		//����ʱ�䣬�ڲ��Է�Ϊ��λ��24Сʱ����1440����
}SCHEDULE_UNIT; 

typedef enum _LOOP_TYPE
{
    LOOPTYPE_START,					//�����η����ĵ��������㹹��
    LOOPTYPE_THREE,					//�ĵ㹹��
    LOOPTYPE_FOUR,					//...
	LOOPTYPE_FIVE,
	LOOPTYPE_SIX,
	LOOPTYPE_SEVEN,
    LOOPTYPE_EIGHT,					//���Ϊ8���㹹��һ��������
    LOOPTYPE_END
}LOOP_TYPE;

typedef enum _COLOR_CHOOSE
{
    COLOR_START,					//������ɫ
    COLOR_GREEN,					//��
    COLOR_BLUE,						//��
	COLOR_YELLOW,					//��
	COLOR_BLACK,					//��
	COLOR_ORANGE,					//��
	COLOR_PURPLE,					//��
	COLOR_PINK,                     //��
    COLOR_END
}COLOR_CHOOSE;

typedef struct _UNIT_AREA
{
	SHORT x;				 //�������	
	SHORT y;
	UINT   valid;            //�õ��Ƿ���Ч��־���п���Ϊ8������ֻ��С��8������Ч
}UNIT_AREA;    //��СΪ2 UINT

typedef  enum _LINE_DIR
{
	LINE_START,
	LINE_RIGHT2LEFT,    	//��������
	LINE_LEFT2RIGHT,     	//��������
	LINE_BOTH,		//˫��
	LINE_END
}LINE_DIR;

#define MAX_SEGMENT_ONEDAY	8
//��������(������)
typedef struct _UNIT_REC
{
    USHORT valid;   	//�û����������Ƿ���Ч
	USHORT width;  		//�û���������Ŀ���
	
	//���궨�壺������Ϊ��Ȧ��˳������Ϊx1,x3,x5,x7,x9,x11,x13,x15��ż����Ϊ���ζ�Ӧ����Ȧ��x2��x1��Ӧ
	SHORT x1_start;	//
	SHORT y1_start;
	SHORT x2_start;
	SHORT y2_start;
	SHORT x3_start;
	SHORT y3_start;
	SHORT x4_start;
	SHORT y4_start;
	SHORT x5_start;
	SHORT y5_start;
	SHORT x6_start;
	SHORT y6_start;
	SHORT x7_start;
	SHORT y7_start;
	SHORT x8_start;
	SHORT y8_start;
	SHORT x9_start;
	SHORT y9_start;
	SHORT x10_start;
	SHORT y10_start;
	SHORT x11_start;
	SHORT y11_start;
	SHORT x12_start;
	SHORT y12_start;
	SHORT x13_start;
	SHORT y13_start;
	SHORT x14_start;
	SHORT y14_start;
	SHORT x15_start;
	SHORT y15_start;
	SHORT x16_start;
	SHORT y16_start;   	//���8�߻����ε�����
	COLOR_CHOOSE  color;	//�������������ɫ����
	LOOP_TYPE     looptype;	//���������ͣ������Ρ��ı���...�˱��Σ�
   	SCHEDULE_UNIT scheduleunit[7][MAX_SEGMENT_ONEDAY];//�������ÿ��8��ʱ��Σ���СΪ224�ֽ�
	UINT   preset;
	UINT   pretime;
}UNIT_REC;   //��СΪ32UINT���޸ĺ��Ϊ72+224=296�ֽ�

#define MAX_UNIT_RECNUM	128
typedef struct _CONFIG_REC
{
	UNIT_REC REC[MAX_UNIT_RECNUM];   //�������128����������ÿ��MTU���ֻ�ܴ���4��������������Ҫ����128������Ҫ���͵���ĿΪ32��
}CONFIG_REC; //�����СΪ64*33 = 1152UINT


//��������(����)
#define MAX_UNIT_AREANUM	8
typedef struct _CONFIG_UNIT
{
    UNIT_AREA       config_unit[MAX_UNIT_AREANUM];  //ÿ�����������8���㹹�ɣ���СΪ16 UINT
    COLOR_CHOOSE    color;   //�����������ɫ���ã��ߣ�
	SCHEDULE_UNIT   scheduleunit[7][MAX_SEGMENT_ONEDAY]; //�������ÿ��8��ʱ��Σ���СΪ224�ֽ�
    UINT            validflag;  //�˽�����Ч�Ա�־   
	UINT            preset;// Ԥ�õ�
	UINT			pretime;
}CONFIG_UNIT; //39��UNIT��Ԫ 18*4 + 224 = 296 bytes

#define MAX_CONFIG_AREANUM	32
typedef struct _CONFIG_AREA
{
    CONFIG_UNIT config_area[MAX_CONFIG_AREANUM];  //������32����������СΪ32*39 UINT����Ҫ����8��
}CONFIG_AREA;


//��������(����)
typedef struct _UNIT_LINE
{
	SHORT        x_start;		
	SHORT        y_start;
	SHORT        x_end;
	SHORT        y_end;
	LINE_DIR      direction; 	//ö������
	COLOR_CHOOSE  color; 	//���ߵ���ɫ����
	SCHEDULE_UNIT scheduleunit[7][MAX_SEGMENT_ONEDAY]; //�������ÿ��8��ʱ��Σ���СΪ224�ֽ�
	UINT          validflag;  //�˰�����Ч�Ա�־
	UINT          preset;// Ԥ�õ�
	UINT		  pretime;
}UNIT_LINE;  //��СΪ20UINT��4*5+224 = 244 bytes

#define CONFIG_LINENUM	16
typedef struct _CONFIG_LINE
{
    UNIT_LINE config_line[CONFIG_LINENUM];     //������16���������ã���СΪ320 UINT����Ϊ4�ν��д���
}CONFIG_LINE;


//��������(����)
typedef struct _UNIT_LINE_COUNT
{
	SHORT        x_start;
	SHORT        y_start;
	SHORT        x_end;
	SHORT        y_end;
	LINE_DIR      direction; //ö������
	UINT          count;
	COLOR_CHOOSE  color;//���ߵ���ɫ����
	SCHEDULE_UNIT scheduleunit[7][MAX_SEGMENT_ONEDAY]; //�������ÿ��8��ʱ��Σ���СΪ224�ֽ�
	UINT          validflag;          //�˼�����������Ч�Ա�־
	UINT          preset;// Ԥ�õ�
	UINT		  pretime;
}UNIT_COUNT;  //��СΪ21 UINT 6*4+224 = 248Bytes

#define UNIX_COUNTNUM	16
typedef struct _COUNT_LINE
{
    UNIT_COUNT count_line[UNIX_COUNTNUM];    //������16����������,21*16*4����Ϊ4�ν��д���
}COUNT_LINE;


//Ŀ�����(����)
typedef struct _UNIT_LINE_ENTER
{
	SHORT        x_start;
	SHORT        y_start;
	SHORT        x_end;
	SHORT        y_end;
	LINE_DIR      direction; //ö������
	COLOR_CHOOSE  color;//���ߵ���ɫ����
	SCHEDULE_UNIT scheduleunit[7][MAX_SEGMENT_ONEDAY];//�������ÿ��8��ʱ��Σ���СΪ224�ֽ�
	UINT          validflag;  //�˰�����Ч�Ա�־
	UINT          preset;// Ԥ�õ�
	UINT		  pretime;
}UNIT_ENTER;  //��СΪ20 UINT����СΪ5*4+224 = 244Bytes
#define UNIT_ENTERNUM	16
typedef struct _TARGET_ENTER
{
   UNIT_ENTER  UNITENTER[UNIT_ENTERNUM];    //������16���������ߣ���Ϊ4�ν��д���
}TARGET_ENTER;


//Ŀ���뿪(����)
typedef struct _UNIT_OUT
{
	SHORT        x_start;
	SHORT        y_start;
	SHORT        x_end;
	SHORT        y_end;
	LINE_DIR      direction; //ö������
	COLOR_CHOOSE  color;     //���ߵ���ɫ����
	SCHEDULE_UNIT scheduleunit[7][MAX_SEGMENT_ONEDAY];//�������ÿ��8��ʱ��Σ���СΪ224�ֽ�
    UINT          validflag;  //�˰�����Ч�Ա�־
	UINT          preset;// Ԥ�õ�
	UINT		  pretime;
}UNIT_OUT;  //��СΪ5 UINT����СΪ5*4+224 = 244bytes
#define TARGET_OUTNUM	16
typedef struct _TARGET_OUT
{
    UNIT_OUT UNITOUT[TARGET_OUTNUM];    //������16���������ߣ���Ϊ4�ν��д���
}TARGET_OUT;


//��������(����)
#define MAX_UNIT_STOP	10
typedef struct _CONFIG_STOP
{
	UNIT_AREA     config_unit[MAX_UNIT_STOP];  //ÿ�����������10���㹹�ɣ���СΪ20 UINT
	COLOR_CHOOSE  color;//���ߵ���ɫ����
	SCHEDULE_UNIT scheduleunit[7][MAX_SEGMENT_ONEDAY];//�������ÿ��8��ʱ��Σ���СΪ224�ֽ�
	UINT          validflag;  //�˰�����Ч�Ա�־
	UINT          preset;// Ԥ�õ�
	UINT		  pretime;
}CONFIG_STOP;  //��СΪ45��UNIT 22*4+224 = 312Bytes

#define CONFIG_STOPNUM	6
typedef struct _OBJECT_STOP
{
	CONFIG_STOP configstop[CONFIG_STOPNUM]; //������6����������СΪ270 UINT����Ϊ2�ν��д���
}OBJECT_STOP;

//������ʧ(����)
typedef struct _CONFIG_LOST
{
	UNIT_AREA     config_unit[10];  //ÿ�����������10���㹹�ɣ���СΪ20 UINT
	COLOR_CHOOSE  color;//���ߵ���ɫ����
	SCHEDULE_UNIT scheduleunit[7][MAX_SEGMENT_ONEDAY];//�������ÿ��8��ʱ��Σ���СΪ224�ֽ�
	UINT          validflag;  //�˰�����Ч�Ա�־
	UINT          preset;// Ԥ�õ�
	UINT		  pretime;
}CONFIG_LOST;

#define CONFIG_LOSTNUM	6
typedef struct _OBJECT_LOST
{
    CONFIG_LOST configlost[CONFIG_LOSTNUM]; //������6����������СΪ270 UINT����Ϊ2�ν��д���
}OBJECT_LOST;

//���򱨾�(����)
typedef struct _UNIT_DIRECTION
{
	SHORT        x_start;
	SHORT		  y_start;
	SHORT		  x_end;
	SHORT        y_end;
	LINE_DIR      direction; //ö������
	COLOR_CHOOSE  color;//���ߵ���ɫ����
	SCHEDULE_UNIT scheduleunit[7][MAX_SEGMENT_ONEDAY];//�������ÿ��8��ʱ��Σ���СΪ224�ֽ�
    UINT          validflag;  //�˰�����Ч�Ա�־
	UINT          preset;// Ԥ�õ�
	UINT		  pretime;
}UNIT_DIRECTION;  //��СΪ20UINT����СΪ5*4+224 = 244bytes

#define UNIX_DIRECTIONNUM	16
typedef struct _DIRECTION_ALARM
{
    UNIT_DIRECTION  UNITDIRECTION[UNIX_DIRECTIONNUM]; //������16���������ߣ���Ϊ4�ν��д���
}DIRECTION_ALARM;


//�������(����)
typedef struct _UNIT_DETECTION
{
	SHORT        x_start;
	SHORT        y_start;
	SHORT        x_end;
	SHORT        y_end;
	LINE_DIR      direction; //ö������
	COLOR_CHOOSE  color;//���ߵ���ɫ����
	SCHEDULE_UNIT scheduleunit[7][MAX_SEGMENT_ONEDAY];//�������ÿ��8��ʱ��Σ���СΪ224�ֽ�
	UINT          validflag;  //�˰�����Ч�Ա�־
	UINT          preset;// Ԥ�õ�
	UINT		  pretime;
}UNIT_DETECTION;   //��СΪ5 UINT��5*4+224 = 244bytes

#define UNIT_DETECTIONNUM	16
typedef struct _INVADE_DETECTION
{
	UNIT_DETECTION  UNITDETECTION[UNIT_DETECTIONNUM]; //������16���������ߣ���Ϊ4�ν��д���
}INVADE_DETECTION;


//������״ĳ4������(0~127)����
#define UNIT_REC_EVERY_PACKET	4
typedef  struct  _ALG_QUAD_PARA
{
    UNIT_REC REC[UNIT_REC_EVERY_PACKET];
}ALG_QUAD_PARA;


//������״ĳ4������(0~31)����
#define CONFIG_UNIT_EVERY_PACKET	4
typedef  struct _ALG_AREA_PARA
{
    CONFIG_UNIT configunit [CONFIG_UNIT_EVERY_PACKET];
}ALG_AREA_PARA;

//��������(0~15)
#define UNIT_LINE_EVERY_PACKET	4
typedef  struct _ALG_LINE_PARA
{
    UNIT_LINE  lineunit[UNIT_LINE_EVERY_PACKET];
}ALG_LINE_PARA;

//��������(0~15)
#define UNIT_COUNT_EVERY_PACKET	4
typedef  struct _ALG_COUNT_PARA
{
    UNIT_COUNT  countunit[UNIT_COUNT_EVERY_PACKET];
}ALG_COUNT_PARA;

//Ŀ���������(0~15)
#define UNIT_ENTER_EVERY_PACKET 4
typedef  struct _ALG_ENTER_PARA
{
    UNIT_ENTER  enterunit[UNIT_ENTER_EVERY_PACKET];
}ALG_ENTER_PARA;

//Ŀ���뿪����(0~15)
#define UNIT_OUT_EVERY_PACKET 4
typedef  struct _ALG_OUT_PARA
{
    UNIT_OUT outunit[UNIT_OUT_EVERY_PACKET];
}ALG_OUT_PARA;

//������������(0~5)
#define CONFIG_STOP_EVERY_PACKET 3
typedef  struct _ALG_STOP_PARA
{
    CONFIG_STOP stopunit[CONFIG_STOP_EVERY_PACKET];
}ALG_STOP_PARA;

//������ʧ����(0~5)
#define CONFIG_LOST_EVERY_PACKET 3
typedef  struct _ALG_LOST_PARA
{
    CONFIG_LOST lostunit[CONFIG_LOST_EVERY_PACKET];
}ALG_LOST_PARA;

//���򱨾�����(0~15)
#define UNIT_DIRECTION_EVERY_PACKET 4
typedef  struct _ALG_DIRECT_PARA
{
    UNIT_DIRECTION directunit[UNIT_DIRECTION_EVERY_PACKET];
}ALG_DIRECT_PARA;

//�����������(0~15)
#define UNIT_DETECTION_EVERY_PACKET 4
typedef  struct _ALG_DETECT_PARA
{
    UNIT_DETECTION dectunit[UNIT_DETECTION_EVERY_PACKET];
}ALG_DETECT_PARA;


//�����㷨��������  
//����(����)
/*
#define MAX_UNIT_PEOPEL_DENSE_NUM	4
typedef struct _PEOPLEDENSE_UNIT
{
UNIT_AREA       config_unit[MAX_UNIT_PEOPEL_DENSE_NUM];  //ÿ�����������8���㹹�ɣ���СΪ16 UINT
COLOR_CHOOSE    color;   //�����������ɫ���ã��ߣ�
SCHEDULE_UNIT   scheduleunit[7][MAX_SEGMENT_ONEDAY]; //�������ÿ��8��ʱ��Σ���СΪ224�ֽ�
UINT            validflag;  //�˽�����Ч�Ա�־   
}CONFIG_UNIT; //39��UNIT��Ԫ 18*4 + 224 = 296 bytes
*/
//��Ⱥ�ܶ�
#define UNIT_PEOPLE_DENSE_PACKET 4
typedef struct _ALG_PEOPLEDENSE_PARA{
	CONFIG_UNIT peopledenseunit[UNIT_PEOPLE_DENSE_PACKET];
}ALG_PEOPLEDENSE_PARA;
//�ۼ��㷨��������
#define UNIT_CLUSTER_DETE_PACKET 4
typedef struct _ALG_CLUSTERDETE_PARA{
	CONFIG_UNIT clusterdeteunit[UNIT_CLUSTER_DETE_PACKET];
}ALG_CLUSTERDETE_PARA;
//��������⣨ҹ���𴲣�
#define UNIT_MOTION_HISTORY_PACKET 4
typedef struct _ALG_MOTIONHISTORY_PARA{
	CONFIG_UNIT motionhistoryunit[UNIT_MOTION_HISTORY_PACKET];
}ALG_MOTIONHISTORY_PARA;
//�ǻ��㷨��������
#define UNIT_HOVERDETE_PACKET 4
typedef struct _ALG_HOVERDETE_PARA{
	CONFIG_UNIT hoverdeteUnit[UNIT_HOVERDETE_PACKET];
}ALG_HOVERDETE_PARA;



//modify (add) by hzc 2012-05-20

//�����ߣ����ߣ�
typedef struct _UNIT_ALARMLINENOTDIR_ENTER
{
	SHORT        x_start;
	SHORT        y_start;
	SHORT        x_end;
	SHORT        y_end;
//	LINE_DIR      direction; //ö������
	COLOR_CHOOSE  color;//�����ߵ���ɫ����
	SCHEDULE_UNIT scheduleunit[7][MAX_SEGMENT_ONEDAY];//�������ÿ��8��ʱ��Σ���СΪ224�ֽ�
	UINT          validflag;  //�˾�������Ч�Ա�־
	UINT          preset;// Ԥ�õ�
	UINT		  pretime;
}UNIT_ALARMLINENOTDIR_ENTER;  //��СΪ20 UINT����СΪ5*4+224 = 244Bytes

#define UNIT_ALARMLINENUM	16
typedef struct _ALARMAREA_ENTER
{
	UNIT_ALARMLINENOTDIR_ENTER  UNIT_ALARMLINE_ENTER[UNIT_ALARMLINENUM];    //������16���������ߣ���Ϊ4�ν��д���
}ALARMAREA_ENTER;

#define UNIT_ALARMLINE_PACKET 4
typedef struct _ALG_ALARMLINE_PARA{
	UNIT_ALARMLINENOTDIR_ENTER alarmLine[UNIT_ALARMLINE_PACKET];
}ALG_ALARMLINE_PARA;

//��������
#define UNIT_FINDBAG_PACKET 4 
typedef struct _ALG_FINDBAG_PARA{
//	CONFIG_UINT findBagUnit[UNIT_FINDBAG_PACKET];
	CONFIG_STOP findBagUnit[UNIT_FINDBAG_PACKET];
}ALG_FINDBAG_PARA;

//����������
#define UNIT_RUNNINGOBJ_PACKET 4 
typedef struct _ALG_RUNNINGOBJ_PARA{
	CONFIG_UNIT runningObjUnit[UNIT_RUNNINGOBJ_PACKET];
}ALG_RUNNINGOBJ_PARA;


#ifdef _cplusplus
}
#endif

#endif // AREACONF_H_INCLUDED