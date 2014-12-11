#ifndef PTZ_H_INCLUDED
#define PTZ_H_INCLUDED
#include "TypeDefs.h"
#ifdef _cplusplus
extern "C" {
#endif

/*云台相关参数*/
//////////////////////////////////////////////////////////////////////////////////
//通信口定义
typedef enum _COM_PORT
{
    COM_START,
    COM1,
    COM2,
    COM3,
    COM4,
    COM5,
    COM_END
}COMM_PORT;
//波特率定义
typedef enum _COM_BAUD
{
    COM_BAUD_START,
    COM_BAUD_1200,
    COM_BAUD_2400,
    COM_BAUD_4800,
    COM_BAUD_9600,
    COM_BAUD_19200,
    COM_BAUD_38400,
    COM_BAUD_57600,
    COM_BAUD_115200,
    COM_BAUD_END
}COM_BAUD;
//协议
typedef enum _PTZ_PROTOCAL
{
    PTZ_START,
    PTZ_PELCO_P,
    PTZ_PELCO_D,
    PTZ_YAAN,
    PTZ_END
}PTZ_PROTOCAL;

/*云台参数结构体定义*/
typedef struct _PTZ_PARA
{
    COMM_PORT    commport;
    COM_BAUD     combaud;
    UINT         addr;//有效地址范围为从0x01~0xFF
    UINT         speed;
    PTZ_PROTOCAL protocol;
}PTZ_PARA;

#define GET_PTZ_BAUDVAL(val) 1200*(1<<(val-1))
#define GET_PTZ_COMVAL(val)  val
//云台动作定义
typedef enum  _PTZ_ACTION
{
    PTZ_ACT_START,
    PTZ_LEFT,
    PTZ_RIGHT,
    PTZ_UP,
    PTZ_DOWN,
	PTZ_ZOOMIN,
	PTZ_ZOOMOUT,
    PTZ_AUTO_FOCUS,
    PTZ_AUTO_IRIS,
    PTZ_SETPRESET,
    PTZ_GOTOPRESET,
    PTZ_SETSPEED,
    PTZ_ACT_END
}PTZ_ACTION;

typedef struct _PTZ_ACTION_SET
{
	PTZ_ACTION ptz_action;
	UINT       ptz_value;
}PTZ_ACTION_SET;

#ifdef _cplusplus
}
#endif

#endif // PTZ_H_INCLUDED
