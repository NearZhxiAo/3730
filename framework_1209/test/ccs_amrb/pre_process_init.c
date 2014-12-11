/*
********************************************************************************
*
*      GSM AMR-NB speech codec   R98   Version 7.5.0   March 2, 2001
*                                R99   Version 3.2.0                
*                                REL-4 Version 4.0.0                
*
********************************************************************************
*/
#include "pre_process.h"
#include "pre_process_reset.h"
 
/*************************************************************************
*
*  Function:   Pre_Process_init
*  Purpose:    Allocates state memory and initializes state memory
*
**************************************************************************
*/
int Pre_Process_init (Pre_ProcessState *state)
{
  
  Pre_Process_reset(state);
  
  return 0;
}
 
