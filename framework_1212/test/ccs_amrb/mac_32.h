/*
********************************************************************************
*
*      GSM AMR-NB speech codec   R98   Version 7.5.0   March 2, 2001
*                                R99   Version 3.2.0                
*                                REL-4 Version 4.0.0                
*
********************************************************************************
*/

#ifndef mac_32_h
#define mac_32_h "$Id $"

/*
********************************************************************************
*                         INCLUDE FILES
********************************************************************************
*/
#include "typedef.h"
#include "basic_op.h"
#include "oper_32b.h"

/*
********************************************************************************
*                         PUBLIC PROGRAM CODE
********************************************************************************
*/

/*****************************************************************************
 * Function Mac_32()                                                         *
 *                                                                           *
 *   Multiply two 32 bit integers (DPF) and accumulate with (normal) 32 bit  *
 *   integer. The multiplication result is divided by 2**31                  *
 *                                                                           *
 *   L_32 = L_32 + (hi1*hi2)<<1 + ( (hi1*lo2)>>15 + (lo1*hi2)>>15 )<<1       *
 *                                                                           *
 *   This operation can also be viewed as the multiplication of two Q31      *
 *   number and the result is also in Q31.                                   *
 *                                                                           *
 * Arguments:                                                                *
 *                                                                           *
 *  hi1         hi part of first number                                      *
 *  lo1         lo part of first number                                      *
 *  hi2         hi part of second number                                     *
 *  lo2         lo part of second number                                     *
 *                                                                           *
 *****************************************************************************
*/

Word32 Mac_32 (Word32 L_32, Word16 hi1, Word16 lo1, Word16 hi2, Word16 lo2);

/*****************************************************************************
 * Function Mac_32_16()                                                      *
 *                                                                           *
 *   Multiply a 16 bit integer by a 32 bit (DPF) and accumulate with (normal)*
 *   32 bit integer. The multiplication result is divided by 2**15           *
 *                                                                           *
 *                                                                           *
 *   L_32 = L_32 + (hi1*lo2)<<1 + ((lo1*lo2)>>15)<<1                         *
 *                                                                           *
 * Arguments:                                                                *
 *                                                                           *
 *  hi          hi part of 32 bit number.                                    *
 *  lo          lo part of 32 bit number.                                    *
 *  n           16 bit number.                                               *
 *                                                                           *
 *****************************************************************************
*/

Word32 Mac_32_16 (Word32 L_32, Word16 hi, Word16 lo, Word16 n);

#endif
