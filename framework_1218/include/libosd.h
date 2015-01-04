#ifndef __LIBOSD_H__
#define __LIBOSD_H__





typedef enum _type
{
	TIME=0,
	CHAR,
	CHINESE,
}DrawType;

typedef enum _action
{
	IDLE = 0,
	UP ,
	UP_IDLE,
	DOWN,
	DOWN_IDLE
}ACTION;


int set_osd(char *data, DrawType drawtype, char *text, int x, int y, int colorValue);
















#endif
