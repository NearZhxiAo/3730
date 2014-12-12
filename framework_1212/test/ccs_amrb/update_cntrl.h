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
#ifndef update_cntrl_h
#define update_cntrl_h "$Id $"

#include "typedef.h"
#include "vad1.h"

/****************************************************************************
 *
 *     Function   : update_cntrl
 *     Purpose    : Control update of the background noise estimate.
 *     Inputs     : pitch:      flags for pitch detection
 *                  stat_count: stationary counter
 *                  tone:       flags indicating presence of a tone
 *                  complex:      flags for complex  detection
 *                  vadreg:     intermediate VAD flags
 *     Output     : stat_count: stationary counter
 *
 ***************************************************************************/
void update_cntrl(vadState1 *st,  /* i/o : State struct                       */
                         Word16 level[] /* i   : sub-band levels of the input frame */
                         );

#endif

