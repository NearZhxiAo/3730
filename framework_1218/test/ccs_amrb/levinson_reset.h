/*
*****************************************************************************
*
*      GSM AMR-NB speech codec   R98   Version 7.5.0   March 2, 2001
*                                R99   Version 3.2.0                
*                                REL-4 Version 4.0.0                
*
*****************************************************************************
*/
#ifndef Levinson_reset_h
#define Levinson_reset_h "$Id $"

#include "levinson.h"
 
/*************************************************************************
*
*  Function:   Levinson_reset
*  Purpose:    Initializes state memory to zero
*
**************************************************************************
*/
int Levinson_reset (LevinsonState *state);

#endif

