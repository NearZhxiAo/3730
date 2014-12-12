/*************************************************************************
*
*      GSM AMR-NB speech codec   R98   Version 7.5.0   March 2, 2001
*                                R99   Version 3.2.0                
*                                REL-4 Version 4.0.0                
*
********************************************************************************
*/
#include "cb_gain_average.h"
#include "Cb_gain_average_reset.h"
/*
**************************************************************************
*
*  Function    : Cb_gain_average_init
*  Purpose     : Allocates and initializes state memory
*
**************************************************************************
*/
Word16 Cb_gain_average_init (Cb_gain_averageState *state)
{
   Cb_gain_average_reset(state);
  
   return 0;
}
