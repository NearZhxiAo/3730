#ifndef _WORKMODE_H_
#define _WORKMODE_H_

#ifdef _cplusplus
extern "C" {
#endif
#include <Afxmt.h>

#define EN_WORKINRUN WorkMode::en_WorkInRun
#define EN_WORKINDEBUG WorkMode::en_WorkInDebug

#define SET_WORKMODE WorkMode_instance.SetModeV
#define GET_WORKMODE WorkMode_instance.GetModeV

//��Ҫ���ǵ����߳����� ���쵥��ģʽ/ʹ��ȫ�ֱ��� �����ʹ��ȫ�ֱ���ʵ��
class WorkMode
{
public:
	WorkMode ();
	~WorkMode ();
public:
	//eType ����ģʽ
	enum EN_TYPE
	{
		en_WorkInRun = 0,	// ����״̬
		en_WorkInDebug		// ����״̬
	};
	static BOOL Initilize();
	static BOOL SetModeV(EN_TYPE mode);
	static EN_TYPE GetModeV();

private:
	static CCriticalSection mylock;
	static bool m_bInited;
	static EN_TYPE m_enWorkType;
};

extern WorkMode WorkMode_instance;

#ifdef _cplusplus
}
#endif



#endif /*#ifndef _WORKMODE_H_*/