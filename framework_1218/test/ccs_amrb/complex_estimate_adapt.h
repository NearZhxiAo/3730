/*
*****************************************************************************
**-------------------------------------------------------------------------**
**                                                                         **
**     GSM AMR-NB speech codec   R98   Version 7.5.0   March 2, 2001       **
**                               R99   Version 3.2.0                       **
**                               REL-4 Version 4.0.0                       **
**                                                                         **
**-------------------------------------------------------------------------**
*****************************************************************************
*/
#ifndef complex_estimate_adapt_h
#define complex_estimate_adapt_h "$Id $"

#include "typedef.h"
#include "vad1.h"
/****************************************************************************
 *
 *     Function   : complex_estimate_adapt
 *     Purpose    : Update/adapt of complex signal estimate
 *     Inputs     : low_power:   low signal power flag 
 *     Outputs    : st->corr_hp_fast:   long term complex signal estimate
 *
 ***************************************************************************/
void complex_estimate_adapt(
         vadState1 *st,       /* i/o : VAD state struct                       */
         Word16 low_power    /* i   : very low level flag of the input frame */
         );

#endif
