/*
********************************************************************************
*
*      GSM AMR-NB speech codec   R98   Version 7.5.0   March 2, 2001
*                                R99   Version 3.2.0                
*                                REL-4 Version 4.0.0                
*
********************************************************************************
*/
#ifndef Bin2int_h
#define Bin2int_h "$Id $"

#include "typedef.h"

/*
**************************************************************************
*
*  Function    : Bin2int
*  Purpose     : Read "no_of_bits" bits from the array bitstream[] 
*                and convert to integer.
*
**************************************************************************
*/
Word16 Bin2int ( /* Reconstructed parameter                      */
    Word16 no_of_bits,  /* input : number of bits associated with value */
    Word16 *bitstream   /* output: address where bits are written       */
);
#endif
