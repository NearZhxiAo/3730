#ifndef _INCSERIAL_AUTOGEN_
#define _INCSERIAL_AUTOGEN_
/*	This file is a autogenerate file .
 *	DONOT EDIT THIS FILE!!
 *  Edit incserial.def and incserial.tpl instead
 */
#ifdef __cplusplus
extern "C"{
#endif


#define DRIVER_DAVINCI_SERIAL  		 "davinci_serial"
#define DRIVER_DAVINCI_SERIAL_MAJOR       (239)



typedef struct _ioctl_struct
{
	unsigned int regaddr;
	unsigned int regvalue;
}ioctl_struct_system;

typedef enum _system_cmd
{
	SIMCARD_CMD_ENABLE,
	SIMCARD_CMD_DISABLE,
	SIMCARD_BAUD_SET,
	SIMCARD_CMD_SEND,
	SIMCARD_CMD_GET,
	SIMCARD_END	
}system_cmd;


typedef struct SIMCARD_SEND_
{
	unsigned int sendlen;
	unsigned char sendbuf[256];
}SIMCARD_SEND;

#ifdef __cplusplus
}
#endif 

#endif // _INCSERIAL_AUTOGEN_
    


