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
#ifndef vad_complex_detection_update_h
#define vad_complex_detection_update_h "$Id $"

#include "typedef.h"
#include "vad1.h"
/****************************************************************************
 *
 *     Function     : vad_complex_detection_update
 *     Purpose      : update vad->bestCorr_hp  complex signal feature state 
 *
 ***************************************************************************/
void vad_complex_detection_update (vadState1 *st,       /* i/o : State struct */
                                   Word16 best_corr_hp /* i   : best Corr    */
                                   );
#endif

