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
#ifndef vad_pitch_detection_h
#define vad_pitch_detection_h "$Id $"

#include "typedef.h"
#include "vad1.h"
/****************************************************************************
 *
 *     Function     : vad_pitch_detection
 *     Purpose      : Test whether signal contains pitch or other periodic
 *                    component.
 *     Return value : Boolean voiced / unvoiced decision in state variable 
 *
 ***************************************************************************/
void vad_pitch_detection (vadState1 *st,   /* i/o : State struct                  */
                          Word16 T_op[]   /* i   : speech encoder open loop lags */
                          );
#endif
