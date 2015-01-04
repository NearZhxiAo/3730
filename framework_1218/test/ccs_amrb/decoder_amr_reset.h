/*************************************************************************
*
*      GSM AMR-NB speech codec   R98   Version 7.5.0   March 2, 2001
*                                R99   Version 3.2.0                
*                                REL-4 Version 4.0.0                
*
*****************************************************************************
*/
#ifndef Decoder_amr_reset_h
#define Decoder_amr_reset_h "$Id $"

#include "mode.h"
#include "decoder_amr.h"


/*
**************************************************************************
*
*  Function    : Decoder_amr_reset
*  Purpose     : Resets state memory
*
**************************************************************************
*/
int Decoder_amr_reset (Decoder_amrState *state, enum Mode mode);

#endif
