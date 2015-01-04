/*
********************************************************************************
*
*      GSM AMR-NB speech codec   R98   Version 7.5.0   March 2, 2001
*                                R99   Version 3.2.0                
*                                REL-4 Version 4.0.0                
*
********************************************************************************
*/
#ifndef preemphasis_reset_h
#define preemphasis_reset_h "$Id $"

#include "preemphasis.h"

/*************************************************************************
*
*  Function:   preemphasis_reset
*  Purpose:    Initializes state memory to zero
*
**************************************************************************
*/
int preemphasis_reset (preemphasisState *state);

#endif
