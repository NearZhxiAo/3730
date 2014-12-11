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

//需要考虑到多线程问题 构造单件模式/使用全局变量 简单起见使用全局变量实现
class WorkMode
{
public:
	WorkMode ();
	~WorkMode ();
public:
	//eType 运行模式
	enum EN_TYPE
	{
		en_WorkInRun = 0,	// 工作状态
		en_WorkInDebug		// 调试状态
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