#include "SerialComm.h"
#include "PtzControl.h"

EXPORT_API bool CreatePtzControl(PtzControlInterface** ptzControl)
{
	*ptzControl = new CPtzControl();
	if(*ptzControl){
		return true;
	}else{
		return false;
	}
}
