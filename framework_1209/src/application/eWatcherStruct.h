#ifndef eWatcherStruct_H_
#define eWatcherStruct_H_
#endif 


//���ݲɼ��� ������ݽṹ
#define EWATCHER_MAX_CH_CNT			16   //�����ŵ���Ŀ

typedef struct _EWATCHER_CAPTURE_t
{
	void * pHandle;//�漰���Ķ���
	int nDevNum;  //�ɼ������豸��
	int nChNum;   //�ź�ͨ��
	int nPicCnt;  //����ͼƬ��
	bool bAlarm; //��ͨ�����Ƿ��б���
	int iAlarmType;//��ͨ���ϱ���������
	DWORD dPtzTime;//�ȴ�ptz������ʱ��
}EWATCHER_CAPTURE_T;
