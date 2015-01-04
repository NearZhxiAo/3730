/*
*****************************************************************************
*
*      GSM AMR-NB speech codec   R98   Version 7.5.0   March 2, 2001
*                                R99   Version 3.2.0                
*                                REL-4 Version 4.0.0                
*
*****************************************************************************
*/
#ifndef agc_init_h
#define agc_init_h "$Id $"

#include "agc.h"

/*
**************************************************************************
*
*  Function    : agc_init
*  Purpose     : Allocates memory for agc state and initializes
*                state memory
*
**************************************************************************
*/
int agc_init (agcState *state);

#endif
