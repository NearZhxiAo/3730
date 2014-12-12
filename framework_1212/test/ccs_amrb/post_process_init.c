/*
********************************************************************************
*
*      GSM AMR-NB speech codec   R98   Version 7.5.0   March 2, 2001
*                                R99   Version 3.2.0                
*                                REL-4 Version 4.0.0                
*
********************************************************************************
*/
#include "post_process.h"
#include "post_process_reset.h"

/*************************************************************************
*
*  Function:   Post_Process_init
*  Purpose:    Allocates state memory and initializes state memory
*
**************************************************************************
*/
int Post_Process_init (Post_ProcessState *state)
{
  
  Post_Process_reset(state);
  
  return 0;
}
