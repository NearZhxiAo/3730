#ifndef eWatcherStruct_H_
#define eWatcherStruct_H_
#endif 


//数据采集卡 相关数据结构
#define EWATCHER_MAX_CH_CNT			16   //最大的信道数目

typedef struct _EWATCHER_CAPTURE_t
{
	void * pHandle;//涉及到的对象
	int nDevNum;  //采集卡的设备号
	int nChNum;   //信号通道
	int nPicCnt;  //保存图片数
	bool bAlarm; //该通道上是否有报警
	int iAlarmType;//该通道上报警的类型
	DWORD dPtzTime;//等待ptz拉升的时间
}EWATCHER_CAPTURE_T;
