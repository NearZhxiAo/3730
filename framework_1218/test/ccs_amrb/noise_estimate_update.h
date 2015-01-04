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
#ifndef noise_estimate_update_h
#define noise_estimate_update_h "$Id $"

#include "typedef.h"
#include "vad1.h"
/****************************************************************************
 *
 *     Function   : noise_estimate_update
 *     Purpose    : Update of background noise estimate
 *     Inputs     : bckr_est:   background noise estimate
 *                  pitch:      flags for pitch detection
 *                  stat_count: stationary counter
 *     Outputs    : bckr_est:   background noise estimate
 *
 ***************************************************************************/
void noise_estimate_update(
                  vadState1 *st,    /* i/o : State struct                       */
                  Word16 level[]   /* i   : sub-band levels of the input frame */
                  );
#endif
