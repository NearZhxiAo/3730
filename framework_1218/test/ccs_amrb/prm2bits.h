/*
********************************************************************************
*
*      GSM AMR-NB speech codec   R98   Version 7.5.0   March 2, 2001
*                                R99   Version 3.2.0                
*                                REL-4 Version 4.0.0                
*
********************************************************************************
*/
#ifndef Prm2bits_h
#define Prm2bits_h "$Id $"

#include "typedef.h"

/*************************************************************************
 *
 *  FUNCTION:    Prm2bits
 *
 *  PURPOSE:     converts the encoder parameter vector into a vector of serial
 *               bits.
 *
 *  DESCRIPTION: depending on the mode, different numbers of parameters
 *               (with differing numbers of bits) are processed. Details
 *               are found in bitno.tab
 *
 *************************************************************************/
void Prm2bits (
    enum Mode mode,    /* i : AMR mode                                      */
    Word16 prm[],      /* i : analysis parameters (size <= MAX_PRM_SIZE)    */
    Word16 bits[]      /* o : serial bits         (size <= MAX_SERIAL_SIZE) */
);

#endif

