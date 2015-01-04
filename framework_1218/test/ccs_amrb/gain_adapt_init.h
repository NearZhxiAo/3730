/*
********************************************************************************
*
*      GSM AMR-NB speech codec   R98   Version 7.5.0   March 2, 2001
*                                R99   Version 3.2.0                
*                                REL-4 Version 4.0.0                
*
********************************************************************************
*/
#ifndef gain_adapt_init_h
#define gain_adapt_init_h "$Id $"

#include "gain_adapt.h"

/*************************************************************************
*
*  Function:   gain_adapt_init
*  Purpose:    Allocates state memory and initializes state memory
*
**************************************************************************
*/
int gain_adapt_init (GainAdaptState *st);

#endif
