/*
*****************************************************************************
*
*      GSM AMR-NB speech codec   R98   Version 7.5.0   March 2, 2001
*                                R99   Version 3.2.0                
*                                REL-4 Version 4.0.0                
*
*****************************************************************************
*/
#ifndef cod_amr_first_h
#define cod_amr_first_h "$Id $"

#include "typedef.h"
#include "cod_amr.h"
/***************************************************************************
 *   FUNCTION:   cod_amr_first
 *
 *   PURPOSE:  Copes with look-ahead.
 *
 *   INPUTS:
 *       No input argument are passed to this function. However, before
 *       calling this function, 40 new speech data should be copied to the
 *       vector new_speech[]. This is a global pointer which is declared in
 *       this file (it points to the end of speech buffer minus 200).
 *
 ***************************************************************************/
 
int cod_amr_first(cod_amrState *st,     /* i/o : State struct           */
                  Word16 new_speech[]   /* i   : speech input (L_FRAME) */
				  );  
#endif
