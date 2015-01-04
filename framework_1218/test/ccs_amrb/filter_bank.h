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
#ifndef filter_bank_h
#define filter_bank_h "$Id $"

#include "typedef.h"
#include "vad1.h"
/****************************************************************************
 *
 *     Function     : filter_bank
 *     Purpose      : Divides input signal into 9-bands and calculas level of
 *                    the signal in each band 
 *
 ***************************************************************************/
void filter_bank(vadState1 *st,  /* i/o : State struct               */
                        Word16 in[],   /* i   : input frame                */
                        Word16 level[] /* 0   : signal levels at each band */
                        );
#endif
