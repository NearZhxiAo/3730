/*
********************************************************************************
*
*      GSM AMR-NB speech codec   R98   Version 7.5.0   March 2, 2001
*                                R99   Version 3.2.0                
*                                REL-4 Version 4.0.0                
*
********************************************************************************
*/
#include "typedef.h"
#include "basic_op.h"
#include "mode.h"
#include "table.h"

#define MASK      0x0001
 
/*************************************************************************
 *
 *  FUNCTION:  Int2bin
 *
 *  PURPOSE:  convert integer to binary and write the bits to the array
 *            bitstream[]. The most significant bits are written first.
 *
 *************************************************************************/

void Int2bin (
    Word16 value,       /* input : value to be converted to binary      */
    Word16 no_of_bits,  /* input : number of bits associated with value */
    Word16 *bitstream   /* output: address where bits are written       */
)
{
    Word16 *pt_bitstream, i, bit;

    pt_bitstream = &bitstream[no_of_bits];       

    for (i = 0; i < no_of_bits; i++)
    {
        bit = value & MASK;                      
         
        if (bit == 0)
        {
            *--pt_bitstream = BIT_0;             
        }
        else
        {
            *--pt_bitstream = BIT_1;             
        }
        value = value >> 1;
    }
}
