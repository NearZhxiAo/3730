/*
*****************************************************************************
*
*      GSM AMR-NB speech codec   R98   Version 7.5.0   March 2, 2001
*                                R99   Version 3.2.0                
*                                REL-4 Version 4.0.0                
*
*****************************************************************************
*/
#ifndef cod_amr_init_h
#define cod_amr_init_h "$Id $"

#include "typedef.h"
#include "cod_amr.h"
/*
**************************************************************************
*
*  Function    : cod_amr_init
*  Purpose     : Allocates memory and initializes state variables
*
**************************************************************************
*/
int cod_amr_init (cod_amrState *state, Flag dtx);

#endif
