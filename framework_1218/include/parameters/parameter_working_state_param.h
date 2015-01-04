#ifndef __PARAMETER_WORKING_STATE_PARAM_H__
#define __PARAMETER_WORKING_STATE_PARAM_H__

#ifdef __cplusplus
extern "C"{
#endif

typedef enum _work_state
{
	WORK_STATE_START,
	WORK_STATE_WORKING,
	WORK_STATE_DEBUGING,
	WORK_STATE_END
}work_state;
typedef struct working_state_param
{
	work_state workstate;
}working_state_param;


#ifdef __cplusplus
}
#endif



#endif
